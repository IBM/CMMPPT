C Copyright (C) 2002, Carnegie Mellon University and others.
C All Rights Reserved.
C This code is published under the Common Public License.
C*******************************************************************************
C
      subroutine GET_STEP_FULL(ITER, NORIG, N, NIND, M, NZORIG, X,
     1     XORIG, CSCALE, B, NLB, ILB, NUB, IUB, NFIX, IFIX, IVAR, G, C,
     1     S_L, S_U, V_L, V_U, BNDS_L, BNDS_U, LSLACKS,
     1     SIGMA_L, SIGMA_U, MU, DX, DV_L, DV_U, LAM, LAMOLD, PZ, ALPHA,
     1     ALPHA_DUAL, ALPHA_CUT, C_ALPHA, NEWBAS, IEIGS, YPY, REGU,
     1     WYPY, WCORR, THETA1, THETA2, SOC_FLAG, ERR_BAR, ERR_CNSTR,
     1     RESTO, PRWKEEP, PIWKEEP, INIT_LAM,
     1     KCONSTR, LRS, LRS_END, RS, LIS, LIS_END, IS,
     1     LRW, RW, LIW, IW, IERR)
C
C*******************************************************************************
C
C    $Id: get_step_full.F,v 1.11 2003/04/11 02:55:23 andreasw Exp $
C
C-------------------------------------------------------------------------------
C                                 Title
C-------------------------------------------------------------------------------
C
CT    Compute primal and dual steps from FULL system
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
CP   ITER      I    INT    iteration counter
CP                            (if -1: initialize pointers for storage space)
CP   NORIG     I    INT    number of all variables including fixed vars
CP   N         I    INT    number of (free) variables; first M variables
CP                         are dependent; remaining independent
CP   NIND      I    INT    number of independent variables
CP   M         I    INT    number of dependent variables
CP   NZORIG    I    INT    (only for ITER=-1): number of nonzeros in Jacobian
CP                            including columns for fixed vars
CP   X         I    DP     actual primal iterate
CP   XORIG     I    DP     actual iterate
CP   CSCALE    I    DP     scaling factors for constraints
CP   B         I    DP     Quasi-Newton estimate of reduced Hessian of
CP                            original NLP
CP   NLB       I    INT    number of lower bounds (excluding fixed vars)
CP   ILB       I    INT    indices of lower bounds
CP                            (e.g. S_L(i) is slack for X(ILB(i)) )
CP   NUB       I    INT    number of upper bounds (excluding fixed vars)
CP   IUB       I    INT    indices of upper bounds
CP                            (e.g. S_U(i) is slack for X(IUB(i)) )
CP   NFIX      I    INT    number of fixed variables
CP   IFIX      I    INT    specifies variables that are fixed by bounds:
CP                            i = 1..NORIG-N   XORIG(IFIX(i)) is fixed
CP                            (assumed to be in increasing order)
CP   IVAR      I    INT    information about partitioning
CP                            i = 1..M      XORIG(IVAR(i)) dependent
CP                            i = (M+1)..N  XORIG(IVAR(i)) independent
CP                            Note: fixed variables do not occur in IVAR
CP                            X(i) corresponds to XORIG(IVAR(i))
CP   G         I    DP     gradient of objective function
CP   C         I    DP     values of constraints at X
CP   S_L       I    DP     slack variables for lower bounds
CP   S_U       I    DP     slack variables for upper bounds
CP   V_L       I    DP     dual variables for lower bounds
CP   V_U       I    DP     dual variables for upper bounds
CP   BNDS_L    I    DP     values of lower bounds (ordered as S_L)
CP   BNDS_U    I    DP     values of upper bounds (ordered as S_U)
CP   LSLACKS   I    LOG    =.true.: There are slacks that don't satisfy
CP                                  "slack equation"
CP   SIGMA_L   I    DP     primal-dual Hessian of lower bound barrier term
CP                            (NLB diagonal elements only)
CP   SIGMA_U   I    DP     primal-dual Hessian of upper bound barrier term
CP                            (NUB diagonal elements only)
CP   MU        I    DP     barrier parameter (=0, if Error should be computed
CP                            for overall NLP)
CP   DX        O    DP     step for X (primal)
CP   DV_L      O    DP     step for V_L (dual variables for lower bounds)
CP   DV_U      O    DP     step for V_U (dual variables for upper bounds)
CP   LAM      I/O   DP     multipliers (only if QQUASI = 0,2)
CP   LAMOLD   I/O   DP     multipliers (only if QQUASI = 0,2)
CP   PZ        O    DP     null space step (indepentent variables)
CP                            (only if QQUASI.ne.0)
CP   ALPHA     O    DP     maximal steps size compatible with fraction
CP                            to boudary rule (primal vars)
CP   ALPHA_DUAL O   DP     maximal steps size compatible with fraction
CP                            to boudary rule (dual vars)
CP   ALPHA_CUT O    DP     first alpha on output
CP   C_ALPHA   O    C*1    for output on ALPHA
CP   NEWBAS    I    LOG    =.true. if basis changed (need new pivots!)
CP   IEIGS     O    I      number of trials for regularization
CP   YPY       I    DP     range space step
CP                           (only if penalty parameter is based on LAM, i.e.
CP                            QLAMBDA <> 0)
CP   REGU      O    DP     regularization factor (added regu*I to diagonal)
CP   WYPY      O    DP     product W YpY  (needed in linesearch)
CP                           (only if penalty parameter is based on LAM, i.e.
CP                            QLAMBDA <> 0)
CP   WCORR     O    DP     cross term Z^T W YpY (for QMERIT=-2,-1,1,2)
CP   THETA1    O    DP     dogleg parameter (=1 <-> full Newton step)
CP   THETA2    O    DP     dogleg parameter (= -c^T*A^T*d_CS )
CP   SOC_FLAG  I    INT    =0: not in SOC computation
CP                          1: in SOC computation (don't need to refactorize)
CP   ERR_BAR   I    DP     error tolerance for the barrier problem
CP   ERR_CNSTR I    DP     current infeasibility error
CP   RESTO     I    LOG    =.true.: we are in restoration phase
CP   PRWKEEP   O    INT    If this routine is called lateron for SOC, the
CP                           calling routine must keep the first PRWKEEP
CP                           entries in RW
CP   PIWKEEP   O    INT    If this routine is called lateron for SOC, the
CP                           calling routine must keep the first PIWKEEP
CP                           entris in IW
CP   INIT_LAM I/O   LOG    Indicates whether equality multipliers are to be
CP                           initialized (might be set to. false. in line
CP                           search)
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
CS    DAXPY
CS    DSCAL
CS    DNRM2
CS    GET_H
CS    GET_YPY
CS    MA27_CALL
CS    MA47_CALL
CS    MA57_CALL
CS    CONSTR
CS    CUTALPHA
CS    MC19_CALL
CS    MC30AD
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
      integer ITER
      integer NORIG
      integer N
      integer NIND
      integer M
      integer NZORIG
      double precision X(N)
      double precision XORIG(NORIG)
      double precision CSCALE(*)
      double precision B(*)
      integer NLB
      integer ILB(NLB)
      integer NUB
      integer IUB(NUB)
      integer NFIX
      integer IFIX(NFIX)
      integer IVAR(N)
      double precision G(N)
      double precision C(M)
      double precision S_L(NLB)
      double precision S_U(NUB)
      double precision V_L(NLB)
      double precision V_U(NUB)
      double precision BNDS_L(NLB)
      double precision BNDS_U(NUB)
      logical LSLACKS
      double precision SIGMA_L(NLB)
      double precision SIGMA_U(NUB)
      double precision MU
      double precision DX(N)
      double precision DV_L(NLB)
      double precision DV_U(NUB)
      double precision LAM(M+N) ! for LSLACKS
      double precision LAMOLD(M+N) ! for LSLACKS
      double precision PZ(NIND)
      double precision ALPHA
      double precision ALPHA_DUAL
      double precision ALPHA_CUT
      character*1 C_ALPHA
      logical NEWBAS
      integer IEIGS
      double precision YPY(N)
      double precision REGU
      double precision WYPY(N)
      double precision WCORR(NIND)
      double precision THETA1
      double precision THETA2
      integer SOC_FLAG
      double precision ERR_BAR
      double precision ERR_CNSTR
      logical RESTO
      integer PRWKEEP
      integer PIWKEEP
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
C                            Local varibales
C-------------------------------------------------------------------------------
C
      integer p_rwend, p_iwend, p_kkt, p_jcntmp, p_check
      integer p_rhs, p_w, p_hcopy, p_rhscopy, p_iwma, p_rhs_store
      integer p_scale, p_dl, p_tmp, p_cnew, p_xtry
      integer p_rgb, p_scale_old, p_a, p_realw, pb_uk, pb_vk
      integer inz, i, j, idummy, kr, kc, ifail, nz, k
      integer neigs, nzero, nlm, nlmmax
      double precision dummy, ele, vin(2), condc_tmp, nrm_ratio_old
      double precision nrm_ratio, gzbw, gzbzg, regu_old, cnrm
      double precision dxi, alpha_dl, nrm_dl, nrm_nw, nrm_res, nrm_rhs
      double precision sigmak
      character*80 line(2)
      logical ldummy, newbas_tmp
      logical l_cs              ! for RESTO: if true, do CS and ignore Newton
      logical l_iter_ref, l_incuma27, recomp_regu, skip_jac

      double precision REGU_STORE, DEPCONDIAGVAL
      save             REGU_STORE, DEPCONDIAGVAL
      integer NZKKT, NZA, NNZH, LIWMA, LA
      save    NZKKT, NZA, NNZH, LIWMA, LA
      logical HAVE_PIV, L_INC_LA, L_INC_LIWMA
      save    HAVE_PIV, L_INC_LA, L_INC_LIWMA

      integer P_JCN, P_IRN
      save    P_JCN, P_IRN

      double precision DNRM2, DDOT, DASUM
      integer FFINITE

c$$$CTRY2
c$$$      double precision BL_SOC(10000), BU_SOC(10000)
c$$$      common /SOC/ BL_SOC, BU_SOC

C     Parameter QKKTSCALE
C
C     0 = no scaling
C     1 = use MC30 to get scaling factors
C     2 = use S_L and S_U to scale the matrix

C
C*******************************************************************************
C
C                           Executable Statements
C
C*******************************************************************************
C

C
C     ITER = -1: initialize data structures
C
      if( ITER.eq.-1 ) then
         NZKKT = N + NZORIG     ! worst estimate
         if( QQUASI.ne.0 .and. abs(QQUASI).lt.6 ) then
            NZKKT = NZKKT + (NIND*(NIND+1))/2
         elseif( QQUASI.eq.0 ) then
C
C     call EVAL_H to obtain number of nonzeros in Hessian
C
            call GET_H(-1, N, NIND, idummy, dummy, idummy, NORIG,
     1           XORIG, dummy, M, NNZH, dummy, dummy, idummy, idummy,
     1           idummy, dummy, idummy, idummy, idummy)
            NZKKT = NZKKT + NNZH
            write(line,100) NNZH
 100        format('Number of nonzeros in Hessian : ',i8)
            call C_OUT(2,0,1,line)
         endif
         if( QDEPCONDIAG.gt.0.d0 ) then
            NZKKT = NZKKT + M   ! space heuristic for dependent constraints
         endif
C
C     reserve space for solver
C
         if( QSYMSOLV.eq.0 ) then
            call MA47_CALL(0, N+M, NZKKT, dummy, idummy, idummy, dummy,
     1           idummy, ldummy, idummy, idummy, ldummy, idummy, idummy,
     1           LRS_END, dummy, LIS_END, idummy, idummy, dummy, idummy,
     1           idummy, IERR)
         elseif( QSYMSOLV.eq.1 ) then
            call MA27_CALL(0, N+M, NZKKT, dummy, idummy, idummy, dummy,
     1           idummy, ldummy, idummy, idummy, ldummy, idummy, idummy,
     1           LRS_END, dummy, LIS_END, idummy, idummy, dummy, idummy,
     1           idummy, IERR)
         elseif( QSYMSOLV.eq.2 ) then
            call MA57_CALL(0, N+M, NZKKT, dummy, idummy, idummy, dummy,
     1           dummy, idummy, ldummy, idummy, idummy, ldummy,
     1           idummy, idummy,
     1           LRS_END, dummy, LIS_END, idummy, idummy, dummy, idummy,
     1           idummy, IERR)
         else
            call C_OUT(2,0,1,'get_step_full: Invalid QSYMSOLV.')
            IERR = 4
            goto 9999
         endif

         P_JCN   = LIS_END
         P_IRN   = P_JCN + NZKKT
         LIS_END = P_IRN + NZKKT

         HAVE_PIV    = .false.
         L_INC_LA    = .false.
         L_INC_LIWMA = .false.
         LA          = NZKKT
         LIWMA       = 0
         REGU_STORE = 0d0
         goto 9999
      endif
C
      p_rwend = 0
      p_iwend = 0

      if( abs(QQUASI).ge.6 ) then
C
C     For limited memory quasi-Newton: Get the structure of B right
C
         nlm       = int(B(1))
         nlmmax    = QLMLEN
         sigmak    = B(2)
         pb_uk     = 2
         pb_vk     = pb_uk + N*nlmmax
      endif
C
      l_cs       = .false.
      l_iter_ref = .false.
      l_incuma27 = .false.
      skip_jac   = .false.
C
C     Set THETA's to default values for Newton step
CTODO CAN BE REMOVED (is remainder for ac_heuristic)
C
      THETA1 = 1.d0
      THETA2 = 0.d0
C
C     Initialize
C
      if( SOC_FLAG.eq.0 ) then
         REGU   = 0d0
         IEIGS  = 0
         DEPCONDIAGVAL = 0.d0
C
C     if necessary, increase LA or LIWMA
C
         if( L_INC_LA ) then
            LA = 2*LA
         endif
         if( L_INC_LIWMA ) then
            LIWMA = 2*LIWMA
         endif
         L_INC_LA    = .false.
         L_INC_LIWMA = .false.

      elseif( SOC_FLAG.eq.1 ) then
C
C     Compute SOC step - reuse old factorization and scaling factors
C     reassigne work space pointers
C
         p_kkt   = p_rwend
         p_rwend = p_kkt + LA 
         p_scale   = p_rwend
         if( QKKTSCALE.eq.0 ) then
            p_rwend = p_scale
         else
            p_rwend = p_scale + M+N
         endif
         if( abs(QMERIT).eq.1 .or. abs(QMERIT).eq.2 ) then
            p_hcopy = p_rwend
            p_rwend = p_hcopy   + NZKKT
         endif
         if( p_rwend.gt.LRW ) then
            IERR = 98
            goto 9999
         endif
         p_iwma = p_iwend
         p_iwend = p_iwma + LIWMA
         if( p_iwend.gt.LIW ) then
            IERR = 99
            goto 9999
         endif
         goto 1050
      endif
C
C     If basis changed, need to call MA?7AD again
C
      if( NEWBAS ) then
         HAVE_PIV = .false.
      endif
C
 1000 continue
C
C     Fill the KKT matrix
C
      p_kkt   = p_rwend
      p_rwend = p_kkt + LA      ! If HAVE_PIV = .false. we have to correct
                                ! p_rwend lateron!
      if( p_rwend.gt.LRW ) then
         IERR = 98
         goto 9999
      endif
C
C     Build full space KKT matrix
C
      inz = 0
C
C     1. Diagonal in Hessian for regularization and barrier term
C
      if( RESTO ) then
C
C     Put scaling matrix for restoration into matrix
C     (Make sure this corresponds to the same matrix as used in resto_dogleg)
C
         call DCOPY(N, QDDMAX, 0, RW(p_kkt+inz+1), 1)
         if( QRESTO.gt.0 ) then
            do i = 1, NLB
               k = ILB(i)
               RW(p_kkt+inz+k) = dmax1(RW(p_kkt+inz+k),
     1              1d0/(S_L(i)**2))
            enddo
            do i = 1, NUB
               k = IUB(i)
               RW(p_kkt+inz+k) = dmax1(RW(p_kkt+inz+k),
     1              1d0/(S_U(i)**2))
            enddo
         endif
         if( REGU.gt.0d0 ) then
            CALL DAXPY(N, 1d0, REGU, 0, RW(p_kkt+inz+1), 1)
         endif
      elseif( INIT_LAM ) then
         call DCOPY(N, 1.d0, 0, RW(p_kkt+inz+1), 1)
      else
         call DCOPY(N, REGU, 0, RW(p_kkt+inz+1), 1)
         if( .false. ) then
CTODO         if( QCORRECT.eq.2 .and. REGU.gt.0.d0 ) then
            do i = 1, NLB
               j = ILB(i)
               RW(p_kkt+inz+j) =
     1              dmax1(RW(p_kkt+inz+j),REGU/(S_L(i)**2))
            enddo
            do i = 1, NUB
               j = IUB(i)
               RW(p_kkt+inz+j) =
     1              dmax1(RW(p_kkt+inz+j),REGU/(S_U(i)**2))
            enddo
         endif
         if( abs(QQUASI).eq.6 ) then
            call DAXPY(N, 1.d0, sigmak, 0, RW(p_kkt+inz+1), 1)
         endif
      endif
C
C     If we have a square problem, the upper left corner doesn't really matter
C     and should be ignored (for now, we set all entries to zero...)
C
      if( NIND.gt.0 .and. .not.INIT_LAM ) then
         do i = 1, NLB
            k = ILB(i)
            RW(p_kkt+inz+k) = RW(p_kkt+inz+k) + SIGMA_L(i)
         enddo
         do i = 1, NUB
            k = IUB(i)
            RW(p_kkt+inz+k) = RW(p_kkt+inz+k) + SIGMA_U(i)
         enddo
      endif

      if( .not.HAVE_PIV ) then
         do i = 1, N
            IS(P_JCN+i) = i
            IS(P_IRN+i) = i
         enddo
      endif
      inz = inz + N
C
C     2. Hessian matrix
C
      if( QQUASI.ne.0 .and. abs(QQUASI).lt.6 ) then
         if( .not.RESTO .and. .not.INIT_LAM .and. NIND.gt.0 ) then
            call DCOPY( (NIND*(NIND+1))/2, B, 1, RW(p_kkt+inz+1), 1)
         else
            call DCOPY( (NIND*(NIND+1))/2, 0d0, 0,
     1           RW(p_kkt+inz+1), 1)
         endif
         if( .not.HAVE_PIV ) then
            do i = 1, NIND
               do j = 1, i
                  inz = inz + 1
                  IS(P_JCN+inz) = i+M
                  IS(P_IRN+inz) = j+M
               enddo
            enddo
         else
            inz = inz + (NIND*(NIND+1))/2
         endif
      elseif( QQUASI.eq.0 ) then
         if( QLAMBDA.eq.0 ) then
            call C_OUT(2,0,1,
     1           'Need multipliers for exact Hessian! Abort')
            IERR = 4
            goto 9999
         endif
         if( (.not.RESTO .and. .not.INIT_LAM) .or. .not.HAVE_PIV ) then
            call GET_H(ITER, N, NIND, NFIX, X, IVAR, NORIG, XORIG,
     1           CSCALE, M, NNZH, LAM, RW(p_kkt+inz+1), IS(P_IRN+inz+1),
     1           IS(P_JCN+inz+1), LRW-p_rwend, RW(p_rwend+1),
     1           LIW-p_iwend, IW(p_iwend+1), IERR)
            if( IERR.gt.0 ) then
               write(line,*) 'get_step_full: GET_H returns IERR =',IERR
               call C_OUT(2,0,1,line)
            endif
         endif
         if( RESTO .or. INIT_LAM .or. NIND.eq.0 ) then
C     If in restoration phase we ignore second order information
            call DCOPY(NNZH, 0d0, 0, RW(p_kkt+inz+1), 1)
         endif
         inz = inz + NNZH
      endif
C
C     3. Jacobian of constraints
C
      if( M.gt.0 ) then
         if( .not.HAVE_PIV ) then
            call CONSTR(10, ITER, N, NIND, M, IVAR, NFIX, IFIX,
     1           NORIG, XORIG, CSCALE, vin,
     2           RW(p_kkt+inz+1), IS(P_JCN+inz+1), IS(P_IRN+inz+1),
     3           KCONSTR(1), RS(KCONSTR(2)+1), KCONSTR(3),
     4           IS(KCONSTR(4)+1), LRW-p_rwend, RW(p_rwend+1),
     5           LIW-p_iwend, IW(p_iwend+1), IERR)
            if( IERR.ne.0 ) goto 9999
            NZA = int(vin(1))
C
            do i = 1, NZA       ! Correct row indices
               IS(P_JCN+inz+i) = IS(P_JCN+inz+i) + N
            enddo
         elseif( .not.skip_jac ) then
            p_jcntmp = p_iwend
            p_iwend  = p_jcntmp + NZA
            if( p_rwend.gt.LRW ) then
               IERR = 98
               goto 9999
            elseif( p_iwend.gt.LIW ) then
               IERR = 99
               goto 9999
            endif
            call CONSTR(10, ITER, N, NIND, M, IVAR, NFIX, IFIX,
     1           NORIG, XORIG, CSCALE, vin,
     2           RW(p_kkt+inz+1), IW(p_jcntmp+1), IS(P_IRN+inz+1),
     3           KCONSTR(1), RS(KCONSTR(2)+1), KCONSTR(3),
     4           IS(KCONSTR(4)+1), LRW-p_rwend, RW(p_rwend+1),
     5           LIW-p_iwend, IW(p_iwend+1), IERR)
            if( IERR.ne.0 ) goto 9999
            p_iwend = p_jcntmp
         endif
      else
         NZA = 0
      endif
C
      inz = inz + NZA
C
C     4. Diagonal in right lower corner to deal with dependent constraints
C
      if( QDEPCONDIAG.gt.0.d0 ) then
         call DCOPY(M, DEPCONDIAGVAL, 0, RW(p_kkt+inz+1), 1)
         if( .not.HAVE_PIV ) then
            do i = 1, M
               IS(P_JCN+inz+i) = N + i
            enddo
            do i = 1, M
               IS(P_IRN+inz+i) = N + i
            enddo
         endif
         inz = inz + M
      endif
C
C     Obtain pivot sequence if we don't have it yet
C
      if( .not.HAVE_PIV ) then
C
C     Correct NZKKT (in case of fixed vars...) and reallocate memory
C
         NZKKT   = inz
         p_rwend = p_kkt + NZKKT ! correct memory allocation
C
C     Now get pivots from MA47AD
C
         if( QSYMSOLV.eq.0 ) then
            call MA47_CALL(1, N+M, NZKKT, RW(p_kkt+1), IS(P_IRN+1),
     1           IS(P_JCN+1), dummy, LIWMA, L_INC_LIWMA, idummy, LA,
     2           L_INC_LA, neigs, nzero,
     3           LRS, RS, LIS, IS, LRW-p_rwend, RW(p_rwend+1),
     4           LIW-p_iwend, IW(p_iwend+1), IERR)
         elseif( QSYMSOLV.eq.1 ) then
            call MA27_CALL(1, N+M, NZKKT, RW(p_kkt+1), IS(P_IRN+1),
     1           IS(P_JCN+1), dummy, LIWMA, L_INC_LIWMA, idummy, LA,
     2           L_INC_LA, neigs, nzero,
     3           LRS, RS, LIS, IS, LRW-p_rwend, RW(p_rwend+1),
     4           LIW-p_iwend, IW(p_iwend+1), IERR)
         elseif( QSYMSOLV.eq.2 ) then
            call MA57_CALL(1, N+M, NZKKT, dummy, IS(P_IRN+1),
     1           IS(P_JCN+1), dummy, dummy, LA, L_INC_LA, idummy, LIWMA,
     2           L_INC_LIWMA, neigs, nzero,
     3           LRS, RS, LIS, IS, LRW-p_rwend, RW(p_rwend+1),
     4           LIW-p_iwend, IW(p_iwend+1), IERR)
         endif
         if( IERR.ne.0 ) then
            write(line,*) 'get_step_full: MA?7_CALL(1) returns IERR =',
     1           IERR
            call C_OUT(2,0,1,line)
            if( IERR.ne.99 ) IERR = 505
            goto 9999
         endif
C
C     Correct storage space for KKT matrix
C
         p_rwend = p_kkt + LA
         if( p_rwend.gt.LRW ) then
            IERR = 98
            goto 9999
         endif
C
         HAVE_PIV = .true.

      endif
C
C     Reserve memory
C
      p_scale   = p_rwend
      if( QKKTSCALE.eq.0 ) then
         p_rwend = p_scale
      else
         p_rwend = p_scale + M+N
      endif
      p_hcopy   = p_rwend
      p_rhscopy = p_hcopy   + NZKKT
      p_rwend   = p_rhscopy + N+M
      if( p_rwend.gt.LRW ) then
         IERR = 98
         goto 9999
      endif
C
C     Let calling routine know much to remember for another SOC call
C
      if( abs(QMERIT).eq.1 .or. abs(QMERIT).eq.2 ) then
         PRWKEEP = p_rhscopy
      else
         PRWKEEP = p_hcopy
      endif
      PIWKEEP = LIWMA
C
C     Keep a copy of the elements of the KKT matrix
C
      call DCOPY(NZKKT, RW(p_kkt+1), 1, RW(p_hcopy+1), 1)
 1500 continue
C
C     Compute scaling parameters if desired
C
      if( QKKTSCALE.eq.1 ) then
C
C     Check, whether there is a NaN in the KKT matrix
C     (otherwise MC30 might hang)
C
         dummy = DASUM(NZKKT, RW(p_kkt+1), 1)
         if( FFINITE(dummy).eq.0 ) then
            call C_OUT(2,0,1,'get_step_full: NaN or Inf in KKT matrix.')
            IERR = 11
            goto 9999
         endif
C
C     call MC30 or MC19 to obtain scaling parameters
C
#ifndef NO_MC30
         p_w     = p_rwend
         p_rwend = p_w     + 4*(N+M)
         if( p_rwend.gt.LRW ) then
            IERR = 98
            goto 9999
         endif
         call MC30AD(N+M, NZKKT, RW(p_kkt+1), IS(P_IRN+1),
     1        IS(P_JCN+1), RW(p_scale+1), RW(p_w+1), QCNR, ifail)
         p_rwend = p_w
         if( ifail.ne.0 ) then
            write(line,*) 'MC30AD returns ifail = ',ifail
            call C_OUT(2,0,1,line)
            IERR = 517
            goto 9999
         endif
#else
#ifndef NO_MC19
C
         p_realw  = p_rwend     ! This will be REAL work space in MC19_CALL
         p_rwend  = p_realw + (7*(N+M))/2 + 1
         if( p_rwend.gt.LRW ) then
            IERR = 98
            goto 9999
         endif
         call MC19_CALL(N+M, NZKKT, RW(p_kkt+1), IS(P_IRN+1),
     1        IS(P_JCN+1), RW(p_scale+1), RW(p_realw+1),
     2        LRW-p_rwend, RW(p_rwend+1),
     4        LIW-p_iwend, IW(p_iwend+1), IERR)
         p_rwend = p_realw
#else
         IERR = 97
         call C_OUT(2,0,1,
     1        'get_step_full:  Compiled without MC30 or MC19.')
         call C_OUT(2,0,1,
     1        '                Choose a different value for IKKTSCALE.')
         goto 9999
#endif
#endif
         do i = 1, N+M
            RW(p_scale+i) = exp(RW(p_scale+i))
         enddo
      elseif( QKKTSCALE.eq.2 ) then
C
C     Use affine scaling based on slacks
C
         call DCOPY(N+M, 1.d0, 0, RW(p_scale+1), 1)
         do i = 1, NLB
            j = ILB(i)
            RW(p_scale+j) = dmin1(RW(p_scale+j), S_L(i))
         enddo
         do i = 1, NUB
            j = IUB(i)
            RW(p_scale+j) = dmin1(RW(p_scale+j), S_U(i))
         enddo
      elseif( QKKTSCALE.ne.0 ) then
         call C_OUT(2,0,1,'get_step_full: Invalid value for QKKTSCALE.')
         IERR = 4
         goto 9999
      endif
C
C     Scale the matrix
C
 1600 continue
      if( QKKTSCALE.ne.0 ) then
         do i = 1, NZKKT
            kr = IS(P_IRN+i)
            kc = IS(P_JCN+i)
            RW(p_kkt+i) = RW(p_kkt+i)*RW(p_scale+kr)*RW(p_scale+kc)
         enddo
      endif
C
C     Now let's do the factorization
C
      p_iwma  = p_iwend
      p_iwend = p_iwma + LIWMA
      if( p_iwend.gt.LIW ) then
         IERR = 99
         goto 9999
      endif
      if( QSYMSOLV.eq.0 ) then
         call MA47_CALL(2, N+M, NZKKT, RW(p_kkt+1), IS(P_IRN+1), 
     1        IS(P_JCN+1), dummy, LIWMA,
     1        L_INC_LIWMA, IW(p_iwma+1), LA, L_INC_LA, neigs, nzero,
     1        LRS, RS, LIS, IS, LRW-p_rwend, RW(p_rwend+1), LIW-p_iwend,
     1        IW(p_iwend+1), IERR)
      elseif( QSYMSOLV.eq.1 ) then
         call MA27_CALL(2, N+M, NZKKT, RW(p_kkt+1), IS(P_IRN+1), 
     1        IS(P_JCN+1), dummy, LIWMA,
     1        L_INC_LIWMA, IW(p_iwma+1), LA, L_INC_LA, neigs, nzero,
     1        LRS, RS, LIS, IS, LRW-p_rwend, RW(p_rwend+1), LIW-p_iwend,
     1        IW(p_iwend+1), IERR)
      elseif( QSYMSOLV.eq.2 ) then
         p_a     = p_rwend
         p_rwend = p_a + NZKKT
         if( p_rwend.gt.LRW ) then
            IERR = 98
            goto 9999
         endif
         call DCOPY(NZKKT, RW(p_kkt+1), 1, RW(p_a+1), 1)
         call MA57_CALL(2, N+M, NZKKT, RW(p_a+1), IS(P_IRN+1),
     1        IS(P_JCN+1), dummy, RW(p_kkt+1), LA, L_INC_LA,
     1        IW(p_iwma+1), LIWMA, L_INC_LIWMA, neigs, nzero,
     3        LRS, RS, LIS, IS, LRW-p_rwend, RW(p_rwend+1),
     4        LIW-p_iwend, IW(p_iwend+1), IERR)
         p_rwend = p_a
      endif
      if( IERR.eq.-1 ) then
C
C     LIWMA or LA too small - increase storage for KKT matrix factorization,
C     rearrange memory, and factorize again
C
         p_iwend     = p_iwma

         p_scale_old = p_scale
         p_rwend     = p_kkt + LA

         inz       = NZKKT
         p_scale   = p_rwend
         if( QKKTSCALE.eq.0 ) then
            p_rwend = p_scale
         else
            p_rwend = p_scale + M+N
            inz     = inz + M+N
         endif
         p_hcopy   = p_rwend
         p_rhscopy = p_hcopy   + NZKKT
         p_rwend   = p_rhscopy + N+M
         if( p_rwend.gt.LRW ) then
            IERR = 98
            goto 9999
         endif
C
C     Move scale and hcopy
C
         do i = inz, 1, -1      ! I'm not sure if we can use DCOPY here if
                                ! the two arrays overlap...?
            RW(p_scale+i) = RW(p_scale_old+i)
         enddo
         call DCOPY(NZKKT, RW(p_hcopy+1), 1, RW(p_kkt+1), 1)

         goto 1600

      elseif( IERR.ne.0 ) then
         write(line,*) 'get_step_full: MA?7_CALL(2) returns IERR =',IERR
         call C_OUT(2,0,1,line)
         IERR = 506
         goto 9999
      endif
C
C     Check inertia
C
      if( (neigs.ne.M .or. nzero.ne.0) .and. SOC_FLAG.eq.0 ) then

         write(line,*) 'get_step_full: Inertia is neigs =',neigs,
     1        ', nzero =',nzero,' M = ',M
         call C_OUT(1,2,1,line)
         if( RESTO ) then
            call C_OUT(2,1,1,
     1           'get_step_full: Inertia not correct for RESTO!')
C            call DCOPY(N, 0.d0, 0, DX, 1)
C            goto 9999
         endif
         goto 5000              ! Modify KKT matrix
      endif
C
      write(line,1010) REGU
 1010 format(/,'get_step_full: Regularization is REGU = ',d12.5)
      call C_OUT(1,2,2,line)

 1050 continue
C
C     Compute RHS
C
      p_rhs   = p_rwend
      p_rwend = p_rhs + N + M
      if( p_rwend.gt.LRW ) then
         IERR = 98
         goto 9999
      endif
      if( .not.RESTO ) then
         call DCOPY(N, G, 1, RW(p_rhs+1), 1)
         call DSCAL(N, -1.d0, RW(p_rhs+1), 1)
CCTRY2
         if( QMERIT.eq.2 .and. QSOC.eq.2 .and. SOC_FLAG.eq.1 ) then
c$$$            if( QDAMP.ne.0 ) then
c$$$               call C_OUT(2,0,1,
c$$$     1              'get_step_full: hmmm... bounds being violated. ;-)')
               IERR = 4
               goto 9999
c$$$            endif
c$$$            do i = 1, NLB
c$$$               j = ILB(i)
c$$$               RW(p_rhs+j) = RW(p_rhs+j) + BL_SOC(i)
c$$$            enddo
c$$$            do i = 1, NUB
c$$$               j = IUB(i)
c$$$               RW(p_rhs+j) = RW(p_rhs+j) + BU_SOC(i)
c$$$            enddo
         else
            do i = 1, NLB
               j = ILB(i)
               RW(p_rhs+j) = RW(p_rhs+j) + MU/S_L(i)
            enddo
            do i = 1, NUB
               j = IUB(i)
               RW(p_rhs+j) = RW(p_rhs+j) - MU/S_U(i)
            enddo
         endif
         if( LSLACKS ) then
            do i = 1, NLB
               j = ILB(i)
               RW(p_rhs+j) = RW(p_rhs+j) -
     1              SIGMA_L(i)*(X(j) - S_L(i) - BNDS_L(i))
            enddo
            do i = 1, NUB
               j = IUB(i)
               RW(p_rhs+j) = RW(p_rhs+j) -
     1              SIGMA_U(i)*(X(j) + S_U(i) - BNDS_U(i))
            enddo
         endif
      else                      ! RESTO = .true.
      endif
      if( INIT_LAM .and. .not.RESTO ) then
         call DCOPY(M, 0.d0, 0, RW(p_rhs+N+1), 1)
      else
         call DCOPY(M, C, 1, RW(p_rhs+N+1), 1)
         call DSCAL(M, -1.d0, RW(p_rhs+N+1), 1)
         if( DEPCONDIAGVAL.ne.0.d0 ) then
C     According to Larry's comment:
            call DAXPY(M, DEPCONDIAGVAL, LAMOLD, 1, RW(p_rhs+N+1), 1)
         endif
      endif

      if( SOC_FLAG.ne.1 ) then
         call DCOPY(M+N, RW(p_rhs+1), 1, RW(p_rhscopy+1), 1)
      endif
C
C     Do the backsolve
C
 1100 continue
      if( abs(QQUASI).lt.6 .or. RESTO .or. INIT_LAM ) then
         call GSF_BACKSOLVE(N, M, NZKKT, RW(p_kkt+1), IS(P_IRN+1),
     1        IS(P_JCN+1), RW(p_rhs+1), LIWMA, L_INC_LIWMA,
     1        IW(p_iwma+1), LA, L_INC_LA, neigs, nzero, RW(p_scale+1),
     1        LRS, RS, LIS, IS, LRW-p_rwend, RW(p_rwend+1),
     1        LIW-p_iwend, IW(p_iwend+1), IERR)
         if( IERR.ne.0 ) then
            write(line,*)
     1           'get_step_full: GSF_BACKSOLVE returns IERR =',IERR
            call C_OUT(2,0,1,line)
            goto 9999
         endif
      else
         call LMBACKSOLVE(N, M, NZKKT, RW(p_kkt+1), IS(P_IRN+1),
     1        IS(P_JCN+1), RW(p_rhs+1), LIWMA, L_INC_LIWMA,
     1        IW(p_iwma+1), LA, L_INC_LA, neigs, nzero, RW(p_scale+1),
     1        nlm, B(pb_uk+1), B(pb_vk+1),
     1        LRS, RS, LIS, IS, LRW-p_rwend, RW(p_rwend+1),
     1        LIW-p_iwend, IW(p_iwend+1), IERR)
         if( IERR.ne.0 ) then
            write(line,*)
     1           'get_step_full: LMBACKSOLVE returns IERR =',IERR
            call C_OUT(2,0,1,line)
            goto 9999
         endif
      endif
C
C     If in SOC step computation, skip iterative refinement
C
      if( SOC_FLAG.eq.1 ) goto 2400
C
C     Improve solution from iterative refinement
C
      if( l_iter_ref ) then
         call DAXPY(N+M, 1d0, RW(p_rhs_store+1), 1, RW(p_rhs+1), 1)
         p_rwend = p_check
      endif
C
C     Validate solution (in case of very badly scaled matrix, almost singular)
C
      p_check = p_rwend
      p_rwend = p_check + M+N
      if( p_rwend.gt.LRW ) then
         IERR = 98
         goto 9999
      endif
      call DCOPY(N+M, 0d0, 0, RW(p_check+1), 1)
      do i = 1, NZKKT
         kr  = IS(P_IRN+i)
         kc  = IS(P_JCN+i)
         ele = RW(p_hcopy+i)
         if( kr.eq.kc ) then
            RW(p_check+kr) = RW(p_check+kr) - ele*RW(p_rhs+kr)
         else
            RW(p_check+kr) = RW(p_check+kr) - ele*RW(p_rhs+kc)
            RW(p_check+kc) = RW(p_check+kc) - ele*RW(p_rhs+kr)
         endif
      enddo
      if( abs(QQUASI).ge.6 .and. .not.RESTO .and. .not.INIT_LAM ) then
         call LMMULT(N, nlm, B(pb_uk+1), B(pb_vk+1), RW(p_rhs+1),
     1        RW(p_check+1), LRW-p_rwend, RW(p_rwend+1), IERR)
         if( IERR.ne.0 ) then
            write(*,*) 'get_step_full: LMMULT returns IERR = ',IERR
            call C_OUT(2,0,1,line)
            goto 9999
         endif
      endif
      call DAXPY(N+M, 1d0, RW(p_rhscopy+1), 1, RW(p_check+1), 1)
      nrm_res = DNRM2(N+M, RW(p_check+1)  , 1)
      nrm_rhs = DNRM2(N+M, RW(p_rhscopy+1), 1)
      if( nrm_rhs.gt.0.d0 ) then
         nrm_ratio = nrm_res/nrm_rhs
      else
         nrm_ratio = 0.d0
      endif
CTODO      if( nrm_ratio.gt.1d-6 ) then
CTODO      if( nrm_ratio.gt.min(1d-6, MU*1d-2) ) then
CTODO      if( nrm_ratio.gt.min(1d-6, ERR_BAR) ) then
      if( nrm_ratio.gt.min(1d-4, 1.d2*ERR_BAR) ) then
         if( QPIVTOL.lt.QPIVTOLMAX .and. .not.l_incuma27 ) then
            QPIVTOL = min(QPIVTOL*1.d1, QPIVTOLMAX)
            write(line,*) 'get_step_full: increase QPIVTOL to ', QPIVTOL
            call C_OUT(2,0,1,line)
            call DCOPY(NZKKT, RW(p_hcopy+1), 1, RW(p_kkt+1), 1)
            l_incuma27 = .true. ! Make sure we increase QPIVTOL only once per
                                ! iteration
            goto 1500
         endif
         if( l_iter_ref ) then
C
C     Check iterative refinement really helped
C
            if( 0.1d0*nrm_ratio_old .le. nrm_ratio ) then
C
C     No, it didn't.  Modify KKT matrix instead
C
               call C_OUT(2,1,1,
     1              'get_step_full: iterative refinement didn''t help.')
               l_iter_ref = .false.
               goto 5000
            endif
         else
            l_iter_ref = .true.
         endif
         nrm_ratio_old = nrm_ratio
C
         p_rhs_store = p_rwend
         p_rwend     = p_rhs_store + N+M
         if( p_rwend.gt.LRW ) then
            IERR = 98
            goto 9999
         endif
         call DCOPY(N+M, RW(p_rhs+1), 1, RW(p_rhs_store+1), 1)
         call DCOPY(N+M, RW(p_check+1), 1, RW(p_rhs+1), 1) ! well, inefficient
         call C_OUT(2,1,1,'get_step_full: Try iterative refinement.')
         goto 1100

      endif
 5344 continue
      p_rwend = p_check
C
C     If we just computed least square multipliers, get those and go back
C
      if( INIT_LAM ) then
         INIT_LAM = .false.
         call C_OUT(1,1,1,'get_step_full: LAMBDA initialized!')
         call DCOPY(M, RW(p_rhs+N+1), 1, LAM, 1)
         call DCOPY(M, LAM, 1, LAMOLD, 1) ! need this to make sure filter
                                ! is not overwriting lambda for SOC
         call DCOPY(NZKKT, RW(p_hcopy+1), 1, RW(p_kkt+1), 1)
         skip_jac = .true.
         REGU     = 0.d0
         DEPCONDIAGVAL = 0.d0
         p_iwend = p_iwma
         p_rwend = p_kkt
         goto 1000
      endif
 2400 continue
C
C     Store current regularization parameter for next regularization
C
      if( REGU.gt.0.d0 ) then
         REGU_STORE = REGU
      endif
C
C     Get primal search direction
C
      call DCOPY(N, RW(p_rhs+1), 1, DX, 1)
C
C     Get multipliers if requested
C
CTODO CHECK!
      if( QLAMBDA.eq.2 .and. .not.RESTO ) then
         call DCOPY(M, RW(p_rhs+N+1), 1, LAM, 1)
      endif
C
C     Now can forget everything 
C
      p_rwend = p_hcopy
C
C     Get DV_L
C
      if( .not.RESTO ) then
         if( .not.LSLACKS ) then
            do i = 1, NLB
               DV_L(i) = MU/S_L(i) - SIGMA_L(i)*DX(ILB(i)) - V_L(i)
            enddo
         else
            do i = 1, NLB
               DV_L(i) = MU/S_L(i) -
     1              SIGMA_L(i)*(DX(ILB(i)) + X(ILB(i))-S_L(i)-BNDS_L(i))
     2              - V_L(i)
            enddo
         endif
      else
         call DCOPY(NLB, 0d0, 0, DV_L, 1)
      endif
C
C     Get DV_U
C
      if( .not.RESTO ) then
         if( .not.LSLACKS ) then
            do i = 1, NUB
               DV_U(i) = MU/S_U(i) + SIGMA_U(i)*DX(IUB(i)) - V_U(i)
            enddo
         else
            do i = 1, NUB
               DV_U(i) = MU/S_U(i) +
     1              SIGMA_U(i)*(DX(IUB(i)) + X(IUB(i))+S_U(i)-BNDS_U(i))
     2              - V_U(i)
            enddo
         endif
      else
         call DCOPY(NUB, 0d0, 0, DV_U, 1)
      endif
C
      if( RESTO.and.(abs(QRESTO).eq.1.or.abs(QRESTO).eq.3) ) goto 9999
C
C     Compute maximal step size alpha within bounds
C
      call CUTALPHA(N, X, DX, DV_L, DV_U, NLB, ILB, NUB, IUB,
     1              BNDS_L, BNDS_U, S_L, S_U, V_L, V_U, QTAU,
     2              LSLACKS, ALPHA, ALPHA_DUAL, C_ALPHA)
C
      ALPHA_CUT = ALPHA
C
C     Check if need to compute Cauchy step in restoration phase
C
 2500 continue
      if( RESTO.and.(ALPHA_CUT.lt.1d0.or.l_cs) ) then
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
     3                THETA1, THETA2, newbas_tmp, condc_tmp,
     1                KCONSTR, LRS, RS, LIS, IS,
     2                LRW-p_rwend, RW(p_rwend+1),
     4                LIW-p_iwend, IW(p_iwend+1), IERR)
         if( IERR.gt.0 ) then
            write(line,*)
     1      'get_step_full: Error: get_ypy ends with IERR = ',IERR
            call C_OUT(2,0,1,line)
            goto 9999
         elseif( IERR.ne.0 ) then
            write(line,*)
     1      'get_step_full: Warning: get_ypy ends with IERR = ',IERR
            call C_OUT(2,0,1,line)
            IERR = 0
         endif
         call CUTALPHA(N, X, RW(p_dl+1), DV_L, DV_U, NLB, ILB,
     1        NUB, IUB, BNDS_L, BNDS_U, S_L, S_U, V_L, V_U, QTAU,
     2        LSLACKS, ALPHA, ALPHA_DUAL, C_ALPHA)
         if( QALPHA.ne.0 ) then
            write(line,*)
     1           'get_step_full: Not sure if this works with QALPHA = ',
     2           ALPHA
         endif
         if( ALPHA.eq.1d0.and..not.l_cs ) then
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
         if( .not.l_cs ) then
            goto 1235
