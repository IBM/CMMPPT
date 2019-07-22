C Copyright (C) 2002, Carnegie Mellon University and others.
C All Rights Reserved.
C This code is published under the Common Public License.

      subroutine MULTMAT(TRANS, PLUS, NNZMAT, IRMAT, JCMAT, MAT,
     1     VIN, VOUT)
C
C     $Id: multmat.f,v 1.1 2002/05/02 18:52:15 andreasw Exp $
C
C     Multiply vector with sparse matrix
C
C     Author:  Andreas Waechter
C              c/o Group of Larry Biegler
C              Department of Chemical Engineering
C              Carnegie Mellon University
C              Pittsburgh, PA
C
      implicit none

      character*(*) TRANS       !if = 't' or 'T', compute vout += FZ^T*vin
                                !otherwise vout += FZ*vin
                                !NOTE: VOUT is NOT INITIALIZED here!!!
      logical PLUS              !if .true. then add elements in matrix,
                                !otherwise substract
      integer NNZMAT
      integer IRMAT(NNZMAT)
      integer JCMAT(NNZMAT)
      double precision MAT(NNZMAT)
      double precision VIN(*)
      double precision VOUT(*)

      integer i, ir, jc

      if( TRANS(1:1).eq.'t' .or. TRANS(1:1).eq.'T' ) then
         if( PLUS ) then
            do i = 1, NNZMAT
               ir = JCMAT(i)
               jc = IRMAT(i)
               VOUT(ir) = VOUT(ir) + MAT(i)*VIN(jc)
            enddo
         else
            do i = 1, NNZMAT
               ir = JCMAT(i)
               jc = IRMAT(i)
               VOUT(ir) = VOUT(ir) - MAT(i)*VIN(jc)
            enddo
         endif
      else
         if( PLUS ) then
            do i = 1, NNZMAT
               ir = IRMAT(i)
               jc = JCMAT(i)
               VOUT(ir) = VOUT(ir) + MAT(i)*VIN(jc)
            enddo
         else
            do i = 1, NNZMAT
               ir = IRMAT(i)
               jc = JCMAT(i)
               VOUT(ir) = VOUT(ir) - MAT(i)*VIN(jc)
            enddo
         endif
      endif

      return
      end
