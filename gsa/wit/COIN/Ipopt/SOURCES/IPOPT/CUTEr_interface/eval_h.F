C Copyright (C) 2002, Carnegie Mellon University and others.
C All Rights Reserved.
C This code is published under the Common Public License.
C*******************************************************************************
C

      subroutine EVAL_H(TASK, N, X, M, LAM, NNZH, HESS, IRNH, ICNH)
C
C*******************************************************************************
C
C    $Id: eval_h.F,v 1.1 2002/06/07 20:55:05 andreasw Exp $
C
C-------------------------------------------------------------------------------
C                                 Title
C-------------------------------------------------------------------------------
C
CT    Compute Jacobian of constraints to CUTE problem
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
CA    Andreas Waechter      03/23/00
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
CP   TASK      I    INT     =0: Obtain NZ
CP                         <>0: Compute Jacobian
CP   N         I    INT    number of variables in problem statement
CP                            (including slacks for inequality constraints)
CP   X         I    DP     point where A is to be evaluated
CP   NZ       I/O   INT    TASK = 0: O: number of nonzero elements
CP                         otherwise: number of nonzero elements
CP                                     (size of A, AVAR, ACON)
CP   A         O    DP     (only TASK<>0) values in Jacobian
CP   ACON      O    INT    (only TASK<>0) row indices
CP   AVAR      O    INT    (only TASK<>0) column indices
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
CS    CDIMSH
CS    CSH
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
#include "CUTE.INC"
C
C-------------------------------------------------------------------------------
C                             Parameter list
C-------------------------------------------------------------------------------
C
      integer TASK
      integer N
      integer M 
      integer NNZH
      double precision LAM(M)
      double precision X(N)
      double precision HESS(NNZH)
      integer IRNH(NNZH)
      integer ICNH(NNZH)
C
C-------------------------------------------------------------------------------
C                            Local varibales
C-------------------------------------------------------------------------------
C
      double precision v(CUTE_MMAX)
      double precision h(CUTE_NZMAX)
      integer indirnh(CUTE_NZMAX), indicnh(CUTE_NZMAX)
      integer nnzh2

      integer NNZH_STORE
      save    NNZH_STORE
C
C*******************************************************************************
C
C                           Executable Statements
C
C*******************************************************************************
C
      if( TASK.eq.0 ) then
C
C     Get number of nonzeros in Hessian of the Lagrangian
C
         CALL CDIMSH( NNZH )
         if( NNZH.gt.CUTE_NZMAX ) then
            write(*,*) 'CUTE_NZMAX too small in cute_h'
            write(*,*) 'Increase to at least ', NNZH
            stop
         endif
         NNZH_STORE = NNZH
      else
C
C     Call CSH to obtain Hessian for constraints
C
         call CSH(CUTE_N, CUTE_M, X, M, LAM, nnzh2, NNZH, HESS,
     1        IRNH, ICNH)
      endif

 9999 continue
      return
      end
