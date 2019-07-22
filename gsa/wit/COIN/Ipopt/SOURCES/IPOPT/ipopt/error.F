C Copyright (C) 2002, Carnegie Mellon University and others.
C All Rights Reserved.
C This code is published under the Common Public License.
C*******************************************************************************
C
      subroutine ERROR(N, NIND, M, X, G, RG, RV, C, CSCALE, NLB, ILB,
     1     NUB, IUB,
     1     BNDS_L, BNDS_U, S_L, S_U, V_L, V_U, SIGMA_L, SIGMA_U,
     1     LAM, MU, LSLACKS,
     2     ERR, C_ERR, ERR_DINF, ERR_CNSTR, ERR_CMPL, ERR_DINF_US,
     1     ITER, IVAR, NFIX, IFIX, NORIG, XORIG, DX,
     1     KCONSTR, LRS, RS, LIS, IS,
     1     LRW, RW, LIW, IW, IERR)
C
C*******************************************************************************
C
C    $Id: error.F,v 1.2 2002/07/24 03:14:37 andreasw Exp $
C
C-------------------------------------------------------------------------------
C                                 Title
C-------------------------------------------------------------------------------
C
CT    reduced KKT error for barrier problem
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
CP   N         I    INT    number of (free) variables; first M variables
CP                         are dependent; remaining independent
CP   NIND      I    INT    number of independent variables
CP   M         I    INT    number of equality constraints
CP   X         I    DP     actual primal iterate
CP   G         I    DP     gradient of objective function
CP   RG        I    DP     reduced gradient of objective function
CP   RV        I    DP     reduced dual variables
CP   C         I    DP     values of constraints
CP   CSCALE    I    DP     scaling factors for constraints
CP   NLB       I    INT    number of lower bounds (excluding fixed vars)
CP   ILB       I    INT    indices of lower bounds
CP                            (e.g. S_L(i) is slack for X(ILB(i)) )
CP   NUB       I    INT    number of upper bounds (excluding fixed vars)
CP   ILB       I    INT    indices of upper bounds
CP                            (e.g. S_U(i) is slack for X(IUB(i)) )
CP   BNDS_L    I    DP     values of lower bounds (ordered as S_L)
CP   BNDS_U    I    DP     values of upper bounds (ordered as S_U)
CP   S_L       I    DP     slack variables for lower bounds
CP   S_U       I    DP     slack variables for upper bounds
CP   V_L       I    DP     dual variables for lower bounds
CP   V_U       I    DP     dual variables for upper bounds
CP   SIGMA_L   I    DP     primal-dual Hessian of lower bound barrier term
CP                            (NLB diagonal elements only)
CP   SIGMA_U   I    DP     primal-dual Hessian of upper bound barrier term
CP                            (NUB diagonal elements only)
CP   LAM       I    DP     Lagrangian multipliers
CP   MU        I    DP     barrier parameter (=0, if Error should be computed
CP                            for overall NLP)
CP   LSLACKS   I    LOG    =.true.:  There are slacks that are violating
CP                                   X - S +- BNDS = 0
CP                         =.false.: not .true.
CP   ERR       O    DP     computed error
CP   C_ERR     O    C*1    characterizes worst violation
CP                            ='d':  reduced gradient of Lagranian
CP                            ='m':  complementarity equation
CP                            ='c':  constraints
CP                            ='s':  slack constraints
CP   ERR_DINF  O    DP     unscaled/scaled max norm of dual infeasibility
CP                            (red. gradient in case of reduced space approach)
CP   ERR_CNSTR O    DP     unscaled/scaled max norm of infeasibility
CP   ERR_CMPL  O    DP     unscaled/scaled max norm for relaxed complementarity
CP   ERR_DINF_US  O    DP     like ERR_DINF, but unscaled in any case
CP   ITER      I    INT    iteration counter
CP                         if -1: Initialize LASTITER
CP                         otherwise: It is assumed that for subsequent calls
CP                                    with same ITER, only nrm2 has to be
CP                                    recomputed.
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
CP   DX        I    DP     primal search direction from previous iteration
CP                            (needed for LSLACKS and QLAMBDA=2)
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
CS    DASUM
CS    DCOPY
CS    DAXPY
CS    IDAMAX
CS    C_OUT
CS    CONSTR
CS    EVAL_CCERR
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
      double precision G(N)
      double precision RG(NIND)
      double precision RV(NIND)
      double precision C(M)
      double precision CSCALE(M)
      integer NLB
      integer ILB(NLB)
      integer NUB
      integer IUB(NUB)
      double precision BNDS_L(NLB)
      double precision BNDS_U(NUB)
      double precision S_L(NLB)
      double precision S_U(NUB)
      double precision V_L(NLB)
      double precision V_U(NUB)
      double precision SIGMA_L(NLB)
      double precision SIGMA_U(NUB)
      double precision LAM(M)
      double precision MU
      logical LSLACKS
      double precision ERR
      character*1 C_ERR
      double precision ERR_DINF
      double precision ERR_CNSTR
      double precision ERR_CMPL
      double precision ERR_DINF_US
      integer ITER
      integer IVAR(N)
      integer NFIX
      integer IFIX(NFIX)
      integer NORIG
      double precision XORIG(NORIG)
      double precision DX(N)
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
      double precision DASUM, DNRM2
      integer IDAMAX
      integer i, j, p_iwend, p_rwend, p_ilb1, p_iub1, p_tmp, idummy
      integer p_nvl, p_lams

      double precision val
      character*80 line

      integer scaltype          ! = 0: no scaling
                                ! = 1: first NITRO scaling
                                ! = 2: current NITRO scaling

      double precision NRM1, NRM2, NRM3, NRM4, SCAL1, SCAL2
      save             NRM1, NRM2, NRM3, NRM4, SCAL1, SCAL2
      integer LASTITER
      save    LASTITER
