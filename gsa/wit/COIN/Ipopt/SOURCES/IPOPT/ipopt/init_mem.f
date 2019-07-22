C Copyright (C) 2002, Carnegie Mellon University and others.
C All Rights Reserved.
C This code is published under the Common Public License.
C*******************************************************************************
C
      subroutine INIT_MEM(NORIG, XORIG, N, NIND, M, NLB, NUB, NZORIG,
     1     KCONSTR, LRS_END, LIS_END, LRW, RW, LIW, IW, IERR)

C
C*******************************************************************************
C
C    $Id: init_mem.f,v 1.4 2002/11/24 21:42:10 andreasw Exp $
C
C-------------------------------------------------------------------------------
C                                 Title
C-------------------------------------------------------------------------------
C
CT    Main (outer) loop of algorithm
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
CP   NORIG     I    INT    total number of variables (incl. fixed vars)
CP   XORIG     I    DP     initial values of variables (incl. fixed vars)
CP   N         I    INT    number of variables (without fixed)
CP   NIND      I    INT    number of independent variables
CP   M         I    INT    number of constraints
CP   NLB       I    INT    number of lower bounds (excluding fixed vars)
CP   NUB       I    INT    number of upper bounds (excluding fixed vars)
CP   NZORIG    I    INT    number of nonzeros in Jacobian of constraints
CP                            (including rows to fixed variables!)
CP   KCONSTR   I    INT    KCONSTR(1): LRS for CONSTR
CP   LRS_END  I/O   INT    last used reserved entry in RS
CP   LRS_END  I/O   INT    last used reserved entry in IS
CP   LRW       I    INT    length of RW
CP   RW       I/O   DP     can be used as DP work space but content will be
CP                            changed between calls
CP   LIW       I    INT    length of IW
CP   IW       I/O   INT    can be used as INT work space but content will be
CP                            changed between calls
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
CS    CONSTR
CS    MAINLOOP
CS    LINESEARCH
CS    EXTRAPOL
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
      include 'TIMER.INC'
C
C-------------------------------------------------------------------------------
C                             Parameter list
C-------------------------------------------------------------------------------
C
      integer NORIG
      double precision XORIG(NORIG)
      integer N
      integer NIND
      integer M
      integer NLB
      integer NUB
      integer NZORIG
      integer KCONSTR(6)
      integer LRS_END
      integer LIS_END
      integer LRW
      double precision RW(LRW)
      integer LIW
      integer IW(LIW)
      integer IERR
C
C-------------------------------------------------------------------------------
C                            Local variables
C-------------------------------------------------------------------------------
C
      integer lrs_constr, lis_constr, lrw_constr, liw_constr
      integer idummy
      double precision dummy
      logical ldummy
      character*1 cdummy
      character*80 line
C
C*******************************************************************************
C
C                           Executable Statements
C
C*******************************************************************************
C

C
C     Initialize CPU times
C
      TIME_BB     = 0.d0
      TIME_CG     = 0.d0
      TIME_YPY    = 0.d0
      TIME_EXACTW = 0.d0
      TIME_ZWZY_BACKS = 0.d0
      TIME_ZWZY_EVALA = 0.d0
      TIME_PZ_CHOL    = 0.d0
      TIME_HV         = 0.d0
      COUNT_CG         = 0
      COUNT_RESTO_ITER = 0
      COUNT_NEG_CURV   = 0
      COUNT_RESTO_CALL = 0
      COUNT_TRON_CG    = 0
      COUNT_HV         = 0
      COUNT_DEPCON     = 0
C
C     call subroutines to initialize their pointers for the storage space
C

C
C     CONSTR
C
      if( M.ne.0 ) then
         lrw_constr = LRW
         liw_constr = LIW
         call CONSTR(0, idummy, N, NIND, M, idummy, idummy, idummy,
     1               NORIG, XORIG, dummy, dummy, dummy, NZORIG,
     2               idummy, lrs_constr, dummy, lis_constr, idummy,
     3               lrw_constr, RW, liw_constr, IW, IERR)
         if( IERR.ne.0 ) then
            write(line,*) 'init_mem: CONSTR returns IERR = ', IERR
            call C_OUT(2,0,1,line)
            goto 9999
         endif
         KCONSTR(1) = lrs_constr
         KCONSTR(3) = lis_constr
         KCONSTR(5) = lrw_constr
         KCONSTR(6) = liw_constr

         KCONSTR(2) = LRS_END
         LRS_END    = KCONSTR(2) + lrs_constr
         KCONSTR(4) = LIS_END
         LIS_END    = KCONSTR(4) + lis_constr
      else
         NZORIG = 0
      endif
      write(line,1000) NZORIG
 1000 format('Number of nonzeros in Jacobian: ',i8)
      call C_OUT(2,0,1,line)
C
C     MAINLOOP
C
      call MAINLOOP(-1, N, NIND, M, NORIG, dummy,
     1              idummy, dummy, idummy, idummy, NLB, idummy, NUB,
     2              idummy, dummy, dummy, NZORIG, dummy,
     1              dummy, dummy, dummy, idummy, idummy,
     3              LRS_END, dummy, idummy, LIS_END, idummy,
     4              idummy, dummy, idummy, idummy, IERR)
