C Copyright (C) 2002, Carnegie Mellon University and others.
C All Rights Reserved.
C This code is published under the Common Public License.
C*******************************************************************************
C
      SUBROUTINE FILTER(ITER, N, NIND, M, X, IVAR, NFIX, IFIX,
     1     NLB, ILB, NUB,
     1     IUB, LSLACKS, BNDS_L, BNDS_U, DX, DV_L, DV_U, S_L, S_U,
     2     V_L, V_U, SIGMA_L, SIGMA_U, PZ, NORIG, XORIG, CSCALE,
     3     MU, LAM, LAMOLD, G, F, C, CNRM0, ALPHA, ALPHA_DUAL,
     3     THETA1, THETA2, RESTO, LS_COUNT, NU_OUT, C_ALPHA, C_ACCEPT,
     4     SOC_FLAG, INIT_LAM,
     5     KCONSTR, LRS, RS, LIS, IS,
     6     LRW, RW, LIW, IW, IERR)
C
C*******************************************************************************
C
C    $Id: filter.f,v 1.4 2002/11/24 21:37:28 andreasw Exp $
C
C-------------------------------------------------------------------------------
C                                 Title
C-------------------------------------------------------------------------------
C
CT    Filter method implementation with line search
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
CP   ITER      I    INT    -1: initialize memory
CP                          0: first iteration
CP                         >0: otherwise
CP   N         I    INT    number of variables (without fixed)
CP   NIND      I    INT    number of independent variables
CP   M         I    INT    number of constraints
CP   X        I/O   DP     actual iterate (reordered without fixed vars:
CP                             first M entries belong to dependent
CP                             variables, remaining to independent variables)
CP                            I: old point
CP                            O: point after line search
CP   IVAR      I    INT    information about partitioning
CP                            i = 1..M      XORIG(IVAR(i)) dependent
CP                            i = (M+1)..N  XORIG(IVAR(i)) independent
CP                            Note: fixed variables do not occur in IVAR
CP                            X(i) corresponds to XORIG(IVAR(i))
CP   NFIX      I    INT    number of fixed variables
CP   IFIX      I    INT    specifies variables that are fixed by bounds:
CP                            i = 1..NORIG-N   XORIG(IFIX(i)) is fixed
CP                            (assumed to be in increasing order)
CP   NLB       I    INT    number of lower bounds (excluding fixed vars)
CP   ILB       I    INT    indices of lower bounds
CP                            (e.g. S_L(i) is slack for X(ILB(i)) )
CP   NUB       I    INT    number of upper bounds (excluding fixed vars)
CP   IUB       I    INT    indices of upper bounds
CP                            (e.g. S_U(i) is slack for X(IUB(i)) )
CP   LSLACKS  I/O   LOG    I: =.true.: There are slacks that don't satisfy
CP                                     "slack equation"
CP                         O: set to .false., if full step was taken
CP   BNDS_L    I    DP     values of lower bounds (ordered as S_L)
CP   BNDS_U    I    DP     values of upper bounds (ordered as S_U)
CP   DX        I    DP     step for X (primal)
CP   DV_L      I    DP     step for V_L (dual variables for lower bounds)
CP   DV_U      I    DP     step for V_U (dual variables for upper bounds)
CP   S_L      I/O   DP     slacks to lower bounds
CP                            I: for start of line search
CP                            O: after line search
CP   S_U      I/O   DP     slacks to upper bounds
CP                            I: for start of line search
CP                            O: after line search
CP   V_L      I/O   DP     dual variables for lower bounds
CP                            I: for start of line search
CP                            O: after line search
CP   V_U      I/O   DP     dual variables for upper bounds
CP                            I: for start of line search
CP                            O: after line search
CP   SIGMA_L   I    DP     primal-dual Hessian of lower bound barrier term
CP                            (NLB diagonal elements only)
CP   SIGMA_U   I    DP     primal-dual Hessian of upper bound barrier term
CP                            (NUB diagonal elements only)
CP   PZ       I/O   DP     kept update-to-date depending on SOC
CP   NORIG     I    INT    number of all variables including fixed vars
CP   XORIG    I/O   DP     actual iterate
CP                            XORIG is ordered in ORIGINAL order (i.e. not
CP                            partitioned into independent and dependent
CP                            variables) (on output: as X)
CP   CSCALE    I    DP     scaling factors for constraints
CP   MU        I    DP     barrier parameter
CP   LAM      I/O   DP     multipliers for equality constraints
CP   LAMOLD    I    DP     multipliers for equality constraints from last iter
CP   G         I    DP     gradient of objective function
CP   F        I/O   DP     value of objective function at X
CP                            I: for start of line search
CP                            O: after line search
CP   C        I/O   DP     values of constraints at X
CP                            I: for start of line search
CP                            O: after line search (or for SOC computation)
CP   CNRM0     O    DP     2-norm of constraints at old point
CP   ALPHA    I/O   DP     step size: I: where to start line search (primal)
CP                                    O: step size from X to X_NEW
CP   ALPHA_DUAL I/O DP     step size for dual variables
CP   THETA1    I    DP     dogleg parameter (=1 <-> full Newton step)
CP   THETA2    I    DP     dogleg parameter (= -c^T*A^T*d_CS ) 
CP   RESTO    I/O   LOG    =.true.: we are in restoration phase
CP   LS_COUNT  O    INT    number of trial steps
CP   NU_OUT    O    DP     actual value of penalty parameter
CP                            (only output; value is stored internally!)
CTODO That's BS, I don't do this <--- ?????
CP   C_ALPHA  I/O   C*1    This flag is set to 'R' if we are in the restoration
CP                            phase.
CP   C_ACCEPT  O    C*1    ='f' if improvement in barrier function
CP                         ='c' if improvement in constraint violation
CP                         ='b' if improvement in both
CP   SOC_FLAG I/O   INT    Flag for Second order correction:
CP                           =0: no SOC has been tried yet (I)
CP                           =1: try SOC correction now (O)
CP                               input step corresponds to SOC (I)
CP                           =2: SOC has been tried but not successful
CP   INIT_LAM I/O   LOG    Is changed by this routine, if the equality
CP                           multipliers are to be reinitialized.
CP                           (e.g. after restoration phase)
CP   KCONSTR   I    INT    KCONSTR(1): LRS for CONSTR
CP                         KCONSTR(2): P_LRS for CONSTR
CP                         KCONSTR(3): LIS for CONSTR
CP                         KCONSTR(4): P_LIS for CONSTR
CP                         KCONSTR(5): LRW for CONSTR
CP                         KCONSTR(6): LIW for CONSTR
CP   LRS      I/O   INT    (ITER.ne.-1) I: total length of RS
CP                         (ITER.eq.-1) O: increased by needed storage space
CP   RS       I/O   DP     DP storage space (all!)
CP   LIS      I/O   INT    (ITER.ne.-1) I: total length of IS
CP                         (ITER.eq.-1) O: increased by needed storage space
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
CS    D1MACH
CS    DDOT
CS    DCOPY
CS    DAXPY
CS    DNRM2
CS    DSCAL
CS    C_OUT
CS    CALC_BAR
CS    CALC_NRM
CS    GET_F
CS    GET_C
CS    FFINITE
CS    RESTO_TRON
CS    RESTO_KKT
CS    RESTO_DOGLEG
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
      integer ITER
      integer N
      integer NIND
      integer M
      double precision X(N)
      integer IVAR(N)
      integer NFIX
      integer IFIX(NFIX)
      integer NLB
      integer ILB(NLB)
      integer NUB
      integer IUB(NUB)
      logical LSLACKS
      double precision BNDS_L(NLB)
      double precision BNDS_U(NUB)
      double precision DX(N)
      double precision DV_L(NLB)
      double precision DV_U(NUB)
      double precision S_L(NLB)
      double precision S_U(NUB)
      double precision V_L(NLB)
      double precision V_U(NUB)
      double precision SIGMA_L(NLB)
      double precision SIGMA_U(NUB)
      double precision PZ(NIND)
      integer NORIG
      double precision XORIG(NORIG)
      double precision CSCALE(*)
      double precision MU
      double precision LAM(M+N)    ! larger dimension for LSLACKS
      double precision LAMOLD(M+N) ! larger dimension for LSLACKS
      double precision G(N)
      double precision F
      double precision C(M)
      double precision CNRM0
      double precision ALPHA
      double precision ALPHA_DUAL
      double precision THETA1
      double precision THETA2
      logical RESTO
      integer LS_COUNT
      double precision NU_OUT
      character*1 C_ALPHA
      character*1 C_ACCEPT
      integer SOC_FLAG
      logical INIT_LAM
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
      integer p_rwend, p_iwend, p_xnew, p_slnew, p_sunew
      integer p_cnew, p_tmp, p_gb, p_dxsoc, p_lamnew
      integer p_xstore, p_dl, p_du, p_tmp2, p_dlam
      integer i, k, ndelete, irej, a_not_b_count, ierr_store
      logical ftype, augment
      double precision cnrm_new, phi_new, f_new, bar_n, bar0
      double precision lhs, rhs, fi, ci, dphi, del_f, dummy
      double precision margin, new_phi, new_c, alpha_c, alpha_f, alphad
      double precision cnrm_add, phi_add, smin, alpha_tmp, dltd
      double precision tmp1, tmp2, tmp3, tmp4, model_alpha, gnrm
      double precision xnrm, dxnrm, vnrm, dvnrm, lamnrm, dlamnrm, tmp(2)
      double precision factor, a, b, ctmp
      character*150 line(2)

      double precision CALC_NRM, CALC_BAR, D1MACH, DDOT, FILTER_MODEL
      double precision DNRM2
      integer FFINITE
      integer FILTER_CHECK

      integer NFILTER, P_FILTER_PHI, P_FILTER_C, RESTO_KKT_FLAG
      save    NFILTER, P_FILTER_PHI, P_FILTER_C, RESTO_KKT_FLAG

      integer P_XL, P_XU, COUNT_ITER_BLOCK, IREJ_LAST
      save    P_XL, P_XU, COUNT_ITER_BLOCK, IREJ_LAST

      integer P_DXSTORE, P_DVLSTORE, P_DVUSTORE, P_LAMSTORE, SOC_ITER
      save    P_DXSTORE, P_DVLSTORE, P_DVUSTORE, P_LAMSTORE, SOC_ITER

      integer P_WXSTORE, P_WVLSTORE, P_WVUSTORE, P_PZSTORE, P_CSTORE
      save    P_WXSTORE, P_WVLSTORE, P_WVUSTORE, P_PZSTORE, P_CSTORE

      integer P_WDXSTORE, P_WDVLSTORE, P_WDVUSTORE, P_WLAMSTORE
      save    P_WDXSTORE, P_WDVLSTORE, P_WDVUSTORE, P_WLAMSTORE

      double precision CNRM_NEW_STORE, PHI0, GD, CL, CDL
      save             CNRM_NEW_STORE, PHI0, GD, CL, CDL

      double precision LAST_MU, CNRM_MAX, MACHEPS, MACHTINY, ALPHA_MIN
      save             LAST_MU, CNRM_MAX, MACHEPS, MACHTINY, ALPHA_MIN
      double precision CNRM_SML, CNRM_STORE, ALPHA_PRIMAL
      save             CNRM_SML, CNRM_STORE, ALPHA_PRIMAL

      double precision ALPHA_STORE, ALPHA_DUAL_STORE
      save             ALPHA_STORE, ALPHA_DUAL_STORE

      double precision WALPHA_STORE, WALPHA_DUAL_STORE
      save             WALPHA_STORE, WAlpha_DUAL_STORE
      double precision WALPHA_MIN_STORE
      save             WALPHA_MIN_STORE

      integer WFLAG, ITER_DOGLEG
      save    WFLAG, ITER_DOGLEG

      integer ITER_DOGLEG_MAX
      parameter( ITER_DOGLEG_MAX = 30 )

      logical SMALL_STEP_BEFORE
      save    SMALL_STEP_BEFORE

      double precision PRECFACT
      parameter( PRECFACT = 1d1 )

      double precision BACKTRACK_FAC
      parameter( BACKTRACK_FAC = 5.d-1 )

      double precision DELTA
      double precision ETA_F
      double precision ALPHA_MIN_FRAC
      parameter( DELTA       = 1d-4 )
      parameter( ETA_F       = 1d-4 )
      parameter( ALPHA_MIN_FRAC = 5d-2 )

      integer SOC_ITER_MAX
      parameter( SOC_ITER_MAX = 10 )

      double precision CNRM_SML_FACT
      parameter( CNRM_SML_FACT = 1.d-4 )
      double precision CNRM_MAX_FAC
