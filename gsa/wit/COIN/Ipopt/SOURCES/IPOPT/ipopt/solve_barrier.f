C Copyright (C) 2002, Carnegie Mellon University and others.
C All Rights Reserved.
C This code is published under the Common Public License.
C*******************************************************************************
C
      subroutine SOLVE_BARRIER(ITER, N, NIND, M, NORIG, XORIG, CSCALE,
     1     IVAR, NFIX, IFIX, NLB, ILB, NUB,
     1     IUB, BNDS_L, BNDS_U, MU, X, LSLACKS, S_L,
     1     S_U, V_L, V_U, F, G, C, ERR_BAR,
     1     ERR, ERR_DINF, ERR_CNSTR, ERR_CMPL, RV, SIGMA_L, SIGMA_U, B,
     1     W, WCORR, RG, RGOLD, RGB, YPY,
     1     THETA1, THETA2, NEWBAS, CONDC,
     1     PZ, ZPZ, LAM, LAMOLD, DX, DV_L, DV_U, GOLD, RESTO, INIT_LAM,
     1     KCONSTR, LRS, LRS_END, RS, LIS, LIS_END, IS,
     1     LRW, RW, LIW, IW, IERR)
C
C*******************************************************************************
C
C    $Id: solve_barrier.f,v 1.6 2003/04/11 02:55:24 andreasw Exp $
C
C-------------------------------------------------------------------------------
C                                 Title
C-------------------------------------------------------------------------------
C
CT    Solve barrier problem (to relaxed accuracy ERR_BAR)
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
CP   ITER      I    INT    iteration counter (if 0, initialize)
CP   N         I    INT    number of variables (without fixed)
CP   NIND      I    INT    number of independent variables
CP   M         I    INT    number of constraints
CP   NORIG    I/O   INT    total number of variables (incl. fixed vars)
CP   XORIG    I/O   INT    actual iterate
CP                            (original order as in problem statement)
CP   CSCALE    I    DP     scaling factors for constraints
CP   IVAR     I/O   INT    information about partitioning
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
CP                            (e.g. BNDS_L(i) is bound for X(ILB(i)) )
CP   NUB       I    INT    number of upper bounds (excluding fixed vars)
CP   IUB       I    INT    indices of upper bounds
CP                            (e.g. BNDS_U(i) is bound for X(IUB(i)) )
CP   BNDS_L   I/O   DP     values of lower bounds (ordered as S_L)
CP   BNDS_U   I/O   DP     values of upper bounds (ordered as S_U)
CP   MU        I    DP     barrier parameter
CP   X        I/O   DP     actual iterate (reordered without fixed vars:
CP                             first M entries belong to dependent
CP                             variables, remaining to independent variables)
CP   LSLACKS  I/O   LOG    =.true.: slacks violate linear slack constraint
CP   S_L      I/O   DP     slacks to lower bounds
CP   S_U      I/O   DP     slacks to upper bounds
CP   V_L      I/O   DP     dual variables for lower bounds
CP   V_U      I/O   DP     dual variables for upper bounds
CP   F        I/O   DP     value of objective function at X
CP                            (at first iteration assumed to be already
CP                             computed)
CP   G        I/O   DP     gradient of objective function
CP                            (at first iteration assumed to be already
CP                             computed)
CP   C        I/O   DP     values of constraints at X
CP                            (at first iteration assumed to be already
CP                             computed)
CP   ERR_BAR   I    DP     required tolerance for barrier problem
CP   ERR       O    DP     actual KKT-error of barrier problem
CP   ERR_DINF I/O   DP     unscaled max norm of dual infeasibility
CP                            (red. gradient in case of reduced space approach)
CP   ERR_CNSTR I/O  DP     unscaled max norm of infeasibility
CP   ERR_CMPL I/O   DP     unscaled max norm for relaxed complementarity
CP   RV       I/O   DP     reduced (V_U-V_L)
CP                            (needed for computation of KKT error)
CP                            (at first iteration assumed to be already
CP                             computed)
CP   SIGMA_L   S    DP     primal-dual Hessian of lower bound barrier term
CP                            (NLB diagonal elements only)
CP   SIGMA_U   S    DP     primal-dual Hessian of upper bound barrier term
CP                            (NUB diagonal elements only)
CP   B         S    DP     Quasi-Newton estimate of reduced Hessian of
CP                            original NLP
CP                         for CG: containts preconditioner
CP   W         S    DP     reduced Hessian of barrier problem
CP   WCORR     S    DP     correction term for null space step
CP   RG        S    DP     reduced gradient of objective function
CP   RGOLD     S    DP     reduced gradient of objective function of
CP                            last iteration
CP                            (for Quasi-Newton update)
CP   RGB       S    DP     reduced gradient of barrier function
CP   YPY      I/S   DP     range space step
CP                            (at first iteration assumed to be already
CP                             computed)
CP   THETA1   I/S   DP     dogleg parameter (=1 <-> full Newton step)
CP   THETA2   I/S   DP     dogleg parameter (damping of Cauchy step)
CP   NEWBAS    I    LOG    
CP   PZ        S    DP     null space step (independent variables)
CP   ZPZ       S    DP     null space step (dependent variables)
CP   LAM       S    DP     Lagrangian multipliers
CP   LAMOLD    S    DP     Lagrangian multipliers from last iteration
CP   DX        S    DP     step for X (primal)
CP   DV_L      S    DP     step for V_L (dual variables for lower bounds)
CP   DV_U      S    DP     step for V_U (dual variables for upper bounds)
CP   GOLD      S    DP     gradient from previous iteration
CP                            (only for QQUASI < 0 )
CP   RESTO    I/O   LOG    =.true.: we are in restoration phase
CP   INIT_LAM I/O   LOG    Indicates whether equality multipliers are to be
CP                           initialized (might be set to. false. in line
CP                           search) (for get_step_full)
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
CS    DNRM2
CS    ERROR
CS    GET_LAMBDA
CS    GET_SIGMA
CS    GET_BB
CS    UPDATE_B
CS    GET_STEP_FULL
CS    GET_STEP_RED
CS    GET_WCORR
CS    GET_ZPZ
CS    LINESEARCH
CS    GET_G
CS    GET_YPY
CS    GET_RG
CS    GET_RV
CS    ITER_OUT
CS    NEW_BASIS
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
      double precision CSCALE(*)
      integer IVAR(N)
      integer NFIX
      integer IFIX(NFIX)
      integer NLB
      integer ILB(NLB)
      integer NUB
      integer IUB(NUB)
      double precision BNDS_L(NLB)
      double precision BNDS_U(NUB)
      double precision MU
      double precision X(N)
      logical LSLACKS
      double precision S_L(NLB)
      double precision S_U(NUB)
      double precision V_L(NLB)
      double precision V_U(NUB)
      double precision F
      double precision G(N)
      double precision C(M)
      double precision ERR_BAR
      double precision ERR
      double precision ERR_DINF
      double precision ERR_CNSTR
      double precision ERR_CMPL
      double precision RV(NIND)
      double precision SIGMA_L(NLB)
      double precision SIGMA_U(NUB)
      double precision B(*)
      double precision W((NIND*(NIND+1))/2)
      double precision WCORR(NIND)
      double precision RG(NIND)
      double precision RGOLD(NIND)
      double precision RGB(NIND)
      double precision YPY(N)
      double precision THETA1
      double precision THETA2
      logical NEWBAS
      double precision CONDC
      double precision PZ(NIND)
      double precision ZPZ(M)
      double precision LAM(M)
      double precision LAMOLD(M)
      double precision DX(N)
      double precision DV_L(NLB)
      double precision DV_U(NUB)
      double precision GOLD(N)
      logical RESTO
      logical INIT_LAM
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
      logical SKIP_UPDATE
      double precision     ALPHA
      save    SKIP_UPDATE, ALPHA

      double precision alpha_cut, dummy, nu_out, cnrm0, regu, vin(2)
      double precision alpha_dual
      integer ls_count, ieigs
      character*1 c_err, c_alpha, c_watch, c_skip
      integer p_rwend, p_iwend, p_wypy, i, idummy

      integer ac_heuristic, soc_flag, iter_start
      integer p_rwfull, p_iwfull, prwkeep, piwkeep
      logical small_x_stop, newbas_store, ex, force_newbas

      double precision DNRM2

      double precision PYNRM_OLD
      save             PYNRM_OLD

      character*128 line

      integer ITER_LASTPRINT
      save    ITER_LASTPRINT

