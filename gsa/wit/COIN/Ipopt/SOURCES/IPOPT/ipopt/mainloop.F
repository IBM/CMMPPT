C Copyright (C) 2002, Carnegie Mellon University and others.
C All Rights Reserved.
C This code is published under the Common Public License.
C*******************************************************************************
C
      subroutine MAINLOOP(ITER, N, NIND, M, NORIG, XORIG,
     1     IVAR, X, NFIX, IFIX, NLB, ILB, NUB,
     1     IUB, BNDS_L, BNDS_U, NZORIG,
     1     V_L, V_U, LAM, ERR, KCONSTR, LRS,
     1     LRS_END, RS, LIS, LIS_END, IS,
     1     LRW, RW, LIW, IW, IERR)

C
C*******************************************************************************
C
C    $Id: mainloop.F,v 1.5 2003/04/11 02:55:24 andreasw Exp $
C
C-------------------------------------------------------------------------------
C                                 Title
C-------------------------------------------------------------------------------
C
CT    Main (outer) loop of algorithm
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
CP   ITER     I/O   INT    iteration counter (if -1, initialize)
CP                         I: is expected to be 0 at first call for algorithm
CP   N         I    INT    number of variables (without fixed)
CP   NIND      I    INT    number of independent variables
CP   M         I    INT    number of constraints
CP   NORIG     I    INT    total number of variables (incl. fixed vars)
CP   XORIG    I/O   INT    I: user provided initial point (incl fixed vars)
CP                         O: solution (hopefully)
CP                            (original order as in problem statement)
CP   IVAR     I/O   INT    information about partitioning
CP                            i = 1..M      XORIG(IVAR(i)) dependent
CP                            i = (M+1)..N  XORIG(IVAR(i)) independent
CP                            Note: fixed variables do not occur in IVAR
CP                            X(i) corresponds to XORIG(IVAR(i))
CP                         I: no partition done, only sort out fixed vars
CP   X        I/O   INT    as XORIG but without fixed vars
CP   NFIX      I    INT    number of fixed variables
CP   IFIX      I    INT    specifies variables that are fixed by bounds:
CP                            i = 1..NORIG-N   XORIG(IFIX(i)) is fixed
CP                            (assumed to be in increasing order)
CP   NLB       I    INT    number of lower bounds (excluding fixed vars)
CP   ILB       I    INT    indices of lower bounds
CP                            (e.g. BNDS_L(i) is bound for X(ILB(i)) )
CP   NUB       I    INT    number of upper bounds (excluding fixed vars)
CP   IUB       I    INT    indices of upper bounds
CP                            (e.g. BNDS_U(i) is bound for X(IUB(i)) )
CP   BNDS_L   I/O   DP     values of lower bounds
CP   BNDS_U   I/O   DP     values of upper bounds
CP   NZORIG    I    INT    number of nonzeros in Jacobian of constraints
CP                            (including rows for FIXED variables!)
CP   V_L       O    DP     multipliers corresponding to the lower bounds
CP   V_U       O    DP     multipliers corresponding to the upper bounds
CP   LAM       O    DP     multipliers corresponding to the equality
CP                            constraints
CP   ERR       O    DP     actual KKT-error of barrier problem
CP   KCONSTR   I    INT    KCONSTR(1): LRS for CONSTR
CP                         KCONSTR(2): P_LRS for CONSTR
CP                         KCONSTR(3): LIS for CONSTR
CP                         KCONSTR(4): P_LIS for CONSTR
CP                         KCONSTR(5): LRW for CONSTR
CP                         KCONSTR(6): LIW for CONSTR
CP   LRS       I    INT    total length of RS
CP   LRS_END  I/O   INT    last used reserved entry in RS
CP   RS       I/O   DP     DP storage space (all!)
CP   LIS       I    INT    total length of IS
CP   LRS_END  I/O   INT    last used reserved entry in IS
CP   IS       I/O   INT    INT storage space (all!)
CP   LRW       I    INT    length of RW
CP   RW       I/O   DP     can be used as DP work space but content will be
CP                            changed between calls
CP   LIW       I    INT    length of IW
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
CS    DCOPY
CS    INITPOINT
CS    PARTITON
CS    REORDER_IB
CS    REORDER_X
CS    GET_F
CS    GET_G
CS    GET_C
CS    GET_YPY
CS    GET_RG
CS    GET_RV
CS    ERROR
CS    UPDATE_MU
CS    SOLVE_BARRIER
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
      integer ITER
      integer N
      integer NIND
      integer M
      integer NORIG
      double precision XORIG(NORIG)
      integer IVAR(N)
      double precision X(N)
      integer NFIX
      integer IFIX(NFIX)
      integer NLB
      integer ILB(NLB)
      integer NUB
      integer IUB(NUB)
      double precision BNDS_L(NLB)
      double precision BNDS_U(NUB)
      integer NZORIG
      double precision V_L(NLB)
      double precision V_U(NUB)
      double precision LAM(M)
      double precision ERR
      integer KCONSTR(6)
      integer LRS
      integer LRS_END
      double precision RS(LRS)
      integer LIS
      integer LIS_END
      integer IS(LIS)
      integer LRW
      double precision RW(LRW)
      integer LIW
      integer IW(LIW)
      integer IERR