C      parameter( CNRM_MAX_FAC = 1.d4 )
      parameter( CNRM_MAX_FAC = 1.d10 )
C
C*******************************************************************************
C
C                           Executable Statements
C
C*******************************************************************************
C

C
C     Initialize Storage space
C
      if( ITER.eq.-1 ) then

         NFILTER = 0
         P_FILTER_PHI = LRS
         P_FILTER_C   = P_FILTER_PHI + QMAXITER ! That's actually generous ;)
         LRS          = P_FILTER_C   + QMAXITER

         if( QWATCHTOL.gt.0.d0 ) then
C
C     Reserve storage for watchdog
C
            P_WXSTORE   = LRS
            P_WVLSTORE  = P_WXSTORE  + N
            P_WVUSTORE  = P_WVLSTORE  + NLB
            P_WDXSTORE  = P_WVUSTORE  + NUB
            P_WDVLSTORE = P_WDXSTORE  + N
            P_WDVUSTORE = P_WDVLSTORE + NLB
            LRS         = P_WDVUSTORE + NUB
            if( QLAMBDA.eq.2 ) then
               P_WLAMSTORE = LRS
               LRS         = P_WLAMSTORE + M
            endif
         endif

         if( QSOC.ne.0 ) then
C
C     Reserve storage space for second order correction option
C
            P_DXSTORE  = LRS
            P_DVLSTORE = P_DXSTORE  + N
            P_DVUSTORE = P_DVLSTORE + NLB
            P_CSTORE   = P_DVUSTORE + NUB
            LRS        = P_CSTORE   + M
            if( QLAMBDA.eq.2 ) then
               P_LAMSTORE = LRS
               LRS        = P_LAMSTORE + M
            endif
            if( QQUASI.ne.0 .and. abs(QQUASI).lt.6 ) then
               P_PZSTORE = LRS
               LRS       = P_PZSTORE + NIND
            endif

         endif

         LAST_MU  = -1d0
         MACHEPS  = D1MACH(4)
         MACHTINY = sqrt(D1MACH(1))

         WFLAG            = 0
         COUNT_ITER_BLOCK = 0
         RESTO_KKT_FLAG   = 0
         ITER_DOGLEG      = 0

         SMALL_STEP_BEFORE = .false.

         goto 9999
      endif
C
C     Initialization
C
      if( abs(QMERIT).eq.5 .and. QLAMBDA.ne.2 ) then
         IERR = 4
         call C_OUT(2,0,1,
     1        'filter:  Need QLAMBDA = 2 for this filter option.')
         goto 9999
      endif

 5    continue
      IERR = 0
      p_rwend = 0
      p_iwend = 0

C      if( LSLACKS ) then
C         write(line,*) 'filter: not yet for LSLACKS'
C         call C_OUT(2,0,1,line)
C         IERR = 4
C         goto 9999
C      endif
      C_ACCEPT = ' '
C
C     If in restoration phase, go directly there
C
      if( RESTO ) then
         CNRM0 = CALC_NRM(M, C)
         LS_COUNT = 0
         goto 3015
      endif
C
C     If in trial step for watchdog, use values for previous point
C
      if( WFLAG.eq.1 ) goto 6
C
C     The following calculations only have to be done once per iteration
C
      if( QSOC.ne.0 .and. SOC_FLAG.eq.1 ) goto 6
C
C     If mu changed, delete all entries in filter
C
      if( LAST_MU.ne.MU ) then
         NFILTER = 0
         LAST_MU = MU
         SMALL_STEP_BEFORE = .false.
      endif
