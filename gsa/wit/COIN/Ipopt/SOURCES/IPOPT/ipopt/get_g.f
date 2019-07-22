C Copyright (C) 2002, Carnegie Mellon University and others.
C All Rights Reserved.
C This code is published under the Common Public License.
C*******************************************************************************
C
      subroutine GET_G(N, X, IVAR, NORIG, XORIG, G, LRW, RW, IERR)
C
C*******************************************************************************
C
C    $Id: get_g.f,v 1.1 2002/05/02 18:52:17 andreasw Exp $
C
C-------------------------------------------------------------------------------
C                                 Title
C-------------------------------------------------------------------------------
C
CT    Evaluate gradient of constraint functions at X (after copying
CT    into XORIG) and eliminate fixed variables
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
CP   N         I    INT    number of free variables
CP   X         I    DP     values of free variables, where F should be
CP                            evaluated
CP   IVAR      I    INT    information about partitioning
CP                            i = 1..M      XORIG(IVAR(i)) dependent
CP                            i = (M+1)..N  XORIG(IVAR(i)) independent
CP                            Note: fixed variables do not occur in IVAR
CP                              X(i) corresponds to XORIG(IVAR(i))
CP   NORIG     I    INT    number of all variables including fixed vars
CP   XORIG    I/O   DP     I: values of fixed variables
CP                         O: values of all variables (from X)
CP   G         O    DP     gradient of objective function
CP                            (w/o fixed vars, partitioned into ind. and
CP                             dep. varoables)
CP   LRW      I/O   INT    length of RW
CP   RW       I/O   DP     can be used as DP work space but content will be
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
CS    DSCAL
CS    EVAL_G
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
      double precision X(N)
      integer IVAR(N)
      integer NORIG
      double precision XORIG(NORIG)
      double precision G(N)
      integer LRW
      double precision RW(LRW)
      integer IERR
C
C-------------------------------------------------------------------------------
C                            Local varibales
C-------------------------------------------------------------------------------
C
      integer i, p_rwend, p_gorigtmp
C
C*******************************************************************************
C
C                           Executable Statements
C
C*******************************************************************************
C
      p_rwend = 0
      IERR = 0
C
C     Copy new values to XORIG
C
      do i = 1, N
         XORIG(IVAR(i)) = X(i)
      enddo
C
C     Evaluate gradient of objective function (incl. fixed vars)
C
      p_gorigtmp = p_rwend
      p_rwend    = p_gorigtmp + NORIG
      if( p_rwend.gt.LRW ) then
         IERR = 98
         goto 9999
      endif
      call EVAL_G(NORIG, XORIG, RW(p_gorigtmp+1))
C
C     reorder accoring to partition
C
      do i = 1, N
         G(i) = RW(p_gorigtmp+IVAR(i))
      enddo
      p_rwend = p_gorigtmp
C
C     Scale accoring to QFSCALE and scaling factors
C
      if( QFSCALE.ne.1d0 ) then
         call DSCAL(N, QFSCALE, G, 1)
      endif
C
C     I think that's it...
C
 9999 continue
      return
      end
      