C
C-------------------------------------------------------------------------------
C                            Local variables
C-------------------------------------------------------------------------------
C
      integer P_SL, P_SU, P_G, P_C, P_YPY, P_RG, P_RV
      save    P_SL, P_SU, P_G, P_C, P_YPY, P_RG, P_RV
      integer P_SIGMAL, P_SIGMAU, P_B, P_W, P_WCORR, P_RGOLD
      save    P_SIGMAL, P_SIGMAU, P_B, P_W, P_WCORR, P_RGOLD
      integer P_RGB, P_PZ, P_ZPZ, P_DX, P_DVL, P_DVU, P_LAMOLD
      save    P_RGB, P_PZ, P_ZPZ, P_DX, P_DVL, P_DVU, P_LAMOLD
      integer P_GOLD, P_CSCALE
      save    P_GOLD, P_CSCALE
      double precision MU, ERR_BAR, F
      save             MU, ERR_BAR, F
      double precision THETA1, THETA2, CONDC
      save             THETA1, THETA2, CONDC
      logical FIRSTMU, LSLACKS, NEWBAS, RESTO
      save    FIRSTMU, LSLACKS, NEWBAS, RESTO
      double precision ERR_DINF, ERR_CNSTR, ERR_CMPL
      save             ERR_DINF, ERR_CNSTR, ERR_CMPL
      logical INIT_LAM
      save    INIT_LAM

      integer p_rwend, p_iwend
      integer p_ivarold, p_ivarnew1, p_ib, p_xtmp
      integer i, icmax, blen
      double precision err_dinf_us
      logical small_x_stop, ex

      character*1 c_err
      character*90 line(3)

      integer IDAMAX

      integer fevals, cevals
      common /EVALS/ fevals, cevals
C
C*******************************************************************************
C
C                           Executable Statements
C
C*******************************************************************************
C

C
C     Initialize pointer to stored variables
C
      if( ITER.eq.-1 ) then
         P_SL     = LRS_END
         P_SU     = P_SL     + NLB
         P_G      = P_SU     + NUB
         P_C      = P_G      + N
         P_YPY    = P_C      + M
         P_RG     = P_YPY    + N
         P_RV     = P_RG     + NIND
         P_SIGMAL = P_RV     + NIND
         P_SIGMAU = P_SIGMAL + NLB
         if( QQUASI.ne.0 .or. QFULL.eq.0 ) then
            if( QQUASI.lt.0.and.QQUASI.gt.-6 ) then
               P_GOLD = P_SIGMAU + NUB
               P_B    = P_GOLD   + N
            else
               P_GOLD = 0
               P_B    = P_SIGMAU + NUB
            endif
C            if( QFULL.eq.0 .and. QCG.ne.0 ) then
C               P_W      = 0     ! don't need W for CG
C               P_WCORR  = P_B      + (NIND*(NIND+1))/2
C            else
            if( abs(QQUASI).ge.6 ) then
               P_W      = P_B + 2+2*N+QLMLEN*(1+4*N+2*QLMLEN)
               P_WCORR  = P_W
            else
               P_W      = P_B      + (NIND*(NIND+1))/2
               P_WCORR  = P_W      + (NIND*(NIND+1))/2
            endif
