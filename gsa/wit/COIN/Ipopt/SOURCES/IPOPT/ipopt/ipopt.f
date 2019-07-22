C Copyright (C) 2002, Carnegie Mellon University and others.
C All Rights Reserved.
C This code is published under the Common Public License.
C*******************************************************************************
C
      subroutine IPOPT(NORIG, XORIG, M, NLBO, ILBO, BNDS_LO,
     1     NUBO, IUBO, BNDS_UO, V_LO, V_UO, LAM, LRW, RW, LIW, IW,
     1     ITER, IERR)

C !DEC$ ATTRIBUTES DLLEXPORT::IPOPT
C
C*******************************************************************************
C
C    $Id: ipopt.f,v 1.3 2002/07/24 03:07:54 andreasw Exp $
C
C-------------------------------------------------------------------------------
C                                 Title
C-------------------------------------------------------------------------------
C
CT    Driver routine for IPOPT
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
CP   XORIG    I/O   INT    I: user provided initial point (incl fixed vars)
CP                         O: solution (hopefully)
CP                            (original order as in problem statement)
CP   M         I    INT    number of equality constraints
CP   NLBO      I    INT    number of lower bounds (incl fixed vars)
CP   ILBO      I    INT    indices of lower bounds
CP                            (e.g. BNDS_LO(i) is bound for XORIG(ILBO(i)) )
CP   BNDS_LO   I    DP     values of lower bounds
CP   NUBO      I    INT    number of upper bounds (incl fixed vars)
CP   IUBO      I    INT    indices of upper bounds
CP                            (e.g. BNDS_UO(i) is bound for XORIG(IUBO(i)) )
CP   BNDS_UO   I    DP     values of upper bounds
CP   V_LO      O    DP     multipliers corresponding to the lower bounds
CP                            (ordered like BNDS_LO) - set to zero for fixed
CP                            variables
CP   V_UO      O    DP     multipliers corresponding to the upper bounds
CP                            (ordered like BNDS_UO) - set to zero for fixed
CP                            variables
CP   LAM       O    DP     multipliers corresponding to the equality
CP                            constraints
CP   LRW       I    INT    length of RW
CP   RW        W    DP     DP work space
CP                            (internal usage: first RS, then RW)
CP   LIW       I    INT    length of IW
CP   IW        W    INT    INT work space
CP                            (internal usage: first IS, then IW)
CP   ITER      O    INT    number of iterations
CP   IERR      O    INT    =0: everything OK (solution found)
CP                         =1: maximal number of iterations exceeded
CP                             (increase IMAXITER)
CP                         =2: line search failure
CP                             (try different line search?)
CP                         =3: line search failure during active
CP                             watchdog
CP                         =4: some algorithmic parameters wrong
CP                             (Check PARAMS.DAT)
CP                         =5: too few degrees of freedom
CP                         =6: it seems we have linear dependent
CP                             equality constraints
CP                         =7: some of the bounds are inconsistent
CP                         =8: error opening or reading a file
CP                         =9: Step size getting too small
CP                             (tolerance too tight?)
CP                        =10: Regularization parameter (for indefinite
CP                             reduced Hessian) is becoming too large.
CP                             (equality constraints linearly dependent?)
CP                        =11: NaN or Inf occured
CP                        =15: Index in ILB or IUB out of range
CP                        =16: Point is (almost) feasible, but
CP                             IPOPT is in restoration phase.
CP                             (At this point, we are not sure under
CP                              which circumstances this can happen,
CP                              but it seems that in many those
CP                              cases the constraint gradients are
CP                              ill-conditioned...)
CP                        =17: Convergence to stationary point for
CP                             infeasibility
CP                             (Problem locally infeasible?)
CP                        =18: Restoration phase cannot further improve
C                               feasibility
CP                        =19: A slack variable becomes too small
CP                             (Problem has no strict interior?)
CP                        =97: An option was chosen for which a third party
CP                             component has not been compiled into excutable
CP                        =98: LRW too small
CP                        =99: LIW too small
CP                      other: Look at output... ;-)
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
CS    GET_IFIX
CS    INIT_MEM
CS    INIT_BND
CS    MAINLOOP
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
      include 'TIMER.INC'