CTMP
      include 'TIMER.INC'
C
C*******************************************************************************
C
C                           Executable Statements
C
C*******************************************************************************
C
      p_rwend = 0
      p_iwend = 0

C Initialize flag for anti-crash heuristic
CTODO: DELETE THIS
      ac_heuristic = 0

C Store current iteration at start of solution of this barrier problem
      iter_start = ITER

C     This flag is set to true if the algorithm terminates because the
C     search direction is getting too small
      small_x_stop = .false.

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C                                                                       C
C         START MAIN LOOP FOR BARRIER PROBLEM                           C
C                                                                       C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC

 100  continue

      force_newbas = .false.

C Initialize second order correction flag
      soc_flag = 0

C
C     Compute Error for barrier problem with barrier parameter MU
C
      call ERROR(N, NIND, M, X, G, RG, RV, C, CSCALE, NLB, ILB, NUB,
     1     IUB, BNDS_L, BNDS_U, S_L, S_U, V_L, V_U, SIGMA_L, SIGMA_U,
     1     LAM, MU, LSLACKS, ERR, c_err, ERR_DINF, ERR_CNSTR, ERR_CMPL,
     1     dummy, ITER, IVAR, NFIX, IFIX, NORIG, XORIG,
     1     DX, KCONSTR, LRS, RS, LIS, IS,
     1     LRW-p_rwend, RW(p_rwend+1), LIW-p_iwend, IW(p_iwend+1),
     1     IERR)
      if( IERR.gt.0 ) then
         write(line,*) 
     1        'solve_barrier: Error returns IERR = ',IERR
         call C_OUT(2,0,1,line)
         goto 9999
      elseif( IERR.lt.0 ) then
         write(line,*)
     1        'solve_barrier: Error returns IERR = ',IERR
         call C_OUT(2,0,1,line)
         IERR = 0
      endif
