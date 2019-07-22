C Copyright (C) 2002, Carnegie Mellon University and others.
C All Rights Reserved.
C This code is published under the Common Public License.
C*******************************************************************************
C
      subroutine LINESEARCH(ITER, N, NIND, M, X, IVAR, NLB, ILB, NUB,
     1     IUB, LSLACKS, BNDS_L, BNDS_U, DX, DV_L, DV_U, S_L, S_U,
     2     V_L, V_U, SIGMA_L, SIGMA_U, NORIG, XORIG, CSCALE, MU, ERR,
     3     YPY, THETA1, THETA2, LAM, LAMOLD, REGU,
     2     PZ, ZPZ, G, WCORR, F, C, CNRM0, NEWBAS,
     3     ALPHA, ALPHA_DUAL, LS_COUNT, C_WATCH, NU_OUT,
     4     SKIP_UPDATE, SOC_FLAG, KCONSTR,
     5     LRS, LRS_END, RS, LIS, LIS_END, IS,
     6     LRW, RW, LIW, IW, IERR)
C
C*******************************************************************************
C
C    $Id: linesearch.f,v 1.3 2002/11/24 21:49:57 andreasw Exp $
C
C-------------------------------------------------------------------------------
C                                 Title
C-------------------------------------------------------------------------------
C
CT    Do the linesearch /w watchdog (primal-dual l_2 penalty function)
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
CP   NORIG     I    INT    number of all variables including fixed vars
CP   XORIG    I/O   DP     actual iterate
CP                            XORIG is ordered in ORIGINAL order (i.e. not
CP                            partitioned into independent and dependent
CP                            variables) (on output: as X)
CP   CSCALE    I    DP     scaling factors for constraints
CP   MU        I    DP     barrier parameter
CP   ERR       I    DP     actual KKT-error (needed for switching on watchdog)
CP   YPY       I    DP     range space step (all variables; ordered like X)
CP   THETA1    I    DP     dogleg parameter (=1 <-> full Newton step)
CP   THETA2    I    DP     dogleg parameter (= -c^T*A^T*d_CS )
CP   LAM      I/O   DP     multipliers for equality constraints
CP   LAMOLD   I/O   DP     multipliers for equality constraints from last iter
CP   REGU      I    DP     regularization factor (added regu*I to diagonal)
CP                            (from get_step_full)
CP   PZ        I    DP     null space step (only independent variables)
CP   ZPZ       I    DP     null space step (only dependent variables)
CP                           (only needed if NU based on LAM, i.e. QLAMBDA<>2)
CP   G         I    DP     gradient of objective function
CP   WCORR     I    DP     correction term for PZ
CP   F        I/O   DP     value of objective function at X
CP                            I: for start of line search
CP                            O: after line search
CP   C        I/O   DP     values of constraints at X
CP                            I: for start of line search
CP                            O: after line search
CP   CNRM0     O    DP     2-norm of constraints at old point
CP   NEWBAS    I    L      only .true. if variables have been repartitioned
CP   ALPHA    I/O   DP     step size: I: where to start line search
CP                                    O: step size from X to X_NEW
CP   ALPHA_DUAL I/O DP     step size for dual variables
CP   LS_COUNT  O    INT    number of trial steps
CP   C_WATCH   O    C*1    information about watchdog
CP                            ' ' : WFLAG = 0
CP                            'a' : WFLAG = 1
CP                            'b' : WFLAG = 2
CP                            'c' : WFLAG = 3
CP   NU_OUT    O    DP     actual value of penalty parameter
CP                            (only output; value is stored internally!)
CP   SKIP_UPDATE I/O LOG   if set to .true. if next Quasi-Newton update has
CP                            to be skipped
CP   SOC_FLAG I/O   INT    flag for second order correction:
CP                            =0: entered first time in this iteration
CP                            =1: OUT: compute SOC direction
CP                                IN : directions have SOC direction part in it
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
CS    DDOT
CS    DAXPY
CS    DCOPY
CS    DSCAL
CS    CALC_BAR
CS    CALC_NRM
CS    UPDATE_NU
CS    ARMIJO
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
      double precision X(N)
      integer IVAR(N)
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
      integer NORIG
      double precision XORIG(NORIG)
      double precision CSCALE(*)
      double precision MU
      double precision ERR
      double precision YPY(N)
      double precision THETA1
      double precision THETA2
      double precision LAM(M)
      double precision LAMOLD(M)
      double precision REGU
      double precision PZ(NIND)
      double precision ZPZ(M)
      double precision G(N)
      double precision WCORR(NIND)
      double precision F
      double precision C(M)
      double precision CNRM0
      logical NEWBAS
      double precision ALPHA
      double precision ALPHA_DUAL
      integer LS_COUNT
      character*1 C_WATCH
      double precision NU_OUT
      logical SKIP_UPDATE
      integer SOC_FLAG
      integer KCONSTR(6)
      integer LRS
      integer LRS_END
      double precision RS(*)
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
      double precision DDOT, CALC_BAR, CALC_NRM, D1MACH
      integer IDAMAX

      double precision PHI0_STORE, DPHI_STORE, ALPHA_STORE, NU_STORE, NU
      save             PHI0_STORE, DPHI_STORE, ALPHA_STORE, NU_STORE, NU
      double precision PHI0_STORE_B, CNRM0_STORE, ALPHA_DUAL_STORE
      save             PHI0_STORE_B, CNRM0_STORE, ALPHA_DUAL_STORE
      double precision DPHI_STORE_B, DPHI_STORE_C, phi0, dphi
      save             DPHI_STORE_B, DPHI_STORE_C, phi0, dphi
      integer          WFLAG, SOC_ITER
      save             WFLAG, SOC_ITER
      logical          LSLACKS_STORE
      save             LSLACKS_STORE

      integer P_XSTORE, P_VLSTORE, P_VUSTORE, P_SLSTORE, P_NUS
      save    P_XSTORE, P_VLSTORE, P_VUSTORE, P_SLSTORE, P_NUS
      integer P_SUSTORE, P_DXSTORE, P_DVLSTORE, P_DVUSTORE
      save    P_SUSTORE, P_DXSTORE, P_DVLSTORE, P_DVUSTORE
      integer P_NUSSTORE, P_CSTORE, P_LAMSTORE, P_PZSTORE
      save    P_NUSSTORE, P_CSTORE, P_LAMSTORE, P_PZSTORE

      double precision gd, tmp1, tmp2
      double precision rhs, phi_new, f_new, alpha_cut
      double precision phi0_b, cnrm_new, phi_new_b, dphi_b, dphi_c
      double precision phi0_old_newnu
      double precision dphi_old_newnu, alpha_min, machtiny
      integer p_xnew, p_slnew, p_sunew, p_vlnew, p_vunew, p_cnew
      integer i, k, p_rwend, p_iwend, p_gb, p_tmp, p_cs
      logical takefull
      character*100 line(4)
