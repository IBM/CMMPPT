C Copyright (C) 2002, Carnegie Mellon University and others.
C All Rights Reserved.
C This code is published under the Common Public License.

C $Id: drawframe_w.f,v 1.1 2002/05/02 18:52:12 andreasw Exp $
      subroutine DrawFrame_W(ipar)
      
      USE DFLIB
      include 'DYNAUX.INC'
      include 'DYNGRA.INC'
!DEC$ ATTRIBUTES DLLIMPORT :: /DYNAUX/, /GRAPH/

      integer ipar(*)

      CALL clearscreen($GCLEARSCREEN)
!     call graphicsmode()  
!     call graphis(ipar)
      call gridshape()
      return
      end   subroutine DrawFrame_W
