C Copyright (C) 2002, Carnegie Mellon University and others.
C All Rights Reserved.
C This code is published under the Common Public License.
C*******************************************************************************
C
      subroutine GET_WCORR(N, NIND, M, X, ITER, IVAR, NFIX, IFIX,
     1                     NORIG, XORIG, CSCALE, LAM,
     1                     NLB, ILB, NUB, IUB, LSLACKS,
     2                     S_L, S_U, BNDS_L, BNDS_U,
     1                     SIGMA_L, SIGMA_U, YPY, REGU, WCORR, RESTO,
     1                     KCONSTR, LRS, RS, LIS, IS,
     2                     LRW, RW, LIW, IW, IERR)
C
C*******************************************************************************
C
C    $Id: get_wcorr.f,v 1.3 2002/11/24 21:41:10 andreasw Exp $
C
C-------------------------------------------------------------------------------
C                                 Title
C-------------------------------------------------------------------------------
C
CT    Compute barrier correction term for null space step
C
C-------------------------------------------------------------------------------
C                          Programm description
C-------------------------------------------------------------------------------
C
CB    
C
C-------------------------------------------------------------------------------
C                             Author, date
C-------------------------------------------------------------------------------
C
CA    Andreas Waechter      05/01/02  Release as version IPOPT 2.0
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
CP   N         I    INT    number of variables
CP   NIND      I    INT    number of independent variables
CP   M         I    INT    number of equality constraints / dependent variables
CP   X         I    DP     actual primal iterate
CP                             (first M entries belong to dependent
CP                             variables, remaining to independent variables)
CP   ITER      I    INT    iteration counter
CP   IVAR      I    INT    information about partitioning
CP                            i = 1..M      XORIG(IVAR(i)) dependent
CP                            i = (M+1)..N  XORIG(IVAR(i)) independent
CP                            Note: fixed variables do not occur in IVAR
CP   NFIX      I    INT    number of fixed variables
CP   IFIX      I    INT    specifies variables that are fixed by bounds:
CP                            i = 1..NORIG-N   XORIG(IFIX(i)) is fixed
CP   NORIG     I    INT    total number of all variables (incl. fixed vars)
CP   XORIG     I    DP     (only TASK = 1,2,3): actual iterate
CP                            XORIG is ordered in ORIGINAL order (i.e. not
CP                            partitioned into independent and dependent
CP                            variables)
CP   CSCALE    I    DP     scaling factor for constraints
CP   LAM       I    DP     Lagrangian multipliers
CP   NLB       I    INT    number of lower bounds (excluding fixed vars)
CP   ILB       I    INT    indices of lower bounds
CP                            (e.g. S_L(i) is slack for X(ILB(i)) )
CP   NUB       I    INT    number of upper bounds (excluding fixed vars)
CP   IUB       I    INT    indices of upper bounds
CP                            (e.g. S_U(i) is slack for X(IUB(i)) )
CP   LSLACKS   I    LOG    =.true.: There are slacks that don't satisfy
CP                                  "slack equation"
CP   S_L       I    DP     slacks to lower bounds
CP   S_U       I    DP     slacks to upper bounds
CP   BNDS_L    I    DP     values of lower bounds (ordered as S_L)
CP   BNDS_U    I    DP     values of upper bounds (ordered as S_U)
CP   SIGMA_L   I    DP     primal-dual Hessian of lower bound barrier term
CP                            (NLB diagonal elements only)
CP   SIGMA_U   I    DP     primal-dual Hessian of upper bound barrier term
CP                            (NUB diagonal elements only)
CP   YPY       I    DP     range space step
CP   REGU      I    DP     regularization factor (added regu*I to diagonal)
CP                            needs to be included in wcorr
CP   WCORR     O    DP     correction term for null space step
CP   RESTO     I    LOG    =.true.: we are in restoration phase
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
CS    CONSTR
CS    DSCAL
CS    DCOPY
CS    DAXPY
CS    C_OUT
CS    GET_HV
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
      double precision LAM(M)
      integer NLB
      integer ILB(NLB)
      integer NUB
      integer IUB(NUB)
      logical LSLACKS
      double precision S_L(NLB)
      double precision S_U(NUB)
      double precision BNDS_L(NLB)
      double precision BNDS_U(NUB)
      double precision SIGMA_L(NLB)
      double precision SIGMA_U(NUB)
      double precision YPY(N)
      double precision REGU
      double precision WCORR(NIND)
      logical RESTO
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
      integer p_iwend, p_rwend, p_rhs, p_tmp, p_dscal
      integer idummy, i, k
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
C     There is no correction if there are no constraints
C
C      if( M.eq.0 .or. QCORRECT.eq.0 ) then
      if( M.eq.0 .or. NIND.eq.0 ) then
         call DCOPY(NIND, 0.d0, 0, WCORR, 1)
         if( LSLACKS ) then
            goto 1000
         else
            goto 9999
         endif
      endif

      if( abs(QQUASI).ge.6 ) then
         write(line,*) 'get_wcorr: QQUASI = ',QQUASI,' not implemented.'
         call C_OUT(2,0,1,line)
         IERR = 4
         goto 9999
      endif

      if( RESTO .or. (QQUASI.ne.0.and.QCG.eq.0) ) then
         p_rhs   = p_rwend
         p_rwend = p_rhs + M
      else
         p_rhs   = p_rwend
         p_rwend = p_rhs + N
      endif
      if( p_rwend.gt.LRW ) then
         IERR = 98
         goto 9999
      endif

      if( .not.RESTO ) then