C
C     Compute constraint violation of both Newton and Cauchy step
C
            p_xtry  = p_rwend
            p_cnew  = p_xtry + N
            p_rwend = p_cnew + M
            if( p_rwend.gt.LRW ) then
               IERR = 98
               goto 9999
            endif
C
            call DCOPY(N, X, 1, RW(p_xtry+1), 1)
            call DAXPY(N, ALPHA, RW(p_dl+1), 1, RW(p_xtry+1), 1)
            call GET_C(ITER, N, NIND, RW(p_xtry+1), IVAR, NORIG, XORIG,
     1           M, CSCALE, RW(p_cnew+1), KCONSTR, LRS, RS, LIS, IS,
     2           LRW-p_rwend, RW(p_rwend+1),
     4           LIW-p_iwend, IW(p_iwend+1), IERR)
            if( IERR.gt.0 ) then
               write(line,*)
     1              'get_step_full: Error: get_c returns IERR = ',IERR
               call C_OUT(2,0,1,line)
               goto 9999
            elseif( IERR.ne.0 ) then
               write(line,*)
     1              'get_step_full: Warning: get_c returns IERR = ',IERR
               call C_OUT(2,0,1,line)
            endif
            nrm_dl = DNRM2(M, RW(p_cnew+1), 1)
C
            call DCOPY(N, X, 1, RW(p_xtry+1), 1)
            call DAXPY(N, ALPHA_CUT, DX, 1, RW(p_xtry+1), 1)
            call GET_C(ITER, N, NIND, RW(p_xtry+1), IVAR, NORIG, XORIG,
     1           M, CSCALE, RW(p_cnew+1), KCONSTR, LRS, RS, LIS, IS,
     2           LRW-p_rwend, RW(p_rwend+1),
     4           LIW-p_iwend, IW(p_iwend+1), IERR)
            if( IERR.gt.0 ) then
               write(line,*)
     1              'get_step_full: Error: get_c2 returns IERR = ',IERR
               call C_OUT(2,0,1,line)
               goto 9999
            elseif( IERR.ne.0 ) then
               write(line,*)
     1             'get_step_full: Warning: get_c2 returns IERR = ',IERR
               call C_OUT(2,0,1,line)
            endif
            nrm_nw = DNRM2(M, RW(p_cnew+1), 1)
            goto 4242

