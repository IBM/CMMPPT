C Copyright (C) 2002, Carnegie Mellon University and others.
C All Rights Reserved.
C This code is published under the Common Public License.
C*******************************************************************************
C
      subroutine GET_YPY(N, NIND, M, ITER, IVAR, NFIX, IFIX,
     1                   NORIG, XORIG, CSCALE, NLB, ILB, NUB, IUB,
     2                   S_L, S_U, C, LSLACKS, AC_HEURISTIC,
     3                   YPY, THETA1, THETA2, NEWBAS, CONDC,
     1                   KCONSTR, LRS, RS, LIS, IS,
     2                   LRW, RW, LIW, IW, IERR)
C
C*******************************************************************************
C
C    $Id: get_ypy.f,v 1.3 2002/11/24 21:41:32 andreasw Exp $
C
C-------------------------------------------------------------------------------
C                                 Title
C-------------------------------------------------------------------------------
C
CT    Compute range space step
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
CP   ITER      I    INT    iteration counter
CP   IVAR      I    INT    information about partitioning
CP                            i = 1..M      XORIG(IVAR(i)) dependent
CP                            i = (M+1)..N  XORIG(IVAR(i)) independent
CP                            Note: fixed variables do not occur in IVAR
CP   NFIX      I    INT    number of fixed variables
CP   IFIX      I    INT    specifies variables that are fixed by bounds:
CP                            i = 1..NORIG-N   XORIG(IFIX(i)) is fixed
CP   NORIG     I    INT    total number of all variables (incl. fixed vars)
CP   XORIG     I    DP     actual iterate
CP                            XORIG is ordered in ORIGINAL order (i.e. not
CP                            partitioned into independent and dependent
CP                            variables)
CP   CSCALE    I    DP     scaling factors for constraints
CP   NLB       I    INT    number of lower bounds (excluding fixed vars)
CP   ILB       I    INT    indices of lower bounds
CP                            (e.g. S_L(i) is slack for X(ILB(i)) )
CP   NUB       I    INT    number of upper bounds (excluding fixed vars)
CP   IUB       I    INT    indices of upper bounds
CP                            (e.g. S_U(i) is slack for X(IUB(i)) )
CP   S_L       I    DP     slacks to lower bounds
CP   S_U       I    DP     slacks to upper bounds
CP   C         I    DP     values of constraint function
CP   LSLACKS   I    LOG    =.true.: There are slacks that don't satisfy
CP                                  "slack equation"
CP                             (Not implemented for .true.!)
CP   AC_HEURISTIC I INT    =0: oldstyle traditional coordinate range step
CP                         =1: compute scaled Cauchy step
CP   YPY       O    DP     range space step (ordered like X)
CP   THETA1    O    DP     dogleg parameter (=1 <-> full Newton step)
CP   THETA2    O    DP     dogleg parameter (damping of Cauchy step)
CP   NEWBAS    O    LOG    =.true.: Basis has become pretty bad; get new one!
CP   CONDC     O    DP     estimated condition number of basis matrix C
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
CS    DDOT
CS    DNRM2
CS    DCOPY
CS    CHECK_BASIS
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
      include 'TIMER.INC'
C
C-------------------------------------------------------------------------------
C                             Parameter list
C-------------------------------------------------------------------------------
C
      integer N
      integer NIND
      integer M
      integer ITER
      integer IVAR(N)
      integer NFIX
      integer IFIX(NFIX)
      integer NORIG
      double precision XORIG(NORIG)
      double precision CSCALE(*)
      integer NLB
      integer ILB(NLB)
      integer NUB
      integer IUB(NUB)
      double precision S_L(NLB)
      double precision S_U(NUB)
      double precision C(M)
      logical LSLACKS
      integer AC_HEURISTIC
      double precision YPY(N)
      double precision THETA1
      double precision THETA2
      logical NEWBAS
      double precision CONDC
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
      logical CHECK_BASIS

      double precision den, alpha_cs, norm_cs, norm_c, times, timef
      integer i, k, idummy

      integer p_iwend, p_rwend, p_dd, p_ac

      character*80 line(3)

