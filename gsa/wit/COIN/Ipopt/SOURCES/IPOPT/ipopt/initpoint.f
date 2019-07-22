C Copyright (C) 2002, Carnegie Mellon University and others.
C All Rights Reserved.
C This code is published under the Common Public License.
C*******************************************************************************
C
      subroutine INITPOINT(N, X, NORIG, XORIG, IVAR, M, NIND,
     1     NLB, ILB, BNDS_L, V_L, S_L, NUB,
     1     IUB, BNDS_U, V_U, S_U, MU, CSCALE, LSLACKS,
     2     KCONSTR, LRS, RS, LIS, IS, LRW, RW, LIW, IW, IERR)
C
C*******************************************************************************
C
C    $Id: initpoint.f,v 1.2 2002/05/08 18:17:29 andreasw Exp $
C
C-------------------------------------------------------------------------------
C                                 Title
C-------------------------------------------------------------------------------
C
CT    Determine initial point (primal and dual) and scaling factors
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
CP   N         I    INT    number of variables (without fixed vars)
CP   X        I/O   DP     I: user supplied initial point
CP                         O: corrected to keep strictly inside bounds
CP   NORIG     I    INT    number of variables in problem statement
CP                            (incl. fixed variables)
CP   XORIG    I/O   DP     I: user supplied initial point (incl. fixed vars)
CP                         O: corrected to keep strictly inside bounds
CP                            (only corrected if QMOVEINIT = 1)
CP   IVAR      I    INT    information about partition:
CP                            X(i) corresponds to XORIG(IVAR(i))
CP   M         I    INT    number of dependent variables
CP   NIND      I    INT    number of independent variables
CP   NLB       I    INT    number of lower bounds (without fixed vars)
CP   ILB       I    INT    BNDS_L(i) is lower bound for X(ILB(i))
CP                            (Note: does not have to be ordered)
CP   BNDS_L    I    DP     lower bounds
CP   V_L       O    DP     initialized dual variables for lower bounds
CP                            (ordered as BNDS_L)
CP   S_L       O    DP     = X - BNDS_L
CP                            (ordered as BNDS_L)
CP   NUB       I    INT    number of upper bounds (without fixed vars)
CP   IUB       I    INT    BNDS_U(i) is upper bound for X(ILB(i))
CP                            (Note: does not have to be ordered)
CP   BNDS_U    I    DP     upper bounds
CP   V_U       O    DP     initialized dual variables for lower bounds
CP                            (ordered as BNDS_U)
CP   S_U       O    DP     = X - BNDS_U
CP                            (ordered as BNDS_U)
CP   MU        O    DP     initial value of barrier parameter
CP   CSCALE    O    DP     (only for QSCALE = 1):
CP                            scaling factors for constraints:
CP                              C_algorithm(i) = C_orig(i)*CSCALE(i)
CP   LSLACKS   O    LOG    .false.:  All orinial points sufficiently away
CP                                   from bounds, i.e. X-S = 0
CP                         .true.:   Some variables not sufficiently in the
CP                                   interior; some slacks different from
CP                                   X-BNDS_L or BNDS_U-X
CP   KCONSTR   I    INT    KCONSTR(1): LRS for CONSTR
CP                         KCONSTR(2): P_LRS for CONSTR
CP                         KCONSTR(3): LIS for CONSTR
CP                         KCONSTR(4): P_LIS for CONSTR
CP                         KCONSTR(5): LRW for CONSTR
CP                         KCONSTR(6): LIW for CONSTR
CP   LRS       I    INT    total length of RS
CP   RS        S    DP     DP storage space (all!)
CP   LIS       I    INT    total length of IS
CP   IS        S    INT    INT storage space (all!)
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
      integer N
      double precision X(N)
      integer NORIG
      double precision XORIG(NORIG)
      integer IVAR(N)
      integer M
      integer NIND
      integer NLB
      integer ILB(NLB)
      double precision BNDS_L(NLB)
      double precision V_L(NLB)
      double precision S_L(NLB)
      integer NUB
      integer IUB(NUB)
      double precision BNDS_U(NUB)
      double precision V_U(NUB)
      double precision S_U(NUB)
      double precision MU
      double precision CSCALE(M)
      logical LSLACKS
      integer KCONSTR(6)
      integer LRS
      double precision RS(LRS)
      integer LIS
      integer IS(LIS)
      integer LRW
      double precision RW(LRW)
      integer LIW
      integer IW(LIW)
      integer IERR