c$$$CTRY2
c$$$      double precision BL_SOC(10000), BU_SOC(10000)
c$$$      common /SOC/ BL_SOC, BU_SOC
C
C*******************************************************************************
C
C                           Executable Statements
C
C*******************************************************************************
C

C
C     if ITER = -1 do initializations
C
      if( ITER.eq.-1 ) then

         if( QSOC.ne.0 ) then
C
C     Reserve storage space for second order correction option
C
            P_DXSTORE  = LRS_END
            P_DVLSTORE = P_DXSTORE  + N
            P_DVUSTORE = P_DVLSTORE + NLB
            P_LAMSTORE = P_DVUSTORE + NUB
            LRS_END    = P_LAMSTORE + M
            if( QQUASI.ne.0 .and. abs(QQUASI).lt.6 ) then
               P_PZSTORE = LRS_END
               LRS_END   = P_PZSTORE + NIND
            endif
C
C        If Watchdog is chosen, reserve storage space
C
         elseif( QWATCHTOL.ne.0.d0 ) then
            P_XSTORE   = LRS_END
            P_VLSTORE  = P_XSTORE   + N
            P_VUSTORE  = P_VLSTORE  + NLB
            P_SLSTORE  = P_VUSTORE  + NUB
            P_SUSTORE  = P_SLSTORE  + NLB
            P_DXSTORE  = P_SUSTORE  + NUB
            P_DVLSTORE = P_DXSTORE  + N
            P_DVUSTORE = P_DVLSTORE + NLB
            LRS_END    = P_DVUSTORE + NUB
            if( QMERIT.lt.0 ) then
               P_CSTORE   = LRS_END
               P_NUSSTORE = P_CSTORE   + M
               LRS_END    = P_NUSSTORE + M
               if( QMOVEINIT.eq.0 ) then
                  LRS_END = P_NUSSTORE + M+N
               endif
            endif
         endif