C
C     Do the output
C
      if( ITER.eq.0 .or. ITER.ne.ITER_LASTPRINT ) then
         if( ITER.eq.0 ) then
            c_skip = ' '
            c_alpha = ' '
            c_watch = ' '
            ALPHA_DUAL = 0.d0
            alpha_cut = 0.d0
            ALPHA = 0.d0
            ls_count = 0
            nu_out = 0.d0
            ieigs = 0
            condc = 0.d0
         endif
         cnrm0 = DNRM2(M, C, 1)
         ITER_LASTPRINT = ITER
         call ITER_OUT(ITER, MU, ERR, c_err, ERR_DINF, ERR_CNSTR,
     1        ERR_CMPL, F, M, C, cnrm0, YPY, NIND, PZ, c_skip,
     1        ALPHA_DUAL,  alpha_cut, ALPHA, c_alpha, c_watch, ls_count,
     2        nu_out, ieigs, condc, LSLACKS,
     2        RV, RG, RGB, B, SIGMA_L, SIGMA_U, W, ZPZ, DX,
     3        X, S_L, S_U, V_L, V_U, N, NLB, ILB, NUB, IUB, IVAR, LAM,
     4        XORIG)
      endif
C
C     Check the error; if small enough leave this loop
C        (barrier problem solve well enough)
C
      if( (ERR.lt.ERR_BAR .or. small_x_stop) .and. .not.RESTO ) then
         if( small_x_stop ) then
            IERR = -2
         endif
C
C     Don't decrease MU if it just has been decreased
C
         if( ITER.eq.0 .or. ITER.gt.iter_start .or. small_x_stop)
     1        goto 9000
      endif
C
      if( QFULL.ne.1 ) then

         if( QCG.eq.0 .and.(QQUASI.ne.0 .or. RESTO) ) then
C
C     Compute reduced primal-dual Hessian for barrier part
C        (store it in W)
C
            call GET_BB(N, NIND, M, ITER, IVAR, NFIX, IFIX, NORIG,
     1           XORIG, CSCALE, NLB, ILB, NUB, IUB, SIGMA_L, SIGMA_U,
     1           S_L, S_U, W, RESTO, KCONSTR, LRS, RS, LIS, IS,
     2           LRW-p_rwend, RW(p_rwend+1),
     4           LIW-p_iwend, IW(p_iwend+1), IERR)
            if( IERR.gt.0 ) then
               write(line,*) 
     1              'solve_barrier: get_bb returns IERR = ',IERR
               call C_OUT(2,0,1,line)
               goto 9999
            elseif( IERR.lt.0 ) then
               write(line,*)
     1              'solve_barrier: get_bb returns IERR = ',IERR
               call C_OUT(2,0,1,line)
               IERR = 0
            endif
C     
         else
C
C     Compute exact reduced Hessian
C
            if( QCG.eq.0 ) then
               call GET_EXACTW(N, NIND, M, ITER, IVAR, NFIX, IFIX,
     1              NORIG, XORIG, X, CSCALE, LAM, NLB, ILB, NUB, IUB,
     2              S_L, S_U, SIGMA_L, SIGMA_U, W, RESTO,
     3              KCONSTR, LRS, RS, LIS, IS,
     2              LRW-p_rwend, RW(p_rwend+1),
     4              LIW-p_iwend, IW(p_iwend+1), IERR)
               if( IERR.gt.0 ) then
                  write(line,*) 
     1                 'solve_barrier: get_exactw returns IERR = ',IERR
                  call C_OUT(2,0,1,line)
                  goto 9999
               elseif( IERR.lt.0 ) then
                  write(line,*)
     1                 'solve_barrier: get_exactw returns IERR = ',IERR
                  call C_OUT(2,0,1,line)
                  IERR = 0
               endif
            endif
C
         endif
C
      endif