C
C     Compute progress in linear model by dogleg step
C
 1235       continue
            p_tmp = p_rwend
            p_rwend = p_tmp + M
            if( p_rwend.gt.LRW ) then
               IERR= 98
               goto 9999
            endif
            call CONSTR(9, ITER, N, NIND, M, IVAR, NFIX, IFIX,
     1           NORIG, XORIG, CSCALE, RW(p_dl+1), RW(p_tmp+1), idummy,
     3           idummy, KCONSTR(1), RS(KCONSTR(2)+1), KCONSTR(3),
     4           IS(KCONSTR(4)+1), LRW-p_rwend, RW(p_rwend+1),
     5           LIW-p_iwend, IW(p_iwend+1), IERR)
            if( IERR.lt.0 ) then
               write(line,*)
     1              'get_step_full: Warning in CONSTR-9, IERR = ',IERR
               call C_OUT(2,0,1,line)
            elseif( IERR.ne.0 ) then
               write(line,*)
     1              'get_step_full: Error in CONSTR-9, IERR = ',IERR
               call C_OUT(2,0,1,line)
               goto 9999
            endif
            if( ALPHA.lt.1d0 ) then
               call DSCAL(M, ALPHA, RW(p_tmp+1), 1)
            endif
            call DAXPY(M, 1d0, C, 1, RW(p_tmp+1), 1)
            nrm_dl = DNRM2(M, RW(p_tmp+1), 1)
            nrm_nw = (1-ALPHA_CUT)*DNRM2(M, C, 1)


 4242       continue
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
         else
            ALPHA_CUT = ALPHA
            C_ALPHA = 'i'
            call DCOPY(N, RW(p_dl+1), 1, DX, 1)
         endif
      endif
 3000 continue