C
C     If indidual penalty parameters for each constraint get storage for those
C
         if( QMERIT.lt.0 ) then
            P_NUS      = LRS_END
            LRS_END    = P_NUS + M
            if( QMOVEINIT.eq.0 ) then
               LRS_END = P_NUS + M+N
            endif
         else
            P_NUS      = 0
         endif

         goto 9999
      endif

      p_rwend = 0
      p_iwend = 0
C
C     The following is not necessary if SOC direction has just been computer
C
      if( SOC_FLAG.eq.0 ) then

         SOC_ITER = 0

C
C     Do some initializations in first iteration
C
         if( ITER.eq.0 ) then
            WFLAG = 0
            if( QMERIT.lt.0 ) then
               call DCOPY(M, QNUMIN, 0, RS(P_NUS+1), 1)
               if( LSLACKS ) then
                  call DCOPY(N, QNUMIN, 0, RS(P_NUS+M+1), 1)
               endif
            else
               NU = QNUMIN
            endif
         endif
C
C     Compute current value of constraint violation
C
         CNRM0 = CALC_NRM(M, C)
C
         if( LSLACKS ) then
C
C     compute additional slack constraints
C
            if( QCNRM.ne.2 ) then
               IERR = 4
               goto 9999
            endif
            p_cs = p_rwend
            p_rwend = p_cs + N
            if( p_rwend.gt.LRW ) then
               IERR = 98
               goto 9999
            endif
            call DCOPY(N, 0d0, 0, RW(p_cs+1), 1)
            do i = 1, NLB
               k = ILB(i)
               RW(p_cs+k) = X(k) - S_L(i) - BNDS_L(i)
            enddo
            do i = 1, NUB
               k = IUB(i)
               RW(p_cs+k) = X(k) + S_U(i) - BNDS_U(i)
            enddo
            CNRM0 = dsqrt(CNRM0**2 +
     1           DDOT(N, RW(p_cs+1), 1, RW(p_cs+1), 1))
         endif
C
C     Check, if watchdog need to be waken up or put to sleep again
C
         if( NEWBAS ) then
            if( WFLAG.ne.0 ) then
               write(line,*)
     1              'WARNING: Set WFLAG = 0 after Change of Basis!'
               call C_OUT(2,0,1,line)
               WFLAG = 0
            endif
         endif
         if( M.eq.0 .or. QMERIT.eq.0 .or. QSOC.ne.0 ) then
            WFLAG = 0
         elseif( QWATCHTOL.lt.0.d0 ) then
            if( WFLAG.eq.0 .and. CNRM0.lt.-QWATCHTOL ) then
               WFLAG = 1
            elseif( WFLAG.eq.1 .and. CNRM0.ge.-QWATCHTOL ) then
               WFLAG = 0
            endif
         else
            if( WFLAG.eq.0 .and. ERR.lt.QWATCHTOL ) then
               WFLAG = 1
            elseif( WFLAG.eq.1 .and. ERR.ge.QWATCHTOL ) then
               WFLAG = 0
            endif
         endif
C
C     Compute gradient of barrier function
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
C
C     Update penalty parameter
C
         call UPDATE_NU(ITER, N, NIND, M, X, NLB, ILB, NUB, IUB,
     1        LSLACKS, BNDS_L, BNDS_U, S_L, S_U, SIGMA_L, SIGMA_U, MU,
     1        ERR, YPY, THETA1, THETA2,
     3        LAM, REGU, ZPZ, PZ, DX, RW(p_gb+1), WCORR, F, C,
     2        CNRM0, WFLAG, NU, RS(P_NUS+1), LRW-p_rwend, RW(p_rwend+1),
     4        IERR)
         if( IERR.gt.0 ) then
            write(line,*) 'linesearch: UPDATE_NU returns IERR = ', IERR
            call C_OUT(2,0,1,line)
            goto 9999
         elseif( IERR.lt.0 ) then
            write(line,*) 'linesearch: Warning: ',
     1           'UPDATE_NU returns IERR = ', IERR
            call C_OUT(2,0,1,line)
            IERR = 0
         endif