C
C-------------------------------------------------------------------------------
C                            Local varibales
C-------------------------------------------------------------------------------
C
      double precision DNRM2, DASUM, D1MACH

      integer p_rwend, p_iwend, p_ilbinv, p_iubinv, p_g, p_c
      integer i, ibl, ibu, ii
      double precision bl, bu, diff, xtmp, vinit, macheps
      logical upp, low
      character*80 line(3)

C
C     QINIT   =  1: use MU=QMUINIT, v = 1d0
C               -1: use MU=QMUINIT, v = MU/s
C                0: use MU=QMUINIT, take v's as given by user
C                2: v = max(1d0, ||G(x0)||_2), MU = max(1, (S^T V)/(NLB+NUB))
C
C     QSCALE  =  0: use QFSCALE as in PARAMS.DAT, don't scale constraints
C                1: use Yamashita heuristic to overwrite QFSCALE and to obtain
C                   scalar scaling factor CSCALE for ALL constraints
C                2: read vector of scaling factor for constraints from file
C                   CSCALE.DAT
C


C
C*******************************************************************************
C
C                           Executable Statements
C
C*******************************************************************************
C
      IERR = 0
      LSLACKS = .false.
      macheps = D1MACH(4)
      p_rwend = 0
      p_iwend = 0
C
C     Output
C
      if( QCNR.gt.0 .and. QPRINT.gt.1 ) then
         write(line,700)
 700     format(/,'Information about initial point:',/)
         call C_OUT(1,2,3,line)
      endif
C
C     Compute initial values of slacks; if original point too close to or
C        violating bounds, X-S not 0.
C

C     Determine inverse of ILB and IUB
      p_ilbinv = p_iwend
      p_iubinv = p_ilbinv + N
      p_iwend  = p_iubinv + N
      if( p_iwend.gt.LIW ) then
         IERR = 99
         goto 9999
      endif
      do i = 1, N
         IW(p_ilbinv+i) = 0
         IW(p_iubinv+i) = 0
      enddo
      do i = 1, NLB
         IW(p_ilbinv+ILB(i)) = i
      enddo
      do i = 1, NUB
         IW(p_iubinv+IUB(i)) = i
      enddo

C     Loop over all variables to determine if they have lower and/or upper
C        bounds.  If so, initialize slacks

      do i = 1, N
         ibl = IW(p_ilbinv+i)
         if( ibl.ne.0 ) then
            low = .true.
CTRY
            BNDS_L(ibl) = BNDS_L(ibl) -
     1           10.d0*macheps*dabs(BNDS_L(ibl))
            bl = BNDS_L(ibl)
         else
            low = .false.
         endif
         ibu = IW(p_iubinv+i)
         if( ibu.ne.0 ) then
            upp = .true.
CTRY
            BNDS_U(ibu) = BNDS_U(ibu) +
     1           10.d0*macheps*dabs(BNDS_U(ibu))
            bu = BNDS_U(ibu)
         else
            upp = .false.
         endif

         if( low ) then
            if( upp ) then
               diff = bu - bl
               if( diff.le.0.d0 ) then
                  write(line(1),*) 'Bounds ',i,' are inconsistent:'
                  write(line(2),*) 'BL = ',bl,', BU = ',bu
                  call C_OUT(2,0,2,line)
                  IERR = 7
                  goto 9999
               endif
ccc               xtmp = dmax1(bl + QFRAC*diff,
ccc     1                      dmin1(bu - QFRAC*diff, X(i)))
               xtmp = dmax1(dmin1(bl + QFRAC*diff,bl + QTHETA),
     1                      dmin1(dmax1(bu - QFRAC*diff, bu - QTHETA),
     1                            X(i)))
               if( QMOVEINIT.eq.1 ) then
                  X(i) = xtmp
               endif
               ii = IVAR(i)
               if( XORIG(ii).ne.xtmp ) then
                  if( QCNR.gt.0 .and. QPRINT.gt.1 ) then
                     write(line,710) ii,XORIG(ii),xtmp
 710                 format('  XORIG(',i5,') corrected from ',
     1                      d12.5,' to ',d12.5)
                     call C_OUT(1,2,1,line)
                  endif
                  LSLACKS = .true.
                  XORIG(ii) = X(i)
               endif
