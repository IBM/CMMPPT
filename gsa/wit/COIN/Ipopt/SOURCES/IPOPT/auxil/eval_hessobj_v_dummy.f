C Copyright (C) 2002, Carnegie Mellon University and others.
C All Rights Reserved.
C This code is published under the Common Public License.

C $Id: eval_hessobj_v_dummy.f,v 1.1 2002/05/02 18:52:16 andreasw Exp $
      subroutine EVAL_HESSOBJ_V(TASK, N, X, M, VIN, VOUT)
      implicit none
      integer TASK, N, M
      double precision VIN(N), X(N), VOUT(N)

      write(*,*)
     1  'Wrong choice of IPOPT parameters: Got into EVAL_HESSOBJ_V...'
      stop

      return
      end