C
C     In case of Quasi Newton updates, store PZ
C
      if( NIND.gt.0 .and..not.RESTO .and. abs(QQUASI).lt.6 ) then
         call DCOPY(NIND, DX(M+1), 1, PZ, 1)
C This is necessary for QORTHO = 1 in get_ypy:
         call DAXPY(NIND, -1.d0, YPY(M+1), 1, PZ, 1)
      endif
C
C     If we have additional slack equations and use augmented Lagrangian
C     compute those multipliers explicitly
C
      if( LSLACKS .and. QMERIT.eq.3 ) then
         call DCOPY(N, 0d0, 0, LAM(M+1), 1)
         do i = 1, NLB
            j = ILB(i)
            LAM(M+j) = SIGMA_L(i)*
CTODO Decide which one
C     1           (DX(j) + X(j) - S_L(i) - BNDS_L(i)) - MU/S_L(i)
     1           (DX(j) + X(j) - S_L(i) - BNDS_L(i)) - V_L(i)
         enddo
         do i = 1, NUB
            j = IUB(i)
            LAM(M+j) = LAM(M+j) + SIGMA_U(i)*
CTODO Decide which one
C     1           (DX(j) + X(j) + S_U(i) - BNDS_U(i)) + MU/S_U(i)
     1           (DX(j) + X(j) + S_U(i) - BNDS_U(i)) + V_U(i)
         enddo
      endif
