C Copyright (C) 2002, Carnegie Mellon University and others.
C All Rights Reserved.
C This code is published under the Common Public License.
C*******************************************************************************
C
      subroutine UPDATE_B_LM(N, NIND, M, X, ITER, NFIX, IFIX, NORIG,
     1     XORIG, CSCALE, G, LAM, SKIP_UPDATE, B, C_SKIP,
     2     KCONSTR, LRS, RS, LIS, IS, LRW, RW, LIW, IW, IERR)
C
C*******************************************************************************
C
C    $Id: update_b_lm.f,v 1.1 2002/11/24 21:50:30 andreasw Exp $
C
C-------------------------------------------------------------------------------
C                                 Title
C-------------------------------------------------------------------------------
C
CT    Update compact representation of limited memory BFGS
C
C-------------------------------------------------------------------------------
C                          Programm description
C-------------------------------------------------------------------------------
C
CB    Output will be SIGMAK, UK and VK, so that the limited memory
CB    BFGS matrix will be
CB
CB    B_0 = SIGMAK * I  +  UK * UK^T  -  VK * VK^T
CB
CB    This is stored in B (required total length: 2+2*N+NLMMAX*(1+4*N+2*NLMMAX)
CB
CB    Stucture of B:
CB     dble(NLM)        1     number of currently stored (s,y) pairs
CB     SIGMAK           1     factor for B_0
CB     UK          (N,NLMMAX)
CB     VK          (N,NLMMAX)
CB     SK          (N,NLMMAX) stored s_k values
CB     YK          (N,NLMMAX) stored y_k values
CB     LK          (NLMMAX,NLMMAX)
CB     SSK         (NLMMAX,NLMMAX)
CB     DK             NLMMAX
CB     XOLD             N     (this is waistfull, I know, but I'm lazy :)
CB     GOLD             N
C
C-------------------------------------------------------------------------------
C                             Author, date
C-------------------------------------------------------------------------------
C
CA    Andreas Waechter      09/21/02  First version
C
C-------------------------------------------------------------------------------
C                             Documentation
C-------------------------------------------------------------------------------
C
CD
C
C-------------------------------------------------------------------------------
C                             Parameter list    
C-------------------------------------------------------------------------------
C
C    Name     I/O   Type   Meaning
C
CP   N         I    INT    number of (free) variables; first NIND variables
CP                         are independent; remaining dependent
CP                         (it is assumed that there is never a repartition)
CP   NIND      I    INT    number of independent variables
CP   M         I    INT    number of equality constraints / dependent variables
CP   X         I    DP     actual primal iterate
CP   ITER      I    INT    iteration counter (if ITER = 0 B is initialized)
CP   NFIX      I    INT    number of fixed variables
CP   IFIX      I    INT    specifies variables that are fixed by bounds:
CP                            i = 1..NORIG-N   XORIG(IFIX(i)) is fixed
CP   NORIG     I    INT    total number of all variables (incl. fixed vars)
CP   XORIG     I    DP     actual iterate
CP                            XORIG is ordered in ORIGINAL order (i.e. not
CP                            partitioned into independent and dependent
CP                            variables)
CP   CSCALE    I    DP     scaling factors for constraints
CP   G         I    DP     actual gradient of objective function
CP   LAM       I    DP     Lagrangian multipliers
CP   SKIP_UPDATE I/O DP    I: if true, do not do Quasi-Newton update
CP                         O: set to false
CP   B        I/O   DP     estimate for Hessian of original Lagrangian function
CP                            (structure descirbed above)
CP   C_SKIP    O    C*1    ='*' if update is skipped, otherwise = ' '
CP   KCONSTR   I    INT    KCONSTR(1): LRS for CONSTR
CP                         KCONSTR(2): P_LRS for CONSTR
CP                         KCONSTR(3): LIS for CONSTR
CP                         KCONSTR(4): P_LIS for CONSTR
CP                         KCONSTR(5): LRW for CONSTR
CP                         KCONSTR(6): LIW for CONSTR
CP   LRS       I    INT    total length of RS
CP   RS       I/O   DP     DP storage space (all!)
CP   LIS       I    INT    total length of IS
CP   IS       I/O   INT    INT storage space (all!)
CP   LRW      I/O   INT    length of RW
CP   RW       I/O   DP     can be used as DP work space but content will be
CP                            changed between calls
CP   LIW      I/O   INT    length of IW
CP   IW       I/O   INT    can be used as INT work space but content will be
CP                            changed between calls
CP   IERR      O    INT    =0: everything OK
CP                         >0: Error occured; abort optimization
CP                         <0: Warning; message to user
C
C-------------------------------------------------------------------------------
C                             local variables
C-------------------------------------------------------------------------------
C
CL
C
C-------------------------------------------------------------------------------
C                             used subroutines
C-------------------------------------------------------------------------------
C
CS    DDOT
CS    DCOPY
CS    DSCAL
CS    DAXPY
CS    CONSTR
CS    C_OUT
C
C*******************************************************************************
C
C                              Declarations
C
C*******************************************************************************
C
      IMPLICIT NONE
C
C*******************************************************************************
C
C                              Include files
C
C*******************************************************************************
C
      include 'IPOPT.INC'
C
C-------------------------------------------------------------------------------
C                             Parameter list
C-------------------------------------------------------------------------------
C
      integer N
      integer NIND
      integer M
      double precision X(N)
      integer ITER
      integer IVAR(N)
      integer NFIX
      integer IFIX(NFIX)
      integer NORIG
      double precision XORIG(NORIG)
      double precision CSCALE(*)
      double precision G(N)
      double precision LAM(M)
      logical SKIP_UPDATE
      double precision B(*)
      character*1 C_SKIP
      integer KCONSTR(6)
      integer LRS
      double precision RS(LRS)
      integer LIS
      integer IS(LIS)
      integer LRW
      double precision RW(LRW)
      integer LIW
      integer IW(LIW)
      integer IERR
C
C-------------------------------------------------------------------------------
C                            Local varibales
C-------------------------------------------------------------------------------
C
      double precision DDOT, DNRM2
      integer i, j, nlm, nlmmax, idummy, blen
      integer p_rwend, p_iwend, p_snew, p_ynew, p_tmp
      integer pb_nlm, pb_sigmak, pb_uk, pb_vk, pb_sk, pb_yk, pb_lk
      integer pb_ssk, pb_dk, pb_xold, pb_gold, p_bs
      double precision sty, snrm, ynrm, sBs, theta
      logical ex

      character*80 line(2)
C
C*******************************************************************************
C
C                           Executable Statements
C
C*******************************************************************************
C
      IERR = 0
      p_iwend = 0
      p_rwend = 0

      nlmmax = QLMLEN
C
C     Get the structure of B right
C
      pb_nlm    = 0
      pb_sigmak = 1
      pb_uk     = 2
      pb_vk     = pb_uk + N*nlmmax
      pb_sk     = pb_vk + N*nlmmax
      pb_yk     = pb_sk + N*nlmmax
      pb_lk     = pb_yk + N*nlmmax
      pb_ssk    = pb_lk + nlmmax*nlmmax
      pb_dk     = pb_ssk+ nlmmax*nlmmax
      pb_xold   = pb_dk + nlmmax
      pb_gold   = pb_xold + N

      nlm = int(B(pb_nlm+1))
C
C     If no degrees of freedom, there is nothing to be done
C
      if( NIND.eq.0 ) then
         C_SKIP = 's'
         B(pb_sigmak+1) = 1.d0
         nlm = 0
         goto 9999
      endif

      C_SKIP = ' '

      if( ITER.eq.0 ) then
C
C     If QBWARMSTART is set and file 'BWARM.DAT' exists, read initial
C     quasi-Newton estimate from file
C
         if( QBWARMSTART.eq.1 ) then
            inquire(file='BWARM.DAT', exist=ex)
            blen = 2+2*N+QLMLEN*(1+4*N+2*QLMLEN)
            if( ex ) then
               open(80,file='BWARM.DAT',status='old',err=9008)
               read(80,'(i16)',end=9009,err=9009) i
               if( i.ne.blen ) then
                  write(line,100)
 100              format('update_b: Warning: Number of elements in',
     1                 ' BWARM.DAT for warm start',/,
     2      '          does not match.  Ignoring warm start option.')
                  call C_OUT(2,0,2,line)
                  close(80)
               else
                  do i = 1, blen
                     read(80,'(d23.16)',end=9009,err=9009) B(i)
                  enddo
                  close(80)
                  goto 8000
               endif
            else
               write(line,101)
 101           format('update_b:  File BWARM.DAT does not exist;',
     1              ' performing usual initialization.')
               call C_OUT(2,0,1,line)
            endif
         endif
C
C     Initialize
C
         nlm = 0
         B(pb_sigmak+1) = 1.d0

      elseif( .not.SKIP_UPDATE ) then

         p_snew  = p_rwend
         p_ynew  = p_snew + N
         p_rwend = p_ynew + N
         if( p_rwend.gt.LRW ) then
            IERR = 98
            goto 9000
         endif
C
C     Compute y = grad L(x,lam) - grad L(x_old,lam)
C
         if( M.gt.0 ) then
            call CONSTR(8, ITER, N, NIND, M, IVAR, NFIX, IFIX,
     1           NORIG, XORIG, CSCALE, LAM, RW(p_ynew+1),
     2           idummy, idummy,
     3           KCONSTR(1), RS(KCONSTR(2)+1), KCONSTR(3),
     4           IS(KCONSTR(4)+1), LRW-p_rwend, RW(p_rwend+1),
     5           LIW-p_iwend, IW(p_iwend+1), IERR)
            if( IERR.lt.0 ) then
               write(line,*) 
     1              'update_b_lm: Warning in CONSTR-8, IERR = ', IERR
               call C_OUT(2,0,1,line)
            elseif( IERR.ne.0 ) then
               write(line,*)
     1              'update_b_lm: Error in CONSTR-8, IERR = ',IERR
               call C_OUT(2,0,1,line)
               goto 9999
            endif
C
            p_tmp   = p_rwend
            p_rwend = p_tmp + N
            if( p_rwend.gt.LRW ) then
               IERR = 98
               goto 9999
            endif
            call CONSTR(12, ITER, N, NIND, M, IVAR, NFIX, IFIX,
     1           NORIG, XORIG, CSCALE, LAM, RW(p_tmp+1),
     2           idummy, idummy,
     3           KCONSTR(1), RS(KCONSTR(2)+1), KCONSTR(3),
     4           IS(KCONSTR(4)+1), LRW-p_rwend, RW(p_rwend+1),
     5           LIW-p_iwend, IW(p_iwend+1), IERR)
            if( IERR.lt.0 ) then
               write(line,*) 
     1              'update_b_lm: Warning in CONSTR-12, IERR = ', IERR
               call C_OUT(2,0,1,line)
            elseif( IERR.ne.0 ) then
               write(line,*)
     1              'update_b_lm: Error in CONSTR-12, IERR = ',IERR
               call C_OUT(2,0,1,line)
               goto 9999
            endif
            call DAXPY(N, -1.d0, RW(p_tmp+1), 1, RW(p_ynew+1), 1)
            p_rwend = p_tmp
C
            call DAXPY(N, 1d0, G, 1, RW(p_ynew+1), 1)
         else
            call DCOPY(N, G, 1, RW(p_ynew+1), 1)
         endif
         call DAXPY(N, -1.d0, B(pb_gold+1), 1, RW(p_ynew+1), 1)
C
C     Compute s
C
         call DCOPY(N, X, 1, RW(p_snew+1), 1)
         call DAXPY(N, -1.d0, B(pb_xold+1), 1, RW(p_snew+1), 1)
C
C     Check if we want to skip the update
C
         sty  = DDOT(N, RW(p_snew+1), 1, RW(p_ynew+1), 1)
         snrm = DNRM2(N, RW(p_snew+1), 1)
         ynrm = DNRM2(N, RW(p_ynew+1), 1)
         if( QQUASI.eq.-6 ) then
C
C     Compute Bs
C
            p_bs    = p_rwend
            p_tmp   = p_bs + N
            p_rwend = p_tmp + nlm
            if( p_rwend.gt.LRW ) then
               IERR = 98
               goto 9999
            endif
C     SIGMAK I
            call DCOPY(N, RW(p_snew+1), 1, RW(p_bs+1), 1)
            call DSCAL(N, B(pb_sigmak+1), RW(p_bs+1), 1)
C     UK^T UK
            call DGEMV( 'T', N, nlm, 1.d0, B(pb_uk+1), N, RW(p_snew+1),
     1           1, 0.d0, RW(p_tmp+1), 1)
            call DGEMV( 'N', N, nlm, 1.d0, B(pb_uk+1), N, RW(p_tmp+1),
     1           1, 1.d0, RW(p_bs+1), 1)
C     VK^T VK
            call DGEMV( 'T', N, nlm, 1.d0, B(pb_vk+1), N, RW(p_snew+1),
     1           1, 0.d0, RW(p_tmp+1), 1)
            call DGEMV( 'N', N, nlm, -1.d0, B(pb_vk+1), N, RW(p_tmp+1),
     1           1, 1.d0, RW(p_bs+1), 1)
C           sBs = s'*bs
            sBs = DDOT(N, RW(p_snew+1), 1, RW(p_bs+1), 1)
            if( sty.lt.0.2d0*sBs) then
               theta = 0.8d0*sBs/(sBs - sty)
               call DSCAL(N, theta, RW(p_ynew+1), 1)
               call DAXPY(N, (1.d0-theta), RW(p_bs+1), 1,
     1              RW(p_ynew+1), 1)
               sty = theta*sty + (1.d0-theta)*sBs
               C_SKIP = 'd'
            endif
            p_rwend = p_bs
         endif
         if( sty.gt.1.d-8*snrm*ynrm ) then
            call LMBFGS(N, nlm, nlmmax, RW(p_snew+1), RW(p_ynew+1),
     1           B(pb_sk+1), B(pb_yk+1), B(pb_lk+1), B(pb_ssk+1),
     1           B(pb_dk+1), B(pb_sigmak+1), B(pb_uk+1), B(pb_vk+1),
     1           LRW-p_rwend, RW(p_rwend+1), LIW-p_iwend, IW(p_iwend+1),
     1           IERR)
            if( IERR.eq.631 ) then
               call C_OUT(2,0,1,'         Restarting L-BFGS...')
               nlm = 0
               B(pb_sigmak+1) = 1.d0
               C_SKIP = 'r'
               IERR = 0
            elseif( IERR.ne.0 ) then
               write(line,*) 'update_b_lm: LMBFGS returns IERR = ',IERR
               call C_OUT(2,0,1,line)
               goto 9999
            endif
         else
            C_SKIP = '*'
         endif
      endif
C
C     Store current X and gradient of Lagrangian function
C
 8000 continue
      call DCOPY(N, X, 1, B(pb_xold+1), 1)
      call DCOPY(N, G, 1, B(pb_gold+1), 1)
      p_rwend = p_snew

 9000 continue
      B(pb_nlm+1) = dble(nlm)

 9999 continue
      return

 9008 IERR = 8
      call C_OUT(2,0,1,'Error while trying to open BWARM.DAT')
      goto 9999
 9009 IERR = 8
      call C_OUT(2,0,1,'Error while writing to BWARM.DAT')
      goto 9999
      end

C*******************************************************************************
C
      subroutine LMBFGS(N, NLM, NLMMAX, SNEW, YNEW, SK, YK, LK,
     1     SSK, DK, SIGMAK, UK, VK, LRW, RW, LIW, IW, IERR)
C
C*******************************************************************************
C
C    $Id: update_b_lm.f,v 1.1 2002/11/24 21:50:30 andreasw Exp $
C
C-------------------------------------------------------------------------------
C                                 Title
C-------------------------------------------------------------------------------
C
CT    Update compact representation of limited memory BFGS
C
C-------------------------------------------------------------------------------
C                          Programm description
C-------------------------------------------------------------------------------
C
CB    Output will be SIGMAK, UK and VK, so that the limited memory
CB    BFGS matrix will be
CB
CB    B_0 = SIGMAK * I  +  UK * UK^T  -  VK * VK^T
C
C-------------------------------------------------------------------------------
C                             Author, date
C-------------------------------------------------------------------------------
C
CA    Andreas Waechter      09/19/02  First version
C
C-------------------------------------------------------------------------------
C                             Documentation
C-------------------------------------------------------------------------------
C
CD
C
C-------------------------------------------------------------------------------
C                             Parameter list    
C-------------------------------------------------------------------------------
C
C    Name     I/O   Type   Meaning
C
CP   N         I    INT    number of (free) variables
CP   NLM       I    INT    number of (s,y) pairs already stored
CP   NLMMAX    I    INT    maximal number of (s,y) pairs
CP   SNEW      I    DP     new vector S
CP   YNEW      I    DP     new vector Y
CP   SK       I/O   DP     stored S vectors
CP   YK       I/O   DP     stored Y vectors
CP   LK       I/O   DP     matrix L_k from BNS paper
CP   SSK      I/O   DP     S_k^T S_k
CP   DK       I/O   DP     diagonal matrix D_k from BNS paper
CP   SIGMAK    O    DP     factor from identity in B_0
CP   UK        O    DP     matrix containing dense representation
CP   VK        O    DP     matrix containing dense representation
CP   LRW      I/O   INT    length of RW
CP   RW       I/O   DP     can be used as DP work space but content will be
CP                            changed between calls
CP   LIW      I/O   INT    length of IW
CP   IW       I/O   INT    can be used as INT work space but content will be
CP                            changed between calls
CP   IERR      O    INT    =0: everything OK
CP                         >0: Error occured; abort optimization
CP                         <0: Warning; message to user
C
C-------------------------------------------------------------------------------
C                             local variables
C-------------------------------------------------------------------------------
C
CL
C
C-------------------------------------------------------------------------------
C                             used subroutines
C-------------------------------------------------------------------------------
C
CS    DDOT
CS    DCOPY
CS    DSCAL
CS    DTPSV
CS    DGEMV
CS    DPPTRF
CS    DPPTRS
CS    C_OUT
C
C*******************************************************************************
C
C                              Declarations
C
C*******************************************************************************
C
      IMPLICIT NONE
C
C*******************************************************************************
C
C                              Include files
C
C*******************************************************************************
C
      include 'IPOPT.INC'
C
C-------------------------------------------------------------------------------
C                             Parameter list
C-------------------------------------------------------------------------------
C                        
      integer N
      integer NLM
      integer NLMMAX
      double precision SNEW(N)
      double precision YNEW(N)
      double precision SK(N,NLMMAX)
      double precision YK(N,NLMMAX)
      double precision LK(NLMMAX,NLMMAX)
      double precision SSK(NLMMAX,NLMMAX)
      double precision DK(NLMMAX)
      double precision SIGMAK
      double precision UK(N,NLMMAX)
      double precision VK(N,NLMMAX)
      integer LRW
      double precision RW(LRW)
      integer LIW
      integer IW(LIW)
      integer IERR
C
C-------------------------------------------------------------------------------
C                            Local varibales
C-------------------------------------------------------------------------------
C
      double precision DDOT
      integer i, j, info, ind, p_iwend, p_rwend, p_jk, p_ldk

      character*80 line
C
C*******************************************************************************
C
C                           Executable Statements
C
C*******************************************************************************
C
      IERR = 0
      p_iwend = 0
      p_rwend = 0
C
C     Store new (s,y) pair
C
      if( NLM.lt.NLMMAX ) then
         call DCOPY(N, SNEW, 1, SK(1,NLM+1), 1)
         call DCOPY(N, YNEW, 1, YK(1,NLM+1), 1)
      else
         do i = 1, NLM-1
            call DCOPY(N, SK(1,i+1), 1, SK(1,i), 1)
            call DCOPY(N, YK(1,i+1), 1, YK(1,i), 1)
         enddo
         call DCOPY(N, SNEW, 1, SK(1,NLMMAX), 1)
         call DCOPY(N, YNEW, 1, YK(1,NLMMAX), 1)
      endif
C
C     Update matrix L
C
      if( NLM.lt.NLMMAX ) then
         call DCOPY(NLM+1, 0.d0, 0, LK(1,NLM+1), 1)
         do j = 1, NLM
            LK(NLM+1,j) = DDOT(N, SK(1,NLM+1), 1, YK(1,j), 1)
         enddo
      else
         do i = 2, NLM-1
            call DCOPY(NLM-i, LK(i+1,i), 1, LK(i,i-1), 1)
         enddo
         do j = 1, NLM-1
            LK(NLM,j) = DDOT(N, SK(1,NLM), 1, YK(1,j), 1)
         enddo
      endif
C
C     Update SSK
C
      if( NLM.lt.NLMMAX ) then
         do j = 1, NLM+1
            SSK(NLM+1,j) = DDOT(N, SK(1,NLM+1), 1, SK(1,j), 1)
         enddo
         call DCOPY(NLM, SSK(NLM+1,1), NLMMAX, SSK(1,NLM+1), 1)
      else
         do i = 2, NLM
            call DCOPY(NLM-1, SSK(2,i), 1, SSK(1,i-1), 1)
         enddo
         do j = 1, NLM
            SSK(NLM,j) = DDOT(N, SK(1,NLM), 1, SK(1,j), 1)
         enddo
         call DCOPY(NLM-1, SSK(NLM,1), NLM, SSK(1,NLM), 1)
      endif
C
C     Update DK
C
      if( NLM.lt.NLMMAX ) then
         DK(NLM+1) = DDOT(N, SK(1,NLM+1), 1, YK(1,NLM+1), 1)
      else
         do i = 1, NLM-1
            DK(i) = DK(i+1)
         enddo
         DK(NLM) = DDOT(N, SK(1,NLM), 1, YK(1,NLM), 1)
      endif
C
C     Update NLM
C
      if( NLM.lt.NLMMAX) NLM = NLM + 1
C
C     Compute SIGMAK
C
c      SIGMAK = 1.d0
      SIGMAK = DK(NLM)/SSK(NLM,NLM)

      p_jk    = p_rwend
      p_ldk   = p_jk  + (NLM*(NLM+1))/2
      p_rwend = p_ldk + NLM*NLM
      if( p_rwend.gt.LRW ) then
         IERR = 98
         goto 9999
      endif
C
C     Compute L_k * D_k^{-1/2} first
C
      do j = 1, NLM
         call DCOPY(NLM, LK(1,j), 1, RW(p_ldk+1+NLM*(j-1)), 1)
         call DSCAL(NLM, 1.d0/sqrt(DK(j)), RW(p_ldk+1+NLM*(j-1)), 1)
      enddo
C
C     Compute SIGMAK * SSK + LK * DK^-1 * LK^T
C
      ind = 1
      do j = 1, NLM
         do i = j, NLM
            RW(p_jk+ind) = SIGMAK * SSK(i,j) +
     1           DDOT(NLM, RW(p_ldk+i), NLM, RW(p_ldk+j), NLM)
            ind = ind + 1
         enddo
      enddo
C
C     Compute Cholesky factorization 
C
      call DPPTRF( 'L', NLM, RW(p_jk+1), info )
      if( info.ne.0 ) then
         write(line,*) 'lmbfgs: DPPTRF returns info = ',info
         call C_OUT(2,0,1,line)
         IERR = 631
         goto 9999
      endif
C
C     Now compute UK
C
      do j = 1, NLM
         call DCOPY( N, YK(1,j), 1, UK(1,j), 1 )
         call DSCAL( N, 1/sqrt(DK(j)), UK(1,j), 1)
      enddo
C
C     Do MANY backsolves etc to get VK
C
      do i = 1, N
         call DGEMV('N', NLM, NLM, 1.d0, RW(p_ldk+1), NLM, UK(i,1), N,
     1        0.d0, VK(i,1), N)
         call DAXPY(NLM, SIGMAK, SK(i,1), N, VK(i,1), N)
         call DTPSV('L', 'N', 'N', NLM, RW(p_jk+1), VK(i,1), N)
      enddo

 9999 continue
      return
      end