C
C     Check if step is too small (close to solution already?) so that we can
C     quit with a warning message?
C
      xnrm  = CALC_NRM(N, X)
      dxnrm = CALC_NRM(N, DX)
      factor = PRECFACT*MACHEPS
      if( dxnrm.le.factor*xnrm .and. ALPHA.gt.0.d0 .and.
     1     dxnrm.lt.1.d-2 ) then
         write(line,2) xnrm, dxnrm
 2       format(' filter: Step is not making much progress:',/,
     1        '         ||X|| = ',d14.6,',   ||DX|| = ',d14.6)
         call C_OUT(1,0,2,line)
         LS_COUNT = 0
         NU_OUT   = 0d0
         call DAXPY(N  , ALPHA     , DX  , 1, X  , 1)
         do i = 1, NLB
            k = ILB(i)
            S_L(i) = dmax1(MACHTINY, X(k) - BNDS_L(i))
            X(k) = S_L(i) + BNDS_L(i)
         enddo
         do i = 1, NUB
            k = IUB(i)
            S_U(i) = dmax1(MACHTINY, BNDS_U(i) - X(k))
            X(k) = BNDS_U(i) - S_U(i)
         enddo
         call GET_F(N, X, IVAR, NORIG, XORIG, F)
         call GET_C(ITER, N, NIND, X, IVAR, NORIG,
     1        XORIG, M, CSCALE, C, KCONSTR, LRS, RS,
     2        LIS, IS, LRW-p_rwend, RW(p_rwend+1),
     4        LIW-p_iwend, IW(p_iwend+1), IERR)
         call DAXPY(NLB, ALPHA_DUAL, DV_L, 1, V_L, 1)
         call DAXPY(NUB, ALPHA_DUAL, DV_U, 1, V_U, 1)
         if( QLAMBDA.eq.2 ) then
            call DSCAL(M, ALPHA, LAM, 1)
            call DAXPY(M, (1.d0-ALPHA), LAMOLD, 1, LAM, 1)
            call DCOPY(M, LAM, 1, LAMOLD, 1)
         endif
         if( SMALL_STEP_BEFORE ) then
            C_ACCEPT = 's'
            IERR = -2
         else
            C_ACCEPT = 'S'
         endif
         SMALL_STEP_BEFORE = .true.
         goto 9999
      else
         SMALL_STEP_BEFORE = .false.
      endif
C
      IREJ_LAST = 0
      SOC_ITER = 0
      ALPHA_PRIMAL = ALPHA
C
C     Compute current THETA (in CNRM0)
C
      CNRM0 = CALC_NRM(M, C)
      if( LSLACKS ) then
C         IERR = 4
C         goto 9999
         p_tmp   = p_rwend
         p_rwend = p_tmp + max(N, 2)
         if( p_rwend.gt.LRW ) then
            IERR = 98
            goto 9999
         endif
         call DCOPY(N, 0d0, 0, RW(p_tmp+1), 1)
         do i = 1, NLB
            k = ILB(i)
            RW(p_tmp+k) = X(k) - S_L(i) - BNDS_L(i)
         enddo
         do i = 1, NUB
            k = IUB(i)
            RW(p_tmp+k) = X(k) + S_U(i) - BNDS_U(i)
         enddo
         RW(p_tmp+1) = CALC_NRM(N, RW(p_tmp+1))
         RW(p_tmp+2) = CNRM0
         CNRM0 = CALC_NRM(2, RW(p_tmp+1))
         p_rwend = p_tmp
      endif
C
C     If in KKT restoration phase, go exactly there
C
      if( RESTO_KKT_FLAG.ne.0 ) goto 3010
C
C     Get value of barrier function at current point
C
      bar0 = CALC_BAR(NLB, NUB, S_L, S_U, V_L, V_U, MU)
      PHI0 = F + bar0
C
C     For Lagrangian add contraint times multiplier part
C
      if( abs(QMERIT).eq.5 ) then
         PHI0 = PHI0 + DDOT(M, C, 1, LAMOLD, 1)
      endif
C
C     Determine maximal allowed constrained violation
C
      if( ITER.eq.0 ) then
         CNRM_MAX = CNRM_MAX_FAC*dmax1(1d0, CNRM0)
         if( QMERIT.gt.0 ) then
            CNRM_SML = CNRM_SML_FACT*dmax1(1d0, CNRM0)
         else
            CNRM_SML = CNRM_MAX
         endif
      endif
C
C     Get linear prediction in barrier function g^T d
C
      p_gb    = p_rwend
      p_rwend = p_gb + N
      if( p_rwend.gt.LRW ) then
         IERR = 98
         goto 9999
      endif
      call DCOPY(N, G, 1, RW(p_gb+1), 1)
      do i = 1, NLB
         k = ILB(i)
         RW(p_gb+k) = RW(p_gb+k) - MU/S_L(i)
      enddo
      do i = 1, NUB
         k = IUB(i)
         RW(p_gb+k) = RW(p_gb+k) + MU/S_U(i)
      enddo
      GD = DDOT(N, DX, 1, RW(p_gb+1), 1)
      p_rwend = p_gb
C
C     For Lagranian filter option computer CL and CDL
C
      if( abs(QMERIT).eq.5 ) then
         if( LSLACKS ) then
            IERR = 4
            call C_OUT(2,0,1,
     1      'filter: LSLACKS not implemented for this filter option.')
         endif
         CL  = DDOT(M, C, 1, LAMOLD, 1)
         CDL = DDOT(M, C, 1, LAM   , 1) - CL
      endif
C
C     Determine minimal step size ALPHA_MIN
C
      if( CNRM0.gt.0d0 ) then
         ALPHA_MIN = QGAMMA_THETA
      else
         ALPHA_MIN = QGAMMA_THETA ! 1d300 ?
CTRY
         if( GD.gt.0d0 ) then
            if( SOC_FLAG.ne.1 ) then
               gnrm = DNRM2(N, G, 1)
               if( GD.gt.PRECFACT*MACHEPS*gnrm ) then
                  write(line,*)
     1                 'filter: Error.  CNRM0 = 0, but GD = ',GD
                  call C_OUT(2,0,1,line)
                  IERR = 581
                  goto 9999
               endif
            endif
            GD = 0.d0
            ALPHA_MIN = ALPHA
         endif
      endif
C
      if( abs(QMERIT).eq.4 ) then
         if( GD.lt.0d0 ) then
            ALPHA_MIN = min(ALPHA_MIN, -QGAMMA_F*CNRM0/GD)
            if( CNRM0.le.CNRM_SML ) then
               ALPHA_MIN = min(ALPHA_MIN,
     1              DELTA*(CNRM0**QS_THETA)/((-GD)**QS_F))
            endif
         endif
      elseif( abs(QMERIT).eq.5 ) then
CTODO I have no clue what rule shoule be here...
c$$$         a = GD - CL + CDL
c$$$         b = CDL
c$$$         if( a.lt.0.d0 ) then
c$$$            ALPHA_MIN = min(ALPHA_MIN, -QGAMMA_F*CNRM0/a)
c$$$         endif
c$$$         if( b.gt.0.d0 ) then
c$$$            ALPHA_MIN = min(ALPHA_MIN,
c$$$     1           (a+(DELTA*(CNRM0**QS_THETA)/ALPHA_MIN)**(-QS_F))/b)
c$$$         elseif( a.lt.0.d0 ) then
c$$$            ALPHA_MIN = min(ALPHA_MIN,
c$$$     1           DELTA*(CNRM0**QS_THETA)/((-a)**QS_F))
c$$$         endif
         dltd = GD - CL
         if( CDL.eq.0.d0 ) then
            if( dltd .lt. 0.d0 ) then
               ALPHA_MIN = min(ALPHA_MIN,
     1              min(QGAMMA_F*CNRM0,
     2              DELTA*(CNRM0**QS_THETA))/((-dltd)**QS_F))
            endif
         else
            tmp1 = (1+dltd/CDL)/2.d0
            tmp2 = DELTA*(CNRM0**QS_THETA)/(CDL**QS_F)
            tmp3 = QGAMMA_F*CNRM0/CDL
            if( CDL.gt.0.d0 ) then
               tmp4 = tmp1 + dsqrt(tmp1**2+tmp2)
               if( tmp4.gt.0.d0 ) ALPHA_MIN = min(ALPHA_MIN, tmp4)
                                ! in extremely ill-conditioned cases, tmp4
                                ! can become zero numerically
               tmp4 = tmp1 + dsqrt(tmp1**2+tmp3)
               if( tmp4.gt.0.d0 ) ALPHA_MIN = min(ALPHA_MIN, tmp4)
            else
               tmp4 = tmp1 - dsqrt(tmp1**2+tmp2)
               if( tmp4.gt.0.d0 ) then
                  ALPHA_MIN = min(ALPHA_MIN, tmp4)
               endif
               tmp4 = tmp1 - dsqrt(tmp1**2+tmp3)
               if( tmp4.gt.0.d0 ) then
                  ALPHA_MIN = min(ALPHA_MIN, tmp4)
               endif
            endif
         endif
      endif