C
C     Compute directional derivative
C
C     objective function
         gd = DDOT(N, DX, 1, RW(p_gb+1), 1)
         p_rwend = p_gb
         if( LSLACKS ) then
            do i = 1, NLB
               k = ILB(i)
               gd = gd - MU/S_L(i)*(X(k) - S_L(i) - BNDS_L(i))
            enddo
            do i = 1, NUB
               k = IUB(i)
               gd = gd + MU/S_U(i)*(X(k) + S_U(i) - BNDS_U(i))
            enddo
         endif
C     constraints
 554     continue
         if( CNRM0.gt.0 ) then
            if( QMERIT.gt.0 ) then
               dphi_c = THETA1*CNRM0 + (1.d0-THETA1)*THETA2/CNRM0
            elseif( QMERIT.ne.0 ) then
               if( QLAMBDA.eq.0 ) then
                  call C_OUT(2,0,1,
     1                 'linesearch: Need multipliers for this option')
                  IERR = 4
                  goto 9999
               endif
               dphi_c = 0d0
               do i = 1, M
                  dphi_c = dphi_c - dabs(RS(P_NUS+i)*C(i))
               enddo
               if( LSLACKS ) then
                  do i = 1, N
                     dphi_c = dphi_c - dabs(RS(P_NUS+M+i)*RW(p_cs+i))
                  enddo
               endif
            endif
         else
            dphi_c = 0.d0
         endif
C
         dphi_b = gd
         if( abs(QMERIT).eq.1) then
            if( QPRIMAL.eq.1 ) then
               call C_OUT(2,0,1,
     1              'Can''t do primal version and p-d merit function.')
               IERR = 4
               goto 9999
            endif
            do i = 1, NLB
               tmp1 = S_L(i)*V_L(i)
               tmp2 = MU - tmp1
               dphi_b = dphi_b - (tmp2**2)/tmp1
            enddo
            do i = 1, NUB
               tmp1 = S_U(i)*V_U(i)
               tmp2 = MU - tmp1
               dphi_b = dphi_b - (tmp2**2)/tmp1
            enddo
         endif
C
         if( QMERIT.gt.0 ) then
 555        continue
            dphi = dphi_b - NU*dphi_c
            if( QFULL.eq.1 ) then
               if( dphi.gt.-QRHO*NU*dphi_c ) then
                  if( dphi_c.eq.0.d0 ) then
                     dphi = 0.d0
                  else
                     if( NU.gt.1d300 ) then
                        IERR = 736
                        goto 9999
                     endif
                     NU = 10*NU
                     goto 555
                  endif
               endif
            endif
         elseif( QMERIT.lt.0 ) then
            dphi = dphi_b + dphi_c
            if( QFULL.eq.1 ) then
               if( dphi.gt.QRHO*dphi_c ) then
                  if( dphi_c.eq.0.d0 ) then
                     dphi = 0.d0
                  else
                     call DSCAL(M, 1.d1, RS(P_NUS+1), 1)
                     if( LSLACKS ) then
                        call DSCAL(N, 1.d1, RS(P_NUS+M+1), 1)
                     endif
                     if( .not.LSLACKS ) then
                        i = IDAMAX(M, RS(P_NUS+1), 1)
                     else
                        i = IDAMAX(M+N, RS(P_NUS+1), 1)
                     endif
                     if( RS(P_NUS+i).gt.1d300 ) then
                        IERR = 736
                        goto 9999
                     endif
                     goto 554
                  endif
               endif
            endif
         endif
         if( LSLACKS ) then
            p_rwend = p_cs
         endif
