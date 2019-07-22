C     ( Last modified on Tue May 28 14:07:02 CDT 2002 )
      PROGRAM           IPOPTMA
C
C     IPOPT CUTEr driver.
C     D. Orban,  adapted from Andreas Wachters CUTE driver.
C
      IMPLICIT NONE
      INTEGER IOUT
      PARAMETER( IOUT = 6 )

#include "CUTE.INC"

      INTEGER LRW, LIW
CTOY  PARAMETER (LRW = 20000, LIW = 10000)
CMED  PARAMETER (LRW = 10000000, LIW = 10000000)
CBIG  PARAMETER (LRW = 30000000, LIW = 30000000)
C
      INTEGER N, M, NLB, NUB
      DOUBLE PRECISION X( CUTE_NMAX )
      INTEGER ILB( CUTE_NMAX )
      INTEGER IUB( CUTE_NMAX )
      DOUBLE PRECISION BNDS_L( CUTE_NMAX )
      DOUBLE PRECISION BNDS_U( CUTE_NMAX )
      DOUBLE PRECISION V_L( CUTE_NMAX )
      DOUBLE PRECISION V_U( CUTE_NMAX )
      DOUBLE PRECISION LAM( CUTE_MMAX )
      INTEGER IW( LIW )
      DOUBLE PRECISION RW( LRW )
C
C     Algorithmic Parameters (INITPARAMS)
C
      INTEGER NARGS
      DOUBLE PRECISION ARGS( 50 )
      CHARACTER*20 CARGS( 50 )

      INTEGER ITER
      INTEGER IERR

      INTEGER fevals, cevals
      COMMON /EVALS/ fevals, cevals

      REAL CALLS( 7 ), CPU( 2 )
      CHARACTER*10 PNAME
      CHARACTER*10 VNAMES( CUTE_NMAX ), GNAMES( CUTE_MMAX )
      DOUBLE PRECISION FinalF, DummyG, cnrm
      INTEGER IDAMAX
C
      fevals = 0
      cevals = 0
C
C     Get problem dimensions and initialize
C
      CALL CUTE_INIT(N, M, CUTE_NMAX, X, NLB, ILB, BNDS_L,
     .     NUB, IUB, BNDS_U)
C
C     Get problem name.
C
      CALL CNAMES( CUTE_N, CUTE_M, PNAME, VNAMES, GNAMES )
C
C     Initialize parameters
C
      NARGS = 0
      CALL INITPARAMS(IERR, NARGS, ARGS, CARGS)
      IF( IERR.NE.0 ) THEN
         WRITE(*,*) 'Error initializing algorithmic parameters.'
         WRITE(*,*) 'IERR = ',IERR
         WRITE(*,*)
         WRITE(*,*) 
     .        'You might want to check IPOPT.DAT (if you set QCNR>0)'
         WRITE(*,*)
         STOP
      ENDIF

      CALL IPOPT(N, X, M, NLB, ILB, BNDS_L, NUB, IUB, BNDS_U, V_L, V_U,
     .     LAM, LRW, RW, LIW, IW, ITER, IERR)
C
C     Retrieve final value of objective function and constraints
C
      CALL COFG( CUTE_N, CUTE_M, X, FinalF, CUTE_M, LAM )
      if( CUTE_M.gt.0 ) then
         cnrm = abs(LAM(IDAMAX( CUTE_M, LAM )))
      else
         cnrm = 0.d0
      endif
C
C     Display CUTEr statistics
C
      CALL CREPRT( CALLS, CPU )
      WRITE ( IOUT, 2000 ) PNAME, CUTE_N, CUTE_M, CALLS(1), CALLS(2),
     .     CALLS(3), CALLS(4), CALLS(5), CALLS(6), CALLS(7),
     .     IERR, FinalF, cnrm, CPU(1), CPU(2)
c
 2000 FORMAT( /, 24('*'), ' CUTEr statistics ', 24('*') //
     *     ,' Code used               :  IPOPT',    /
     *     ,' Problem                 :  ', A10,    /
     *     ,' # variables             =      ', I10 /
     *     ,' # constraints           =      ', I10 /
     *     ,' # objective functions   =      ', E15.7 /
     *     ,' # objective gradients   =      ', E15.7 / 
     *     ,' # objective Hessians    =      ', E15.7 / 
     *     ,' # Hessian-vector prdct  =      ', E15.7 / 
     *     ,' # constraints functions =      ', E15.7 /
     *     ,' # constraints gradients =      ', E15.7 /
     *     ,' # constraints Hessians  =      ', E15.7 / 
     *     ,' Exit code               =      ', I10 /
     *     ,' Final f                 = ', E23.15 /
     *     ,' Final ||c||_inf         = ', E23.15 /
     *     ,' Set up time             =      ', 0P, F10.2, ' seconds' /
     *     ,' Solve time              =      ', 0P, F10.2, ' seconds' //
     *     ,66('*') / )

 9999 CONTINUE
      END