C            endif
         else
            P_B      = 0
            P_W      = 0
            P_WCORR  = P_SIGMAU + NUB
         endif
         P_RGOLD  = P_WCORR  + NIND
         P_RGB    = P_RGOLD  + NIND
         P_PZ     = P_RGB    + NIND
         P_ZPZ    = P_PZ     + NIND
         P_DX     = P_ZPZ    + M
         P_DVL    = P_DX     + N
         P_DVU    = P_DVL    + NLB
         LRS_END  = P_DVU    + NUB
         if( QMOVEINIT.eq.1 .or. QMERIT.ne.3 ) then
            P_LAMOLD = LRS_END
            LRS_END  = P_LAMOLD + M
         else
            call C_OUT(2,0,1,
     1           'mainloop: QMOVEINIT = 0 no longer supported')
            IERR = 4
            goto 9999
C            P_LAMOLD = P_LAM    + M+N ! need space for multipliers in additional
C            LRS_END  = P_LAMOLD + M+N ! slack equations
         endif
C
         if( QSCALE.eq.2 ) then
            P_CSCALE  = LRS_END
            LRS_END   = P_CSCALE + M
         else
            P_CSCALE  = LRS_END
            LRS_END   = P_CSCALE + 1
         endif
C
C     Initialize INIT_LAM (for get_step_full)
C
         if( QLAMBDA.eq.2 .and. QINIT.ne.0 ) then
            INIT_LAM = .true.
         else
            INIT_LAM = .false.
         endif

         goto 9999
      endif

      p_rwend = 0
      p_iwend = 0
C
C     Make sure, no STOP file exists
C
      inquire(file='STOP', exist=ex)
      if( ex ) then
         open(80,file='STOP',status='old')
         close(80,status='delete')
      endif
C
C     initialize flag for barrier parameter update
C
      FIRSTMU = .true.
C
C     For filter: We start not in restoration phase
C
CWEG
C      RESTO = .true.
      RESTO = .false.
      small_x_stop = .false.
C
C     Compute initial point (primal, slack and dual variables) and
C        determine initial barrier parameter
C     Also, compute values of scaling factors for F and C if requested
C
      call INITPOINT(N, X, NORIG, XORIG, IVAR, M, NIND,
     1     NLB, ILB, BNDS_L, V_L, RS(P_SL+1), NUB, IUB, BNDS_U, V_U,
     2     RS(P_SU+1), MU, RS(P_CSCALE+1), LSLACKS,
     2     KCONSTR, LRS, RS, LIS, IS, LRW-p_rwend, RW(p_rwend+1),
     4     LIW-p_iwend, IW(p_iwend+1), IERR)
      if( IERR.ne.0 ) then
         write(line,*) 'mainloop: INITPOINT returns IERR = ', IERR
         call C_OUT(2,0,1,line)
         goto 9999
      endif
C
C     Store old partition IVAR in IVAROLD
C
      p_ivarold = p_iwend
      p_iwend   = p_ivarold + N
      if( p_iwend.gt.LIW ) then
         IERR = 99
         goto 9999
      endif
      do i = 1, N
         IW(p_ivarold+i) = IVAR(i)
      enddo
C
C     Determine initial partition IVAR
C     (only if necessary for non-full space approach and qN udpates)
C
C      if( QQUASI.ne.0 .or. QFULL.eq.0 .or. QLAMBDA.eq.1 .or.
C     1     abs(QERROR).eq.1 .or. QLAMBDA.eq.0 ) then
      if( QFULL.eq.0 .or. QLAMBDA.eq.1 .or.
     1     (QQUASI.ne.0.and.abs(QQUASI).lt.6) .or.
     1     abs(QERROR).eq.1 .or. QLAMBDA.eq.0 ) then
         call PARTITION(QSELBAS, ITER, N, NIND, M, NFIX, IFIX, IVAR,
     1        NORIG, XORIG, RS(P_CSCALE+1), KCONSTR, LRS, RS, LIS, IS,
     3        LRW-p_rwend, RW(p_rwend+1),
     4        LIW-p_iwend, IW(p_iwend+1), IERR)
         if( IERR.gt.0 ) then
            write(line,*)
     1           'mainloop: Error: partition ends with IERR = ',IERR
            goto 9999
            call C_OUT(2,0,1,line)
         elseif( IERR.ne.0 ) then
            write(line,*)
     1           'mainloop: Warning: partition ends with IERR = ',IERR
            call C_OUT(2,0,1,line)
         endif