C
      ALPHA_MIN = ALPHA_MIN_FRAC*ALPHA_MIN
      if( QCNR.gt.0 .and. QPRINT.ge.3 ) then
         write(line,*) 'filter: ALPHA_MIN = ',ALPHA_MIN
         call C_OUT(1,3,1,line)
      endif
C
C     reserve work space for trial points
C
 6    p_xnew    = p_rwend
      p_slnew   = p_xnew   + N
      p_sunew   = p_slnew  + NLB
      p_cnew    = p_sunew  + NUB
      p_rwend   = p_cnew   + M
      if( abs(QMERIT).eq.5 ) then
         p_lamnew = p_rwend
         p_rwend  = p_lamnew + M
      endif
      if( p_rwend.gt.LRW ) then
         IERR = 98
         goto 9999
      endif
C
C     Now do the actual line search
C
      LS_COUNT = 0
      cnrm_new = 0d0
      phi_new  = 0d0
      f_new    = 0d0
C
C ==============================================================================
C
C     BEGINNING OF BACKTRACKING LINE SEARCH LOOP
C
C ==============================================================================
C
 10   continue

      if( ALPHA.eq.0.d0 ) goto 3000 ! jump directly to restoration
                                ! (ALPHA = 0 set in get_step_full or get_pz)

      LS_COUNT = LS_COUNT + 1
C
C     Compute new trial point and evaluate objective and constraint
C     function at that point
C
      call DCOPY(N, X, 1, RW(p_xnew+1), 1)
      call DAXPY(N, ALPHA, DX, 1, RW(p_xnew+1), 1)
      if( abs(QMERIT).eq.5 ) then
         call DCOPY(M, LAM, 1, RW(p_lamnew+1), 1)
         call DSCAL(M, ALPHA, RW(p_lamnew+1), 1)
         call DAXPY(M, (1.d0-ALPHA), LAMOLD, 1, RW(p_lamnew+1), 1)
      endif
C
      call GET_F(N, RW(p_xnew+1), IVAR, NORIG, XORIG, f_new)
      if( FFINITE(f_new).eq.0 ) then ! we don't want NaN here; cut step!
         write(line,*) 'f_new =',f_new
         irej = -2
         call C_OUT(2,0,1,line)
         goto 800
      endif
C
      call GET_C(ITER, N, NIND, RW(p_xnew+1), IVAR, NORIG, XORIG, M,
     1     CSCALE, RW(p_cnew+1), KCONSTR, LRS, RS, LIS, IS,
     2     LRW-p_rwend, RW(p_rwend+1),
     4     LIW-p_iwend, IW(p_iwend+1), IERR)
      if( IERR.gt.0 ) then
         write(line,*)
     1        'filter: Error: get_c returns IERR = ',IERR
         call C_OUT(2,0,1,line)
         goto 9999
      elseif( IERR.ne.0 ) then
         write(line,*)
     1        'filter: Warning: get_c returns IERR = ',IERR
         call C_OUT(2,0,1,line)
      endif
C
C     Compute new trial slack variables
C     (If slacks get too small, the problem probably doesn't have a
C      feasible strictly interior point)
C
      smin = 10e10*MACHTINY
      if( .not.LSLACKS ) then
         do i = 1, NLB
            RW(p_slnew+i) = RW(p_xnew+ILB(i)) - BNDS_L(i)
            if( RW(p_slnew+i).lt.smin ) then
               write(line,*) 
     1              'filter: Slack for lower bound ',ILB(i),
     1              ' = ',RW(p_slnew+i)
               call C_OUT(2,1,1,line)
               BNDS_L(i) = BNDS_L(i) -
     1              PRECFACT*MACHEPS*max(1d0, abs(BNDS_L(i)))
               RW(p_slnew+i) = RW(p_xnew+ILB(i)) - BNDS_L(i)
               write(line,*) 
     1              '        Decrease lower bound to ,',BNDS_L(i)
               call C_OUT(2,1,1,line)
C               LSLACKS = .true.
C               IERR = 19
C               goto 9999
            endif
         enddo
         do i = 1, NUB
            RW(p_sunew+i) = BNDS_U(i) - RW(p_xnew+IUB(i))
            if( RW(p_sunew+i).lt.smin ) then
               write(line,*) 
     1              'filter: Slack for upper bound ',IUB(i),
     1              ' = ',RW(p_sunew+i)
               call C_OUT(2,1,1,line)
               BNDS_U(i) = BNDS_U(i) +
     1              PRECFACT*MACHEPS*max(1d0, abs(BNDS_U(i)))
               RW(p_sunew+i) = BNDS_U(i) - RW(p_xnew+IUB(i))
               write(line,*) 
     1              '        Increase upper bound to ,',BNDS_U(i)
               call C_OUT(2,1,1,line)
C               LSLACKS = .true.
C               IERR = 19
C               goto 9999
            endif
         enddo
      else
         do i = 1, NLB
            k = ILB(i)
            RW(p_slnew+i) = S_L(i) + 
     1           ALPHA*( DX(k)+X(k)-S_L(i)-BNDS_L(i))
            if( RW(p_slnew+i).lt.smin ) then
               IERR = 19
               goto 9999
            endif
         enddo
         do i = 1, NUB
            k = IUB(i)
            RW(p_sunew+i) = S_U(i) + 
     1           ALPHA*(-DX(k)-X(k)-S_U(i)+BNDS_U(i))
            if( RW(p_sunew+i).lt.smin ) then
               IERR = 19
               goto 9999
            endif
         enddo
      endif
      cnrm_new = CALC_NRM(M, RW(p_cnew+1))
      if( LSLACKS ) then
C         IERR = 4
C         goto 9999
         p_tmp = p_rwend
         p_rwend = p_tmp + max(N, 2)
         if( p_rwend.gt.LRW ) then
            IERR = 98
            goto 9999
         endif
         call DCOPY(N, 0d0, 0, RW(p_tmp+1), 1)
         do i = 1, NLB
            k = ILB(i)
            RW(p_tmp+k) = RW(p_xnew+k) - RW(p_slnew+i) - BNDS_L(i)
         enddo
         do i = 1, NUB
            k = IUB(i)
            RW(p_tmp+k) = RW(p_xnew+k) + RW(p_sunew+i) - BNDS_U(i)
         enddo
         RW(p_tmp+1) = CALC_NRM(N, RW(p_tmp+1))
         RW(p_tmp+2) = cnrm_new
         cnrm_new = CALC_NRM(2, RW(p_tmp+1))
         p_rwend = p_tmp
      endif
      if( FFINITE(cnrm_new).eq.0 ) then ! we don't want NaN here; cut step!
         write(line,*) 'cnrm_new =', cnrm_new
         call C_OUT(2,0,1,line)
         irej = -2
         goto 800
      endif
C
C     Check if constraint violation is 'exploding'
C
      if( cnrm_new.gt.CNRM_MAX ) then
         if( QCNR.gt.0 .and. QPRINT.ge.2 ) then
            write(line,*) 'filter: cnrm = ',cnrm_new,
     1           ' is getting too large.' 
            call C_OUT(1,2,1,line)
         endif
         irej = -3
         goto 800
      endif
C
C     Compute new value of barrier (or Lagrangian) function
C
      bar_n = CALC_BAR(NLB, NUB, RW(p_slnew+1),
     1     RW(p_sunew+1), dummy, dummy, MU)
      phi_new = f_new + bar_n
      if( abs(QMERIT).eq.5 ) then
         phi_new = phi_new + DDOT(M, RW(p_cnew+1), 1, RW(p_lamnew+1), 1)
      endif
      if( FFINITE(phi_new).eq.0 ) then ! we don't want NaN here; cut step!
         write(line,*) 'phi_new =',phi_new
         call C_OUT(2,0,1,line)
         irej = -2
         goto 800
      endif