C
         if( dphi.gt.0.d0 .and. QMERIT.ne.0 ) then
            write(line,*) 'Problem in linesearch: dphi = ',dphi
            call C_OUT(2,0,1,line)
            IERR = 589
            goto 9999
         endif
C
C     Compute merit function at current point
C
         phi0_b = F + CALC_BAR(NLB, NUB, S_L, S_U, V_L, V_U, MU)
C
         if( QMERIT.gt.0 ) then
            phi0 = phi0_b + NU*CNRM0
         else
            phi0 = phi0_b - dphi_c
         endif

      endif
C
      if( QCNR.gt.0 .and. QPRINT.ge.3 ) then
         write(line,711) NU, dphi, phi0
 711     format('NU = ', d20.12,' dphi = ', d20.12,' phi0 = ', d20.12)
         call C_OUT(1,3,1,line)
         write(line,712) phi0_b, cnrm0
 712     format('phi0_bv = ', d20.12,' cnrm0 = ', d20.12)
         call C_OUT(1,3,1,line)
      endif
C
C     reserve work space for trial points
C
      p_xnew    = p_rwend
      p_slnew   = p_xnew   + N
      p_sunew   = p_slnew  + NLB
      p_vlnew   = p_sunew  + NUB ! don't need that for QMERIT <> 1
      p_vunew   = p_vlnew  + NLB ! don't need that for QMERIT <> 1
      p_cnew    = p_vunew  + NUB
      p_rwend   = p_cnew   + M
      if( p_rwend.gt.LRW ) then
         IERR = 98
         goto 9999
      endif
C
C     if in watchdog, might want to take full step
C
      if( WFLAG.eq.1 .or. QMERIT.eq.0 .or. QSOC.ne.0 ) then
         takefull = .true.
      else
         takefull = .false.
      endif
C
C     If in watchdog trial, have earier ls failure
C
      if( WFLAG.eq.2 ) then
         alpha_min = 1.d-5
      else
         alpha_min = 1.d-14
      endif
C     Do the armijo line search and obtain new values for everything
C
 50   alpha_cut = ALPHA
      call ARMIJO(ITER, N, M, NIND, X, IVAR, NLB, ILB, NUB, IUB,
     1            LSLACKS, BNDS_L, BNDS_U, DX, DV_L, DV_U,
     1            S_L, S_U, V_L, V_U, NORIG, XORIG, CSCALE, takefull,
     2            alpha_min, ALPHA, phi0, dphi, NU, RS(P_NUS+1), MU,
     4            RW(p_xnew+1), RW(p_slnew+1), RW(p_sunew+1),
     4            RW(p_vlnew+1), RW(p_vunew+1),
     4            f_new, RW(p_cnew+1),
     4            phi_new, phi_new_b, cnrm_new, LS_COUNT,
     1            KCONSTR, LRS, RS, LIS, IS,
     2            LRW-p_rwend, RW(p_rwend+1),
     4            LIW-p_iwend, IW(p_iwend+1), IERR)
      if( IERR.gt.0 ) then
         write(line,*) 'linesearch: ARMIJO returns IERR = ', IERR
         call C_OUT(2,0,1,line)
         if( IERR.eq.2 ) then
            call C_OUT(2,0,1,'line search failure.')
         endif
         goto 9999
      elseif( IERR.lt.0 ) then
         write(line,*) 'linesearch: Warning: ',
     1        'ARMIJO returns IERR = ', IERR
         call C_OUT(2,0,1,line)
         IERR = 0
      endif
C
C     For Second Order Correction: Check, if SOC needed
C
      if( QSOC.ne.0 .and. SOC_FLAG.eq.0 .and. .not.takefull ) then
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
         if( QLAMBDA.eq.2 ) then
            call DCOPY(M, LAM, 1, RS(P_LAMSTORE+1), 1)
            call DCOPY(M, LAMOLD, 1, LAM, 1)
         endif
         if( QQUASI.ne.0 .and. abs(QQUASI).lt.6 ) then
            call DCOPY(NIND, PZ, 1, RS(P_PZSTORE+1), 1)
         endif
         ALPHA_STORE      = alpha_cut
         ALPHA_DUAL_STORE = ALPHA_DUAL
         CNRM0_STORE      = cnrm_new
