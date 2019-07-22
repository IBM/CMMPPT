C Copyright (C) 2002, Carnegie Mellon University and others.
C All Rights Reserved.
C This code is published under the Common Public License.
      subroutine EVAL_HESSOBJ_V(TASK, N, X, M, VIN, VOUT)
C
C    $Id: eval_hessobj_v.F,v 1.1 2002/06/07 20:55:06 andreasw Exp $
C
C     TASK = 0: reevaluate Hessian (because X or LAM changed)
C            1: do not need to reevaluate - but X and LAM are still
C               set to the correct values
      implicit none
      integer TASK, N, M
      double precision  VIN(N), X(N), VOUT(N)

#include "CUTE.INC"
      double precision lam(CUTE_MMAX)

      call DCOPY(CUTE_M, 0d0, 0, lam, 1)

      call EVAL_HESSLAG_V(0, N, X, M, lam, VIN, VOUT)

      return
      end