C
C     Calculate or initialize B (Quasi-Newton matrix)
C     and compute overall reduced Hessian (store it in W)
C     or update limited memory representation
C
      if( QQUASI.ne.0 .and..not.RESTO ) then
         if( QFULL.eq.0 .and. QCG.ne.0 ) then
            if( (abs(QCG).ne.1 .and. abs(QCG).ne.2) .or.
     1           (abs(QQUASI).ne.1 .and. abs(QQUASI).ne.5) ) then
               IERR = 4
               goto 9999
            endif
         endif
         if( abs(QQUASI).lt.6 ) then
            call UPDATE_B(N, NIND, M, X, ITER, ERR, PZ, IVAR, NFIX,
     1           IFIX, NORIG, XORIG, CSCALE, RG, RGOLD, PYNRM_OLD,
     2           dummy, ALPHA, G, GOLD, LAM, SKIP_UPDATE, B, W, c_skip,
     1           NLB, ILB, NUB, IUB, S_L, S_U, SIGMA_L, SIGMA_U,
     3           KCONSTR, LRS, RS, LIS, IS, LRW-p_rwend, RW(p_rwend+1),
     4           LIW-p_iwend, IW(p_iwend+1), IERR)
            if( IERR.gt.0 ) then
               write(line,*) 
     1              'solve_barrier: update_b returns IERR = ',IERR
               call C_OUT(2,0,1,line)
               goto 9999
            elseif( IERR.lt.0 ) then
               write(line,*)
     1              'solve_barrier: update_b returns IERR = ',IERR
               call C_OUT(2,0,1,line)
               IERR = 0
            endif
         else
            call UPDATE_B_LM(N, NIND, M, X, ITER, NFIX, IFIX, NORIG,
     1           XORIG, CSCALE, G, LAM, SKIP_UPDATE, B, C_SKIP,
     3           KCONSTR, LRS, RS, LIS, IS, LRW-p_rwend, RW(p_rwend+1),
     4           LIW-p_iwend, IW(p_iwend+1), IERR)
            if( IERR.ne.0 ) then
               write(line,*) 
     1              'solve_barrier: update_b_lm returns IERR = ',IERR
               call C_OUT(2,0,1,line)
               goto 9999
            endif
         endif
      else
         c_skip = ' '
      endif
C
C     Get space for WYPY
C
      if( abs(QMERIT).lt.3.and.QMERIT.ne.0.and. QFULL.eq.1 ) then
CTODO REMOVE THIS!!!!!

C
C     we do update of penalty parameter based on LAM, i.e. we need to
C     pass W Y pY to linesearch (only for full space approach)
C
         p_wypy  = p_rwend
         p_rwend = p_wypy + N   ! take the big one, for heuristic later...
         if( p_rwend.gt.LRW ) then
            IERR = 98
            goto 9999
         endif
      else
         p_wypy = 0
      endif
C
C     Compute primal and dual search direction
C
 1000 continue                  ! jump here returning for SOC
C
C     First make sure, that YPY is update to date for SOC
C
      if( SOC_FLAG.eq.1 .and. QFULL.eq.0 ) then
         ac_heuristic = 0
         call GET_YPY(N, NIND, M, ITER, IVAR, NFIX, IFIX,
     1        NORIG, XORIG, CSCALE, NLB, ILB, NUB, IUB,
     2        S_L, S_U, C, LSLACKS, ac_heuristic, YPY,
     3        THETA1, THETA2, NEWBAS, CONDC,
     1        KCONSTR, LRS, RS, LIS, IS,
     2        LRW-p_rwend, RW(p_rwend+1),
     4        LIW-p_iwend, IW(p_iwend+1), IERR)
         if( IERR.gt.0 ) then
            write(line,*)
     1       'solve_barrier: Error: get_ypy(soc) ends with IERR = ',IERR
            call C_OUT(2,0,1,line)
            goto 9999
         elseif( IERR.ne.0 ) then
            write(line,*)
     1     'solve_barrier: Warning: get_ypy(soc) ends with IERR = ',IERR
            call C_OUT(2,0,1,line)
         endif
      endif

      if( QFULL.eq.0 ) then

         newbas_store = NEWBAS
         call GET_STEP_RED(N, NIND, M, X, ITER, IVAR, NFIX, IFIX,
     1        NORIG, XORIG, CSCALE, LAM, NLB, ILB, NUB, IUB,
     1        LSLACKS, C, S_L, S_U, V_L, V_U,
     1        BNDS_L, BNDS_U, SIGMA_L, SIGMA_U, YPY, WCORR, RG, MU, RGB,
     1        B, W, PZ, ieigs, ZPZ, DX, DV_L, DV_U, ALPHA,
     1        alpha_dual, alpha_cut, c_alpha, THETA1, THETA2, soc_flag,
     1        NEWBAS, CONDC, RESTO, ERR, ERR_BAR,
     1        KCONSTR, LRS, RS, LIS, IS,
     1        LRW-p_rwend, RW(p_rwend+1),
     1        LIW-p_iwend, IW(p_iwend+1), IERR)
         if( IERR.gt.0 ) then
            write(line,*) 
     1           'solve_barrier: get_step_red returns IERR = ',IERR
            call C_OUT(2,0,1,line)
            goto 9999
         elseif( IERR.lt.0 ) then
            write(line,*)
     1           'solve_barrier: get_step_red returns IERR = ',IERR
            call C_OUT(2,0,1,line)
            IERR = 0
         endif
         if( .not.newbas_store .and. NEWBAS ) then
            force_newbas = .true.
         endif

      elseif( QFULL.eq.1 ) then

         if( QSOC.eq.0 .or. soc_flag.eq.0 ) then
            p_rwfull = p_rwend
            p_iwfull = p_iwend
         endif
         call GET_STEP_FULL(ITER, NORIG, N, NIND, M, idummy, X,
     1        XORIG, CSCALE, B, NLB, ILB, NUB, IUB, NFIX, IFIX, IVAR,
     1        G, C, S_L, S_U, V_L, V_U, BNDS_L, BNDS_U, LSLACKS,
     1        SIGMA_L, SIGMA_U, MU, DX, DV_L, DV_U, LAM,
     1        LAMOLD, PZ, ALPHA,
     1        alpha_dual, alpha_cut, c_alpha, NEWBAS, ieigs, YPY, regu, 
     1        RW(p_wypy+1), WCORR, THETA1, THETA2, soc_flag, ERR_BAR,
     1        ERR_CNSTR, RESTO,  prwkeep, piwkeep, INIT_LAM,
     1        KCONSTR, LRS, LRS_END, RS, LIS, LIS_END, IS,
     1        LRW-p_rwfull, RW(p_rwfull+1),
     1        LIW-p_iwfull, IW(p_iwfull+1), IERR)
         if( IERR.gt.0 ) then
            write(line,*) 
     1           'solve_barrier: get_step_full returns IERR = ',IERR
            call C_OUT(2,0,1,line)
            goto 9999
         elseif( IERR.lt.0 ) then
            write(line,*)
     1           'solve_barrier: get_step_full returns IERR = ',IERR
            call C_OUT(2,0,1,line)
            IERR = 0
         endif
         if( QMERIT.eq.3.and.QAUGITER.ne.0 ) then
            call C_OUT(2,0,1,'Currently, QAUGITER only allowed to be 0')
            IERR = 4
            goto 9999
         endif

         if( QSOC.ne.0 .and. soc_flag.eq.0 .and. QMERIT.ne.3 ) then
