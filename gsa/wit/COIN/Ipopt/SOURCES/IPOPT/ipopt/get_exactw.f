C Copyright (C) 2002, Carnegie Mellon University and others.
C All Rights Reserved.
C This code is published under the Common Public License.
C*******************************************************************************
C
      subroutine GET_EXACTW(N, NIND, M, ITER, IVAR, NFIX, IFIX,
     1     NORIG, XORIG, X, CSCALE, LAM,
     1     NLB, ILB, NUB, IUB, S_L, S_U, SIGMA_L, SIGMA_U,
     1     W, RESTO, KCONSTR, LRS, RS, LIS, IS,
     2     LRW, RW, LIW, IW, IERR)
C
C*******************************************************************************
C
C    $Id: get_exactw.f,v 1.1 2002/05/02 18:52:17 andreasw Exp $
C
C-------------------------------------------------------------------------------
C                                 Title
C-------------------------------------------------------------------------------
C
CT    Compute exact reduced primal-dual Hessian using second derivatives
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
CP   XORIG     I    DP     (only TASK = 1,2,3): actual iterate
CP                            XORIG is ordered in ORIGINAL order (i.e. not
CP                            partitioned into independent and dependent
CP                            variables)
CP   CSCALE    I    DP     scaling factors for constraints
CP   LAM       I    DP     Lagrangian multipliers for constraints
CP   NLB       I    INT    number of lower bounds (excluding fixed vars)
CP   ILB       I    INT    indices of lower bounds
CP                            (e.g. S_L(i) is slack for X(ILB(i)) )
CP   NUB       I    INT    number of upper bounds (excluding fixed vars)
CP   IUB       I    INT    indices of upper bounds
CP                            (e.g. S_U(i) is slack for X(IUB(i)) )
CP   S_L       I    DP     slack variables for lower bounds
CP   S_U       I    DP     slack variables for upper bounds
CP   SIGMA_L   I    DP     primal-dual Hessian of lower bound barrier term
CP                            (NLB diagonal elements only)
CP   SIGMA_U   I    DP     primal-dual Hessian of upper bound barrier term
CP                            (NUB diagonal elements only)
CP   BB        O    DP     reduced primal-dual Hessian
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
CS    DCOPY
CS    C_OUT
CS    CONSTR
CS    DASV2F
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
      double precision X(N)
      double precision CSCALE(*)
      double precision LAM(M)
      integer NLB
      integer ILB(NLB)
      integer NUB
      integer IUB(NUB)
      double precision S_L(NLB)
      double precision S_U(NUB)
      double precision SIGMA_L(NLB)
      double precision SIGMA_U(NUB)
      double precision W((NIND*(NIND+1))/2)
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
      integer p_iwend, p_rwend, p_ei
      integer i, lw
      double precision dummy, times, timef
      character*80 line
C
C*******************************************************************************
C
C                           Executable Statements
C
C*******************************************************************************
C
      call TIMER(times)

      p_iwend = 0
      p_rwend = 0
      IERR = 0

      if( RESTO ) goto 5000

      if( QLAMBDA.eq.0 ) then
         write(line,*) 'get_exactw: Need multipliers for exact Hessian.'
         call C_OUT(2,0,1,line)
         IERR = 4
         goto 9999
      endif
C
C     Compute overall reduced Hessian by obtaining products with unit vectors
C
      lw = 1
      p_ei    = p_rwend
      p_rwend = p_ei    + NIND
      if( p_rwend.gt.LRW ) then
         IERR = 98
         goto 9999
      endif
      call DCOPY(NIND, 0d0, 0, RW(p_ei+1), 1)
      do i = 1, NIND
         RW(p_ei+i) = 1d0
         if( i.gt.1 ) RW(p_ei+i-1) = 0d0
         call GET_ZWZV(1, N, NIND, M, ITER, IVAR, NFIX, IFIX, NORIG,
     1        XORIG, X, CSCALE, NLB, ILB, NUB, IUB, S_L, S_U,
     2        SIGMA_L, SIGMA_U, LAM, RW(p_ei+1), W(lw), dummy,
     1        KCONSTR, LRS, RS, LIS, IS, LRW-p_rwend, RW(p_rwend+1),
     5        LIW-p_iwend, IW(p_iwend+1), IERR)
         if( IERR.lt.0 ) then
            write(line,*)
     1           'get_exactw: Warning in get_zwzv, IERR = ',IERR
            call C_OUT(2,0,1,line)
         elseif( IERR.ne.0 ) then
            write(line,*)
     1           'get_exactw: Error in get_zwzv, IERR = ',IERR
            call C_OUT(2,0,1,line)
            goto 9999
         endif
         lw = lw + i
      enddo
      goto 9999
C
C     Restoration phase
C
 5000 continue
      call C_OUT(2,0,1,'RESTO not yet implemented in get_exactw')
      IERR = 4
      goto 9999

C
C     That's it
C
 9999 continue
      call TIMER(timef)
      TIME_EXACTW = TIME_EXACTW + timef - times
      return
      end