C
C     Reorder bound pointers ILB, IUB
C
         p_ivarnew1 = p_iwend
         p_ib       = p_ivarnew1 + NORIG
         p_iwend    = p_ib       + N
         if( p_iwend.gt.LIW ) then
            IERR = 99
            goto 9999
         endif
         call REORDER_IB(N, NORIG, NLB, ILB, NUB, IUB,
     1        IW(p_ivarold+1), IVAR, IW(p_ivarnew1+1),
     2        IW(p_ib+1))
         p_iwend = p_ivarnew1
C     
C     Reorder X
C
         p_xtmp  = p_rwend
         p_rwend = p_xtmp  + NORIG
         if( p_rwend.gt.LRW ) then
            IERR = 98
            goto 9999
         endif
         call REORDER_X(N, NORIG, X, IW(p_ivarold+1), IVAR,
     1               RW(p_xtmp+1))
         p_rwend = p_xtmp
      endif
      p_iwend = p_ivarold
C
C     Compute value of objective function F
C
      call GET_F(N, X, IVAR, NORIG, XORIG, F)
C
C     Compute gradient of objective function G
C
      call GET_G(N, X, IVAR, NORIG, XORIG, RS(P_G+1),
     1           LRW-p_rwend, RW(p_rwend+1), IERR)
      if( IERR.gt.0 ) then
         write(line,*)
     1        'mainloop: Error: get_g ends with IERR = ',IERR
         call C_OUT(2,0,1,line)
         goto 9999
      elseif( IERR.ne.0 ) then
         write(line,*)
     1        'mainloop: Warning: get_g ends with IERR = ',IERR
         call C_OUT(2,0,1,line)
      endif
C
C     Compute values of constraints C
C
      call GET_C(ITER, N, NIND, X, IVAR, NORIG, XORIG, M,
     1     RS(P_CSCALE+1), RS(P_C+1), KCONSTR, LRS, RS, LIS, IS,
     2     LRW-p_rwend, RW(p_rwend+1),
     4     LIW-p_iwend, IW(p_iwend+1), IERR)
      if( IERR.gt.0 ) then
         write(line,*)
     1        'mainloop: Error: get_c returns IERR = ',IERR
         call C_OUT(2,0,1,line)
         goto 9999
      elseif( IERR.ne.0 ) then
         write(line,*)
     1        'mainloop: Warning: get_c returns IERR = ',IERR
         call C_OUT(2,0,1,line)
      endif
C
C     Obtain first range space step PY
C
      if( QFULL.eq.0 .or. (QQUASI.ne.0.and.abs(QQUASI).lt.6)
     1     .or. QLAMBDA.eq.0 ) then
         call GET_YPY(N, NIND, M, ITER, IVAR, NFIX, IFIX,
     1        NORIG, XORIG, RS(P_CSCALE+1), NLB, ILB, NUB, IUB,
     2        RS(P_SL+1), RS(P_SU+1), RS(P_C+1), LSLACKS,
     3        0, RS(P_YPY+1), THETA1, THETA2,
     4        NEWBAS, CONDC,
     1        KCONSTR, LRS, RS, LIS, IS,
     2        LRW-p_rwend, RW(p_rwend+1),
     4        LIW-p_iwend, IW(p_iwend+1), IERR)
         if( IERR.gt.0 ) then
            write(line,*)
     1           'mainloop: Error: get_ypy returns IERR = ',IERR
            call C_OUT(2,0,1,line)
            goto 9999
         elseif( IERR.ne.0 ) then
            write(line,*)
     1           'mainloop: Warning: get_ypy returns IERR = ',IERR
            call C_OUT(2,0,1,line)
         endif
      else
         NEWBAS = .false.
CTODO This is bad, if we don't allocate memory for YPY!
         call DCOPY(N, 0d0, 0, RS(P_YPY+1), 1)
      endif