C
C     LINESEARCH
C
      if( abs(QMERIT).eq.1 .or. abs(QMERIT).eq.2 .or. 
     1     (QMERIT.eq.3.and.QAUGITER.gt.0) ) then
         call LINESEARCH(-1, N, NIND, M, dummy, idummy, NLB, idummy,
     1        NUB, idummy, ldummy, dummy, dummy, dummy, dummy, dummy,
     2        dummy, dummy, dummy, dummy, dummy, dummy, NORIG,
     3        dummy, dummy, dummy, dummy, dummy, dummy, dummy,
     4        dummy, dummy, dummy, dummy, dummy, dummy, dummy,
     2        dummy, dummy, dummy, ldummy, dummy, dummy, idummy,
     5        cdummy, dummy, ldummy, idummy, idummy,
     5        idummy, LRS_END, dummy, idummy, LIS_END,
     6        idummy, idummy, dummy, idummy, idummy, IERR)
      endif
      if( abs(QMERIT).ge.4 ) then
         call FILTER(-1, N, NIND, M, dummy, idummy, idummy, idummy,
     1        NLB, idummy,
     1        NUB, idummy, ldummy, dummy, dummy, dummy, dummy,
     2        dummy, dummy, dummy, dummy, dummy, dummy, dummy, dummy,
     3        NORIG, dummy, dummy, dummy, dummy, dummy, dummy, dummy,
     4        dummy, dummy, dummy, dummy, dummy, dummy, ldummy, idummy,
     5        dummy, cdummy, cdummy, idummy, ldummy, idummy,
     5        LRS_END, dummy, LIS_END,
     6        idummy, idummy, dummy, idummy, idummy, IERR)
      endif
      if( abs(QMERIT).ge.4 ) then
         if( QRESTO.eq.2 .or. abs(QRESTO).eq.3 ) then
            call RESTO_TRON(-1, N, NIND, M, NORIG, XORIG,
     1           dummy, idummy, idummy, idummy, NLB, idummy, NUB,
     1           idummy, dummy, dummy, dummy, dummy, dummy, NZORIG,
     1           dummy, dummy, dummy, ldummy, dummy, dummy,
     1           dummy, dummy, dummy, dummy, dummy, dummy, dummy,
     1           dummy, dummy, dummy,
     1           idummy, LRS_END, dummy, LIS_END, idummy,
     1           idummy, dummy, idummy, idummy, IERR)
         endif
      endif
C
C     GET_STEP_FULL
C
      if( QFULL.ne.0 ) then
         call GET_STEP_FULL(-1, NORIG, N, NIND, M, NZORIG, dummy,
     1        XORIG, dummy, dummy, NLB, idummy, NUB, idummy, idummy,
     1        idummy, idummy, dummy, dummy,
     1        dummy, dummy, dummy, dummy, dummy, dummy, ldummy,
     1        dummy, dummy, dummy, dummy, dummy, dummy, dummy, dummy,
     1        dummy, dummy,
     1        dummy, dummy, cdummy, ldummy, idummy, dummy, dummy, dummy,
     1        dummy, dummy, dummy, idummy, dummy, dummy, ldummy, idummy,
     1        idummy, ldummy, idummy, idummy, LRS_END, dummy, idummy,
     1        LIS_END, idummy, idummy, dummy, idummy, idummy, IERR)
      endif
C
C     ERROR (just for initialization of LASTITER)
C
      call ERROR(N, NIND, M, dummy, dummy, dummy, dummy, dummy, dummy,
     1     NLB, idummy, NUB, idummy, dummy, dummy, dummy, dummy,
     1     dummy, dummy, dummy, dummy, dummy, dummy, ldummy,
     2     dummy, cdummy, dummy, dummy, dummy, dummy, -1, idummy,
     1     idummy, idummy, NORIG, dummy,
     1     dummy, KCONSTR, idummy, dummy, idummy, idummy,
     1     idummy, dummy, idummy, idummy, IERR)
C
C     GET_HV
C
      call GET_HV(-1, N, NIND, idummy, idummy, dummy, idummy, NORIG,
     1     XORIG, NLB, idummy, NUB, idummy, dummy, dummy,
     2     dummy, M, dummy, dummy, dummy,
     2     KCONSTR, idummy, dummy, idummy, idummy, idummy, dummy,
     3     idummy, idummy, IERR)
C
C     GET_YPY (for orthogonal decomposition)
C
      call GET_YPY(NZORIG, idummy, idummy, -1, idummy, idummy, idummy,
     1     NORIG, dummy, dummy, idummy, idummy, idummy, idummy,
     2     dummy, dummy, dummy, ldummy, idummy,
     3     dummy, dummy, dummy, ldummy, dummy,
     4     KCONSTR, idummy, dummy, idummy, idummy,
     5     idummy, dummy, idummy, idummy, IERR)
C
 9999 continue
      return
      end