C
C     Compute new right hand side for constraints
C
         call DSCAL(M, ALPHA, C, 1)
         call DAXPY(M, 1.d0, RW(p_cnew+1), 1, C, 1)
c$$$CTRY
c$$$         do i = 1, NLB
c$$$            BL_SOC(i) = MU/S_L(i)
c$$$         enddo
c$$$         do i = 1, NUB
c$$$            BU_SOC(i) = - MU/S_U(i)
c$$$         enddo
c$$$         call DSCAL(NLB, ALPHA, BL_SOC, 1)
c$$$         do i = 1, NLB
c$$$            BL_SOC(i) = BL_SOC(i) + MU/RW(p_slnew+i)
c$$$         enddo
c$$$         call DSCAL(NUB, ALPHA, BU_SOC, 1)
c$$$         do i = 1, NUB
c$$$            BU_SOC(i) = BU_SOC(i) - MU/RW(p_sunew+i)
c$$$         enddo
c$$$CTRY END

C
         SOC_FLAG = 1
         SOC_ITER = 1

         goto 9999

      elseif( QSOC.ne.0 .and. SOC_FLAG.eq.1 .and. .not.takefull ) then

CTODO Put some constant in here or so...
         if( cnrm_new.ge.0.99d0*CNRM0_STORE .or. SOC_ITER.gt.10 ) then
C
C     SOC didn't help
C
            if( cnrm_new.ge.0.99d0*CNRM0_STORE ) then
               write(line,*)
     1              'Second order correction rejected with cnrm_new = ',
     2              cnrm_new
               call C_OUT(1,2,1,line)
            elseif( SOC_ITER.gt.10 ) then
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
            if( QLAMBDA.eq.2 ) then
               call DCOPY(M, RS(P_LAMSTORE+1), 1, LAM, 1)
            endif
            if( QQUASI.ne.0 .and. abs(QQUASI).lt.6 ) then
               call DCOPY(NIND, RS(P_PZSTORE+1), 1, PZ, 1)
            endif
            ALPHA      = ALPHA_STORE
            ALPHA_DUAL = ALPHA_DUAL_STORE
C
C     Do Armijo line search with these restored directions
C
C
            SOC_FLAG = 2
            goto 50

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

            CNRM0_STORE = cnrm_new
            if( QLAMBDA.eq.2 ) then
               call DCOPY(M, LAMOLD, 1, LAM, 1)
            endif
C
C     Compute new right hand side for constraints
C
            call DSCAL(M, ALPHA, C, 1)
            call DAXPY(M, 1.d0, RW(p_cnew+1), 1, C, 1)
c$$$CTRY
c$$$            call DSCAL(NLB, ALPHA, BL_SOC, 1)
c$$$            do i = 1, NLB
c$$$               BL_SOC(i) = BL_SOC(i) + MU/RW(p_slnew+i)
c$$$            enddo
c$$$            call DSCAL(NUB, ALPHA, BU_SOC, 1)
c$$$            do i = 1, NUB
c$$$               BU_SOC(i) = BU_SOC(i) - MU/RW(p_sunew+i)
c$$$            enddo
c$$$CTRY END
C
            SOC_ITER = SOC_ITER + 1

            goto 9999

         endif

      endif
C
C     Do the watchdog handling
C
      goto (100, 200, 300) WFLAG

      C_WATCH = ' '
      if( SOC_FLAG.eq.1 ) then
         C_WATCH = 'S'
         SOC_FLAG = 2
      endif

      goto 400

 100  continue

CCHECK         rhs = phi0 + 1.d-1*ALPHA*dphi
         rhs = phi0 + 1.d-4*ALPHA*dphi
         if( phi_new.gt.rhs ) then