C IMPORTANT:  Need S_L + S_U = BU - BL !!!!
               S_L(ibl) = xtmp - bl
               S_U(ibu) = bu - xtmp
            else
               xtmp = dmax1(bl + QTHETA, X(i))
               if( QMOVEINIT.eq.1 ) then
                  X(i) = xtmp
               endif
               ii = IVAR(i)
               if( XORIG(ii).ne.xtmp ) then
                  if( QCNR.gt.0 .and. QPRINT.gt.1 ) then
                     write(line,710) ii,XORIG(ii),xtmp
                     call C_OUT(1,2,1,line)
                  endif
                  LSLACKS = .true.
                  XORIG(ii) = X(i)
               endif
               S_L(ibl) = xtmp - bl
            endif
         else
            if( upp ) then
               xtmp = dmin1(bu - QTHETA, X(i))
               if( QMOVEINIT.eq.1 ) then
                  X(i) = xtmp
               endif
               ii = IVAR(i)
               if( XORIG(ii).ne.xtmp ) then
                  if( QCNR.gt.0 .and. QPRINT.gt.1 ) then
                     write(line,710) ii,XORIG(ii),xtmp
                     call C_OUT(1,2,1,line)
                  endif
                  LSLACKS = .true.
                  XORIG(ii) = X(i)
               endif
               S_U(ibu) = bu - xtmp
            endif
         endif
      enddo
 190  continue
      p_iwend = p_ilbinv

      if( .not.LSLACKS .and. QCNR.gt.0 .and. QPRINT.gt.1 ) then
         call C_OUT(1,2,1,'  All slacks satisfy slack equation.')
      endif
      if( QMOVEINIT.eq.1 ) then
         LSLACKS = .false.
      endif
C
C     Compute scaling factors
C
      if( QSCALE.eq.0 ) then
         if( M.gt.0 ) CSCALE(1) = 1d0
      elseif( QSCALE.eq.1 ) then
C
C     Yamashita heuristic
C
         QFSCALE = 1d0
         p_g     = p_rwend
         p_rwend = p_g + 1
         if( p_rwend.gt.LRW ) then
            IERR = 98
            goto 9999
         endif
         call GET_G(N, X, IVAR, NORIG, XORIG, RW(p_g+1),
     1        LRW-p_rwend, RW(p_rwend+1), IERR)
         QFSCALE = DASUM(N, RW(p_g+1), 1)
c         QFSCALE = 1d0/dmax1(1d0,QFSCALE/N)
         QFSCALE = N/dmax1(1d0,QFSCALE)
         write(line,*) 'initpoint: QFSCALE = ',QFSCALE
         call C_OUT(2,0,1,line)
         p_rwend = p_g
C
         if( M.gt.0 ) then
            p_c     = p_rwend
            p_rwend = p_c + M
            if( p_rwend.gt.LRW ) then
               IERR = 98
               goto 9999
            endif
            CSCALE(1) = 1d0
            call GET_C(0, N, NIND, X, IVAR, NORIG, XORIG, M,
     1           CSCALE, RW(p_c+1), KCONSTR, LRS, RS, LIS, IS,
     2           LRW-p_rwend, RW(p_rwend+1),
     4           LIW-p_iwend, IW(p_iwend+1), IERR)
            if( IERR.gt.0 ) then
               write(line,*)
     1              'initpoint: Error: get_c returns IERR = ',IERR
               call C_OUT(2,0,1,line)
               goto 9999
            elseif( IERR.ne.0 ) then
               write(line,*)
     1              'initpoint: Warning: get_c returns IERR = ',IERR
               call C_OUT(2,0,1,line)
            endif
            CSCALE(1) = DASUM(M, RW(p_c+1), 1)
C            CSCALE(1) = 1d0/dmax1(1d0,CSCALE(1)/M)
            CSCALE(1) = M/dmax1(1d0,CSCALE(1))
            write(line,*) 'initpoint: CSCALE  = ',CSCALE(1)
            call C_OUT(2,0,1,line)
            p_rwend = p_c
         endif
      elseif( QSCALE.eq.2 ) then
         call C_OUT(2,0,1,'initpoint: QSCALE = 2 not yet implemented.')
         IERR = 4
         goto 9999
      else
         call C_OUT(2,0,1,'initpoint: invalid value of QSCALE.')
         IERR = 4
         goto 9999
      endif
