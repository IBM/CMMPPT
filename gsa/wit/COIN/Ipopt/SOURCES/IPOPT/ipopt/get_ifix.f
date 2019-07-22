C Copyright (C) 2002, Carnegie Mellon University and others.
C All Rights Reserved.
C This code is published under the Common Public License.
C*******************************************************************************
C
      subroutine GET_IFIX(NORIG, XORIG, NLBO, ILBO, BNDS_LO,
     1                    NUBO, IUBO, BNDS_UO, NFIX, IFIX,
     2                    ilbo1, iubo1, IERR)
C
C*******************************************************************************
C
C    $Id: get_ifix.f,v 1.1 2002/05/02 18:52:17 andreasw Exp $
C
C-------------------------------------------------------------------------------
C                                 Title
C-------------------------------------------------------------------------------
C
CT    Determine fixed variables
C
C-------------------------------------------------------------------------------
C                          Programm description
C-------------------------------------------------------------------------------
C
CB    
C
C-------------------------------------------------------------------------------
C                             Author, date
C-------------------------------------------------------------------------------
C
CA    Andreas Waechter      05/01/02  Release as version IPOPT 2.0
C
C-------------------------------------------------------------------------------
C                             Documentation
C-------------------------------------------------------------------------------
C
CD
C
C-------------------------------------------------------------------------------
C                             Parameter list    
C-------------------------------------------------------------------------------
C
C    Name     I/O   Type   Meaning
C
CP   NORIG     I    INT    total number of variables (incl. fixed vars)
CP   XORIG    I/O   INT    I: user provided initial point (incl fixed vars)
CP                         O: for entries with BNDS_LO(i) >= BNDS_UO(i)
CP                               set XORIG(i) = BNDS_LO(i)
CP   NLBO      I    INT    number of lower bounds (incl fixed vars)
CP   ILBO      I    INT    indices of lower bounds
CP                            (e.g. BNDS_LO(i) is bound for XORIG(ILBO(i)) )
CP   BNDS_LO   I    DP     values of lower bounds
CP   NUBO      I    INT    number of upper bounds (incl fixed vars)
CP   IUBO      I    INT    indices of upper bounds
CP                            (e.g. BNDS_UO(i) is bound for XORIG(IUBO(i)) )
CP   BNDS_LO   I    DP     values of lower bounds
CP   NFIX      O    INT    number of fixed variables
CP   IFIX      O    INT    indices (in XORIG) of fixed variables
CP                            (in increasing order)
CP   ilbo1     W    INT    inverse of ILBO
CP   iubo1     W    INT    inverse of IUBO
CP   IERR      O    INT    =0: everything OK
CP                         >0: Error occured; abort optimization
CP                         <0: Warning; message to user
C
C-------------------------------------------------------------------------------
C                             local variables
C-------------------------------------------------------------------------------
C
CL
C
C-------------------------------------------------------------------------------
C                             used subroutines
C-------------------------------------------------------------------------------
C
CS    C_OUT
C
C*******************************************************************************
C
C                              Declarations
C
C*******************************************************************************
C
      IMPLICIT NONE
C
C*******************************************************************************
C
C                              Include files
C
C*******************************************************************************
C
      include 'IPOPT.INC'
C
C-------------------------------------------------------------------------------
C                             Parameter list
C-------------------------------------------------------------------------------
C
      integer NORIG
      double precision XORIG(NORIG)
      integer NLBO
      integer ILBO(NLBO)
      double precision BNDS_LO(NLBO)
      integer NUBO
      integer IUBO(NUBO)
      double precision BNDS_UO(NUBO)
      integer NFIX
      integer IFIX(NORIG)
      integer ilbo1(NORIG)
      integer iubo1(NORIG)
      integer IERR
C
C-------------------------------------------------------------------------------
C                            Local varibales
C-------------------------------------------------------------------------------
C
      integer i, il, iu
      character*80 line(3)
C
C*******************************************************************************
C
C                           Executable Statements
C
C*******************************************************************************
C
      IERR = 0

      if( QCNR.gt.0 .and. QPRINT.gt.5 ) then
         write(line,699)
 699     format(/,'   Information about corrected fixed bounds ',
     1            '(if applicable):',/)
         call C_OUT(1,6,3,line)
      endif
C
C     Check whether all bounds are in proper range
C
      do i = 1, NLBO
         if( ILBO(i).le.0 .or. ILBO(i).gt.NORIG ) then
            write(line,690) i,ILBO(i)
 690        format('Error: ILB(',i8,') = ',i8,' is out of range.')
            call C_OUT(2,0,1,line)
            IERR = 15
            goto 9999
         endif
      enddo
      do i = 1, NUBO
         if( IUBO(i).le.0 .or. IUBO(i).gt.NORIG ) then
            write(line,691) i,IUBO(i)
 691        format('Error: IUB(',i8,') = ',i8,' is out of range.')
            call C_OUT(2,0,1,line)
            IERR = 15
            goto 9999
         endif
      enddo
C
C     compute inverse of ILBO, LUBO
C
      do i = 1, NORIG
         ilbo1(i) = 0
         iubo1(i) = 0
      enddo
      do i = 1, NLBO
         ilbo1(ILBO(i)) = i
      enddo
      do i = 1, NUBO
         iubo1(IUBO(i)) = i
      enddo
C
C     Find fixed variables with BNDS_LO(i) >= BNDS_UO(i)
C
      NFIX = 0
      do i = 1, NORIG
         il = ilbo1(i)
         iu = iubo1(i)
         if( il.ne.0 .and. iu.ne.0 ) then
            if( BNDS_LO(il).gt.BNDS_UO(iu) ) then
               write(line(1),*) 'Bounds for X(',i,') are inconsistent:'
               write(line(2),*) 'BL = ',BNDS_LO(il),
     1              ', BU = ',BNDS_UO(iu)
               call C_OUT(2,0,2,line)
               IERR = 7
               goto 9999
            elseif( BNDS_LO(il).ge.BNDS_UO(iu) ) then
               if( XORIG(i).ne.BNDS_LO(il) .and. QCNR.gt.0
     1             .and. QPRINT.gt.5 ) then
                  write(line,700) i,XORIG(i),BNDS_LO(il)
 700              format('XORIG(',i5,') corrected from ',d20.12,
     1                   ' to ',d20.12,' (fixed)')
                  call C_OUT(1,6,1,line)
               endif
               XORIG(i) = BNDS_LO(il)
               NFIX = NFIX + 1
               IFIX(NFIX) = i
            endif
         endif
      enddo

 9999 continue
      return
      end