C
C-------------------------------------------------------------------------------
C                             Parameter list
C-------------------------------------------------------------------------------
C
      integer NORIG
      double precision XORIG(NORIG)
      integer M
      integer NLBO
      integer ILBO(NLBO)
      double precision BNDS_LO(NLBO)
      integer NUBO
      integer IUBO(NUBO)
      double precision BNDS_UO(NUBO)
      double precision V_LO(NLBO)
      double precision V_UO(NUBO)
      double precision LAM(M)
      integer LRW
      double precision RW(LRW)
      integer LIW
      integer IW(LIW)
      integer ITER
      integer IERR
C
C-------------------------------------------------------------------------------
C                            Local variables
C-------------------------------------------------------------------------------
C
      integer KCONSTR(6)
      save    KCONSTR

      integer LRS_END, LIS_END, P_IFIX, P_X, P_BNDSL, P_BNDSU
      save    LRS_END, LIS_END, P_IFIX, P_X, P_BNDSL, P_BNDSU
      integer P_ILB, P_IUB, P_IVAR
      save    P_ILB, P_IUB, P_IVAR
      integer N, NIND, NLB, NUB, NFIX, NZORIG
      save    N, NIND, NLB, NUB, NFIX, NZORIG
      double precision ERR
      save             ERR

      double precision stime, ftime
      integer p_rwend, p_iwend, p_vcopy, p_ibo1, p_ilbo1, p_iubo1

      integer i
      character*80 line(6)
C
C*******************************************************************************
C
C                           Executable Statements
C
C*******************************************************************************
C
      call TIMER(stime)
      IERR = 0
C
C     Initialize work space pointers
C
      p_rwend = 0
      p_iwend = 0
      LRS_END = 0
      LIS_END = 0
C
C     initialize iteration counter
C
      ITER = 0
C
C     Printout inital point provided by the user
C
      if( QCNR.gt.0 .and. QPRINT.gt.1 ) then
         write(line,700)
 700     format(/,'  Initial point provided by the user: ')
         call C_OUT(1,2,2,line)
         do i = 1, NORIG
            write(line,701) i,XORIG(i)
 701        format(' XORIG(',i5,') = ',d25.15)
            call C_OUT(1,2,1,line)
         enddo
      endif
C
C     Find fixed variables
C
      P_IFIX  = LIS_END
      LIS_END = P_IFIX  + NORIG
      p_iwend = LIS_END

      p_ilbo1 = p_iwend
      p_iubo1 = p_ilbo1 + NORIG
      p_iwend = p_iubo1 + NORIG
      if( p_iwend.gt.LIW ) then
         IERR = 99
         goto 9999
      endif
      call GET_IFIX(NORIG, XORIG, NLBO, ILBO, BNDS_LO,
     1              NUBO, IUBO, BNDS_UO, NFIX, IW(P_IFIX+1),
     2              IW(p_ilbo1+1),IW(p_iubo1+1), IERR)
      if( IERR.ne.0 ) then
         write(line,*) 'ipopt: GET_IFIX returns IERR = ', IERR
         call C_OUT(2,0,1,line)
         goto 9999
      endif
      LIS_END = P_IFIX + NFIX
      p_iwend = LIS_END
C
C     Initialize some sizes
C
      N    = NORIG - NFIX
      NIND = N - M
      NLB  = NLBO - NFIX
      NUB  = NUBO - NFIX
      if( NIND.lt.0 ) then
         write(line,*)
     1        'Error: Number of degrees of freedom is NIND = ',NIND
         call C_OUT(2,0,1,line)
         IERR = 5
         goto 9999
      endif
C
C     Print problem size
C
      write(line,710) NORIG, NFIX, M, NLB, NUB
 710  format(/,'Number of variables           : ',i8,
     1       /,'   of which are fixed         : ',i8,
     1       /,'Number of constraints         : ',i8,
     1       /,'Number of lower bounds        : ',i8,
     1       /,'Number of upper bounds        : ',i8)
      call C_OUT(2,0,6,line)
C
C     Determine demand of storage and work space
C
      call INIT_MEM(NORIG, XORIG, N, NIND, M, NLB, NUB, NZORIG, KCONSTR,
     1              LRS_END, LIS_END, LRW, RW, LIW, IW, IERR)
      if( IERR.ne.0 ) then
         write(line,*) 'ipopt: INIT_MEM returns IERR = ', IERR
         call C_OUT(2,0,1,line)
	   goto 9999
      endif