C
C     Check if trial point acceptable to current point
C
      if( WFLAG.eq.1 ) then
         alpha_tmp = WALPHA_STORE
      elseif( SOC_FLAG.eq.1 ) then
         alpha_tmp = ALPHA_STORE
      else
         alpha_tmp = ALPHA
      endif
      model_alpha = FILTER_MODEL(alpha_tmp, GD, CL, CDL)
      if( model_alpha.ge.0.d0 ) then
         ftype = .false.
      else
         ftype = ( ((-model_alpha)**QS_F)*(alpha_tmp**(1.d0-QS_F))
     1        .gt.DELTA*(CNRM0**QS_THETA) .and.
     1        CNRM0.le.CNRM_SML ) ! if true, enforce Armijo cond for obj func
      endif
      if( QCNR.gt.0 .and. QPRINT.ge.2 ) then
         write(line,*) 'ftype test: model = ',model_alpha,
     1        ', CNRM0 = ', CNRM0
         call C_OUT(1,2,1,line)
         write(line,*) 'ftype = ',ftype
         call C_OUT(1,2,1,line)
      endif

      if( ftype ) then
C
C     Check if enough decrease in barrier function
C
         lhs = PHI0-phi_new + PRECFACT*MACHEPS*dabs(PHI0)
         rhs = -ETA_F*model_alpha
         if( lhs.lt.rhs ) then
            irej = -1
            goto 800
         endif

      else
C
C     Check if "filter margin" satisfied at current iterate
C
         fi = PHI0
         ci = CNRM0
         if( cnrm_new-(ci-QGAMMA_THETA*ci).gt.PRECFACT*MACHEPS*ci .and.
     1      phi_new-(fi-QGAMMA_F*ci).gt.PRECFACT*MACHEPS*dabs(fi) ) then
            irej = 0
            goto 800
         endif

      endif
C
C     Check if trial point is acceptable to the filter
C
      irej = FILTER_CHECK(NFILTER, RS(P_FILTER_C+1), RS(P_FILTER_PHI+1),
     1     cnrm_new, phi_new, PRECFACT, MACHEPS)
      if( irej.gt.0 ) goto 800
C
C     OK, point is accepted!
C
      goto 1000

C ==============================================================================
C
C     Not accepted, maybe do some second order correction.
C     Otherwise, cut step size and go back to beginning of loop.
C
C ==============================================================================

 800  continue
      if( QCNR.gt.0 .and. QPRINT.gt.1 ) then
         write(line,*) 'LS_COUNT = ',LS_COUNT,', ALPHA = ',ALPHA
         call C_OUT(1,2,1,line)
         if( irej.eq.0 ) then
            call C_OUT(1,2,1,
     1           'No sufficient decrease w.r.t. current point.')
            write(line,*) 'PHI0    = ',PHI0,
     1           ' C     = ',CNRM0
            call C_OUT(1,2,1,line)
            write(line,*) 'PHI     = ',phi_new,
     1           ' C     = ',cnrm_new
            call C_OUT(1,2,1,line)
         elseif( irej.gt.0 ) then
            write(line,*) 'Filter entry that rejected: ',irej
            call C_OUT(1,2,1,line)
            write(line,*) 'PHI_filter = ',RS(P_FILTER_PHI+irej),
     1           ' C_filter = ',RS(P_FILTER_C+irej)
            call C_OUT(1,2,1,line)
            write(line,*) 'PHI        = ',phi_new,
     1           ' C        = ',cnrm_new
            call C_OUT(1,2,1,line)
         elseif( irej.eq.-1 ) then
            write(line,*) 'f-type iteration test:'
            call C_OUT(1,2,1,line)
            write(line,*) 'phi0 = ',PHI0,' phi_new = ',phi_new
            call C_OUT(1,2,1,line)
            write(line,*) 'rhs = ',rhs,'  lhs = ',lhs
            call C_OUT(1,2,1,line)
         elseif( irej.eq.-2 ) then
            write(line,*) 'Nan occured...'
            call C_OUT(1,2,1,line)
         else
c            write(line,*) 'irej = ',irej
c            call C_OUT(1,2,1,line)
         endif
      endif
C
C     For Second Order Correction: Check, if SOC needed
C
CTRY      if( QSOC.ne.0 .and. SOC_FLAG.eq.0 .and. irej.ne.-2) then
      if( WFLAG.eq.1 ) then
         ctmp = CNRM_NEW_STORE
      else
         ctmp = CNRM0
      endif
      if( QSOC.ne.0 .and. SOC_FLAG.eq.0 .and. irej.ne.-2 .and.
     1     (ctmp.le.cnrm_new) .and. WFLAG.ne.2 ) then
         if( LSLACKS ) then
            IERR = 4
            goto 9999
         endif
C
C     Try SOC
C
         write(line,*) 'Try second order correction with CNRM0 = ',CNRM0
         call C_OUT(1,2,1,line)
         write(line,*) '   and cnrm_new = ', cnrm_new
         call C_OUT(1,2,1,line)
         write(line,*) '   and ALPHA = ', ALPHA
         call C_OUT(1,2,1,line)
C
C     Store current step etc
C
         call DCOPY(N,   DX,   1, RS(P_DXSTORE +1), 1)
         call DCOPY(NLB, DV_L, 1, RS(P_DVLSTORE+1), 1)
         call DCOPY(NUB, DV_U, 1, RS(P_DVUSTORE+1), 1)
         call DCOPY(M  , C   , 1, RS(P_CSTORE  +1), 1)
         if( QLAMBDA.eq.2 ) then
            call DCOPY(M, LAM, 1, RS(P_LAMSTORE+1), 1)
            call DCOPY(M, LAMOLD, 1, LAM, 1)
         endif
         if( QQUASI.ne.0 .and. abs(QQUASI).lt.6 ) then
            call DCOPY(NIND, PZ, 1, RS(P_PZSTORE+1), 1)
         endif
         ALPHA_STORE      = ALPHA
         ALPHA_DUAL_STORE = ALPHA_DUAL
         CNRM_NEW_STORE   = cnrm_new
C
C     Compute new right hand side for constraints
C
         call DSCAL(M, ALPHA, C, 1)
         call DAXPY(M, 1.d0, RW(p_cnew+1), 1, C, 1)
C
         SOC_FLAG = 1
         SOC_ITER = 1

         goto 9999

      elseif( QSOC.ne.0 .and. SOC_FLAG.eq.1 ) then

         if( cnrm_new.ge.0.99d0*CNRM_NEW_STORE .or.
     1        SOC_ITER.gt.SOC_ITER_MAX .or. irej.eq.-2 ) then
C
C     SOC didn't help
C
            if( cnrm_new.ge.0.99d0*CNRM_NEW_STORE ) then
               write(line,*)
     1              'Second order correction rejected with cnrm_new = ',
     2              cnrm_new
               call C_OUT(1,2,1,line)
            elseif( SOC_ITER.gt.SOC_ITER_MAX ) then
               write(line,*) 'Tried ',SOC_ITER,
     1              ' second order corrections. Quit SOC.'
               call C_OUT(1,2,1,line)
            endif
C
C     Restore old search direction
C
            call DCOPY(N,   RS(P_DXSTORE +1), 1, DX,   1)
            call DCOPY(NLB, RS(P_DVLSTORE+1), 1, DV_L, 1)
            call DCOPY(NUB, RS(P_DVUSTORE+1), 1, DV_U, 1)
            call DCOPY(M  , RS(P_CSTORE  +1), 1, C   , 1)
            if( QLAMBDA.eq.2 ) then
               call DCOPY(M, RS(P_LAMSTORE+1), 1, LAM, 1)
            endif
            if( QQUASI.ne.0 .and. abs(QQUASI).lt.6 ) then
               call DCOPY(NIND, RS(P_PZSTORE+1), 1, PZ, 1)
            endif
            ALPHA      = ALPHA_STORE
            ALPHA_DUAL = ALPHA_DUAL_STORE