C
C*******************************************************************************
C
C                           Executable Statements
C
C*******************************************************************************
C
      if( ITER.eq.-1 ) then
         LASTITER = -1
         goto 9999
      endif

      IERR = 0
      p_rwend = 0
      p_iwend = 0
C
C     Compute SCAL1, SCAL2, NRM1, NRM3, NRM4 only, if ITER changed
C
      if( ITER.ne.LASTITER ) then
C
C     Note: After an unsuccessful watchdog iteration G doesn't have the correct
C      value, and therefore the scaling values will be different.  We simply
C      ignore this fact here, and even don't recompute the scaling factors
C

C
C     Decide about scaling factors in error criterion:
C     NITRO like or not? (It's not really like NITRO...)
C
         if( QERROR.gt.0 ) then
            scaltype = 1
         elseif( QERROR.lt.0 ) then
            scaltype = 0
         else
            scaltype = 2
         endif
         if( abs(QERROR).gt.2 ) then
            call C_OUT(2,0,1,'Error: Invalid value of QERROR!')
            IERR = 4
            goto 9999
         endif
         if( (abs(QERROR).eq.2.or.QERROR.eq.0) .and. QLAMBDA.eq.0 ) then
            call C_OUT(2,0,1,'Error: Need multipliers!')
            IERR = 4
            goto 9999
         endif
C
C     For LSLACKS and QLAMBDA = 2, compute full space multipliers for slacks
C     (note that I chose V_L instead of MU/S_L for varying MU)
C
         if( LSLACKS .and. (QLAMBDA.eq.2) ) then
            p_lams  = p_rwend
            p_rwend = p_lams + N
            if( p_rwend.gt.LRW ) then
               IERR = 98
               goto 9999
            endif
            call DCOPY(N, 0d0, 0, RW(p_lams+1), 1)
            if( ITER.ne.0 ) then
               do i = 1, NLB
                  j = ILB(i)
                  RW(p_lams+j) = SIGMA_L(i)*
     1                 (DX(j) + X(j) - S_L(i) - BNDS_L(i)) - V_L(i)
               enddo
               do i = 1, NUB
                  j = IUB(i)
                  RW(p_lams+j) = RW(p_lams+j) + SIGMA_U(i)*
     1                 (DX(j) + X(j) + S_U(i) - BNDS_U(i)) + V_U(i)
               enddo
            endif
         endif
C
C     Determine scaling factors
C
         if( scaltype.eq.1 ) then
C
C     scal1 = 1 + ||x||_1/n
C
CTODO This is not quite the same for LSLACKS
            SCAL1 = 1d0 + DASUM(N, X, 1)/DBLE(N)
C
C     scal2 = 1 + ||(lam,v)||_1/(m+nbounds)
C
            if( QLAMBDA.eq.0 ) then
C
C     In case there are no multipliers, take the gradient of the barrier
C     function with respect to the dependent variables as scaling values...
CTODO (Don't know if this makes perfect sense...?)
C
               if( LSLACKS ) then
                  call C_OUT(2,0,1,
     1                 'error: This choice of error computation '//
     2                 'not supported for QMOVEINIT=0')
                  IERR = 4
                  goto 9999
               endif
               p_ilb1  = p_iwend      ! inverse of ILB
               p_iub1  = p_ilb1 + N   ! inverse of IUB
               p_iwend = p_iub1 + N
               if( p_iwend.gt.LIW ) then
                  IERR = 99
                  goto 9999
               endif
               do i = 1, N
                  IW(p_ilb1+i) = 0
                  IW(p_iub1+i) = 0
               enddo
               do i = 1, NLB
                  IW(p_ilb1+ILB(i)) = i
               enddo
               do i = 1, NUB
                  IW(p_iub1+IUB(i)) = i
               enddo
C
               SCAL2 = 0d0
CTODO Check if this really makes sense, and what to if iLSLACKS
               do i = 1, M
                  val = G(i)
                  if( IW(p_ilb1+i).ne.0 ) then
                     val = val - V_L(IW(p_ilb1+i))
                  endif
                  if( IW(p_iub1+i).ne.0 ) then
                     val = val + V_U(IW(p_iub1+i))
                  endif
                  SCAL2 = SCAL2 + DABS(val)
               enddo
               p_iwend = p_ilb1
            else
               SCAL2 = DASUM(M, LAM, 1)
               if( LSLACKS ) then
                  if( QLAMBDA.eq.1 ) then
                     p_lams  = p_rwend
                     p_nvl   = p_lams + N
                     p_rwend = p_nvl + NLB
                     if( p_rwend.gt.LRW ) then
                        IERR = 98
                        goto 9999
                     endif
                     call DCOPY(NUB, V_L, 1, RW(p_nvl+1), 1)
                     call DSCAL(NUB, -1.d0, RW(p_nvl+1), 1)
                     call DASV2F(N, NLB, ILB, NUB, IUB,
     1                    RW(p_nvl+1), V_U, RW(p_lams+1))
                     SCAL2 = SCAL2 + DASUM(N, RW(p_lams+1), 1)
                     p_rwend = p_lams
                  elseif( QLAMBDA.eq.2 ) then
                     SCAL2 = SCAL2 + DASUM(N, RW(p_lams+1), 1)
                  endif
               endif
            endif
            SCAL2 = SCAL2 + DASUM(NLB,V_L,1)
            SCAL2 = SCAL2 + DASUM(NUB,V_U,1)
            if( .not.LSLACKS ) then
               SCAL2 = 1d0 + SCAL2/DBLE(1+M+NLB+NUB)
            else
CWEG
C               SCAL2 = 1d0 + SCAL2/DBLE(1+M+N+NLB+NUB)
               SCAL2 = 1d0 + SCAL2/DBLE(1+M+NLB+NUB)
            endif
         elseif( scaltype.eq.2 ) then
            if( QLAMBDA.ne.2 ) then
               call C_OUT(2,0,1,
     1              'error: Need ILAMBDA = 2 for IERROR = 0.')
               IERR = 4
               goto 9999
            endif
C
C     scal1 = 1 + ||x||_1/n
C
CTODO This is not quite the same for LSLACKS
            SCAL1 = 1d0 + DASUM(N, X, 1)/DBLE(N)
C
C     scal2 = max_{i}{1, ||\nabla f||, ||A_i||_2*|lambda_i|}
C
            SCAL2 = dmax1(1d0, DNRM2(N, G, 1))
C
C     The following is not very efficient
C
            p_lams  = p_rwend
            p_tmp   = p_lams + M
            p_rwend = p_tmp  + N
            if( p_rwend.gt.LRW ) then
               IERR = 98
               goto 9999
            endif
            call DCOPY(M, 0.d0, 0, RW(p_lams+1), 1)
            do i = 1, M
               RW(p_lams+i) = 1.d0
               call CONSTR(8, ITER, N, NIND, M, IVAR, NFIX, IFIX,
     1              NORIG, XORIG, CSCALE, RW(p_lams+1), RW(p_tmp+1),
     2              idummy, idummy,
     3              KCONSTR(1), RS(KCONSTR(2)+1), KCONSTR(3),
     4              IS(KCONSTR(4)+1), LRW-p_rwend, RW(p_rwend+1),
     5              LIW-p_iwend, IW(p_iwend+1), IERR)
               if( IERR.lt.0 ) then
                  write(line,*) 
     1                 'Error: Warning in CONSTR-8a, IERR = ', IERR
                  call C_OUT(2,0,1,line)
               elseif( IERR.ne.0 ) then
                  write(line,*)
     1                 'Error: Error in CONSTR-8a, IERR = ',IERR
                  call C_OUT(2,0,1,line)
                  goto 9999
               endif
               SCAL2 = dmax1(SCAL2,
     1              DNRM2(N, RW(p_tmp+1), 1)*dabs(LAM(i)))
               RW(p_lams+i) = 0.d0
            enddo
C
            if( NLB.gt.0 ) then
               i = IDAMAX(NLB, V_L, 1)
               SCAL2 = dmax1(SCAL2, V_L(i))
            endif
            if( NUB.gt.0 ) then
               i = IDAMAX(NUB, V_U, 1)
               SCAL2 = dmax1(SCAL2, V_U(i))
            endif
         else
            SCAL1 = 1d0
            SCAL2 = 1d0
         endif
         if( NIND.gt.0 ) then
            if( abs(QERROR).eq.1 ) then
C     nrm1 = ||Red_grad_barrier||_inf
               p_tmp   = p_rwend
               p_rwend = p_tmp + NIND
               if( p_rwend.gt.LRW ) then
                  IERR = 98
                  goto 9999
               endif
               call DCOPY(NIND, RG, 1, RW(p_tmp+1), 1)
               call DAXPY(NIND, 1.d0, RV, 1, RW(p_tmp+1), 1)
               i = IDAMAX(NIND, RW(p_tmp+1), 1)
               NRM1 = dabs(RW(p_tmp+i))
               p_rwend = p_tmp
            else
               p_tmp   = p_rwend
               p_rwend = p_tmp + N
               if( p_rwend.gt.LRW ) then
                  IERR = 98
                  goto 9999
               endif
C
C     Compute A * LAM
C
               if( M.gt.0 ) then
                  call CONSTR(8, ITER, N, NIND, M, IVAR, NFIX, IFIX,
     1                 NORIG, XORIG, CSCALE, LAM, RW(p_tmp+1),
     2                 idummy, idummy,
     3                 KCONSTR(1), RS(KCONSTR(2)+1), KCONSTR(3),
     4                 IS(KCONSTR(4)+1), LRW-p_rwend, RW(p_rwend+1),
     5                 LIW-p_iwend, IW(p_iwend+1), IERR)
                  if( IERR.lt.0 ) then
                     write(line,*) 
     1                    'Error: Warning in CONSTR-8, IERR = ', IERR
                     call C_OUT(2,0,1,line)
                  elseif( IERR.ne.0 ) then
                     write(line,*)
     1                    'Error: Error in CONSTR-8, IERR = ',IERR
                     call C_OUT(2,0,1,line)
                     goto 9999
                  endif
                  call DAXPY(N, 1d0, G, 1, RW(p_tmp+1), 1)
               else
                  call DCOPY(N, G, 1, RW(p_tmp+1), 1)
               endif
               if( LSLACKS ) then
                  call DAXPY( N, 1d0, RW(p_lams+1), 1, RW(p_tmp+1), 1)
               endif
C
C     Add gradient of barrier function
C     NOTE: Here, we takes the dual variables and not the real gradient
C     or the log barrier terms, since for latter changes for changing MU!
C
               if( .not.LSLACKS ) then
                  do i = 1, NLB
                     j = ILB(i)
                     RW(p_tmp+j) = RW(p_tmp+j) - V_L(i)
                  enddo
                  do i = 1, NUB
                     j = IUB(i)
                     RW(p_tmp+j) = RW(p_tmp+j) + V_U(i)
                  enddo
               else
                  do i = 1, NLB
                     j = ILB(i)
                     RW(p_lams+j) = RW(p_lams+j) + V_L(i)
                  enddo
                  do i = 1, NUB
                     j = IUB(i)
                     RW(p_lams+j) = RW(p_lams+j) - V_U(i)
                  enddo
               endif
C
               i = IDAMAX(N, RW(p_tmp+1), 1)
               NRM1 = dabs(RW(p_tmp+i))
               if( LSLACKS ) then
                  i = IDAMAX(N, RW(p_lams+1), 1)
                  NRM1 = dmax1(NRM1, dabs(RW(p_lams+i)))
               endif
               p_rwend = p_tmp
               if( LSLACKS ) then
                  p_rwend = p_lams
               endif
            endif
         else
            NRM1 = 0d0
         endif
C
C     nrm3 = ||c||_inf
C
#ifndef INCLUDE_CC
         if( M.gt.0 ) then
            i = IDAMAX(M, C, 1)
            NRM3 = dabs(C(i))
         else
            NRM3 = 0d0
         endif
#endif
C
C     nrm4 = ||X-S||_inf
C
         NRM4 = 0.d0
         if( NIND.gt.0 ) then
            if( LSLACKS ) then
               do i = 1, NLB
                  NRM4 = dmax1(NRM4, dabs(X(ILB(i))-S_L(i)-BNDS_L(i)))
               enddo
               do i = 1, NUB
                  NRM4 = dmax1(NRM4, dabs(X(IUB(i))+S_U(i)-BNDS_U(i)))
               enddo
            endif
         endif
C
C     Scale values
C
CTODO Should take different scaling for LSLACKS in NRM4 and NRM3 ?
         NRM1 = NRM1/SCAL2
#ifndef INCLUDE_CC
         NRM3 = NRM3/SCAL1
#endif
         NRM4 = NRM4/SCAL2
C
#ifndef INCLUDE_CC
         LASTITER = ITER
#endif

      endif
C
C     nrm2 = ||SV-mu*e||_inf
      NRM2 = 0.d0
      if( NIND.gt.0 ) then
         do i = 1, NLB
            NRM2 = dmax1(NRM2,dabs(V_L(i)*S_L(i)-MU))
         enddo
         do i = 1, NUB
            NRM2 = dmax1(NRM2,dabs(V_U(i)*S_U(i)-MU))
         enddo
      endif
      NRM2 = NRM2/SCAL2
C
C     nrm3 = ||c||_inf
C
C     Modification for calculating error in complementarity constraints 
C     when MU = 0 (i.e. error for original NLP)
C     EVAL_CCERR is a subroutine supplied by AMPL interface to calculate the
C     error in complementarity satisfaction 
C
#ifdef INCLUDE_CC
      if( M.gt.0 ) then
         if (MU.eq.0.d0 .or. LASTITER.eq.ITER) then
            call EVAL_CCERR(N,M,X,C,MU)
         endif
         i = IDAMAX(M,C,1)
         NRM3 = dabs(C(i))
      else
         NRM3 = 0d0
      endif

      NRM3 = NRM3/SCAL1      

      LASTITER = ITER
#endif

      ERR = dmax1(NRM1,NRM2,NRM3,NRM4)
C
C     This is if ERR_* should be the unscaled values
C
C      ERR_DINF = NRM1*SCAL2
C      ERR_CNSTR = dmax1(NRM3,NRM4)*SCAL1
C      ERR_CMPL  = NRM2*SCAL2
C
      ERR_DINF = NRM1
      ERR_CNSTR = dmax1(NRM3,NRM4)
      ERR_CMPL  = NRM2

      ERR_DINF_US = NRM1*SCAL2
C
      if( ERR.eq.NRM1 ) then
         C_ERR = 'd'
      elseif( ERR.eq.NRM2 ) then
         C_ERR = 'm'
      elseif( ERR.eq.NRM3 ) then
         C_ERR = 'c'
      else
         C_ERR = 's'
      endif

 9999 continue
      return
      end