C
C     That's it
C
      goto 9999

C
C     Modifying the KKT matrix if inertia is wrong, or iterative refinement
C     didn't help.
C     (If coming from iterative refinement, l_iter_ref is true.
C
 5000 continue
      recomp_regu = .false.
      regu_old = REGU
C      if( RESTO ) then
C         l_cs = .true.
C         WRITE(*,*) 'AW: That'' not yet correct!'
C         p_rwend = p_hcopy
C         p_iwend = p_iwma
CC
CC     Don't take step for duals in restoration phase
CC
C         call DCOPY(NLB, 0d0, 0, DV_L, 1)
C         call DCOPY(NUB, 0d0, 0, DV_U, 1)
C         goto 2500
c      endif
C
C     First try to regularize by adding
C     a small value for the lower right corner of the KKT matrix
C
      if( INIT_LAM ) then
         INIT_LAM = .false.
         call C_OUT(2,1,1,'Least square system singular while '//
     1        'initializing equality multipliers.')
         call C_OUT(2,1,1,'Setting multipliers to zero.')
         call DCOPY(M, 0.d0, 0, LAM, 1)
         p_iwend = p_iwma
         p_rwend = p_kkt
         goto 1000
      endif
C
      if( QDEPCONDIAG.gt.0.d0 .and. M.gt.0 ) then
         if( DEPCONDIAGVAL.eq.0.d0 ) then
            COUNT_DEPCON = COUNT_DEPCON + 1
            DEPCONDIAGVAL = -QDEPCONDIAG
            if( INIT_LAM ) then
               goto 5100
            endif
         endif
      endif

      if( INIT_LAM ) then
         if( QDEPCONDIAG.gt.0.d0 .and. M.gt.0 .and.
     1        DEPCONDIAGVAL.gt.-1d10 ) then
C
C     Increase QDEPCONDIAG further
C
            DEPCONDIAGVAL = 1.d2 * DEPCONDIAGVAL
            write(line,*) 'get_step_full: Increase DEPCONDIAGVAL to ',
     1           DEPCONDIAGVAL
            call C_OUT(2,1,1,line)
            goto 5100
         endif
         if( abs(QMERIT).lt.4 ) then
            IERR = 943
            goto 9999
         endif
         call C_OUT(2,0,1,
     1       'get_step_full: Constraint Jacobian seems rank deficient.')
         call C_OUT(2,0,1,
     1       '               Try feasibility restoration phase.')
         ALPHA = 0.d0
         ALPHA_CUT = 0.d0
         C_ALPHA = 'R'
         goto 3000
      endif

      if( REGU.eq.0d0 ) then
         COUNT_NEG_CURV = COUNT_NEG_CURV + 1
         if( REGU_STORE.eq.0d0 ) then
            REGU = REGU_INIT
         else
            REGU = REGU_DEC_FACT*REGU_STORE
         endif
      else
         if( RESTO ) then       ! This is the second correction...
            call DCOPY(N, 0.d0, 0, DX, 1)
            goto 9999
         endif
         if( REGU_STORE.eq.0d0 ) then
            REGU = REGU_INIT_FACT*REGU
         else
            REGU = REGU_INC_FACT*REGU
         endif
      endif
      IEIGS = IEIGS + 1
C
      if( REGU.gt.REGU_MAX ) then
C
C     First try a larger value of DEPCONDIAGVAL
C
         if( M.gt.0 .and. QDEPCONDIAG.ne.0.d0 .and.
     1        QDEPCONDIAG.lt.1d3 ) then
C     1        QDEPCONDIAG.lt.1d3 .and. nzero.ne.0 ) then
            QDEPCONDIAG   = 1.d2 * QDEPCONDIAG
            DEPCONDIAGVAL = -QDEPCONDIAG
            COUNT_DEPCON = COUNT_DEPCON + 1
            write(line,*) 'get_step_full: REGU becomes too large,',
     1           ' try larger QDEPCONDIAG = ',QDEPCONDIAG
            call C_OUT(2,1,1,line)
            REGU = 0.d0         ! Reset REGU, since problem seems caused by
                                ! constraints 
            recomp_regu = .true.
            goto 5100
         endif
         write(line,*)
     1        'Regularization parameter getting too large (a):', REGU
         call C_OUT(2,0,1,line)
         if( abs(QMERIT).ge.4 .and. ERR_CNSTR.gt.0.d0 ) then
            ALPHA     = 0d0
            ALPHA_CUT = 0d0
            C_ALPHA   = 'R'
            REGU      = 0d0
            goto 3000
         else
            IERR = 10
            goto 9999
         endif	
      endif