C
         if( QQUASI.ne.0.and.QCG.eq.0 ) then
C
C     Compute rhs = Sigma_d * PY
C     
            call DCOPY(M, 0.d0, 0, RW(p_rhs+1), 1)
            do i = 1, NLB
               k = ILB(i)
               if( k.le.M ) then
                  RW(p_rhs+k) = SIGMA_L(i)*YPY(k)
               endif
            enddo
            do i = 1, NUB
               k = IUB(i)
               if( k.le.M ) then
                  RW(p_rhs+k) = RW(p_rhs+k) + SIGMA_U(i)*YPY(k)
               endif
            enddo
            if( REGU.gt.0d0 ) then
               call DAXPY(M, REGU, YPY, 1, RW(p_rhs+1), 1)
            endif
C     
C     For LSLACKS also take nonsatisfied slack equations into account:
C
            if( LSLACKS ) then
               do i = 1, NLB
                  k = ILB(i)
                  if( k.le.M ) then
                     RW(p_rhs+k) = RW(p_rhs+k) + 
     1                    SIGMA_L(i)*(X(k) - S_L(i) - BNDS_L(i))
                  endif
               enddo
               do i = 1, NUB
                  k = IUB(i)
                  if( k.le.M ) then
                     RW(p_rhs+k) = RW(p_rhs+k) +
     1                    SIGMA_U(i)*(X(k) + S_U(i) - BNDS_U(i))
                  endif
               enddo
               if( REGU.gt.0d0 ) then
                  p_tmp = p_rwend
                  p_rwend = p_tmp + M
                  if( p_rwend.gt.LRW ) then
                     IERR = 98
                     goto 9999
                  endif
                  call DCOPY(M, 0d0, 0, RW(p_tmp+1), 1)
                  do i = 1,NLB
                     k = ILB(i)
                     if( k.le.M ) then
                        RW(p_tmp+k) = X(k) - S_L(i) - BNDS_L(i)
                     endif
                  enddo
                  do i = 1, NUB
                     k = IUB(i)
                     if( k.le.M ) then
                        RW(p_tmp+k) = X(k) + S_U(i) - BNDS_U(i)
                     endif
                  enddo
                  call DAXPY(M, REGU, RW(p_tmp+1), 1, RW(p_rhs+1), 1)
                  p_rwend = p_tmp
               endif
            endif
C
         else                   !QQUASI = 0
C
C     Compute rhs = W * YPY
C
            call GET_HV(ITER, N, NIND, NFIX, IFIX, X, IVAR, NORIG,
     1           XORIG, NLB, ILB, NUB, IUB, S_L, S_U, CSCALE, M, LAM,
     1           YPY, RW(p_rhs+1),
     2           KCONSTR, LRS, RS, LIS, IS, LRW-p_rwend, RW(p_rwend+1),
     5           LIW-p_iwend, IW(p_iwend+1), IERR)
            if( IERR.lt.0 ) then
               write(line,*)
     1              'get_wcorr: Warning in get_hv, IERR = ',IERR
               call C_OUT(2,0,1,line)
            elseif( IERR.ne.0 ) then
               write(line,*)
     1              'get_wcorr: Error in get_hv, IERR = ',IERR
               call C_OUT(2,0,1,line)
               goto 9999
            endif
C
C     Compute rhs = rhs + Sigma * YPY
C
            do i = 1, NLB
               k = ILB(i)
               RW(p_rhs+k) = RW(p_rhs+k) + SIGMA_L(i) * YPY(k)
            enddo
            do i = 1, NUB
               k = IUB(i)
               RW(p_rhs+k) = RW(p_rhs+k) + SIGMA_U(i) * YPY(k)
            enddo
C
C     Add REGU * YPY to rhs (NOT FOR ICORRECT = 2!)
C
            if( REGU.gt.0d0 ) then
               if( QCORRECT.eq.2 ) then
                  call C_OUT(2,0,1,
     1 'get_wcorr: QCORRECT = 2 for full space version not implented.')
                  IERR = 4
                  goto 9999
               endif
               call DAXPY(N, REGU, YPY, 1, RW(p_rhs+1), 1)
            endif
C
            if( LSLACKS ) then
               call C_OUT(2,0,1,'get_wcorr: LSLACKS for exact???')
               IERR = 4
               goto 9999
            endif
C
         endif

      else