C
C     readjust p_rwend and p_iwend if we need to keep entries in RW and IW
C     for later call of get_step_full for SOC
C
            p_rwend = p_rwfull + prwkeep
            p_iwend = p_iwfull + piwkeep
         endif
      else
         call C_OUT(2,0,1,'solve_barrier: invalid choice of QFULL!')
         stop
      endif
C
C     Do the line search and obtain new point (and F and C at new point)
C
CTODO decide which one!
C      if( abs(QMERIT).eq.1 .or. abs(QMERIT).eq.2 ) then
      if( abs(QMERIT).eq.1 .or. abs(QMERIT).eq.2 .or.
     1     (QMERIT.eq.3.and.ITER.lt.QAUGITER) .or. QMERIT.eq.0 ) then
         call LINESEARCH(ITER, N, NIND, M, X, IVAR, NLB, ILB, NUB,
     1        IUB, LSLACKS,
     1        BNDS_L, BNDS_U, DX, DV_L, DV_U, S_L, S_U,
     2        V_L, V_U, SIGMA_L, SIGMA_U, NORIG, XORIG, CSCALE, MU,
     3        ERR, YPY, THETA1, THETA2, LAM, LAMOLD, regu,
     2        PZ, ZPZ, G, WCORR, F, C, cnrm0, NEWBAS,
     3        ALPHA, alpha_dual, ls_count, c_watch, nu_out,
     4        SKIP_UPDATE, soc_flag, KCONSTR,
     5        LRS, LRS_END, RS, LIS, LIS_END, IS,
     2        LRW-p_rwend, RW(p_rwend+1),
     4        LIW-p_iwend, IW(p_iwend+1), IERR)
         if( IERR.gt.0 ) then
            write(line,*) 
     1           'solve_barrier: linesearch returns IERR = ',IERR
            call C_OUT(2,0,1,line)
            goto 9999
         elseif( IERR.lt.0 ) then
            write(line,*)
     1           'solve_barrier: linesearch returns IERR = ',IERR
            call C_OUT(2,0,1,line)
            IERR = 0
         endif
C
C     Check if SOC step computation is requested
C
         if( soc_flag.eq.1 ) goto 1000
C
C     Can forget KKT factorization
C
         if( QSOC.ne.0 .and. QFULL.eq.1 ) then
            p_rwend = p_rwfull
            p_iwend = p_iwfull
         endif
