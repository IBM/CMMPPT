C Copyright (C) 2002, Carnegie Mellon University and others.
C All Rights Reserved.
C This code is published under the Common Public License.

C $Id: find_param.f,v 1.1 2002/05/02 18:52:17 andreasw Exp $
      integer function FIND_PARAM(NPARAMS, CPARAMS, NAME)
C
C     Find the entry NAME in the list of names CPARAMS (case insensitive)
C     Returns 0, if NAME was not found, and index in CPARAMS, if NAME was found.
C
C     Author:   Andreas Waechter    09-10-01
C
      implicit none
      integer NPARAMS           ! length of list CPARAMS
      character*(*) CPARAMS(NPARAMS) ! list in which NAME is to be found
      character*(*) NAME        ! entry to be found in CPARAMS

      logical STRLCMP

      do FIND_PARAM = 1, NPARAMS
         if( STRLCMP(CPARAMS(FIND_PARAM),NAME)) return ! found
      enddo
      FIND_PARAM = 0
      return
      end
C
C
C
      logical function STRLCMP(STR1, STR2)
C
C     Compare two character strings (STR1 and STR2) case-insensitive.
C     Returns .true., if STR1 and STR2 are identical; otherwise .false.
C
C     Author:  Andreas Waechter   09-10-01
C
      implicit none
      character*(*) STR1, STR2
C
      logical LSAME             ! is usually supplied by BLAS
      integer SLEN, i
C
      STRLCMP = .false.
      if( SLEN(STR1).ne.SLEN(STR2) ) return
      do i = 1, SLEN(STR1)
         if( .not.LSAME(STR1(i:i),STR2(i:i)) ) return
      enddo
      STRLCMP = .true.
      return
      end