C
C     Initialize barrier parameter and dual variables
C
      if( abs(QINIT).le.1 ) then
C
C     Initialize barrier parameter
C
         MU = QMUINIT
C
C     Initialize slack multipliers (now as MU*S^{-1}e )
C
         if( QINIT.eq.-1 ) then
            do i = 1, NLB
               V_L(i) = MU/S_L(i)
            enddo
            do i = 1, NUB
               V_U(i) = MU/S_U(i)
            enddo
         elseif( QINIT.eq.1 ) then
            do i = 1, NLB
               V_L(i) = 1.d0
            enddo
            do i = 1, NUB
               V_U(i) = 1.d0
            enddo
         else                   ! warm start
            do i = 1, NLB
               V_L(i) = dmax1( V_L(i), QTHETA )
            enddo
            do i = 1, NUB
               V_U(i) = dmax1( V_U(i), QTHETA )
            enddo
         endif
C
      elseif( QINIT.eq.2 ) then ! This is like Yamshita-Yabe-Tanabe
         if( LSLACKS ) then
            call C_OUT(2,0,1,
     1           'initpoint: Don''t know how to LSLACK yet.')
            IERR = 4
            goto 9999
         endif
C
C     Initialize dual variables based on norm of objective gradient
C
         p_g     = p_rwend
         p_rwend = p_g + N
         if( p_rwend.gt.LRW ) then
            IERR = 98
            goto 9999
         endif
         call GET_G(N, X, IVAR, NORIG, XORIG, RW(p_g+1),
     1        LRW-p_rwend, RW(p_rwend+1), IERR)
CTODO decide which norm (l1 or l2) to use...
         vinit = DNRM2(N, RW(p_g+1), 1)
         p_rwend = p_g
         vinit = dmax1( vinit, 1d0 )
         do i = 1, NLB
            V_L(i) = vinit
         enddo
         do i = 1, NUB
            V_U(i) = vinit
         enddo
C
C     Compute initial value of barrier parameter
C
         if( NLB+NUB.gt.0 ) then
            MU = 0d0
            do i = 1, NLB
               MU = MU + V_L(i)*S_L(i)
            enddo
            do i = 1, NUB
               MU = MU + V_U(i)*S_U(i)
            enddo
C            MU = dmax1( QMUINIT, MU/(NLB+NUB) )
            MU = MU/(NLB+NUB)
         else
            MU = QMUINIT
         endif
         write(line,*) 'initpoint: MUinit  = ',MU
         call C_OUT(2,0,1,line)
      else
         write(line,*) 'initpoint: QINIT = ',QINIT
         call C_OUT(2,0,1,line)
         IERR = 4
         goto 9999
      endif
C
C     Make sure that dual variables don't differ too much from MU*X{-1}*e
C
      do i = 1, NLB
         xtmp = V_L(i)
         V_L(i) = dmax1(V_L(i), MU/QVCORRECTFACT/S_L(i))
         V_L(i) = dmin1(V_L(i), MU*QVCORRECTFACT/S_L(i))
CTMP
         if( xtmp.ne.V_L(i).and.QPRINT.gt.2 ) then
            write(line,*) 'initpoint: V_L(',i,') corrected from'
            call C_OUT(1,0,1,line)
            write(line,*) '           ',xtmp,' to ',V_L(i)
            call C_OUT(1,0,1,line)
         endif
      enddo
      do i = 1, NUB
         xtmp = V_U(i)
         V_U(i) = dmax1(V_U(i), MU/QVCORRECTFACT/S_U(i))
         V_U(i) = dmin1(V_U(i), MU*QVCORRECTFACT/S_U(i))
CTMP
         if( xtmp.ne.V_U(i).and.QPRINT.gt.2 ) then
            write(line,*) 'initpoint: V_U(',i,') corrected from'
            call C_OUT(1,0,1,line)
            write(line,*) '           ',xtmp,' to ',V_U(i)
            call C_OUT(1,0,1,line)
         endif
      enddo
C
C     If we are dealing with a square problem, we MU to less than
C     error tolerance, so that there are no initial dummy iterations
C
      if( NIND.eq.0 .or. (NLB.eq.0 .and. NUB.eq.0) .and. QCG.eq.0 ) then
         MU = 0.5d0*QTOL/QMUERRFAC
      endif

C     Set common block variable for AMPL interface
      AMPLMU = MU

 9999 continue
      return
      end
