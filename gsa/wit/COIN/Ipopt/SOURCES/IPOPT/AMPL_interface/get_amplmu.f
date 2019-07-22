C Copyright (C) 2002, Carnegie Mellon University and others.
C All Rights Reserved.
C This code is published under the Common Public License.

C $Id: get_amplmu.f,v 1.1 2002/05/02 18:52:15 andreasw Exp $

      subroutine GET_AMPLMU( MU )
C
C     Obtain the common block variable AMPLMU from /AMPLMU/
C
      implicit none
      include 'IPOPT.INC'
      double precision MU
C
      MU = AMPLMU
      return
      end