C
C     Now do back tracking line search with original direction
C
            SOC_FLAG = 2
C
C     Since we already had tried the full step, let's start this time with
C     the reduced trial step size
C
            if( CNRM0.ge.QWATCHTOL ) then
               ALPHA = BACKTRACK_FAC*ALPHA
               goto 10
            else
               goto 10
            endif

         else
C
C     Try additional second order corrections
C
            write(line,*)
     1        'Try additional second order correction with cnrm_new = ',
     2           cnrm_new
            call C_OUT(1,2,1,line)
            write(line,*) '   and ALPHA = ', ALPHA
            call C_OUT(1,2,1,line)

            CNRM_NEW_STORE = cnrm_new
            if( QLAMBDA.eq.2 ) then
               call DCOPY(M, LAMOLD, 1, LAM, 1)
            endif
C
C     Compute new right hand side for constraints
C
            call DSCAL(M, ALPHA, C, 1)
            call DAXPY(M, 1.d0, RW(p_cnew+1), 1, C, 1)
C
            SOC_ITER = SOC_ITER + 1

            goto 9999

         endif

      endif
C
C     Watchdog
C
      if( WFLAG.eq.0 .and. CNRM0.lt.QWATCHTOL .and. irej.ne.-2
     1     .and. M.gt.0 ) then
         if( LSLACKS ) then
            IERR = 4
            goto 9999
         endif
C
C     Watchdog currently not active - wake it up
C
         call C_OUT(1,1,1,'filter: Try watchdog step.')
         WFLAG = 1
         call DCOPY(N,   X,    1, RS(P_WXSTORE +1), 1)
         call DCOPY(NLB, V_L,  1, RS(P_WVLSTORE +1), 1)
         call DCOPY(NUB, V_U,  1, RS(P_WVUSTORE +1), 1)
         call DCOPY(N,   DX,   1, RS(P_WDXSTORE +1), 1)
         call DCOPY(NLB, DV_L, 1, RS(P_WDVLSTORE+1), 1)
         call DCOPY(NUB, DV_U, 1, RS(P_WDVUSTORE+1), 1)
         if( QLAMBDA.eq.2 ) then
            call DCOPY(M, LAM, 1, RS(P_WLAMSTORE+1), 1)
         endif
         WALPHA_MIN_STORE  = ALPHA_MIN
         WALPHA_STORE      = ALPHA
         WALPHA_DUAL_STORE = ALPHA_DUAL
         C_ACCEPT         = 'w'
         CNRM_NEW_STORE   = cnrm_new
         goto 1010

      elseif( WFLAG.eq.1 ) then
C
C     Trial step didn't help, restore old step
C
         call C_OUT(1,1,1,
     1 'filter: Watchdog trial step didn''t help. Restore old iterate.')
         WFLAG = 2
         call DCOPY(N,   RS(P_WXSTORE +1),  1, X,    1)
         call DCOPY(NLB, RS(P_WVLSTORE+1),  1, V_L,  1)
         call DCOPY(NUB, RS(P_WVUSTORE+1),  1, V_U,  1)
         call DCOPY(N,   RS(P_WDXSTORE +1), 1, DX,   1)
         call DCOPY(NLB, RS(P_WDVLSTORE+1), 1, DV_L, 1)
         call DCOPY(NUB, RS(P_WDVUSTORE+1), 1, DV_U, 1)
         if( QLAMBDA.eq.2 ) then
            call DCOPY(M, RS(P_WLAMSTORE+1), 1, LAM, 1)
         endif
         ALPHA      = BACKTRACK_FAC * WALPHA_STORE
         ALPHA_DUAL = WALPHA_DUAL_STORE
         ALPHA_MIN  = WALPHA_MIN_STORE
         goto 10
      endif
C
C     Cut back step size
C
      ALPHA = BACKTRACK_FAC*ALPHA
      IREJ_LAST = irej
C
      if( ALPHA.lt.ALPHA_MIN ) then
C
C     Step size becomes too small - switch to feasibility restoration
C
         if( QCNR.gt.0 .and. QPRINT.ge.2 ) then
            write(line,*) 'filter: ALPHA         = ',ALPHA
            call C_OUT(1,2,1,line)
         endif
         if( IREJ_LAST.gt.0 ) then
            COUNT_ITER_BLOCK = COUNT_ITER_BLOCK + 1
            if( QITERBLOCKMAX.gt.0 .and.
     1           COUNT_ITER_BLOCK.gt.QITERBLOCKMAX) then
               call C_OUT(2,0,1,
     1              'filter: Seems that filter blocks progress.')
               if( CNRM_MAX.gt.10d0*CNRM0 ) then
                  write(line,*)
     1 '      Decrease max allowed constraint violation from ',CNRM_MAX
                  call C_OUT(2,0,1,line)
                  CNRM_MAX = CNRM_MAX/10d0
                  write(line,*)
     1               '      to ',CNRM_MAX,' and delete current filter.'
                  call C_OUT(2,0,1,line)
                  NFILTER = 0
CTODO Do we really want to continue with restoration phase here or rather
CTODO do the normal thing?
               else
                  call C_OUT(2,0,1,
     1                 '       However, cannot delete filter.')
               endif
            endif
         else
            COUNT_ITER_BLOCK = 0
         endif
C
C     Can forget about all previously allocated work space
C
         p_rwend = 0
         p_iwend = 0
         goto 3000
      endif

      goto 10

C ==============================================================================
C
C     Ende of loop
C
C ==============================================================================

C ==============================================================================
C
C     Point accepted
C
C ==============================================================================

 1000 continue

C
C     Check if filter block progress and needs to be deleted
C
      if( IREJ_LAST.gt.0 ) then
         COUNT_ITER_BLOCK = COUNT_ITER_BLOCK + 1
         if( QITERBLOCKMAX.gt.0 .and.
     1        COUNT_ITER_BLOCK.gt.QITERBLOCKMAX) then
            call C_OUT(2,0,1,
     1           'filter: It is possible that filter blocks progress.')
            if( CNRM_MAX.gt.10d0*CNRM0 ) then
               write(line,*)
     1'      Decrease max allowed constraint violation from ',CNRM_MAX
               call C_OUT(2,0,1,line)
               CNRM_MAX = CNRM_MAX/10d0
               write(line,*)
     1              '       to ',CNRM_MAX,' and delete current filter.'
               call C_OUT(2,0,1,line)
               NFILTER = 0
            else
               call C_OUT(2,0,1,
     1              '      However, cannot delete filter.')
            endif
         endif
      else
         COUNT_ITER_BLOCK = 0
      endif

C
C     Check if filter is to be augmented
C
      model_alpha = FILTER_MODEL(ALPHA, GD, CL, CDL)
      lhs   = phi_new - PHI0 - PRECFACT*MACHEPS*dabs(PHI0)
      rhs   = ETA_F*model_alpha
      if( model_alpha.ge.0.d0 ) then
         augment = .true.
      else
         augment = ( ((-model_alpha)**QS_F)*(ALPHA**(1.d0-QS_F))
     1        .le.DELTA*(CNRM0**QS_THETA) .or. lhs.gt.rhs)
      endif
      if( .not.augment ) then
         C_ACCEPT = 'f'
      else
         C_ACCEPT = 'h'
         phi_add  = PHI0 - QGAMMA_F*CNRM0
         cnrm_add = CNRM0 - QGAMMA_THETA*CNRM0
         call FILTER_ADD(NFILTER, RS(P_FILTER_C+1), RS(P_FILTER_PHI+1),
     1        cnrm_add, phi_add)
      endif
C
      if( SOC_FLAG.eq.1 ) then
         SOC_FLAG = 2
         if( augment ) then
            C_ACCEPT = 'H'
         else
            C_ACCEPT = 'F'
         endif
      endif
      if( WFLAG.eq.2 ) then
         if( augment ) then
            C_ACCEPT = 'J'
         else
            C_ACCEPT = 'j'
         endif
      endif
      WFLAG = 0
