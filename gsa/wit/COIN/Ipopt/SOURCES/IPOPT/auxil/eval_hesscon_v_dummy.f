C Copyright (C) 2002, Carnegie Mellon University and others.
C All Rights Reserved.
C This code is published under the Common Public License.

C $Id: eval_hesscon_v_dummy.f,v 1.1 2002/05/02 18:52:16 andreasw Exp $

      subroutine EVAL_HESSCON_V(TASK, N, X, M, LAM, VIN, VOUT)
      implicit none
      integer TASK, N, M
      double precision LAM(M), VIN(N), X(N), VOUT(N)

      write(*,*)
     1  'Wrong choice of IPOPT parameters: Got into EVAL_HESSCON_V...'
      stop

      return
      end