C
C        Full step doesn't satisfy armijo condition, store old values
C
            WFLAG = 2
            C_WATCH = 'b'
            call DCOPY(N,   X,    1, RS(P_XSTORE  +1), 1)
            call DCOPY(NLB, V_L,  1, RS(P_VLSTORE +1), 1)
            call DCOPY(NUB, V_U,  1, RS(P_VUSTORE +1), 1)
            call DCOPY(NLB, S_L,  1, RS(P_SLSTORE +1), 1)
            call DCOPY(NUB, S_U,  1, RS(P_SUSTORE +1), 1)
            call DCOPY(N,   DX,   1, RS(P_DXSTORE +1), 1)
            call DCOPY(NLB, DV_L, 1, RS(P_DVLSTORE+1), 1)
            call DCOPY(NUB, DV_U, 1, RS(P_DVUSTORE+1), 1)
            PHI0_STORE = phi0
            DPHI_STORE = dphi
            NU_STORE = NU
            ALPHA_STORE = alpha_cut
            ALPHA_DUAL_STORE = ALPHA_DUAL
            PHI0_STORE_B = phi0_b
            CNRM0_STORE = cnrm0
            DPHI_STORE_B = dphi_b
            LSLACKS_STORE = LSLACKS
            if( QMERIT.gt.0 ) then
               DPHI_STORE_C = dphi_c
            else
               call DCOPY(M, C          , 1, RS(P_CSTORE  +1), 1)
               call DCOPY(M, RS(P_NUS+1), 1, RS(P_NUSSTORE+1), 1)
               if( LSLACKS ) then
                  call DCOPY(N, RS(P_NUS+M+1), 1, RS(P_NUSSTORE+M+1), 1)
               endif
            endif
         else
            C_WATCH = 'a'
         endif
         goto 400

 200  continue

C This changes new values according to old NU (probably wrong!)
C         rhs = PHI0_STORE + 1.d-1*DPHI_STORE
C         phi0_oldnu = phi0_b + NU_STORE*cnrm0
C         phi_new_oldnu = phi_new_b + NU_STORE*cnrm_new
C         if( phi0_oldnu.le.PHI0_STORE .or. phi_new_oldnu.le.rhs ) then
C            WFLAG = 1
C            C_WATCH = 'a'
C         else
C            if( phi_new_oldnu.gt.PHI0_STORE ) then

C This changes old values according to NEW NU
         phi0_old_newnu = PHI0_STORE_B + NU*CNRM0_STORE
         if( QMERIT.gt.0 ) then
            dphi_old_newnu = DPHI_STORE_B - NU*DPHI_STORE_C
         else
            dphi_old_newnu = DPHI_STORE_B
            do i = 1, M
               dphi_old_newnu = dphi_old_newnu -
     1              RS(P_NUS+i)*dabs(RS(P_CSTORE+i))
            enddo
            if( LSLACKS ) then
               p_tmp = p_rwend
               p_rwend = p_tmp + N
               if( p_rwend.gt.LRW ) then
                  IERR = 98
                  goto 9999
               endif
               call DCOPY(N, 0d0, 0, RW(p_tmp+1), 1)
               do i = 1, NLB
                  k = ILB(i)
                  RW(p_tmp+k) = RS(P_XSTORE+k) - RS(P_SLSTORE+i) -
     1                 BNDS_L(i)
               enddo
               do i = 1, NUB
                  k = IUB(i)
                  RW(p_tmp+k) = RS(P_XSTORE+k) + RS(P_SUSTORE+i) -
     1                 BNDS_U(i)
               enddo
               do i = 1, N
                  dphi_old_newnu = dphi_old_newnu -
     1                 RS(P_NUS+M+i)*dabs(RW(p_tmp+i))
               enddo
               p_rwend = p_tmp
            endif
         endif
         rhs = phi0_old_newnu + 1.d-1*dphi_old_newnu
         if( phi0.le.phi0_old_newnu .or. phi_new.le.rhs ) then
            WFLAG = 1
            C_WATCH = 'a'
         else
            if( phi_new.gt.phi0_old_newnu ) then
