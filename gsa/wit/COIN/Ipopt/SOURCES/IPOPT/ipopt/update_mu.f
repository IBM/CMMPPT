C Copyright (C) 2002, Carnegie Mellon University and others.
C All Rights Reserved.
C This code is published under the Common Public License.
C*******************************************************************************
C
      subroutine UPDATE_MU(FIRST_MU, MU, ERR_BAR)
C
C*******************************************************************************
C
C    $Id: update_mu.f,v 1.2 2003/01/27 22:17:03 andreasw Exp $
C
C-------------------------------------------------------------------------------
C                                 Title
C-------------------------------------------------------------------------------
C
CT    Update barrier parameter and determine tolerance for barrier problem
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
CP   FIRST_MU I/O   LOG    I: if true, only change ERR_BAR
CP                         O: set to false
CP   MU       I/O   DP     barrier parameter
CP   ERR_BAR   O    DP     error for barrier problem
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
CS
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
      logical FIRST_MU
      double precision MU
      double precision ERR_BAR

      double precision mu_old
C
C*******************************************************************************
C
C                           Executable Statements
C
C*******************************************************************************
C
      if( QERRSUPER.le.0.d0 ) then
         if( .not.FIRST_MU ) then
            MU = dmin1(QMULIN*MU, MU**QMUSUPER)
         endif
         ERR_BAR = dmax1(QTOL-MU, dmin1(QMAXERR, MU*QMUERRFAC))
      else
C
C     New rule based on Gould, Orban, Sartenaer, Toint
C
         if( .not.FIRST_MU ) then
            mu_old = MU
            MU = dmin1(QMULIN*MU, MU**QMUSUPER)
         else
            mu_old = MU**(1.d0/QERRSUPER)
         endif
         ERR_BAR = dmax1(QTOL-MU,
     1        dmin1(QMAXERR, QMUERRFAC*min(mu_old**QERRSUPER, MU)))
      endif

      FIRST_MU = .false.

      AMPLMU = MU

      return
      end