C
C     Don't need to remember WYPY anymore
C
         if( QMERIT.ne.3.and.QMERIT.ne.0.and.
     1        QFULL.eq.1 ) then
            p_rwend = p_wypy    ! don't need this any longer
         endif

      elseif( QMERIT.eq.3 ) then
         c_watch = ' '
         call AUG_LAG(ITER, N, NIND, M, X, IVAR, NLB, ILB, NUB,
     1        IUB, LSLACKS, BNDS_L, BNDS_U, DX, DV_L, DV_U, S_L, S_U,
     2        V_L, V_U, NORIG, XORIG, CSCALE, MU, LAM, LAMOLD, G, F, C,
     3        cnrm0, ALPHA, alpha_dual, ls_count, nu_out, KCONSTR,
     5        LRS, RS, LIS, IS,
     2        LRW-p_rwend, RW(p_rwend+1),
     4        LIW-p_iwend, IW(p_iwend+1), IERR)
         if( IERR.gt.0 ) then
            write(line,*) 'solve_barrier: aug_lag returns IERR = ',IERR
            call C_OUT(2,0,1,line)
            goto 9999
         elseif( IERR.lt.0 ) then
            write(line,*) 'solve_barrier: aug_lag returns IERR = ',IERR
            call C_OUT(2,0,1,line)
            IERR = 0
         endif
      elseif( abs(QMERIT).ge.4 ) then
         call FILTER(ITER, N, NIND, M, X, IVAR, NFIX, IFIX, NLB, ILB,
     1        NUB, IUB, LSLACKS, BNDS_L, BNDS_U, DX, DV_L, DV_U, S_L,
     2        S_U, V_L, V_U, SIGMA_L, SIGMA_U, PZ, NORIG, XORIG, CSCALE,
     2        MU, LAM, LAMOLD, G, F, C, cnrm0, ALPHA, alpha_dual,
     3        THETA1, THETA2, RESTO, ls_count, nu_out, c_alpha, c_watch,
     5        soc_flag, INIT_LAM, KCONSTR, LRS, RS, LIS, IS,
     2        LRW-p_rwend, RW(p_rwend+1),
     4        LIW-p_iwend, IW(p_iwend+1), IERR)
CTODO maybe jump back to step computation if just switched to restoration phase
         if( IERR.gt.0 ) then
            write(line,*) 'solve_barrier: filter returns IERR = ',IERR
            call C_OUT(2,0,1,line)
            goto 9999
         elseif( IERR.eq.-2 ) then
            small_x_stop = .true.
            IERR = 0
         elseif( IERR.lt.0 ) then
            write(line,*) 'solve_barrier: filter returns IERR = ',IERR
            call C_OUT(2,0,1,line)
            IERR = 0
         endif
C
C     Check if SOC step computation is requested
C
         if( soc_flag.eq.1 ) goto 1000
C
C     Can forget KKT factorization
C
         if( QSOC.ne.0 .and. QFULL.eq.1 ) then
            p_rwend = p_rwfull
            p_iwend = p_iwfull
         endif
      else
         call C_OUT(2,0,1,'solve_barrier: Invalid QMERIT!')
         IERR = 4
         goto 9999
      endif
C     
C     Compute dual multipliers for the primal case (This version is for
C     comparison only, thus it doesn't matter if this is an inefficient
C     implementation)
C
      if( QPRIMAL.eq.1 ) then
         do i = 1, NLB
            V_L(i) = MU/S_L(i)
         enddo
         do i = 1, NUB
            V_U(i) = MU/S_U(i)
         enddo
      else
CC
CC     Make sure that dual variables don't differ too much from MU*X{-1}*e
CC
C         do i = 1, NLB
C            dummy = V_L(i)
C            V_L(i) = dmax1(V_L(i), MU/QVCORRECTFACT/S_L(i))
C            V_L(i) = dmin1(V_L(i), MU*QVCORRECTFACT/S_L(i))
CCTMP
C            if( dummy.ne.V_L(i) ) then
C               write(line,*) 'solve_barrier: V_L(',i,') corrected from'
C               call C_OUT(2,0,1,line)
C               write(line,*) '           ',dummy,' to ',V_L(i)
C               call C_OUT(2,0,1,line)
C            endif
C         enddo
C         do i = 1, NUB
C            dummy = V_U(i)
C            V_U(i) = dmax1(V_U(i), MU/QVCORRECTFACT/S_U(i))
C            V_U(i) = dmin1(V_U(i), MU*QVCORRECTFACT/S_U(i))
CCTMP
C            if( dummy.ne.V_U(i) ) then
C               write(line,*) 'solve_barrier: V_U(',i,') corrected from'
C               call C_OUT(2,0,1,line)
C               write(line,*) '           ',dummy,' to ',V_U(i)
C               call C_OUT(2,0,1,line)
C            endif
C         enddo
      endif