C
C     Compute scaling matrix for restoration phase
C
         p_dscal = p_rwend
         p_rwend = p_dscal + N
         if( p_rwend.gt.LRW ) then
            IERR = 98
            goto 9999
         endif
         call DCOPY(N, QDDMAX, 0, RW(p_dscal+1), 1)
         do i = 1, NLB
            k = ILB(i)
CFFF            RW(p_dscal+k) = dmax1(RW(p_dscal+k), 1d0/(S_L(i)**2))
         enddo
         do i = 1, NUB
            k = IUB(i)
CFFF            RW(p_dscal+k) = dmax1(RW(p_dscal+k), 1d0/(S_U(i)**2))
         enddo
         do i = 1, M
            RW(p_rhs+i) = RW(p_dscal+i)*YPY(i)
         enddo
         if( REGU.gt.0d0 ) then
            call DAXPY(M, REGU, YPY, 1, RW(p_rhs+1), 1)
         endif
         if( LSLACKS ) then
            call C_OUT(2,0,1,'LSLACKS not yet for RESTO in get_wcorr.')
            stop
         endif
      endif
C
C     Call CONSTR to get dependent part of BB
C
      call CONSTR(4, ITER, N, NIND, M, IVAR, NFIX, IFIX,
     1            NORIG, XORIG, CSCALE, RW(p_rhs+1), WCORR, idummy,
     3            idummy, KCONSTR(1), RS(KCONSTR(2)+1), KCONSTR(3),
     4            IS(KCONSTR(4)+1), LRW-p_rwend, RW(p_rwend+1),
     5            LIW-p_iwend, IW(p_iwend+1), IERR)
      if( IERR.lt.0 ) then
         write(line,*) 'get_wcorr: Warning in CONSTR, IERR = ',IERR
         call C_OUT(2,0,1,line)
      elseif( IERR.ne.0 ) then
         write(line,*) 'get_wcorr: Error in CONSTR, IERR = ',IERR
         call C_OUT(2,0,1,line)
         goto 9999
      endif
C
C     Correct sign
C
      call DSCAL(NIND, -1.d0, WCORR, 1)
C
C     Now also take care of part from independent variables
C
      if( .not.RESTO ) then

         if( QQUASI.ne.0.and.QCG.eq.0 ) then
            do i = 1, NLB
               k = ILB(i)
               if( k.gt.M ) then
                  WCORR(k-M) = WCORR(k-M) + SIGMA_L(i)*YPY(k)
               endif
            enddo
            do i = 1, NUB
               k = IUB(i)
               if( k.gt.M ) then
                  WCORR(k-M) = WCORR(k-M) + SIGMA_U(i)*YPY(k)
               endif
            enddo
            if( REGU.gt.0d0 ) then
               call DAXPY(NIND, REGU, YPY(M+1), 1, WCORR, 1)
            endif

         else                   !QQUASI = 0

            call DAXPY(NIND, 1d0, RW(p_rhs+M+1), 1, WCORR, 1)

         endif

      else
         do i = 1, NIND
            WCORR(i) = WCORR(i) + RW(p_dscal+M+i)*YPY(M+i)
         enddo
         if( REGU.gt.0d0 ) then
            call DAXPY(NIND, REGU, YPY(M+1), 1, WCORR, 1)
         endif
      endif
C
C     If LSLACKS there is also a part for independent variables:
C
 1000 continue
      if( LSLACKS ) then
         if( RESTO ) then
c            write(*,*) 'Don''t kow RESTO and LSLACKS in get_wcorr.'
            stop
         endif
         do i = 1, NLB
            k = ILB(i)
            if( k.gt.M ) then
               WCORR(k-M) = WCORR(k-M) + SIGMA_L(i)*
     1              (X(k) - S_L(i) - BNDS_L(i))
            endif
         enddo
         do i = 1, NUB
            k = IUB(i)
            if( k.gt.M ) then
               WCORR(k-M) = WCORR(k-M) + SIGMA_U(i)*
     1              (X(k) + S_U(i) - BNDS_U(i))
            endif
         enddo
         if( REGU.gt.0d0 ) then
            p_tmp = p_rwend
            p_rwend = p_tmp + NIND
            if( p_rwend.gt.LRW ) then
               IERR = 98
               goto 9999
            endif
            call DCOPY(NIND, 0d0, 0, RW(p_tmp+1), 1)
            do i = 1,NLB
               k = ILB(i)
               if( k.gt.M ) then
                  RW(p_tmp+k) = X(k) - S_L(i) - BNDS_L(i)
               endif
            enddo
            do i = 1, NUB
               k = IUB(i)
               if( k.gt.M ) then
                  RW(p_tmp+k) = X(k) + S_U(i) - BNDS_U(i)
               endif
            enddo
            call DAXPY(NIND, REGU, RW(p_tmp+1), 1, WCORR, 1)
            p_rwend = p_tmp
         endif
      endif
      p_rwend = p_rhs
C
C     That's it
C
 9999 continue
      return
      end