C
C     Compute reduced gradient of objective function RG
C
      if( QFULL.eq.0 .or. (QQUASI.ne.0.and.abs(QQUASI).lt.6)
     1     .or. abs(QERROR).eq.1 ) then
         call GET_RG(N, NIND, M, ITER, IVAR, NFIX, IFIX,
     1        NORIG, XORIG, RS(P_CSCALE+1), RS(P_G+1), RS(P_RG+1),
     1        KCONSTR, LRS, RS, LIS, IS,
     2        LRW-p_rwend, RW(p_rwend+1),
     4        LIW-p_iwend, IW(p_iwend+1), IERR)
         if( IERR.gt.0 ) then
            write(line,*)
     1           'mainloop: Error: get_rg ends with IERR = ',IERR
            call C_OUT(2,0,1,line)
            goto 9999
         elseif( IERR.ne.0 ) then
            write(line,*)
     1           'mainloop: Warning: get_rg ends with IERR = ',IERR
            call C_OUT(2,0,1,line)
         endif
      endif
C
C     Compute "reduced" dual variables RV
C
      if( QFULL.eq.0 .or. abs(QERROR).eq.1 ) then
         call GET_RV(N, NIND, M, ITER, IVAR, NFIX, IFIX,
     1        NORIG, XORIG, RS(P_CSCALE+1), NLB, ILB, NUB, IUB,
     1        V_L, V_U, RS(P_RV+1), KCONSTR, LRS, RS, LIS, IS,
     2        LRW-p_rwend, RW(p_rwend+1),
     4        LIW-p_iwend, IW(p_iwend+1), IERR)
         if( IERR.gt.0 ) then
            write(line,*)
     1           'mainloop: Error: get_rv ends with IERR = ',IERR
            goto 9999
            call C_OUT(2,0,1,line)
         elseif( IERR.ne.0 ) then
            write(line,*)
     1           'mainloop: Warning: get_rv ends with IERR = ',IERR
            call C_OUT(2,0,1,line)
         endif
      endif
C
C     For QLAMBDA = 1: get coordinate multipliers
C
      if( QLAMBDA.eq.1 ) then
         call GET_LAMBDA(N, NIND, M, ITER, IVAR, NFIX, IFIX,
     1        NORIG, XORIG, RS(P_CSCALE+1), RS(P_G+1), NLB, ILB, NUB,
     1        IUB, V_L, V_U, LSLACKS, LAM, KCONSTR, LRS, RS, LIS, IS,
     1        LRW-p_rwend, RW(p_rwend+1), LIW-p_iwend, IW(p_iwend+1),
     1        IERR)
         if( IERR.gt.0 ) then
            write(line,*) 
     1           'mainloop: get_lambda returns IERR = ',IERR
            call C_OUT(2,0,1,line)
            goto 9999
         elseif( IERR.lt.0 ) then
            write(line,*)
     1           'mainloop: get_lambda returns IERR = ',IERR
            call C_OUT(2,0,1,line)
            IERR = 0
         endif
      elseif( QLAMBDA.ne.0 .and. QINIT.ne.0 ) then
         call DCOPY(M, 0d0, 0, LAM, 1)
         if( LSLACKS .and. QMERIT.eq.3 ) then
C            call DCOPY(N, 0d0, 0, RS(P_LAM+M+1), 1)
         endif
      endif
C
C     Initialize old copy of Lagrangian multipliers
C
      if( QLAMBDA.ne.0 ) then
         call DCOPY(M, 0d0, 0, RS(P_LAMOLD+1), 1)
         if( LSLACKS .and. QMERIT.eq.3 ) then
            call DCOPY(N, 0d0, 0, RS(P_LAMOLD+M+1), 1)
         endif
      endif
C
C     Compute primal dual Hessian for barrier part SIGMA_L, SIGMA_U
C
      call GET_SIGMA(NLB, NUB, RS(P_SL+1), RS(P_SU+1), V_L,
     1     V_U, RS(P_SIGMAL+1), RS(P_SIGMAU+1))
C

C ------------------------------------------------------------------------------
C     Start of main loop !
C ------------------------------------------------------------------------------
 100  continue