C      write(*,*) 'LRS_END = ',LRS_END,', LIS_END = ',LIS_END
      if( LRS_END.gt.LRW ) then
         IERR = 98
         call C_OUT(2,0,1,'Not enough double precision storage space:')
         write(line,*) 'LRS_END = ',LRS_END,', but LRW = ',LRW
         call C_OUT(2,0,1,line)
         goto 9999
      elseif( LIS_END.gt.LIW ) then
         IERR = 99
         call C_OUT(2,0,1,'Not enough integer storage space:')
         write(line,*) 'LIS_END = ',LIS_END,', but LIW = ',LIW
         call C_OUT(2,0,1,line)
         goto 9999
      endif
C
C     Now reserve storage space for this subroutine
C
      P_X     = LRS_END
      P_BNDSL = P_X     + N
      P_BNDSU = P_BNDSL + NLB
      LRS_END = P_BNDSU + NUB
      P_ILB   = LIS_END
      P_IUB   = P_ILB   + NLB
      P_IVAR  = P_IUB   + NUB
      LIS_END = P_IVAR  + N
C
C     The rest in RW and IW is now available as work space
C
      p_rwend = LRS_END
      p_iwend = LIS_END
      if( p_rwend.gt.LRW ) then
         IERR = 98
         goto 9999
      elseif( p_iwend.gt.LIW ) then
         IERR = 99
         goto 9999
      endif
C
C     Now initialize X, BNDS_L, BNDS_U
C
      call INIT_BND(NORIG, N, NIND, M, NFIX, IW(P_IFIX+1),
     1              XORIG, RW(P_X+1), NLBO, ILBO, BNDS_LO,
     1              V_LO, NLB, IW(P_ILB+1), RW(P_BNDSL+1),
     1              NUBO, IUBO, BNDS_UO, V_UO, NUB, IW(P_IUB+1),
     1              RW(P_BNDSU+1), IW(P_IVAR+1),
     1              LRW-p_rwend, RW(p_rwend+1),
     1              LIW-p_iwend, IW(p_iwend+1), IERR)
      if( IERR.ne.0 ) then
         write(line,*) 'ipopt: init_bnd returns IERR = ', IERR
         call C_OUT(2,0,1,line)
         goto 9999
      endif
C
C     After all this initialization call algorithm
C
      call MAINLOOP(ITER, N, NIND, M, NORIG, XORIG,
     1              IW(P_IVAR+1), RW(P_X+1), NFIX, IW(P_IFIX+1),
     1              NLB, IW(P_ILB+1), NUB, IW(P_IUB+1), 
     1              RW(P_BNDSL+1), RW(P_BNDSU+1), NZORIG, 
     1              V_LO, V_UO, LAM, ERR, KCONSTR,
     1              LRS_END, LRS_END, RW,
     1              LIS_END, LIS_END, IW,
     1              LRW-p_rwend, RW(p_rwend+1),
     1              LIW-p_iwend, IW(p_iwend+1), IERR)
C      if( IERR.ne.0 ) then
C         write(line,*) 'ipopt: MAINLOOP returns IERR = ', IERR
C         call C_OUT(2,0,1,line)
C      endif

C
C     Correct the position of the bound multipliers in V_LO and V_UO
C
      p_vcopy = p_rwend
      p_rwend = p_vcopy + max(NLB,NUB)
      if( p_rwend.gt.LRW ) then
         IERR = 98
         goto 9999
      endif
      p_ibo1  = p_iwend
      p_iwend = p_ibo1 + NORIG
      if( p_iwend.gt.LIW ) then
         IERR = 99
         goto 9999
      endif
      call DCOPY(NLB, V_LO, 1, RW(p_vcopy+1), 1)
      if( NFIX.gt.0 ) then
         call DCOPY(NLBO, 0.d0, 0, V_LO, 1)
      endif
      do i = 1, NLBO
         IW(p_ibo1+ILBO(i)) = i
      enddo
      do i = 1, NLB
         V_LO(IW(p_ibo1+IW(P_IVAR+IW(P_ILB+i)))) = RW(p_vcopy+i)
      enddo
      call DCOPY(NUB, V_UO, 1, RW(p_vcopy+1), 1)
      if( NFIX.gt.0 ) then
         call DCOPY(NUBO, 0.d0, 0, V_UO, 1)
      endif
      do i = 1, NUBO
         IW(p_ibo1+IUBO(i)) = i
      enddo
      do i = 1, NUB
         V_UO(IW(p_ibo1+IW(P_IVAR+IW(P_IUB+i)))) = RW(p_vcopy+i)
      enddo

