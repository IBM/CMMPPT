C Copyright (C) 2002, Carnegie Mellon University and others.
C All Rights Reserved.
C This code is published under the Common Public License.

C $Id: IPOPT.INC,v 1.4 2002/11/24 21:33:10 andreasw Exp $

      integer          QMAXITER
      integer          QCNR
      integer          QPRINT
      integer          QOUTPUT
      integer          QSELBAS
      integer          QQUASI
      integer          QINITB
      integer          QMERIT
      integer          QCNRM
      integer          QCORRECT
      integer          QMOVEINIT
      integer          QPRIMAL
      integer          QFULL
      integer          QLAMBDA
      integer          QERROR
      integer          QSCALERR
      integer          QAUGITER
      integer          QSOC
      integer          QRESTO
      integer          QINIT
      integer          QSCALE
      integer          QCG
      integer          QALPHA
      integer          QHESS
      integer          QDAMP
      integer          QTRONHESS
      integer          QTRON2DERIVS
      integer          QITERBLOCKMAX
      integer          QKKTSCALE
      integer          QSYMSOLV
      integer          QMAXCGITER
      integer          QBWARMSTART
      integer          QLMLEN

      double precision QTOL
      double precision QWATCHTOL
      double precision QLS_SAFE
      double precision QRHO
      double precision QNUMIN
      double precision QMAXCOND
      double precision QSR1TOL
      double precision QMUINIT
      double precision QMULIN
      double precision QMUSUPER
      double precision QMAXERR
      double precision QMUERRFAC
      double precision QFRAC
      double precision QTHETA
      double precision QTAU
      double precision QFSCALE
      double precision QSKIP
      double precision QVCORRECTFACT
      double precision QPIVTOL
      double precision QPIVTOLMAX
      double precision QTRONCGTOL
      double precision QTRONGTOL
      double precision QERRSUPER
      double precision QDEPCONDIAG
      double precision QRESTOKKTRED
      double precision QS_F
      double precision QGAMMA_F
      double precision QS_THETA
      double precision QGAMMA_THETA
      double precision QCGTOL
      double precision QFILLINFACT

      double precision QDDMAX
      parameter      ( QDDMAX = 1d0 )

      common /PARAMS/ 
     1     QTOL, QWATCHTOL, QLS_SAFE, QRHO, QNUMIN, QMAXCOND,
     1     QSR1TOL, QMUINIT, QMULIN, QMUSUPER, QMAXERR, QMUERRFAC,
     1     QFRAC, QTHETA, QTAU, QFSCALE, QSKIP, QVCORRECTFACT,
     1     QPIVTOL, QTRONCGTOL, QERRSUPER, QDEPCONDIAG, QRESTOKKTRED,
     1     QS_F, QGAMMA_F, QS_THETA, QGAMMA_THETA, QTRONGTOL, QCGTOL,
     1     QPIVTOLMAX, QFILLINFACT,
     1     QMAXITER, QCNR, QPRINT, QOUTPUT, QSELBAS, QQUASI, QINITB,
     1     QMERIT, QCNRM, QCORRECT, QMOVEINIT, QPRIMAL, QFULL, QLAMBDA,
     1     QERROR, QSCALERR, QAUGITER, QSOC, QRESTO, QINIT, QSCALE,
     1     QCG, QALPHA, QHESS, QDAMP, QTRONHESS, QTRON2DERIVS,
     1     QITERBLOCKMAX, QKKTSCALE, QSYMSOLV, QMAXCGITER, QBWARMSTART,
     1     QLMLEN
      save /PARAMS/
C
C     For AMPL complementarity constraints
C
      double precision AMPLMU
      common /AMPLMU/ AMPLMU
      save /AMPLMU/

C
C     Constants for indefiniteness regularization
C
      double precision REGU_INIT, REGU_MAX, REGU_DEC_FACT, REGU_INC_FACT
      double precision REGU_INIT_FACT
      parameter( REGU_INIT = 1d-4, REGU_MAX = 1d40,
     1     REGU_DEC_FACT = 1d0/3d0, REGU_INC_FACT = 8d0,
     1     REGU_INIT_FACT = 10d0 )

