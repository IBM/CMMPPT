C Copyright (C) 2002, Carnegie Mellon University and others.
C All Rights Reserved.
C This code is published under the Common Public License.
C*******************************************************************************
C
      subroutine GET_C(ITER, N, NIND, X, IVAR, NORIG, XORIG, M, CSCALE,
     1                 C, KCONSTR, LRS, RS, LIS, IS, LRW, RW, LIW, IW,
     1                 IERR)
C
C*******************************************************************************
C
C    $Id: get_c.f,v 1.1 2002/05/02 18:52:17 andreasw Exp $
C
C-------------------------------------------------------------------------------
C                                 Title
C-------------------------------------------------------------------------------
C
CT    Evaluate constraint functions at X (after copying into XORIG)
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
CP   ITER      I    INT    iteration counter
CP   N         I    INT    number of free variables
CP   NIND      I    INT    number of independent variables
CP   X         I    DP     values of free variables, at which F should be
CP                            evaluated
CP   IVAR      I    INT    information about partitioning
CP                            i = 1..M      XORIG(IVAR(i)) dependent
CP                            i = (M+1)..N  XORIG(IVAR(i)) independent
CP                            Note: fixed variables do not occur in IVAR
CP                              X(i) corresponds to XORIG(IVAR(i))
CP   NORIG     I    INT    number of all variables including fixed vars
CP   XORIG    I/O   DP     I: values of fixed variables
CP                         O: values of all variables (from X)
CP   M         I    INT    number of constraints
CP   CSCALE    I    DP     scaling factors for constraints
CP   C         O    DP     values of constraint functions
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
CS    C_OUT
CS    CONSTR
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
      double precision X(N)
      integer IVAR(N)
      integer NORIG
      double precision XORIG(NORIG)
      integer M
      double precision CSCALE(*)
      double precision C(M)
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
      integer i, idummy, p_rwend, p_iwend
      double precision dummy
      character*80 line
C This common block allows tracking of number of function evaluations.
C It can be accessed from the driver program, which has to initalize it to zero
C before call of IPOPT.
C cevals is increased by one with every call of GET_C
C
      integer fevals, cevals
      common /EVALS/ fevals, cevals
C !DEC$ ATTRIBUTES DLLEXPORT :: /EVALS/
C
C*******************************************************************************
C
C                           Executable Statements
C
C*******************************************************************************
C
      IERR = 0
C
C     Copy new values to XORIG
C
      do i = 1, N
         XORIG(IVAR(i)) = X(i)
      enddo
C
C     There is nothing to do if there are no constraints
C
      if( M.eq.0 ) goto 9999

      p_iwend = 0
      p_rwend = 0
C
C     Call CONSTR to evaluate constraints C
C
      call CONSTR(2, ITER, N, NIND, M, IVAR, idummy, idummy,
     1            NORIG, XORIG, CSCALE, dummy, C,
     2            idummy, idummy,
     3            KCONSTR(1), RS(KCONSTR(2)+1), KCONSTR(3),
     4            IS(KCONSTR(4)+1), LRW-p_rwend, RW(p_rwend+1),
     5            LIW-p_iwend, IW(p_iwend+1), IERR)
      if( IERR.lt.0 ) then
         write(line,*) 'get_c: Warning in CONSTR, IERR = ',IERR
         call C_OUT(2,0,1,line)
         IERR = 0      
      elseif( IERR.ne.0 ) then
         write(line,*) 'get_c: Error in CONSTR, IERR = ',IERR
         call C_OUT(2,0,1,line)
         goto 9999
      endif
C
C     I think that's it...
C
      cevals = cevals + 1
 9999 continue
      return
      end
      
