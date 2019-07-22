C Copyright (C) 2002, Carnegie Mellon University and others.
C All Rights Reserved.
C This code is published under the Common Public License.

C $Id: ipopt_getdata.f,v 1.2 2002/07/24 03:00:53 andreasw Exp $

      subroutine IPOPT_GETDATA(FFUNCS, CFUNCS, NOCG, NORES, NONEGCURV)
      implicit none
      integer FFUNCS, CFUNCS, NOCG, NORES, NONEGCURV
      include 'TIMER.INC'
      include 'IPOPT.INC'
      integer fevals, cevals, nboundsfree
      common /EVALS/ fevals, cevals
      FFUNCS = fevals
      CFUNCS = cevals
      NOCG = COUNT_CG
      NORES = COUNT_RESTO_ITER
      NONEGCURV = COUNT_NEG_CURV
C
C     Also close output file if open
C
      if( QCNR.gt.0 ) close(QCNR)
      return
      end

      subroutine CHECK_FLAGFILE( EX )
      integer EX
      logical exist
      inquire(file='FLAG', exist=exist)
      if( exist ) then
         EX = 1
      else
         EX = 0
      endif
      return
      end