C
C     Copy trial point to real point
C
 1010 F = f_new
      call DCOPY(N,   RW(p_xnew +1), 1, X  , 1)
      call DCOPY(M,   RW(p_cnew +1), 1, C  , 1)
      call DCOPY(NLB, RW(p_slnew+1), 1, S_L, 1)
      call DCOPY(NUB, RW(p_sunew+1), 1, S_U, 1)
      if( abs(QMERIT).eq.5 ) then
         call DCOPY(M, RW(p_lamnew +1), 1, LAM, 1)
         call DCOPY(M, LAM, 1, LAMOLD, 1)
      endif
C
C     Do step in dual variables
C
      if( QALPHA.eq.0 ) then
         ALPHA_DUAL = ALPHA
      elseif( QALPHA.eq.1 ) then
         ALPHA_DUAL = min( ALPHA_DUAL, ALPHA )
      endif

      call DAXPY(NLB, ALPHA_DUAL, DV_L, 1, V_L, 1)
      call DAXPY(NUB, ALPHA_DUAL, DV_U, 1, V_U, 1)
C
C     Make sure that each V_NEW is at least MACHTINY
C
      do i = 1, NLB
         V_L(i) = dmax1(MACHTINY, V_L(i))
      enddo
      do i = 1, NUB
         V_U(i) = dmax1(MACHTINY, V_U(i))
      enddo
C
C     Free work space
C
      p_rwend = p_xnew
C
C     Take step in LAM
C
CTODO this also for QLAMBDA = 1 ??? Do we really need this here?
 1400 continue
      if( QLAMBDA.eq.2 .and. abs(QMERIT).ne.5 ) then
CTODO decide which option
CORIG         if( ITER.gt.0 ) then
         if( .true. ) then
            call DSCAL(M, ALPHA, LAM, 1)
            call DAXPY(M, 1d0-ALPHA, LAMOLD, 1, LAM, 1)
         endif
         call DCOPY(M, LAM, 1, LAMOLD, 1)
      endif
      NU_OUT = 0d0
      if( ALPHA.eq.1d0 ) then
         LSLACKS = .false.
      endif
C
C     That's it
C
      goto 9999
C
C ==============================================================================
C
C                      FEASIBILITY RESTORATION PHASE
C
C ==============================================================================
C
 3000 continue
      call C_OUT(1,2,1,'filter: ENTERING RESTORATION PHASE!')
      if( LSLACKS ) then
         call C_OUT(2,0,1,'filter: can''t do LSLACKS in restoration.')
         IERR = 4
         goto 9999
      endif
C
      if( abs(QMERIT).eq.5 .and. QRESTO.ne.2 ) then
         IERR = 4
         call C_OUT(2,0,1,'filter: Only TRON for QMERIT = +- 5')
         goto 9999
      endif
C
C     Augment filter
C
      phi_add  = PHI0 - QGAMMA_F*CNRM0
      cnrm_add = CNRM0 - QGAMMA_THETA*CNRM0
      call FILTER_ADD(NFILTER, RS(P_FILTER_C+1), RS(P_FILTER_PHI+1),
     1     cnrm_add, phi_add)
C
C     First try a restoration phase using the usual steps trying only to
C     reduce the primal and dual infeasibility
C
 3010 continue
      if( M.gt.0 .and. QRESTOKKTRED.gt.0.d0 .and. QLAMBDA.eq.2 ) then
         call RESTO_KKT(ITER, N, NIND, M, NORIG, XORIG,
     1        CSCALE, IVAR, NFIX, IFIX, NLB, ILB, NUB,
     1        IUB, BNDS_L, BNDS_U, MU, X, CNRM0, NFILTER,
     1        RS(P_FILTER_PHI+1), RS(P_FILTER_C+1),
     1        CNRM_MAX, LSLACKS, S_L, S_U, V_L, V_U, F, G, C,
     1        RESTO_KKT_FLAG, LAMOLD, LAM, DX, DV_L, DV_U,
     1        ALPHA, ALPHA_PRIMAL, ALPHA_DUAL,
     1        PRECFACT, MACHEPS, MACHTINY, NU_OUT, C_ACCEPT,
     1        KCONSTR, LRS, RS, LIS, IS, LRW-p_rwend, RW(p_rwend+1),
     1        LIW-p_iwend, IW(p_iwend+1), IERR)
         if( IERR.ne.0 ) then
            write(line,*) 'filter: RESTO_KKT returned IERR = ',IERR
            call C_OUT(2,0,1,line)
            goto 9999
         endif
         if( RESTO_KKT_FLAG.eq.0 ) then
            goto 9999           ! restoration iteration successful and
                                ! acceptable to filter
         elseif( RESTO_KKT_FLAG.eq.1 ) then
            goto 9999           ! restoration iteration successful, but not yet
                                ! acceptable to filter
         elseif( RESTO_KKT_FLAG.eq.2 ) then
            RESTO_KKT_FLAG = 0  ! need to do 'normal' restoration phase
         else
            write(line,*) 'filter: Invalid RESTO_KKT_FLAG = ',
     1           RESTO_KKT_FLAG
            call C_OUT(2,0,1,line)
            IERR = 943
            goto 9999
         endif
      endif

 3015 continue
      C_ALPHA = 'r'
      if( CNRM0.eq.0d0 ) then
         call C_OUT(2,0,1,
     1   'filter: actual point feasible; can''t do restoration. Abort')
         IERR = 16 !562
         goto 9999
      endif
