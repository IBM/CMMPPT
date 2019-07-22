C Copyright (C) 2002, Carnegie Mellon University and others.
C All Rights Reserved.
C This code is published under the Common Public License.
C*******************************************************************************
C
      subroutine GET_RGB(N, NIND, M, ITER, IVAR, NFIX, IFIX,
     1                   NORIG, XORIG, CSCALE, RG,
     1                   NLB, ILB, NUB, IUB, S_L, S_U,
     1                   MU, RGB, KCONSTR, LRS, RS, LIS, IS,
     2                   LRW, RW, LIW, IW, IERR)
C
C*******************************************************************************
C
C    $Id: get_rgb.f,v 1.1 2002/05/02 18:52:17 andreasw Exp $
C
C-------------------------------------------------------------------------------
C                                 Title
C-------------------------------------------------------------------------------
C
CT    Compute reduced gradient of barrier function
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
CP   RG        I    DP     reduced gradient of objective function
CP   NLB       I    INT    number of lower bounds (excluding fixed vars)
CP   ILB       I    INT    indices of lower bounds
CP                            (e.g. S_L(i) is slack for X(ILB(i)) )
CP   NUB       I    INT    number of upper bounds (excluding fixed vars)
CP   IUB       I    INT    indices of upper bounds
CP                            (e.g. S_U(i) is slack for X(IUB(i)) )
CP   S_L       I    DP     slack variables for lower bounds
CP   S_U       I    DP     slack variables for upper bounds
CP   MU        I    DP     barrier parameter
CP   RGB       O    DP     reduced gradient of barrier function
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
CS    DASV2F
CS    DAXPY
CS    DCOPY
CS    CONSTR
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
      double precision RG(NIND)
      integer NLB
      integer ILB(NLB)
      integer NUB
      integer IUB(NUB)
      double precision S_L(NLB)
      double precision S_U(NUB)
      double precision MU
      double precision RGB(NIND)
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
      integer p_iwend, p_rwend, p_msl, p_msu, p_ms
      integer idummy, i
      character*80 line
C
C*******************************************************************************
C
C                           Executable Statements
C
C*******************************************************************************
C
      IERR = 0
      if( NIND.eq.0 ) then
         goto 9999
      endif

      p_iwend = 0
      p_rwend = 0

      p_ms    = p_rwend
      p_msl   = p_ms    + N
      p_msu   = p_msl   + NLB
      p_rwend = p_msu   + NUB
      if( p_rwend.gt.LRW ) then
         IERR = 98
         goto 9999
      endif
C
C     write MU*S_L^{-1}e and MU*S_U^{-1}e into msl and msu
C
      do i = 1, NLB
         RW(p_msl+i) = MU/S_L(i)
      enddo
      do i = 1, NUB
         RW(p_msu+i) = -MU/S_U(i)
      enddo
C
C     Get full vector ms = msl + msu
C
      call DASV2F(N, NLB, ILB, NUB, IUB,
     1            RW(p_msl+1), RW(p_msu+1), RW(p_ms+1))
      p_rwend = p_msl

      if( M.ne.0 ) then
C
C     Call CONSTR to get RGB for dependent variables
C
         call CONSTR(4, ITER, N, NIND, M, IVAR, NFIX, IFIX,
     1               NORIG, XORIG, CSCALE, RW(p_ms+1), RGB,
     2               idummy, idummy,
     3               KCONSTR(1), RS(KCONSTR(2)+1), KCONSTR(3),
     4               IS(KCONSTR(4)+1), LRW-p_rwend, RW(p_rwend+1),
     5               LIW-p_iwend, IW(p_iwend+1), IERR)
         if( IERR.lt.0 ) then
            write(line,*) 'get_rgb: Warning in CONSTR, IERR = ',IERR
            call C_OUT(2,0,1,line)
         elseif( IERR.ne.0 ) then
            write(line,*) 'get_rgb: Error in CONSTR, IERR = ',IERR
            call C_OUT(2,0,1,line)
            goto 9999
         endif
      else
         call DCOPY(NIND, 0d0, 0, RGB, 1)
      endif
C
C     Add independent part
C
      call DAXPY(NIND, -1.d0, RW(p_ms+M+1), 1, RGB, 1)
C
C     Add gradient of objective function
C
      call DAXPY(NIND, 1.d0, RG, 1, RGB, 1)
C
C     Free work space
C
      p_rwend = p_ms
C
C     That's it
C
 9999 continue
      return
      end
