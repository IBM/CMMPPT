C Copyright (C) 2002, Carnegie Mellon University and others.
C All Rights Reserved.
C This code is published under the Common Public License.

C $Id: eval_h_dummy.f,v 1.1 2002/05/02 18:52:16 andreasw Exp $

      subroutine EVAL_H(TASK, N, X, M, LAM, NNZH, HESS, IRNH, ICNH)

      implicit none
      integer TASK, N, M, NNZH
      double precision LAM(M), X(N), HESS(NNZH)
      integer IRNH(NNZH), ICNH(NNZH)

      write(*,*) 'Wrong choice of IPOPT parameters: Got into EVAL_H...'
      stop

      return
      end