C
C     Correct KKT matrix entries
C
 5100 continue
      p_rwend = p_rhscopy + N+M
      p_iwend = p_iwma
      inz = 0
CTODO      if( QCORRECT.eq.2 .or. recomp_regu ) then
      if( recomp_regu ) then
         call DCOPY(N, REGU, 0, RW(p_hcopy+inz+1), 1)
         if( abs(QQUASI).eq.6 ) then
            call DAXPY(N, 1.d0, sigmak, 0, RW(p_hcopy+inz+1), 1)
         endif
CTODO         if( QCORRECT.eq.2 .and. REGU.gt.0.d0 ) then
         if( .false. ) then
            do i = 1, NLB
               j = ILB(i)
               RW(p_hcopy+inz+j) =
     1              dmax1(RW(p_hcopy+inz+j), REGU/(S_L(i)**2))
            enddo
            do i = 1, NUB
               j = IUB(i)
               RW(p_hcopy+inz+j) =
     1              dmax1(RW(p_hcopy+inz+j), REGU/(S_U(i)**2))
            enddo
         endif
         if( NIND.gt.0 ) then
            do i = 1, NLB
               k = ILB(i)
               RW(p_hcopy+inz+k) = RW(p_hcopy+inz+k) + SIGMA_L(i)
            enddo
            do i = 1, NUB
               k = IUB(i)
               RW(p_hcopy+inz+k) = RW(p_hcopy+inz+k) + SIGMA_U(i)
            enddo
         endif
      elseif( .not.INIT_LAM ) then
         call DAXPY(N, 1.d0, REGU-regu_old, 0, RW(p_hcopy+inz+1), 1)
      endif
      inz = inz + N