C
C     Compute Error for original NLP (MU = 0)
C
      call ERROR(N, NIND, M, X, RS(P_G+1), RS(P_RG+1), RS(P_RV+1),
     1     RS(P_C+1), RS(P_CSCALE+1), NLB, ILB, NUB, IUB, BNDS_L,
     1     BNDS_U, RS(P_SL+1), RS(P_SU+1), V_L,
     1     V_U, RS(P_SIGMAL+1), RS(P_SIGMAU+1),
     1     LAM, 0.d0, LSLACKS, ERR,
     1     c_err, ERR_DINF, ERR_CNSTR, ERR_CMPL, err_dinf_us,
     1     ITER, IVAR, NFIX, IFIX, NORIG, XORIG, RS(P_DX+1),
     1     KCONSTR, LRS, RS, LIS, IS, LRW-p_rwend, RW(p_rwend+1),
     1     LIW-p_iwend, IW(p_iwend+1), IERR)
      if( IERR.gt.0 ) then
         write(line,*)
     1      'mainloop: Error: Error ends with IERR = ',IERR
         call C_OUT(2,0,1,line)
         goto 9000
      elseif( IERR.ne.0 ) then
         write(line,*)
     1      'mainloop: Warning:Error ends with IERR = ',IERR
         call C_OUT(2,0,1,line)
      endif
C
C     Check if done
C
      if( ERR.le.QTOL .or. (err_bar.le.QTOL .and.small_x_stop) ) then
         if( ERR.gt.QTOL ) IERR = 9
         goto 9000
      endif
C
C     Update barrier parameter and determine tolerance for barrier problem
C
      call UPDATE_MU(FIRSTMU, MU, err_bar)
C
C     Solve the barrier problem (uff...)
C
      small_x_stop = .false.
      call SOLVE_BARRIER(ITER, N, NIND, M, NORIG, XORIG, RS(P_CSCALE+1),
     1     IVAR, NFIX, IFIX, NLB, ILB, NUB,
     1     IUB, BNDS_L, BNDS_U, MU, X, LSLACKS,
     1     RS(P_SL+1), RS(P_SU+1), V_L, V_U, F,
     1     RS(P_G+1), RS(P_C+1), ERR_BAR, ERR, ERR_DINF, ERR_CNSTR,
     1     ERR_CMPL, RS(P_RV+1), RS(P_SIGMAL+1),
     1     RS(P_SIGMAU+1), RS(P_B+1), RS(P_W+1),
     1     RS(P_WCORR+1), RS(P_RG+1), RS(P_RGOLD+1),
     1     RS(P_RGB+1), RS(P_YPY+1), THETA1, THETA2,
     1     NEWBAS, CONDC, RS(P_PZ+1), RS(P_ZPZ+1),
     1     LAM, RS(P_LAMOLD+1), RS(P_DX+1),
     1     RS(P_DVL+1), RS(P_DVU+1), RS(P_GOLD+1), RESTO, INIT_LAM,
     1     KCONSTR, LRS, LRS_END, RS, LIS, LIS_END, IS,
     1     LRW-p_rwend, RW(p_rwend+1),
     1     LIW-p_iwend, IW(p_iwend+1), IERR)
      if( IERR.gt.0 ) then
         write(line,*)
     1      'mainloop: Error: solve_barrier ends with IERR = ',IERR
         call C_OUT(2,0,1,line)
         if( IERR.eq.535 ) then
            IERR = 1
         endif
         goto 9000
      elseif( IERR.eq.-2 ) then
         small_x_stop = .true.
         IERR = 0
      elseif( IERR.ne.0 ) then
         write(line,*)
     1      'mainloop: Warning: solve_barrier ends with IERR = ',IERR
         call C_OUT(2,0,1,line)
         IERR = 0
      endif
C ------------------------------------------------------------------------------
C     End of main loop
C ------------------------------------------------------------------------------
      goto 100

 9000 continue
C
C     If desired, write current quasi-Newton estimate to file BWARM.DAT
C
      if( QBWARMSTART.eq.1 .and. QQUASI.ne.0 ) then
         if( abs(QQUASI).eq.6 ) then
            blen = 2+2*N+QLMLEN*(1+4*N+2*QLMLEN)
         else
            blen = (NIND*(NIND+1))/2
         endif
         call C_OUT(2,1,1,
     1  'mainloop: Writing current quasi-Newton estimate to BWARM.DAT.')
         open(80,file='BWARM.DAT',status='unknown',err=9008)
         write(80,'(i16)',err=9009) blen
         do i = 1, blen
            write(80,'(d23.16)',err=9009) RS(P_B+i)
         enddo
         close(80)
      endif