C
C        Do armijo ls starting from old point
C
               takefull = .false.
               ALPHA = ALPHA_STORE
               ALPHA_DUAL = ALPHA_DUAL_STORE
               alpha_min = 1.d-14
               LSLACKS = LSLACKS_STORE
               call ARMIJO(ITER, N, M, NIND, RS(P_XSTORE+1),
     1                     IVAR, NLB, ILB, NUB, IUB,
     1                     LSLACKS, BNDS_L, BNDS_U,
     2                     RS(P_DXSTORE+1), RS(P_DVLSTORE+1),
     3                     RS(P_DVUSTORE+1), RS(P_SLSTORE+1),
     4                     RS(P_SUSTORE+1), RS(P_VLSTORE+1),
     5                     RS(P_VUSTORE+1), NORIG, XORIG, CSCALE,
     5                     takefull, alpha_min,
     6                     ALPHA, PHI0_STORE, DPHI_STORE, NU_STORE,
     7                     RS(P_NUSSTORE+1), MU,
     4                     RW(p_xnew+1), RW(p_slnew+1), RW(p_sunew+1),
     4                     RW(p_vlnew+1), RW(p_vunew+1),
     4                     f_new, RW(p_cnew+1),
     4                     phi_new, phi_new_b, cnrm_new, LS_COUNT,
     1                     KCONSTR, LRS, RS, LIS, IS,
     2                     LRW-p_rwend, RW(p_rwend+1),
     4                     LIW-p_iwend, IW(p_iwend+1), IERR)
               if( IERR.gt.0 ) then
                  write(line,*) 'linesearch: ARMIJO returns IERR = ',
     1                       IERR
                  call C_OUT(2,0,1,line)
                  if( IERR.eq.2 ) then
                     call C_OUT(2,0,1,'ls failure in WFLAG = 2.')
                     IERR = 3
                  endif
                  goto 9999
               elseif( IERR.lt.0 ) then
                   write(2,*) 'linesearch: Warning: ',
     1                  'ARMIJO returns IERR = ', IERR
                   call C_OUT(2,0,1,line)
                   IERR = 0
               endif
               WFLAG = 1
               C_WATCH = 'r'
C Take old value of NU!
               NU = NU_STORE
               call DCOPY(M, RS(P_NUSSTORE+1), 1, RS(P_NUS+1), 1)
               if( LSLACKS ) then
                  call DCOPY(N, RS(P_NUSSTORE+M+1), 1, RS(P_NUS+M+1), 1)
               endif
CTODO skipping BFGS in watchdog?
C
C     ANDREAS: ist das der einzige Fall in dem BFGS nicht stattfinden soll???
C
CCCRAUS?               SKIP_UPDATE = .true.
            else
               WFLAG = 3
               C_WATCH = 'c'
            endif
         endif
         goto 400

 300  continue

         WFLAG = 1
         C_WATCH = 'a'

 400  continue
C
C     Copy trial point to real point
C
      F = f_new
      call DCOPY(N,   RW(p_xnew +1), 1, X  , 1)
      call DCOPY(M,   RW(p_cnew +1), 1, C  , 1)
      call DCOPY(NLB, RW(p_slnew+1), 1, S_L, 1)
      call DCOPY(NUB, RW(p_sunew+1), 1, S_U, 1)
      if( abs(QMERIT).eq.1 ) then
         call DCOPY(NLB, RW(p_vlnew+1), 1, V_L, 1)
         call DCOPY(NUB, RW(p_vunew+1), 1, V_U, 1)
      else
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
C     Make sure that each V_NEW is at least machtiny
C
         machtiny = D1MACH(1)
         do i = 1, NLB
            V_L(i) = dmax1(machtiny, V_L(i))
         enddo
         do i = 1, NUB
            V_U(i) = dmax1(machtiny, V_U(i))
         enddo

      endif
C
C     Free work space
C
      p_rwend = p_xnew
C
C     Take step in LAM
C
CTODO this also for QLAMBDA = 1 ???
CWEG
      if( QLAMBDA.eq.2 ) then
CTODO decide which option
CORIG         if( ITER.gt.0 ) then
         if( .true. ) then
            call DSCAL(M, ALPHA, LAM, 1)
            call DAXPY(M, 1d0-ALPHA, LAMOLD, 1, LAM, 1)
         endif
         call DCOPY(M, LAM, 1, LAMOLD, 1)
      endif
C
C     Copy value of NU into NU_OUT for the output
C
      NU_OUT = NU
C
C     Set LSLACKS
C
      if( ALPHA.eq.1.d0 ) then
         LSLACKS = .false.
      endif
C
C     That's it
C
 9999 continue
      return
      end