C
C     Store old RG
C
      if( (QQUASI.ne.0.and.abs(QQUASI).lt.6) .and.
     1     .not.small_x_stop ) then
         if( QQUASI.gt.0 ) then
            call DCOPY(NIND, RG, 1, RGOLD, 1)
         else
            call DCOPY(N, G, 1, GOLD, 1)
            if( M.gt.0 ) then
               call CONSTR(11, ITER, N, NIND, M, IVAR, NFIX, IFIX,
     1              NORIG, XORIG, CSCALE, dummy, dummy, idummy, idummy,
     2              KCONSTR(1), RS(KCONSTR(2)+1), KCONSTR(3),
     4              IS(KCONSTR(4)+1), LRW-p_rwend, RW(p_rwend+1),
     5              LIW-p_iwend, IW(p_iwend+1), IERR)
               if( IERR.gt.0 ) then
                  write(line,*) 
     1                 'solve_barrier: constr-11 returns IERR = ',IERR
                  call C_OUT(2,0,1,line)
                  goto 9999
               elseif( IERR.lt.0 ) then
                  write(line,*)
     1                 'solve_barrier: constr-11 returns IERR = ',IERR
                  call C_OUT(2,0,1,line)
                  IERR = 0
               endif
            endif
         endif
      elseif( abs(QQUASI).ge.6 ) then
         if( M.gt.0 ) then
            call CONSTR(11, ITER, N, NIND, M, IVAR, NFIX, IFIX,
     1           NORIG, XORIG, CSCALE, dummy, dummy, idummy, idummy,
     2           KCONSTR(1), RS(KCONSTR(2)+1), KCONSTR(3),
     4           IS(KCONSTR(4)+1), LRW-p_rwend, RW(p_rwend+1),
     5           LIW-p_iwend, IW(p_iwend+1), IERR)
            if( IERR.gt.0 ) then
               write(line,*) 
     1              'solve_barrier: constr-11 returns IERR = ',IERR
               call C_OUT(2,0,1,line)
               goto 9999
            elseif( IERR.lt.0 ) then
               write(line,*)
     1              'solve_barrier: constr-11 returns IERR = ',IERR
               call C_OUT(2,0,1,line)
               IERR = 0
            endif
         endif
      endif
C
C     Store old pY norm
C
      if( M.gt.0 ) THEN
CTODO norm of WHOLE YPY (incl indep vars)?
         PYNRM_OLD = DNRM2(M, YPY, 1)
      else
         PYNRM_OLD = 0.d0
      endif
C     
C     Increase iteration counter
C
      ITER = ITER + 1
C
C     Check if there is a forceful stop
C
      inquire(file='STOP', exist=ex)
      if( ex ) then
         call C_OUT(2,0,1,'File ''STOP'' detected.  Abort.')
         IERR = 123
         goto 9999
      endif
C
C     Check maximal number of iterations
C
      if( ITER.eq.QMAXITER .and. .not.small_x_stop ) then
         write(line,7000) ITER
 7000    format('Maximal number of iterations (',i6,') exceeded!')
         call C_OUT(2,0,1,line)
C The following lines might be more appropriate for interactive usage.
C         writee(*,*) 'Please enter new MAXITER (0: quit)'
C         read(*,*) QMAXITER
         if( QMAXITER.le.ITER ) then
            IERR = 1
            goto 9999
         endif
      endif
C
C     Compute range space step PY at new point
C
      if( QFULL.eq.0 .or. (QQUASI.ne.0.and.abs(QQUASI).lt.6)
     1     .or. QLAMBDA.eq.0 ) then
CORIG     1     (QLAMBDA.eq.2.and.abs(QMERIT).lt.3.and.QMERIT.ne.0) ) then
                                ! also need this if NU based on LAM
         ac_heuristic = 0
         call GET_YPY(N, NIND, M, ITER, IVAR, NFIX, IFIX,
     1        NORIG, XORIG, CSCALE, NLB, ILB, NUB, IUB,
     2        S_L, S_U, C, LSLACKS, ac_heuristic, YPY,
     3        THETA1, THETA2, NEWBAS, CONDC,
     1        KCONSTR, LRS, RS, LIS, IS,
     2        LRW-p_rwend, RW(p_rwend+1),
     4        LIW-p_iwend, IW(p_iwend+1), IERR)
         if( IERR.gt.0 ) then
            write(line,*)
     1           'solve_barrier: Error: get_ypy ends with IERR = ',IERR
            call C_OUT(2,0,1,line)
            goto 9999
         elseif( IERR.ne.0 ) then
            write(line,*)
     1          'solve_barrier: Warning: get_ypy ends with IERR = ',IERR
            call C_OUT(2,0,1,line)
         endif
C     
C     if necessary get new partition and reorder everything
C
         if( NEWBAS .or. force_newbas ) then
C     
C     Get new partition and reoder everything
C     
            call C_OUT(2,0,1,'Changing Basis!')
