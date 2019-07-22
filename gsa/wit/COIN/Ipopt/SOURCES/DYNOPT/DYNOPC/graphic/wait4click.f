C Copyright (C) 2002, Carnegie Mellon University and others.
C All Rights Reserved.
C This code is published under the Common Public License.

C $Id: wait4click.f,v 1.1 2002/05/02 18:52:12 andreasw Exp $
      SUBROUTINE wait4click(flag)
      use dflib
	character*(*) flag
      integer(4) ix,iy,i4k,event

	if( flag.eq.'LR') then
! click mouse left or right button to go ahead
         event = MOUSE$RBUTTONDOWN
         event = IOR(event,MOUSE$LBUTTONDOWN )
	else
         event = MOUSE$RBUTTONDOWN
	endif
      i4=waitonmouseevent(event,i4k,ix,iy)
      end
