C Copyright (C) 2002, Carnegie Mellon University and others.
C All Rights Reserved.
C This code is published under the Common Public License.
C*******************************************************************************
C
      subroutine GET_STEP_RED(N, NIND, M, X, ITER, IVAR, NFIX, IFIX,
     1     NORIG, XORIG, CSCALE, LAM, NLB, ILB, NUB, IUB,
     1     LSLACKS, C, S_L, S_U, V_L, V_U,
     1     BNDS_L, BNDS_U, SIGMA_L, SIGMA_U, YPY, WCORR, RG, MU, RGB,
     1     B, W, PZ, IEIGS, ZPZ, DX, DV_L, DV_U, ALPHA,
     1     ALPHA_DUAL, ALPHA_CUT, C_ALPHA, THETA1, THETA2, SOC_FLAG,
     1     NEWBAS, CONDC, RESTO, ERR, ERR_BAR,
     1     KCONSTR, LRS, RS, LIS, IS, LRW, RW, LIW, IW, IERR)
C
C*******************************************************************************
C
C    $Id: get_step_red.f,v 1.1 2002/05/02 18:52:17 andreasw Exp $
C
C-------------------------------------------------------------------------------
C                                 Title
C-------------------------------------------------------------------------------
C
CT    Compute primal and dual steps from reduced system
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
CP   N         I    INT    number of (free) variables; first NIND variables
CP                         are independent; remaining dependent
CP   NIND      I    INT    number of independent variables
CP   M         I    INT    number of dependent variables
CP   X         I    DP     actual primal iterate
CP   ITER      I    INT    iteration counter
CP   IVAR      I    INT    information about partitioning
CP                            i = 1..M      XORIG(IVAR(i)) dependent
CP                            i = (M+1)..N  XORIG(IVAR(i)) independent
CP                            Note: fixed variables do not occur in IVAR
CP                            X(i) corresponds to XORIG(IVAR(i))
CP   NFIX      I    INT    number of fixed variables
CP   IFIX      I    INT    specifies variables that are fixed by bounds:
CP                            i = 1..NORIG-N   XORIG(IFIX(i)) is fixed
CP                            (assumed to be in increasing order)
CP   NORIG     I    INT    total number of variables (incl. fixed vars)
CP   XORIG     I    INT    actual iterate
CP                            (original order as in problem statement)
CP   CSCALE    I    DP     scaling factors for constraints
CP   LAM       I    DP     Lagrangian multipliers
CP   NLB       I    INT    number of lower bounds (excluding fixed vars)
CP   ILB       I    INT    indices of lower bounds
CP                            (e.g. S_L(i) is slack for X(ILB(i)) )
CP   NUB       I    INT    number of upper bounds (excluding fixed vars)
CP   IUB       I    INT    indices of upper bounds
CP                            (e.g. S_U(i) is slack for X(IUB(i)) )
CP   LSLACKS   I    LOG    =.true.: There are slacks that don't satisfy
CP                                  "slack equation"
CP   C         I    DP     values of constraints at X
CP   S_L       I    DP     slack variables for lower bounds
CP   S_U       I    DP     slack variables for upper bounds
CP   V_L       I    DP     dual variables for lower bounds
CP   V_U       I    DP     dual variables for upper bounds
CP   BNDS_L    I    DP     values of lower bounds (ordered as S_L)
CP   BNDS_U    I    DP     values of upper bounds (ordered as S_U)
CP   SIGMA_L   I    DP     primal-dual Hessian of lower bound barrier term
CP                            (NLB diagonal elements only)
CP   SIGMA_U   I    DP     primal-dual Hessian of upper bound barrier term
CP                            (NUB diagonal elements only)
CP   YPY       I    DP     range space step (ordered like X)
CP   WCORR     O    DP     correction term for null space step
CP   RG        I    DP     reduced gradient of objective function
CP   MU        I    DP     barrier parameter
CP   RGB       O    DP     reduced gradient of barrier function
CP   B         I    DP     Quasi-Newton estimate of reduced Hessian of
CP                            original NLP
CP                         for CG: contains preconditioner
CP   W         I    DP     reduced Hessian of barrier problem
CP   PZ        O    DP     null space step (indepentent variables)
CP   IEIGS     O    INT    number of negative eigenvalues in overall
CP                            reduced Hessian
CP   ZPZ       O    DP     null space step (dependent variables)
CP   DX        O    DP     step for X (primal)
CP   DV_L      O    DP     step for V_L (dual variables for lower bounds)
CP   DV_U      O    DP     step for V_U (dual variables for upper bounds)
CP   ALPHA     O    DP     maximal steps size compatible with fraction
CP                            to boudary rule (primal vars)
CP   ALPHA_DUAL O   DP     maximal steps size compatible with fraction
CP                            to boudary rule (dual vars)
CP   ALPHA_CUT O    DP     first alpha on output
CP   C_ALPHA   O    C*1    for output on ALPHA
CP   THETA1   I/O   DP     for computation of NU
CP                            (I: corresponding to incoming YPY)
CP   THETA2   I/O   DP     for computation of NU
CP                            (I: corresponding to incoming YPY)
CP   SOC_FLAG  I    INT    =0: not in SOC computation
CP                          1: in SOC computation (don't need to refactorize)
CP   NEWBAS    O    LOG    =.true.: Basis has become pretty bad; get new one!
CP                            (this is determined during heuristic!)
CP   CONDC     O    DP     estimated condition number of basis matrix C
CP   RESTO     I    LOG    =.true.: we are in restoration phase
CP   ERR       I    DP     current KKT error for barrier problem
CP   ERR_BAR   I    DP     KKT error tolerance for barrier problem
CP   KCONSTR   I    INT    KCONSTR(1): LRS for CONSTR
CP                         KCONSTR(2): P_LRS for CONSTR
CP                         KCONSTR(3): LIS for CONSTR
CP                         KCONSTR(4): P_LIS for CONSTR
CP                         KCONSTR(5): LRW for CONSTR
CP                         KCONSTR(6): LIW for CONSTR
CP   LRS       I    INT    total length of RS
CP   RS        S    DP     DP storage space (all!)
CP   LIS       I    INT    total length of IS
CP   IS        S    INT    INT storage space (all!)
CP   LRW       I    INT    length of RW
CP   RW        W    DP     can be used as DP work space but content will be
CP                            changed between calls
CP   LIW       I    INT    length of IW
CP   IW        W    INT    can be used as INT work space but content will be
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
CS    DAXPY
CS    DASUM
CS    DSCAL
CS    DNRM2
CS    GET_WCORR
CS    GET_RGB
CS    GET_PZ
CS    GET_PZ_CG
CS    GET_ZPZ
CS    GET_D
CS    CUTALPHA
CS    GET_YPY
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
      double precision LAM(M)
      integer NLB
      integer ILB(NLB)
      integer NUB
      integer IUB(NUB)
      logical LSLACKS
      double precision C(M)
      double precision S_L(NLB)
      double precision S_U(NUB)
      double precision V_L(NLB)
      double precision V_U(NUB)
      double precision BNDS_L(NLB)
      double precision BNDS_U(NUB)
      double precision SIGMA_L(NLB)
      double precision SIGMA_U(NUB)
      double precision YPY(N)
      double precision WCORR(NIND)
      double precision RG(NIND)
      double precision MU
      double precision RGB(NIND)
      double precision B( (NIND*(NIND+1))/2 )
      double precision W( (NIND*(NIND+1))/2 )
      double precision PZ(NIND)
      integer IEIGS
      double precision ZPZ(M)
      double precision DX(N)
      double precision DV_L(NLB)
      double precision DV_U(NUB)
      double precision ALPHA
      double precision ALPHA_DUAL
      double precision ALPHA_CUT
      character*1 C_ALPHA
      double precision THETA1
      double precision THETA2
      integer SOC_FLAG
      logical NEWBAS
      double precision CONDC
      double precision ERR
      double precision ERR_BAR
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
      integer p_rwend, p_iwend, p_newtondx, p_newtondvl, p_newtondvu
      integer p_newtonwcorr, p_newtonpz, p_newtonypy, ac_heuristic
      integer p_dl, p_tmp
      integer i, k, idummy

      double precision cnrm, tau, alphap ,alphad, alpha_dl
      double precision dxi, dvi, theta1_tmp, theta2_tmp, condc_tmp
      double precision nrm_dl, nrm_nw
      double precision DASUM, DNRM2
      logical newbas_tmp
      character*80 line

      double precision REGU
      save             REGU

C
C*******************************************************************************
C
C                           Executable Statements
C
C*******************************************************************************
C
      IERR = 0
      p_rwend = 0
      p_iwend = 0
      ac_heuristic = 0   ! initalize flag for anti-crash heuristic
C
C     If we are in computation of SOC step, for now skip computation of
C     new PZ and jump directly to computation of overall D
C
      if( SOC_FLAG.eq.1 ) goto 1000
C
C     Jump back to here, if range space step has been recomputed
C     for anti-crash heuristic
C
 120  continue
C
C     Compute correction term for null space step WCORR
C     (if in second order correction step computation, no need to recompute
C      wcorr if we use pcg, since here SOC does not change PZ for simplicity,
C      only the YPY part is changed.)
C
      if( SOC_FLAG.ne.1 .or. QCG.eq.0 ) then
         call GET_WCORR(N, NIND, M, X, ITER, IVAR, NFIX, IFIX,
     1        NORIG, XORIG, CSCALE, LAM,
     1        NLB, ILB, NUB, IUB, LSLACKS,
     2        S_L, S_U, BNDS_L, BNDS_U,
     1        SIGMA_L, SIGMA_U, YPY, 0d0, WCORR, RESTO,
     1        KCONSTR, LRS, RS, LIS, IS,
     2        LRW-p_rwend, RW(p_rwend+1),
     4        LIW-p_iwend, IW(p_iwend+1), IERR)
         if( IERR.lt.0 ) then
            write(line,*)
     1           'get_step_red: Warning in get_wcorr, IERR = ',IERR
            call C_OUT(2,0,1,line)
         elseif( IERR.ne.0 ) then
            write(line,*)
     1           'get_step_red: Error in get_wcorr, IERR = ',IERR
            call C_OUT(2,0,1,line)
            goto 9999
         endif
      endif
C
C     Get reduced gradient of barrier function RGB
C
      if( .not.RESTO .and. .not.SOC_FLAG.eq.1 ) then
         call GET_RGB(N, NIND, M, ITER, IVAR, NFIX, IFIX,
     1        NORIG, XORIG, CSCALE, RG,
     1        NLB, ILB, NUB, IUB, S_L, S_U,
     1        MU, RGB, KCONSTR, LRS, RS, LIS, IS,
     2        LRW-p_rwend, RW(p_rwend+1),
     4        LIW-p_iwend, IW(p_iwend+1), IERR)
         if( IERR.lt.0 ) then
            write(line,*)
     1           'get_step_red: Warning in get_rgb, IERR = ',IERR
            call C_OUT(2,0,1,line)
         elseif( IERR.ne.0 ) then
            write(line,*)
     1           'get_step_red: Error in get_rgb, IERR = ',IERR
            call C_OUT(2,0,1,line)
            goto 9999
         endif
      endif
C
C     Compute null step PZ
C
      if( SOC_FLAG.ne.1 ) IEIGS = 0
      if( QCG.ne.0 .and. .not.RESTO .and. .not.SOC_FLAG.eq.1 ) then
         if( QDAMP.ne.0 ) then
            call C_OUT(2,0,1,'get_step_red: Can''t do damping for cg.')
            IERR = 4
            goto 9999
         endif
         call GET_PZ_CG(ITER, MU, NIND, RGB, WCORR, B, PZ,
     1        IEIGS, N, M, X, IVAR, NFIX, IFIX, NORIG, XORIG, CSCALE,
     2        LAM, NLB, ILB, NUB, IUB, S_L, S_U, LSLACKS, SIGMA_L,
     3        SIGMA_U, ERR, ERR_BAR, NEWBAS, KCONSTR, LRS, RS, LIS, IS,
     4        LRW-p_rwend, RW(p_rwend+1),
     4        LIW-p_iwend, IW(p_iwend+1), IERR)
         if( IERR.gt.0 ) then
            write(line,*) 'get_step_red: get_pz_cg returns IERR = ',IERR
            call C_OUT(2,0,1,line)
            goto 9999
         elseif( IERR.lt.0 ) then
            write(line,*) 'get_step_red: get_pz_cg returns IERR = ',IERR
            call C_OUT(2,0,1,line)
            IERR = 0
         endif
      endif

CWEG      if( QCG.eq.0 .or. (IEIGS.eq.-1.and.QCG.gt.0) .or. RESTO ) then
      if( QCG.eq.0 .or. RESTO ) then
         if( QCG.ne.0 .and..not.RESTO ) then
c            WRITE(*,*)
c     1           'Hey, need to call GET_EXACTW here in get_step_red!'
            IERR = 4582
            goto 9999
         endif
         cnrm = DASUM(M, C, 1)
         call GET_PZ(ITER, MU, NIND, W, RGB, WCORR, cnrm, B, PZ, IEIGS,
     2        N, M, X, IVAR, NFIX, IFIX, NORIG, XORIG, CSCALE, LAM, NLB,
     2        ILB, NUB, IUB, LSLACKS, S_L, S_U, BNDS_L, BNDS_U, SIGMA_L,
     3        SIGMA_U, YPY, REGU, SOC_FLAG, RESTO,
     1        KCONSTR, LRS, RS, LIS, IS,
     4        LRW-p_rwend, RW(p_rwend+1),
     4        LIW-p_iwend, IW(p_iwend+1), IERR)
         if( IERR.gt.0 ) then
            write(line,*) 'get_step_red: get_pz returns IERR = ',IERR
            call C_OUT(2,0,1,line)
            goto 9999
         elseif( IERR.lt.0 ) then
            write(line,*) 'get_step_red: get_pz returns IERR = ',IERR
            call C_OUT(2,0,1,line)
            IERR = 0
         endif
      endif
C
C     Compute dependent part of null space step ZPZ
C
      call GET_ZPZ(N, NIND, M, ITER, IVAR, NFIX, IFIX,
     1             NORIG, XORIG, CSCALE, PZ, ZPZ,
     1             KCONSTR, LRS, RS, LIS, IS,
     2             LRW-p_rwend, RW(p_rwend+1),
     4             LIW-p_iwend, IW(p_iwend+1), IERR)
      if( IERR.gt.0 ) then
         write(line,*) 'get_step_red: get_zpz returns IERR = ',IERR
         call C_OUT(2,0,1,line)
         goto 9999
      elseif( IERR.lt.0 ) then
         write(line,*) 'get_step_red: get_zpz returns IERR = ',IERR
         call C_OUT(2,0,1,line)
         IERR = 0
      endif
C
C     Compute primal and dual search directions
C
 1000 continue
      call GET_D(N, NIND, M, X, YPY, PZ, ZPZ, NLB, ILB, NUB, IUB,
     1           S_L, S_U, V_L, V_U, BNDS_L, BNDS_U, LSLACKS,
     1           SIGMA_L, SIGMA_U, MU, DX, DV_L, DV_U, RESTO)
C
      if( RESTO.and.QRESTO.eq.3 ) goto 9999
C
C     Compute maximal step size alpha within bounds
C
      tau = QTAU
      call CUTALPHA(N, X, DX, DV_L, DV_U, NLB, ILB, NUB, IUB,
     1              BNDS_L, BNDS_U, S_L, S_U, V_L, V_U, tau,
     2              LSLACKS, ALPHA, ALPHA_DUAL, C_ALPHA)
C
      ALPHA_CUT = ALPHA   ! Show this alpha in normal case
C
C     Check, if step is cut too much; if so recompute range space step as
C     scaled Cauchy step and recompute rest.
C
C
C
C     Uncomment the following line in order to switch off heuristic
C     (Maybve also good for EIGMINA)
C     ( ZIGZAG with QMOVEINIT = 0 and large move)
      if( abs(QMERIT).gt.3 .or. QMERIT.eq.0 ) goto 121
      if( RESTO ) goto 121
      if( LSLACKS ) then
         write(line,*)
     1        'get_step_red: Heristic doesn''t work with LSLACKS yes;',
     2        ' skip it...'
         call C_OUT(2,0,1,line)
         goto 121
      endif
C
      if( QMERIT.eq.3 ) then
CTODO Think about this
         goto 121
         call C_OUT(2,0,1,
     1        'get_step_red: Heuristic doesn''t work with aug_lag!')
         IERR = 4
         goto 9999
      endif
      if( LSLACKS ) then
         call C_OUT(2,0,1,
     1        'get_step_red: Heuristic not with QMOVEINIT=0!')
         IERR = 4
         goto 9999
      endif
CTODO  check if checking cnrm=0 is enough to avoid den = 0 in heuristic
      if( ALPHA.le.1d-4 .and. ac_heuristic.eq.0 .and. cnrm.gt.0d0 ) then

         ac_heuristic = 1        ! There are other options; see get_ypy
         write(line,*) 'alpha_cut = ',ALPHA,', recompute Cauchy'
         call C_OUT(1,0,1,line)
C
C     For overall dolegl, store old directions
C
         p_newtondx    = p_rwend
         p_newtondvl   = p_newtondx    + N
         p_newtondvu   = p_newtondvl   + NLB
         p_newtonwcorr = p_newtondvu   + NUB
         p_newtonpz    = p_newtonwcorr + NIND
         p_newtonypy   = p_newtonpz    + NIND
         p_rwend       = p_newtonypy   + N
         if( p_rwend.gt.LRW ) then
            IERR = 98
            goto 9999
         endif
         call DCOPY( N   , DX   , 1, RW(p_newtondx   +1), 1)
         call DCOPY( NLB , DV_L , 1, RW(p_newtondvl  +1), 1)
         call DCOPY( NUB , DV_U , 1, RW(p_newtondvu  +1), 1)
         call DCOPY( NIND, WCORR, 1, RW(p_newtonwcorr+1), 1)
         call DCOPY( NIND, PZ   , 1, RW(p_newtonpz   +1), 1)
         call DCOPY( N   , YPY  , 1, RW(p_newtonypy  +1), 1)

         call GET_YPY(N, NIND, M, ITER, IVAR, NFIX, IFIX,
     1                NORIG, XORIG, CSCALE, NLB, ILB, NUB, IUB,
     2                S_L, S_U, C, LSLACKS, ac_heuristic, YPY,
     3                THETA1, THETA2, NEWBAS, CONDC,
     1                KCONSTR, LRS, RS, LIS, IS,
     2                LRW-p_rwend, RW(p_rwend+1),
     4                LIW-p_iwend, IW(p_iwend+1), IERR)
CTODO    IF NEWBAS, LEAVE THIS ROUTINE AND GET NEW BASIS FIRST???!?
         if( IERR.gt.0 ) then
            write(line,*)
     1      'get_step_red: Error: get_ypy ends with IERR = ',IERR
            call C_OUT(2,0,1,line)
            goto 9999
         elseif( IERR.ne.0 ) then
            write(line,*)
     1      'get_step_red: Warning: get_ypy ends with IERR = ',IERR
            call C_OUT(2,0,1,line)
            IERR = 0
         endif
         goto 120
C
C     If step has been recomputed, go along dogleg path...
C
      elseif( ac_heuristic.ne.0 ) then

         C_ALPHA = '*'
C
C     If full Cauchy step is within bounds, do overall dogleg
C
C     In order to have pure scaled Cauchy steps all the time, i.e. no overall
C     dogleg involving a part of the original Newton step, uncomment following
C     line
c         goto 122

         if( ALPHA.eq.1d0 ) then

C     Now compute furthest point on dogleg path satisfying tau-rule
            if( LSLACKS ) then
               write(line,*) 'ERROR: not implemented for LSLACKS'
               call C_OUT(2,0,1,line)
               stop
            endif
            alphap = 1.d0
            do i = 1, NLB
               k = ILB(i)
               dxi = RW(p_newtondx+k)-DX(k)
               if( dxi.lt.0.d0 ) then
                  alphap = dmin1(alphap, TAU*(-(S_L(i)+DX(k))/dxi))
C                  alphap = dmin1(alphap, -(TAU*S_L(i)+DX(k))/dxi)
               endif
            enddo
            do i = 1, NUB
               k = IUB(i)
               dxi = RW(p_newtondx+k)-DX(k)
               if( dxi.gt.0.d0 ) then
                  alphap = dmin1(alphap, TAU*( (S_U(i)-DX(k))/dxi))
C                  alphap = dmin1(alphap, (TAU*S_U(i)-DX(k))/dxi)
               endif
            enddo
C
C     Take care of dual variables
C
            alphad = 1.d0
            if( QPRIMAL.ne.1 ) then
               do i = 1, NLB
                  dvi = RW(p_newtondvl+i)-DV_L(i)
                  if( dvi.lt.0.d0 ) then
                     alphad = dmin1(alphad, TAU*(-(V_L(i)+DV_L(i))/dvi))
C                     alphad = dmin1(alphad, -(TAU*V_L(i)+DV_L(i))/dvi)
                  endif
               enddo
               do i = 1, NUB
                  dvi = RW(p_newtondvu+i)-DV_U(i)
                  if( dvi.lt.0.d0 ) then
                     alphad = dmin1(alphad, TAU*(-(V_U(i)+DV_U(i))/dvi))
C                     alphad = dmin1(alphad, -(TAU*V_U(i)+DV_U(i))/dvi)
                  endif
               enddo
            endif
C
C     Get dogleg step
C
            alpha_dl = dmin1(alphad, alphap)
            if( alpha_dl.eq.1.d0 ) then
               C_ALPHA = 'N'
            elseif( alpha_dl.eq.alphap ) then
               C_ALPHA = 'P'
            else
               C_ALPHA = 'D'
            endif
            ALPHA_CUT = alpha_dl    ! In dogleg case show this alpha
C
            call DSCAL(N   , 1.d0-alpha_dl, DX   , 1)
            call DSCAL(NLB , 1.d0-alpha_dl, DV_L , 1)
            call DSCAL(NUB , 1.d0-alpha_dl, DV_U , 1)
            call DSCAL(NIND, 1.d0-alpha_dl, WCORR, 1)
            call DSCAL(NIND, 1.d0-alpha_dl, PZ   , 1)
            call DSCAL(N   , 1.d0-alpha_dl, YPY  , 1)

            call DAXPY(N   , alpha_dl, RW(p_newtondx   +1), 1, DX   , 1)
            call DAXPY(NLB , alpha_dl, RW(p_newtondvl  +1), 1, DV_L , 1)
            call DAXPY(NUB , alpha_dl, RW(p_newtondvu  +1), 1, DV_U , 1)
            call DAXPY(NIND, alpha_dl, RW(p_newtonwcorr+1), 1, WCORR, 1)
            call DAXPY(NIND, alpha_dl, RW(p_newtonpz   +1), 1, PZ   , 1)
            call DAXPY(N   , alpha_dl, RW(p_newtonypy  +1), 1, YPY  , 1)

            THETA1 = alpha_dl

         endif
 122     continue
C     free work space
         p_rwend = p_newtondx
      endif
 121  continue
C
C     Here now heuristic for restoration phase (like in get_step_full)
C
      if( RESTO.and.ALPHA_CUT.lt.1d0 ) then
         p_dl     = p_rwend
         p_rwend  = p_dl + N
         if( p_rwend.gt.LRW ) then
            IERR = 98
            goto 9999
         endif
C
C     get scaled Cauchy step from GET_YPY
C
         call GET_YPY(N, NIND, M, ITER, IVAR, NFIX, IFIX,
     1                NORIG, XORIG, CSCALE, NLB, ILB, NUB, IUB,
     2                S_L, S_U, C, LSLACKS, 1, RW(p_dl+1),
     3                theta1_tmp, theta2_tmp, newbas_tmp, condc_tmp,
     1                KCONSTR, LRS, RS, LIS, IS,
     2                LRW-p_rwend, RW(p_rwend+1),
     4                LIW-p_iwend, IW(p_iwend+1), IERR)
         if( IERR.gt.0 ) then
            write(line,*)
     1      'get_step_red: Error: get_ypy-R ends with IERR = ',IERR
            call C_OUT(2,0,1,line)
            goto 9999
         elseif( IERR.ne.0 ) then
            write(line,*)
     1      'get_step_red: Warning: get_ypy-R ends with IERR = ',IERR
            call C_OUT(2,0,1,line)
            IERR = 0
         endif
         call CUTALPHA(N, X, RW(p_dl+1), DV_L, DV_U, NLB, ILB,
     1        NUB, IUB, BNDS_L, BNDS_U, S_L, S_U, V_L, V_U, QTAU,
     2        LSLACKS, ALPHA, ALPHA_DUAL, C_ALPHA)
         if( QALPHA.ne.0 ) then
            write(line,*)
     1           'get_step_red: Not sure if this works with QALPHA = ',
     2           ALPHA
         endif
         if( ALPHA.eq.1d0 ) then
C
            if( LSLACKS ) then
               write(line,*) 'ERROR: not implemented for LSLACKS'
               call C_OUT(2,0,1,line)
               stop
            endif
            alpha_dl = 1.d0
            do i = 1, NLB
               k = ILB(i)
               dxi = DX(k)-RW(p_dl+k)
               if( dxi.lt.0.d0 ) then
                  alpha_dl = dmin1(alpha_dl,
     1                 QTAU*(-(S_L(i)+RW(p_dl+k))/dxi))
               endif
            enddo
            do i = 1, NUB
               k = IUB(i)
               dxi = DX(k)-RW(p_dl+k)
               if( dxi.gt.0.d0 ) then
                  alpha_dl = dmin1(alpha_dl,
     1                 QTAU*( (S_U(i)-RW(p_dl+k))/dxi))
               endif
            enddo
            call DSCAL(N, 1d0-alpha_dl, RW(p_dl+1), 1)
            call DAXPY(N, alpha_dl, DX, 1,
     1           RW(p_dl+1), 1)
            C_ALPHA = 'D'
         else
            C_ALPHA = 'C'
         endif
C
C     Compute progress in linear model by dogleg step
C
         p_tmp = p_rwend
         p_rwend = p_tmp + M
         if( p_rwend.gt.LRW ) then
            IERR= 98
            goto 9999
         endif
         call CONSTR(9, ITER, N, NIND, M, IVAR, NFIX, IFIX,
     1        NORIG, XORIG, CSCALE, RW(p_dl+1), RW(p_tmp+1), idummy,
     3        idummy, KCONSTR(1), RS(KCONSTR(2)+1), KCONSTR(3),
     4        IS(KCONSTR(4)+1), LRW-p_rwend, RW(p_rwend+1),
     5        LIW-p_iwend, IW(p_iwend+1), IERR)
         if( IERR.lt.0 ) then
            write(line,*)
     1           'get_step_red: Warning in CONSTR-9, IERR = ',IERR
            call C_OUT(2,0,1,line)
         elseif( IERR.ne.0 ) then
            write(line,*)
     1           'get_step_red: Error in CONSTR-9, IERR = ',IERR
            call C_OUT(2,0,1,line)
            goto 9999
         endif
         if( ALPHA.lt.1d0 ) then
            call DSCAL(M, ALPHA, RW(p_tmp+1), 1)
         endif
         call DAXPY(M, 1d0, C, 1, RW(p_tmp+1), 1)
         nrm_dl = DNRM2(M, RW(p_tmp+1), 1)
         nrm_nw = (1-ALPHA_CUT)*DNRM2(M, C, 1)
         if( nrm_dl.lt.nrm_nw ) then
            call DCOPY(N, RW(p_dl+1), 1, DX, 1)
            if( C_ALPHA.eq.'C' ) then
               ALPHA_CUT = ALPHA
            else
               ALPHA_CUT = alpha_dl
            endif
         else
            ALPHA = ALPHA_CUT
            C_ALPHA = 'N'
         endif
         p_rwend = p_dl
      endif
C
 9999 continue
      return
      end