C
C     Write status line
C
      line(1) = ' '
      if( IERR.eq.0 ) then
         line(2) = 'EXIT: OPTIMAL SOLUTION FOUND'
      elseif( IERR.eq.1 ) then
         line(2) = 'EXIT: Maximal number of iterations exceeded'
      elseif( IERR.eq.2 ) then
         line(2) = 'EXIT: Line search failure'
      elseif( IERR.eq.3 ) then
         line(2) = 'EXIT: Line search failure during active watchdog'
      elseif( IERR.eq.4 ) then
         line(2) =
     1  'EXIT: Some algorithmic parameters are wrong or inconsistent'
      elseif( IERR.eq.5 ) then
         line(2) = 'EXIT: Too few degrees of freedom'
      elseif( IERR.eq.6 ) then
         line(2) =
     1      'EXIT: Equality constraints seem to be linearly dependent'
      elseif( IERR.eq.7 ) then
         line(2) = 'EXIT: Some of the bounds are inconsistent'
      elseif( IERR.eq.8 ) then
         line(2) = 'EXIT: Error opening or reading some file'
      elseif( IERR.eq.9 ) then
         line(2) =
     1  'EXIT: Step size getting too small - (tolerance too tight?)'
      elseif( IERR.eq.10 ) then
         line(2) =
     1        'EXIT: Regularization parameter is becoming too large'
      elseif( IERR.eq.11 ) then
         line(2) =
     1        'EXIT: NaN or Inf occured'
      elseif( IERR.eq.16 ) then
         line(2) =
     1 'EXIT: Point is (almost) feasible, but calling restoration phase'
      elseif( IERR.eq.17 ) then
         line(2) =
     1        'EXIT: Convergence to stationary point for infeasibility'
      elseif( IERR.eq.18 ) then
         line(2) =
     1      'EXIT: Restoration phase cannot further improve feasibility'
      elseif( IERR.eq.98 ) then
         line(2) = 'EXIT: Size of double precision work space too small'
      elseif( IERR.eq.99 ) then
         line(2) = 'EXIT: Size of integer work space too small'
      else
         write(line, 1000) IERR
 1000    format(/,'EXIT: IERR =',i5,'; check output.')
      endif
      call C_OUT(2,0,2,line)


c      if( QPRINT.ge.0 ) then
         call C_OUT(1,0,1,' ')
         write(line,*) 'TIME_YPY         = ', TIME_YPY
         call C_OUT(1,0,1,line)
         write(line,*) 'TIME_BB          = ', TIME_BB
         call C_OUT(1,0,1,line)
         write(line,*) 'TIME_CG          = ', TIME_CG
         call C_OUT(1,0,1,line)
         write(line,*) 'TIME_HV          = ', TIME_HV
         call C_OUT(1,0,1,line)
         write(line,*) 'TIME_EXACTW      = ', TIME_EXACTW
         call C_OUT(1,0,1,line)
         write(line,*) 'TIME_ZWZY_BACKS  = ', TIME_ZWZY_BACKS
         call C_OUT(1,0,1,line)
         write(line,*) 'TIME_ZWZY_EGVAL  = ', TIME_ZWZY_EVALA
         call C_OUT(1,0,1,line)
         write(line,*) 'TIME_PZ_CHOL     = ', TIME_PZ_CHOL
         call C_OUT(1,0,1,line)
         write(line,*) 'COUNT_CG         = ', COUNT_CG
         call C_OUT(1,0,1,line)
         write(line,*) 'COUNT_HV         = ', COUNT_HV
         call C_OUT(1,0,1,line)
         write(line,*) 'COUNT_RESTO_CALL = ', COUNT_RESTO_CALL
         call C_OUT(1,0,1,line)
         write(line,*) 'COUNT_RESTO_ITER = ', COUNT_RESTO_ITER
         call C_OUT(1,0,1,line)
         write(line,*) 'COUNT_TRON_CG    = ', COUNT_TRON_CG
         call C_OUT(1,0,1,line)
         write(line,*) 'COUNT_NEG_CURV   = ', COUNT_NEG_CURV
         call C_OUT(1,0,1,line)
         write(line,*) 'COUNT_DEPCON     = ', COUNT_DEPCON
         call C_OUT(1,0,1,line)
         call C_OUT(1,0,1,' ')
c      endif

 9999 continue
      call TIMER(ftime)
      write(line,9998) ftime-stime
 9998 format(/,'CPU seconds spent in IPOPT and function evaluations = ',
     1     f15.7,/)
      call C_OUT(2,0,3,line)
      return
      end