CTODO Check if OK...
C            if( RESTO ) then
C               write(*,*) 'Don''t know yet for RESTO.'
C               stop
C            endif
            call NEW_BASIS(ITER, N, M, NIND, NORIG, X, XORIG, CSCALE,
     1           NFIX, IFIX, IVAR, NLB, ILB, NUB, IUB,
     2           S_L, S_U, C, LAM, LAMOLD,
     2           LSLACKS, YPY, THETA1, THETA2, ALPHA,
     3           DX, PZ, G, RGOLD, B, SKIP_UPDATE,
     3           KCONSTR, LRS, RS, LIS, IS,
     2           LRW-p_rwend, RW(p_rwend+1),
     4           LIW-p_iwend, IW(p_iwend+1), IERR)
            if( IERR.gt.0 ) then
               write(line,*)
     1              'solve_barrier: Error: new_basis return IERR = ',
     2              IERR
               call C_OUT(2,0,1,line)
               goto 9999
            elseif( IERR.ne.0 ) then
               write(line,*)
     1              'solve_barrier: Warning: new_basis returns IERR = ',
     2              IERR
               call C_OUT(2,0,1,line)
            endif
         endif
      endif
C
C     Get G at new point
C
CRES need this for restoration? (for error...)
      call GET_G(N, X, IVAR, NORIG, XORIG, G, LRW-p_rwend,
     2     RW(p_rwend+1), IERR)
      if( IERR.gt.0 ) then
         write(line,*)
     1        'solve_barrier: Error: get_g ends with IERR = ',IERR
         call C_OUT(2,0,1,line)
         goto 9999
      elseif( IERR.ne.0 ) then
         write(line,*)
     1        'solve_barrier: Warning: get_g ends with IERR = ',IERR
         call C_OUT(2,0,1,line)
      endif
C
C     Compute reduced gradient of objective function RG
C
CRES do we update during restoration?
      if( QFULL.eq.0 .or. (QQUASI.ne.0.and.abs(QQUASI).lt.6)
     1     .or. abs(QERROR).eq.1 ) then
         call GET_RG(N, NIND, M, ITER, IVAR, NFIX, IFIX,
     1        NORIG, XORIG, CSCALE, G, RG,
     1        KCONSTR, LRS, RS, LIS, IS,
     2        LRW-p_rwend, RW(p_rwend+1),
     4        LIW-p_iwend, IW(p_iwend+1), IERR)
         if( IERR.gt.0 ) then
            write(line,*)
     1           'solve_barrier: Error: get_rg ends with IERR = ',IERR
            call C_OUT(2,0,1,line)
            goto 9999
         elseif( IERR.ne.0 ) then
            write(line,*)
     1           'solve_barrier: Warning: get_rg ends with IERR = ',IERR
            call C_OUT(2,0,1,line)
         endif
      endif
C     
C     Compute "reduced" dual variables RV
C
      if( QFULL.eq.0 .or. abs(QERROR).eq.1 ) then
         call GET_RV(N, NIND, M, ITER, IVAR, NFIX, IFIX,
     1        NORIG, XORIG, CSCALE,
     1        NLB, ILB, NUB, IUB, V_L, V_U, RV,
     1        KCONSTR, LRS, RS, LIS, IS,
     2        LRW-p_rwend, RW(p_rwend+1),
     4        LIW-p_iwend, IW(p_iwend+1), IERR)
         if( IERR.gt.0 ) then
            write(line,*)
     1           'solve_barrier: Error: get_rv ends with IERR = ',IERR
            call C_OUT(2,0,1,line)
            goto 9999
         elseif( IERR.ne.0 ) then
            write(line,*)
     1           'solve_barrier: Warning: get_rv ends with IERR = ',IERR
            call C_OUT(2,0,1,line)
         endif
      endif
C
C     For QLAMBDA = 1: get coordinate multipliers
C
      if( QLAMBDA.eq.1 ) then
         call GET_LAMBDA(N, NIND, M, ITER, IVAR, NFIX, IFIX,
     1        NORIG, XORIG, CSCALE, G, NLB, ILB, NUB, IUB, V_L, V_U,
     1        LSLACKS, LAM, KCONSTR, LRS, RS, LIS, IS,
     1        LRW-p_rwend, RW(p_rwend+1), LIW-p_iwend, IW(p_iwend+1),
     1        IERR)
         if( IERR.gt.0 ) then
            write(line,*) 
     1           'solve_barrier: get_lambda returns IERR = ',IERR
            call C_OUT(2,0,1,line)
            goto 9999
         elseif( IERR.lt.0 ) then
            write(line,*)
     1           'solve_barrier: get_lambda returns IERR = ',IERR
            call C_OUT(2,0,1,line)
            IERR = 0
         endif
      endif
C
C     Compute primal dual Hessian for barrier part SIGMA_L, SIGMA_U
C
CRES Need this during restoration?
      if( .not.RESTO ) then
         call GET_SIGMA(NLB, NUB, S_L, S_U, V_L, V_U, SIGMA_L, SIGMA_U)
      endif
C
C     Now we have everything to start a new round in our loop
C
      goto 100

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C                                                                       C
C     END OF WHILE LOOP                                                 C
C                                                                       C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC

C
C     Prepare exit
C
 9000 continue

 9999 continue
      return
      end