C
C     Undo the scaling of objetcive function
C
      F = F/QFSCALE

      write(line,9010) ITER
 9010 format(/,'Number of iterations taken .............',i23)
      call C_OUT(2,0,2,line)

      write(line,9020) F
 9020 format('Final value of objective function is....',d23.16)
      call C_OUT(2,0,1,line)

      if( M.gt.0 ) then
         icmax = IDAMAX(M, RS(P_C+1), 1)
         write(line,9021) dabs(RS(P_C+icmax))
      else
         write(line,9021) 0.d0
      endif
 9021 format('Final maximal constraint violation is...',d23.16)
      call C_OUT(2,0,1,line)
C
C     Include complementarity in "dual infeasibility"
C
      do i = 1, NLB
         err_dinf_us = dmax1( err_dinf_us, RS(P_SL+i)*V_L(i) )
      enddo
      do i = 1, NUB
         err_dinf_us = dmax1( err_dinf_us, RS(P_SU+i)*V_U(i) )
      enddo
      write(line,9022) err_dinf_us
 9022 format('Final value for dual infeasibility is...',d23.16)
      call C_OUT(2,0,1,line)

      write(line,9026) fevals
 9026 format(/,'The objective function was evaluated',i7,' times.')
      call C_OUT(2,0,2,line)

      write(line,9027) cevals
 9027 format('The constraints were evaluated',i13,' times.')
      call C_OUT(2,0,1,line)

      if( QPRINT.ge.1 ) then
        write(line,9030)
 9030   format(/,'  Variables at Solution(?):',/)
        call C_OUT(1,0,3,line)
        do i = 1, NORIG
           write(line,9040) i,XORIG(i)
 9040      format(' X(',i5,') = ',d25.15)
           call C_OUT(1,0,1,line)
        enddo
        write(line,9050)
 9050   format(/,'  Multipliers for lower Bounds:',/)
        call C_OUT(1,0,3,line)
        do i = 1, NLB
           write(line,9060) IVAR(ILB(i)),V_L(i)
 9060      format(' V_L(',i5,') = ',d25.15)
           call C_OUT(1,0,1,line)
        enddo
        write(line,9070)
 9070   format(/,'  Multipliers for upper Bounds:',/)
        call C_OUT(1,0,3,line)
        do i = 1, NUB
           write(line,9080) IVAR(IUB(i)),V_U(i)
 9080      format(' V_U(',i5,') = ',d25.15)
           call C_OUT(1,0,1,line)
        enddo
        write(line,9090)
 9090   format(/,'  Multipliers for equality constraints:',/)
        call C_OUT(1,0,3,line)
        do i = 1, M
           write(line,9100) i,LAM(i)
 9100      format(' LAM(',i5,') = ',d25.15)
           call C_OUT(1,0,1,line)
        enddo
      endif
#ifdef OCC
C -------------------------------------------------------------------
C     Report Output to <opt_details.m>
C     (Those lines are contributed by Tobias Jockenhoevel in order
C      to be able to use IPOPT with his OptimizationControlCentre)
C -------------------------------------------------------------------
      open (UNIT = 10, FILE = 'opt_details.m')
      write(10,*) ' % -----------------------------------'
      write(10,*) ' % IPOPT Optimization runtime report        '
      write(10,*) ' % -----------------------------------'
      write(10,*) ' '
      write(10,*) ' % Final value of objective function:'
      write(10,*) ' opt_obj = ',F,';'
      write(10,*) ' '
      write(10,*) ' % Final maximal constraint violation:'
      if( M.gt.0 ) then
         write(10,*) ' c_err = ',dabs(RS(P_C+icmax)),';'
      else
         write(10,*) ' c_err = 0e0;'
      endif
      write(10,*) ' '
      write(10,*) ' % Final value for overall KKT error:'
      write(10,*) ' kkt_err = ', ERR,';'
      write(10,*) ' '
      write(10,*) ' % Evaluation of the objective function:'
      write(10,*) ' obj_iter = ',fevals,';'
      write(10,*) ' '
      write(10,*) ' % Evaluation of the constraints:'
      write(10,*) ' c_iter = ',cevals,';'

      close(10, STATUS = 'KEEP')
#endif
C
C     END
C
 9999 continue
      return

 9008 IERR = 8
      call C_OUT(2,0,1,'Error while trying to open BWARM.DAT')
      goto 9999
 9009 IERR = 8
      call C_OUT(2,0,1,'Error while writing to BWARM.DAT')
      goto 9999
      end
