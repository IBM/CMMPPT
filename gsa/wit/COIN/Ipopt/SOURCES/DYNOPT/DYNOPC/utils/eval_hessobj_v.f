C Copyright (C) 2002, Carnegie Mellon University and others.
C All Rights Reserved.
C This code is published under the Common Public License.

C $Id: eval_hessobj_v.f,v 1.1 2002/05/02 18:52:13 andreasw Exp $
      subroutine EVAL_HESSOBJ_V(TASK, N, X, M, VIN, VOUT)
C
C     Products with Hessian of objective function for IPOPT
C     Since a linear objective function is assumed, results is always 0
C
C     Authors:  Yidong Lang, Andreas Waechter     09-29-01
C
      implicit none
      integer TASK, N, M
      double precision  VIN(N), X(N), VOUT(N)

      call DCOPY(N, 0d0, 0, VOUT, 1)
      return
      end