C
      if( QQUASI.eq.0 ) then
         inz = inz + NNZH
      else
         inz = inz + (NIND*(NIND+1))/2
      endif
      inz = inz + NZA
C      if( QDEPCONDIAG.ne.0.d0 .and. (nzero.ne.0.or.INIT_LAM) ) then
      if( QDEPCONDIAG.ne.0.d0 ) then
         call DCOPY(M, DEPCONDIAGVAL, 0, RW(p_hcopy+inz+1), 1)
         inz = inz + M
      endif
      call DCOPY(NZKKT, RW(p_hcopy+1), 1, RW(p_kkt+1), 1)
C
C     factorize new system
C
      goto 1500


 9999 continue
      return
      end

      subroutine GSF_BACKSOLVE(N, M, NZKKT, KKT, IRN, JCN, RHS, LIWMA,
     1     L_INC_LIWMA, IWMA, LA, L_INC_LA, NEIGS, NZERO, SCALE,
     1     LRS, RS, LIS, IS, LRW, RW, LIW, IW, IERR )
C
C     Perform one backsolve with matrix factorize by Harwell routine
C     including scaling
C
      implicit none
      integer N, M, IRN(*), JCN(*), LIWMA, LA, IWMA(*), NEIGS
      integer NZERO, NZKKT
      double precision KKT(*), RHS(*), SCALE(*)
      logical L_INC_LIWMA, L_INC_LA
      integer LRS
      double precision RS(LRS)
      integer LIS
      integer IS(LIS)
      integer LRW
      double precision RW(LRW)
      integer LIW
      integer IW(LIW)
      integer IERR

      include 'IPOPT.INC'

      integer i
      double precision dummy(1)
      character*80 line

      if( QKKTSCALE.ne.0 ) then
         do i = 1, N+M
            RHS(i) = RHS(i)*SCALE(i)
         enddo
      endif
C
C     Solve the system
C
      if( QSYMSOLV.eq.0 ) then
         call MA47_CALL(3, N+M, NZKKT, KKT, IRN, JCN, RHS, LIWMA,
     1        L_INC_LIWMA, IWMA, LA, L_INC_LA, NEIGS, NZERO,
     1        LRS, RS, LIS, IS, LRW, RW, LIW, IW, IERR)
      elseif( QSYMSOLV.eq.1 ) then
         call MA27_CALL(3, N+M, NZKKT, KKT, IRN, JCN, RHS, LIWMA,
     1        L_INC_LIWMA, IWMA, LA, L_INC_LA, NEIGS, NZERO,
     1        LRS, RS, LIS, IS, LRW, RW, LIW, IW, IERR)
      elseif( QSYMSOLV.eq.2 ) then
         call MA57_CALL(3, N+M, NZKKT, dummy, IRN, JCN, RHS, KKT,
     1        LA, L_INC_LA, IWMA, LIWMA, L_INC_LIWMA, NEIGS, NZERO,
     1        LRS, RS, LIS, IS, LRW, RW, LIW, IW, IERR)
      endif
      if( IERR.ne.0 ) then
         write(line,*) 'gsf_backsolve: MA?7_CALL(3) returns IERR =',IERR
         call C_OUT(2,0,1,line)
         IERR = 507
         goto 9999
      endif
C
C     Undo the scaling
C
      if( QKKTSCALE.ne.0 ) then
         do i = 1, N+M
            RHS(i) = RHS(i)*SCALE(i)
         enddo
      endif

 9999 continue
      return
      end

      subroutine LMBACKSOLVE(N, M, NZKKT, KKT, IRN, JCN, RHS, LIWMA,
     1     L_INC_LIWMA, IWMA, LA, L_INC_LA, NEIGS, NZERO, SCALE,
     1     NLM, UK, VK,
     1     LRS, RS, LIS, IS, LRW, RW, LIW, IW, IERR)