CTODO
C      if( CNRM0.lt.QTOL ) then
C      if( CNRM0.eq.0d0 ) then
C         call C_OUT(2,0,1,
C     1  'filter: actual point almost feasible; can''t do restoration.')
CC         if( COUNT_CG.gt.3 ) then
C            IERR = 563
CC         else
CCTRY
CC            call C_OUT(2,0,1,'filter: OK, just delete entire filter...')
CC            COUNT_CG = COUNT_CG + 1
CC            LAST_MU = -1.d0
CC         endif
C         goto 9999
C      endif
C
C     Store current X (for step in dual variables after successful termination
C     of restoration phase
C
      p_xstore = p_rwend
      p_rwend  = p_xstore + N
      if( p_rwend.gt.LRW ) then
         IERR = 98
         goto 9999
      endif
      call DCOPY(N, X, 1, RW(p_xstore+1), 1)
C
C     Since the proposed step was not good, we don't want to accept the
C     freshly computed mutlipliers
C
      if( QLAMBDA.eq.2 ) then
         call DCOPY(M, LAMOLD, 1, LAM, 1)
      endif
      if( abs(QRESTO).eq.1 .or.
     1     (abs(QRESTO).eq.3.and.ITER_DOGLEG.lt.ITER_DOGLEG_MAX) ) then
      elseif( QRESTO.eq.2 .or. abs(QRESTO).eq.3 ) then
         RESTO = .false.
         COUNT_RESTO_CALL = COUNT_RESTO_CALL + 1
         p_xnew  = p_rwend
         p_cnew  = p_xnew + N
         p_rwend = p_cnew + M
         if( p_rwend.gt.LRW ) then
            IERR = 98
            goto 9999
         endif
         IERR = 0
 3020    ierr_store = IERR
         call RESTO_TRON(ITER, N, NIND, M, NORIG, XORIG,
     1        CSCALE, IVAR, NFIX, IFIX, NLB, ILB, NUB,
     1        IUB, BNDS_L, BNDS_U, MU, X, cnrm_new, NFILTER,
     1        RS(P_FILTER_PHI+1), RS(P_FILTER_C+1), CNRM_MAX, LSLACKS,
     1        S_L, S_U, V_L, V_U, F, C, PRECFACT, MACHEPS, MACHTINY,
     1        RW(p_xnew+1), RW(p_cnew+1), LAM,
     1        KCONSTR, LRS, RS, LIS, IS,
     1        LRW-p_rwend, RW(p_rwend+1), LIW-p_iwend,
     1        IW(p_iwend+1), IERR)
         if( IERR.eq.17 .or. IERR.eq.519 ) then
            if( ierr_store.eq.0 ) then
C
C     now solve feasibility problem with strict bounds!
C
               call C_OUT(2,0,1,
     1              'filter: Solve feasibility problem again with '//
     1              'non-relaxed bounds.')
               goto 3020
            endif
            ierr_store = IERR
C
C     Compute ||Ac||
C
            call GET_C(ITER, N, NIND, X, IVAR, NORIG, XORIG,
     1           M, CSCALE, C, KCONSTR, LRS, RS, LIS, IS,
     2           LRW-p_rwend, RW(p_rwend+1),
     4           LIW-p_iwend, IW(p_iwend+1), IERR)
            if( IERR.ne.0 ) then
               write(line,*)
     1              'filter: Error: get_c-tron returns IERR = ',IERR
               call C_OUT(2,0,1,line)
               goto 9999
            endif
            p_tmp   = p_rwend
            p_tmp2  = p_tmp  + N
            p_rwend = p_tmp2 + N
            if( p_rwend.gt.LRW ) then
               IERR = 98
               goto 9999
            endif
            call CONSTR(8, ITER, N, NIND, M, IVAR, NFIX, IFIX,
     1           NORIG, XORIG, CSCALE, C, RW(p_tmp+1), i, i,
     3           KCONSTR(1), RS(KCONSTR(2)+1), KCONSTR(3),
     4           IS(KCONSTR(4)+1), LRW-p_rwend, RW(p_rwend+1),
     5           LIW-p_iwend, IW(p_iwend+1), IERR)
            if( IERR.ne.0 ) then
               write(line,*)
     1              'filter: Error: constr8-tron returns IERR = ',IERR
               call C_OUT(2,0,1,line)
               goto 9999
            endif
            call DCOPY(N, 1.d0, 0, RW(p_tmp2+1), 1)
            do i = 1, NLB
               k = ILB(i)
               RW(p_tmp2+k) = min(RW(p_tmp2+k), S_L(i))
            enddo
            do i = 1, NUB
               k = IUB(i)
               RW(p_tmp2+k) = min(RW(p_tmp2+k), S_U(i))
            enddo
            do i = 1, N
               RW(p_tmp+i) = RW(p_tmp+i) * RW(p_tmp2+i)
            enddo
            cnrm_new = CALC_NRM(N, RW(p_tmp+1))
            write(line,*) '||XAc|| = ',cnrm_new
            call C_OUT(2,0,1,line)
            cnrm_new = CALC_NRM(M, C)
            if( cnrm_new.le.QTOL ) then
               IERR = 16
            else
               IERR = 17
            endif
            goto 9999
         elseif( ierr_store.ne.0 ) then
            IERR = 520
         endif
         if( IERR.gt.0 ) then
            write(line,*)
     1           'filter: Error: resto_tron returns IERR = ',IERR
            call C_OUT(2,0,1,line)
            goto 9999
         elseif( IERR.ne.0 ) then
            write(line,*)
     1           'filter: Warning: resto_tron returns IERR = ',IERR
            call C_OUT(2,0,1,line)
         endif
         if( QLAMBDA.eq.2 ) then
                                ! tell get_step_full to reinitialize LAMBDA
                                ! at next call
            call C_OUT(2,1,1,
     1           'filter: Request reinitialization of LAMBDA.')
            INIT_LAM = .true.
         endif
      elseif( QRESTO.eq.4 .or. QRESTO.eq.5 .or. QRESTO.eq.6 ) then
         if( QCNRM.ne.2 ) then
            call C_OUT(2,0,1,
     1         'filter: For now this restoration phase needs ICNRM = 2')
            IERR = 4
            goto 9999
         endif
C KEEP THE FOLLOWING LINES FOR TRON SUBSTITUTE
c$$$         call RES_MAIN(ITER, N, NIND, M, NORIG, XORIG,
c$$$     1        CSCALE, IVAR, NFIX, IFIX, NLB, ILB, NUB,
c$$$     1        IUB, BNDS_L, BNDS_U, MU, X, CNRM0, NFILTER,
c$$$     1        RS(P_FILTER_PHI+1), RS(P_FILTER_C+1), CNRM_MAX, LSLACKS,
c$$$     1        S_L, S_U, V_L, V_U, F, C, PRECFACT, MACHEPS, MACHTINY,
c$$$     1        KCONSTR, LRS, RS, LIS, IS,
c$$$     1        LRW-p_rwend, RW(p_rwend+1), LIW-p_iwend,
c$$$     1        IW(p_iwend+1), IERR)
c$$$         if( IERR.gt.0 ) then
c$$$            write(line,*)
c$$$     1           'filter: Error: res_main returns IERR = ',IERR
c$$$            call C_OUT(2,0,1,line)
c$$$            goto 9999
c$$$         elseif( IERR.ne.0 ) then
c$$$            write(line,*)
c$$$     1           'filter: Warning: res_main returns IERR = ',IERR
c$$$            call C_OUT(2,0,1,line)
c$$$            IERR = 0
c$$$         endif
      else
         write(line,*) 'filter: Invalid QRESTO = ',QRESTO
         call C_OUT(2,0,1,line)
         IERR = 4
         goto 9999
      endif
C
C     one step ?
C
      p_dl    = p_rwend
      p_du    = p_dl + NLB
      p_rwend = p_du + NUB
      if( p_rwend.gt.LRW ) then
         IERR = 98
         goto 9999
      endif
      alphad = 1.d0
      do i = 1, NLB
         k = ILB(i)
         RW(p_dl+i) = MU/(RW(p_xstore+k)-BNDS_L(i)) -
     1        SIGMA_L(i)*(X(k)-RW(p_xstore+k)) - V_L(i)
         if( RW(p_dl+i).lt.0.d0 ) then
            alphad = dmin1(alphad, QTAU*(-V_L(i)/RW(p_dl+i)) )
         endif
      enddo
      do i = 1, NUB
         k = IUB(i)
         RW(p_du+i) = MU/(BNDS_U(i)-RW(p_xstore+k)) +
     1        SIGMA_U(i)*(X(k)-RW(p_xstore+k)) - V_U(i)
         if( RW(p_du+i).lt.0.d0 ) then
            alphad = dmin1(alphad, QTAU*(-V_U(i)/RW(p_du+i)) )
         endif
      enddo
C This seems best option (?)
      call DAXPY( NLB, alphad, RW(p_dl+1), 1, V_L, 1)
      call DAXPY( NUB, alphad, RW(p_du+1), 1, V_U, 1)
      write(line,*) 'After Restoration: alpha(dual) = ', alphad
      call C_OUT(2,0,1,line)
      p_rwend = p_xstore
C
C     Make sure that dual variables don't differ too much from MU*X{-1}*e
C
      do i = 1, NLB
         dummy = V_L(i)
         V_L(i) = dmax1(V_L(i), MU/QVCORRECTFACT/S_L(i))
         V_L(i) = dmin1(V_L(i), MU*QVCORRECTFACT/S_L(i))
CTMP
         if( dummy.ne.V_L(i).and.QPRINT.gt.1 ) then
            write(line,*) 'filter: V_L(',i,') corrected from'
            call C_OUT(1,0,1,line)
            write(line,*) '           ',dummy,' to ',V_L(i)
            call C_OUT(1,0,1,line)
         endif
      enddo
      do i = 1, NUB
         dummy = V_U(i)
         V_U(i) = dmax1(V_U(i), MU/QVCORRECTFACT/S_U(i))
         V_U(i) = dmin1(V_U(i), MU*QVCORRECTFACT/S_U(i))
CTMP
         if( dummy.ne.V_U(i).and.QPRINT.gt.1 ) then
            write(line,*) 'filter: V_U(',i,') corrected from'
            call C_OUT(1,0,1,line)
            write(line,*) '           ',dummy,' to ',V_U(i)
            call C_OUT(1,0,1,line)
         endif
      enddo

 9000 continue
      NU_OUT = 0d0
c      C_ACCEPT = 'r'
      goto 9999
C
C     THE END
C
 9999 continue
      return
      end

C
C
C
      double precision function FILTER_MODEL(ALPHA, GD, CL, CDL)
C
C     Compute value of model for f-measure depending on ALPHA
C
      implicit none
      double precision ALPHA, GD, CL, CDL
      include 'IPOPT.INC'

      if( abs(QMERIT).eq.4 ) then
         FILTER_MODEL = ALPHA * GD
      elseif( abs(QMERIT).eq.5 ) then
         FILTER_MODEL = ALPHA * (GD-CL) +
     1        ALPHA*(1.d0-ALPHA) * CDL
      else
         call C_OUT(2,0,1,'filter_model: Invalid QMERIT. Abort')
         stop
      endif
      return
      end