C     for orthogonal decomposition (hacker's edition)
      integer QORTHO
      parameter( QORTHO = 0 )

      integer p_a, p_rhs, p_irn, p_jcn, nnza
      double precision vin(2)

      integer NNZA_STORE
      save    NNZA_STORE
C
C*******************************************************************************
C
C                           Executable Statements
C
C*******************************************************************************
C
      call TIMER(times)
C
C     Need this part only for the orthogonal decomposition
C
      if( ITER.eq.-1 ) then
         NNZA_STORE = N
         goto 9999
      endif

      IERR = 0
C
      THETA1 = 1.d0
      THETA2 = 0.d0
C
C     There is nothing to do if there are no constraints
C
      if( M.eq.0 ) then
         call DCOPY(NIND, 0d0, 0, YPY, 1)
         goto 9999
      endif

      p_iwend = 0
      p_rwend = 0

C
C     This first part is a very preliminary and inefficient implementation
C     for the orthogonal step decomposition...
C
      if( QORTHO.eq.1 ) then

         p_a     = p_rwend
         p_rhs   = p_a + NNZA_STORE + N
         p_rwend = p_rhs + M + N
         if( p_rwend.gt.LRW ) then
            IERR = 98
            goto 9999
         endif
         p_irn   = p_iwend
         p_jcn   = p_irn + NNZA_STORE + N
         p_iwend = p_jcn + NNZA_STORE + N
         if( p_iwend.gt.LIW ) then
            IERR = 99
            goto 9999
         endif
C
C     First fill identity on upper left block
C
         call DCOPY(N, 1.d0, 0, RW(p_a+1), 1)
         do i = 1, N
            IW(p_irn+i) = i
            IW(p_jcn+i) = i
         enddo
C
C     Now fill Jacobian
C
         call CONSTR(10, ITER, N, NIND, M, IVAR, NFIX, IFIX,
     1        NORIG, XORIG, CSCALE, vin,
     2        RW(p_a+N+1), IW(p_jcn+N+1), IW(p_irn+N+1),
     3        KCONSTR(1), RS(KCONSTR(2)+1), KCONSTR(3),
     4        IS(KCONSTR(4)+1), LRW-p_rwend, RW(p_rwend+1),
     5        LIW-p_iwend, IW(p_iwend+1), IERR)
         if( IERR.ne.0 ) then
            write(line,*) 'get_ypy: CONSTR-10 returns IERR = ',IERR
            call C_OUT(2,0,1,line)
            goto 9999
         endif
         nnza = int(vin(1)) + N
         do i = 1, int(vin(1))
            IW(p_jcn+N+i) = IW(p_jcn+N+i) + N
         enddo
C
C     Now set the right hand side
C
         call DCOPY(N, 0.d0, 0, RW(p_rhs+1), 1)
         call DCOPY(M, C, 1, RW(p_rhs+N+1), 1)
         call DSCAL(M, -1.d0, RW(p_rhs+N+1), 1)
C
C     Now solve the system
C
         call RES_MA27(N+M, nnza, RW(p_a+1), IW(p_irn+1), IW(p_jcn+1),
     1        RW(p_rhs+1), LRW-p_rwend, RW(p_rwend+1), LIW-p_iwend,
     2        IW(p_iwend+1), IERR)
         if( IERR.ne.0 ) then
            IERR = 544
            write(line,*) 'get_ypy: RES_MA27 returns IERR = ',IERR
            call C_OUT(2,0,1,line)
            goto 9999
         endif
C
C     Retrieve YPY step
C
         call DCOPY(N, RW(p_rhs+1), 1, YPY, 1)
         p_rwend = p_a
         p_iwend = p_irn

         NEWBAS = .false.
         CONDC = 1.d0

         goto 9999
      endif

C
C     If AC_HEURISTIC = 0 compute normal coordinate range space step
C
      if( AC_HEURISTIC.eq.0 ) then
C
C     Call CONSTR to get -PY and dependent part of BB
C
         call CONSTR(3, ITER, N, NIND, M, IVAR, NFIX, IFIX,
     1        NORIG, XORIG, CSCALE, C, YPY, 1, idummy,
     3        KCONSTR(1), RS(KCONSTR(2)+1), KCONSTR(3),
     4        IS(KCONSTR(4)+1), LRW-p_rwend, RW(p_rwend+1),
     5        LIW-p_iwend, IW(p_iwend+1), IERR)
         if( IERR.lt.0 ) then
            write(line,*) 'get_ypy: Warning in CONSTR-3, IERR = ',IERR
            call C_OUT(2,0,1,line)
            IERR = 0
         elseif( IERR.ne.0 ) then
            write(line,*) 'get_ypy: Error in CONSTR-3, IERR = ',IERR
            call C_OUT(2,0,1,line)
            goto 9999
         endif
C
C     Check condition number of basis
C
         NEWBAS = CHECK_BASIS(ITER, M, C, YPY, CONDC)
         if( NEWBAS ) then
            goto 9999
         endif
C
C     PY has wrong sign
C
         call DSCAL(M, -1.d0, YPY, 1)
         if( NIND.gt.0 ) then
            call DCOPY(NIND, 0d0, 0, YPY(M+1), 1)
         endif
C
      else
C
C     Compute spaced Cauchy step
C
         if( LSLACKS ) then
            call C_OUT(2,0,1,
     1           'get_ypy: Heuristic not implemented for QMOVEINIT = 0')
            IERR = 4
            goto 9999
         endif
C
C     reserve memory
C
         p_dd     = p_rwend
         p_rwend  = p_dd     + N
         if( p_rwend.gt.LRW ) then
            IERR = 98
            goto 9999
         endif
C
C     Determine scaling matrix (store square of the inverse!)
C
         call DCOPY(N, QDDMAX, 0, RW(p_dd+1), 1)
         do i = 1, NLB
            k = ILB(i)
            RW(p_dd+k) = dmax1(1.d0/(S_L(i)**2),RW(p_dd+k))
         enddo
         do i = 1, NUB
            k = IUB(i)
            RW(p_dd+k) = dmax1(1.d0/(S_U(i)**2),RW(p_dd+k))
         enddo
         if( QCNR.gt.0 .and. QPRINT.gt.4 ) then
            write(line,776) ITER
 776        format(/,'  Information about scaling matrix in ITER ',i6,/)
            call C_OUT(1,5,3,line)
            do i = 1, N
               WRITE(line,775) i, RW(p_dd+i)
 775           format(' DD(',i4,') = ',d25.15)
               call C_OUT(1,5,1,line)
            enddo
         endif
C
C     Compute tmp = A*c
C
         p_ac    = p_rwend
         p_rwend = p_ac + N
         if( p_rwend.gt.LRW ) then
            IERR = 98
            goto 9999
         endif
         call CONSTR(8, ITER, N, NIND, M, IVAR, NFIX, IFIX,
     1        NORIG, XORIG, CSCALE, C, RW(p_ac+1),
     2        idummy, idummy,
     3        KCONSTR(1), RS(KCONSTR(2)+1), KCONSTR(3),
     4        IS(KCONSTR(4)+1), LRW-p_rwend, RW(p_rwend+1),
     5        LIW-p_iwend, IW(p_iwend+1), IERR)
         if( IERR.lt.0 ) then
            write(line,*) 'get_ypy: Warning in CONSTR-8, IERR = ',IERR
            call C_OUT(2,0,1,line)
         elseif( IERR.ne.0 ) then
            write(line,*) 'get_ypy: Error in CONSTR-8, IERR = ',IERR
            call C_OUT(2,0,1,line)
            goto 9999
         endif
C
C     Compute cauchy = D^2*A*c
C
         do i = 1, N
            YPY(i) = RW(p_ac+i)/RW(p_dd+i)
         enddo
C
C     Get D*A*c squared norm
C
         norm_cs = DDOT(N, RW(p_ac+1), 1, YPY, 1)
CWEG
         norm_c = DNRM2(M, C, 1) !maybe from somewhere else?
         if( dsqrt(norm_cs)/norm_c.lt.QTOL ) then
            write(line,*) 'get_ypy: ||D A C || = ',dsqrt(norm_cs)
            CALL C_OUT(2,0,1,line)
            write(line,*) '         Problem locally infeasible?'
            CALL C_OUT(2,0,1,line)
            IERR = 12
            goto 9999
         endif
C         write(*,*) 'norm_cs = ',norm_cs
C
C     Get A'*cauchy (store in ac)
C
         call CONSTR(9, ITER, N, NIND, M, IVAR, NFIX, IFIX,
     1        NORIG, XORIG, CSCALE, YPY, RW(p_ac+1), idummy, idummy,
     3        KCONSTR(1), RS(KCONSTR(2)+1), KCONSTR(3),
     4        IS(KCONSTR(4)+1), LRW-p_rwend, RW(p_rwend+1),
     5        LIW-p_iwend, IW(p_iwend+1), IERR)
         if( IERR.lt.0 ) then
            write(line,*) 'get_ypy: Warning in CONSTR-9, IERR = ',IERR
            call C_OUT(2,0,1,line)
         elseif( IERR.ne.0 ) then
            write(line,*) 'get_ypy: Error in CONSTR-9, IERR = ',IERR
            call C_OUT(2,0,1,line)
            goto 9999
         endif
C
C     Get A'*D^2*A*c squared norm
C
         den = DDOT(M, RW(p_ac+1), 1, RW(p_ac+1), 1)
C
C     Get final Cauchy step
C
         alpha_cs = norm_cs/den
         call DSCAL( N, -1.d0*alpha_cs, YPY, 1)
C
         p_rwend = p_dd
C
         THETA1 = 0.d0
C     THETA2 is - c^T * A^T * (Cauchy Step)
         THETA2 = alpha_cs*norm_cs

      endif
C
C     That's it
C
 9999 continue
      call TIMER(timef)
      TIME_YPY = TIME_YPY + timef - times
      return
      end