C
C     A backsolve for overall KKT system including limited memory approximation
C     using Sherman-Morrison formula
C
      implicit none
      integer N, M, IRN(*), JCN(*), LIWMA, LA, IWMA(*), NEIGS
      integer NZERO, NZKKT
      double precision KKT(*), RHS(*), SCALE(*)
      logical L_INC_LIWMA, L_INC_LA
      integer NLM
      double precision UK(N,*), VK(N,*)
      integer LRS
      double precision RS(LRS)
      integer LIS
      integer IS(LIS)
      integer LRW
      double precision RW(LRW)
      integer LIW
      integer IW(LIW)
      integer IERR

      integer i, p_rwend, p_iwend, p_m, p_rhs, p_tmp, p_ipiv, info
      character*80 line

      IERR = 0
      p_rwend = 0
      p_iwend = 0
C
C     if NLM is still zero, do regular backsolve
C
      if( NLM.eq.0 ) then
         call GSF_BACKSOLVE(N, M, NZKKT, KKT, IRN, JCN, RHS,
     1        LIWMA, L_INC_LIWMA, IWMA, LA, L_INC_LA, NEIGS, NZERO,
     1        SCALE, LRS, RS, LIS, IS, LRW-p_rwend, RW(p_rwend+1),
     5        LIW-p_iwend, IW(p_iwend+1), IERR)
         goto 9999
      endif

      p_m     = p_rwend
      p_rhs   = p_m + 4*NLM*NLM
      p_rwend = p_rhs + N+M
      if( p_rwend.gt.LRW ) then
         IERR = 98
         goto 9999
      endif
C
C     First compute the dense matrix in the middle of the Sherman-Morrison
C     formula
C
      call DCOPY(4*NLM*NLM, 0.d0, 0, RW(p_m+1), 1)
      do i = 1, 2*NLM
         RW(p_m+i+2*NLM*(i-1)) = 1.d0
      enddo
C
C     Backsolves for the UK part
C
      do i = 1, NLM
         call DCOPY(N, UK(1,i), 1, RW(p_rhs+1), 1)
         call DCOPY(M, 0.d0, 0, RW(p_rhs+N+1), 1)
         call GSF_BACKSOLVE(N, M, NZKKT, KKT, IRN, JCN, RW(p_rhs+1),
     1        LIWMA, L_INC_LIWMA, IWMA, LA, L_INC_LA, NEIGS, NZERO,
     1        SCALE, LRS, RS, LIS, IS, LRW-p_rwend, RW(p_rwend+1),
     5        LIW-p_iwend, IW(p_iwend+1), IERR)
         if( IERR.ne.0 ) then
            write(line,*)
     1           'lmbacksolve: GSF_BACKSOLVE-1 returns IERR =',IERR
            call C_OUT(2,0,1,line)
            goto 9999
         endif
         call DGEMV('T', N, NLM, 1.d0, UK, N, RW(p_rhs+1), 1, 1.d0,
     1        RW(p_m+1+2*NLM*(i-1)), 1)
         call DGEMV('T', N, NLM, -1.d0, VK, N, RW(p_rhs+1), 1, 1.d0,
     1        RW(p_m+NLM+1+2*NLM*(i-1)), 1)
      enddo
C
C     Backsolves for the VK part
C
      do i = 1, NLM
         call DCOPY(N, VK(1,i), 1, RW(p_rhs+1), 1)
         call DCOPY(M, 0.d0, 0, RW(p_rhs+N+1), 1)
         call GSF_BACKSOLVE(N, M, NZKKT, KKT, IRN, JCN, RW(p_rhs+1),
     1        LIWMA, L_INC_LIWMA, IWMA, LA, L_INC_LA, NEIGS, NZERO,
     1        SCALE, LRS, RS, LIS, IS, LRW-p_rwend, RW(p_rwend+1),
     5        LIW-p_iwend, IW(p_iwend+1), IERR)
         if( IERR.ne.0 ) then
            write(line,*)
     1           'lmbacksolve: GSF_BACKSOLVE-2 returns IERR =',IERR
            call C_OUT(2,0,1,line)
            goto 9999
         endif
         call DGEMV('T', N, NLM, 1.d0, UK, N, RW(p_rhs+1), 1, 1.d0,
     1        RW(p_m+1+2*NLM*(NLM+i-1)), 1)
         call DGEMV('T', N, NLM, -1.d0, VK, N, RW(p_rhs+1), 1, 1.d0,
     1        RW(p_m+NLM+1+2*NLM*(NLM+i-1)), 1)
      enddo
C
C     Compute K^{-1} * RHS -> RHS   (K: KKT matrix with only diagonal)
C
      call GSF_BACKSOLVE(N, M, NZKKT, KKT, IRN, JCN, RHS,
     1     LIWMA, L_INC_LIWMA, IWMA, LA, L_INC_LA, NEIGS, NZERO,
     1     SCALE, LRS, RS, LIS, IS, LRW-p_rwend, RW(p_rwend+1),
     5     LIW-p_iwend, IW(p_iwend+1), IERR)
      if( IERR.ne.0 ) then
         write(line,*)
     1        'lmbacksolve: GSF_BACKSOLVE-3 returns IERR =',IERR
         call C_OUT(2,0,1,line)
         goto 9999
      endif
C
C     Compute tmp = K^{-1} * UU * M^{-1} * VV^T RHS
C
      p_tmp   = p_rwend
      p_rwend = p_tmp + 2*NLM
      if( p_rwend.gt.LRW ) then
         IERR = 98
         goto 9999
      endif
      call DGEMV('T', N, NLM, 1.d0, UK, N, RHS, 1, 0.d0, RW(p_tmp+1), 1)
      call DGEMV('T', N, NLM, -1.d0, VK, N, RHS, 1, 0.d0,
     1     RW(p_tmp+NLM+1), 1)
      p_ipiv  = p_iwend
      p_iwend = p_ipiv + 2*NLM
      if( p_iwend.gt.LIW ) then
         IERR = 99
         goto 9999
      endif
      call DGESV( 2*NLM, 1, RW(p_m+1), 2*NLM, IW(p_ipiv+1), RW(p_tmp+1),
     1     2*NLM, info )
      if( info.ne.0 ) then
         write(line,*) 'lmbacksolve: DBESV returns info = ',info
         call C_OUT(2,0,1,line)
         IERR = 823
         goto 9999
      endif
      p_iwend = p_ipiv
      call DGEMV('N', N, NLM, 1.d0, UK, N, RW(p_tmp+1), 1, 0.d0,
     1     RW(p_rhs+1), 1)
      call DGEMV('N', N, NLM, 1.d0, VK, N, RW(p_tmp+NLM+1), 1, 1.d0,
     1     RW(p_rhs+1), 1)
      call DCOPY(M, 0.d0, 0, RW(p_rhs+N+1), 1)
      call GSF_BACKSOLVE(N, M, NZKKT, KKT, IRN, JCN, RW(p_rhs+1),
     1     LIWMA, L_INC_LIWMA, IWMA, LA, L_INC_LA, NEIGS, NZERO,
     1     SCALE, LRS, RS, LIS, IS, LRW-p_rwend, RW(p_rwend+1),
     5     LIW-p_iwend, IW(p_iwend+1), IERR)
      if( IERR.ne.0 ) then
         write(line,*)
     1        'lmbacksolve: GSF_BACKSOLVE-4 returns IERR =',IERR
         call C_OUT(2,0,1,line)
         goto 9999
      endif
C
C     Finally put everything together
C
      call DAXPY(N+M, -1.d0, RW(p_rhs+1), 1, RHS, 1)

 9999 continue
      return
      end

      subroutine LMMULT(N, NLM, UK, VK, VIN, VOUT, LRW, RW, IERR)
C
C     Compute VOUT = VOUT - (UK * UK^T - VK * VK^T)*VIN
C     (note the MINUS!)   ^
C
      implicit none
      integer N, NLM
      double precision UK(N, NLM)
      double precision VK(N, NLM)
      double precision VIN(N), VOUT(N)
      integer LRW
      double precision RW(LRW)
      integer IERR

      integer p_rwend, p_tmp

      IERR = 0
      p_rwend = 0

      if( NLM.eq.0 ) goto 9999  ! nothing to do

      p_tmp   = p_rwend
      p_rwend = p_tmp + NLM
      if( p_rwend.gt.LRW ) then
         IERR = 98
         goto 9999
      endif

      call DGEMV( 'T', N, NLM, 1.d0, UK, N, VIN, 1, 0.d0,
     1     RW(p_tmp+1), 1)
      call DGEMV( 'N', N, NLM, -1.d0, UK, N, RW(p_tmp+1), 1, 1.d0,
     1     VOUT, 1)

      call DGEMV( 'T', N, NLM, 1.d0, VK, N, VIN, 1, 0.d0,
     1     RW(p_tmp+1), 1)
      call DGEMV( 'N', N, NLM, 1.d0, VK, N, RW(p_tmp+1), 1, 1.d0,
     1     VOUT, 1)

 9999 continue
      return
      end
