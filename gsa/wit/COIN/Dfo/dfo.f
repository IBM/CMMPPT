C Copyright (C) 2000, International Business Machines
C Corporation and others.  All Rights Reserved.
      SUBROUTINE DFO( N    , NX    , X     , LDX   , FX, IFINIV, 
     +                NCLIN, NCNLN , LB    , UB    , A , LDA   , 
     +                LDCJ , XNAMES, PNAME , GNAMES, IT, NF    ,   
     +                INFO , WRK   , LWRK  , IWRK  , LIWRK )

C
C  ****************************************************************
C  THIS SUBROUTINE MINIMIZES A NONLINEAR OBJECTIVE FUNCTION
C  SUBJECT TO LINEAR AND (POSSIBLY) NONLINEAR CONSTRAINTS
C  AND SIMPLE BOUNDS, WITHOUT USING DERIVATIVES OF 
C  THE OBJECTIVE FUNCTION.
C
C  THE PROBLEM IS CONSIDERED TO BE OF THE FORM
C                        
C                           MIN F( X )
C         
C        S.T.
C                               / X  \
C                        LB <= ( AX   ) <= UB
C                               \C(X)/
C
C  THIS PROGRAM IS BASED ON USING QUADRATIC INTERPOLATION OF THE
C  OBJECTIVE FUNCTION IN COMBINATION WITH TRUST REGION FRAMEWORK
C
C
C  PARAMETERS
C  ==========
C
C  (INPUT) 
C
C    X         : ARRAY OF NX  INITIAL POINTS, GIVEN BY THE USER
C                'X' HAS TO CONTAIN AT LEAST ONE STARTING POINT
C                IF NO FUNCTION VALUES ARE PROVIDED FOR THE POINTS
C                IN 'X', THEN ALL BUT THE FIRST POINT ARE IGNORED.
C
C    LDX       : LEADING DIMENSION OF ARRAY 'X' (LDX>= N)
C
C    FX        : ARRAY OF FUNCTION VALUES AT THE INITIAL POINTS
C                OF LENGTH AT LEAST 1 ( MAY BE DUMMY IF NO VALUES  
C                ARE PROVIDED )
C
C    NX        : NUMBER OF INITIAL POINTS FOR WHICH THE VALUE IS
C                PROVIDED. IF NO SUCH POINTS PROVIDED, THEN NX=1
C
C    IFINIV    : LOGICAL VARIABLE, .TRUE. IS INITIAL VALUES FOR
C                NX INITIAL POINTS ARE PROVIDED, .FALSE. OTHERWISE
C
C    N         : PROBLEM DIMENSION
C
C    NCLIN     : NUMBER F LINEAR CONSTRAINTS
C
C    NCNLN     : NUMBER OF NONLINEAR CONSTRAINTS
C
C    LB        : ARRAY OF LOWER BOUNDS OF LENGTH >= (N+NCLIN+NCNLN)
C
C    UB        : ARRAY OF UPPER BOUNDS OF LENGTH >= (N+NCLIN+NCNLN)
C
C    A         : MATRIX OF LINEAR CONSTRAINTS,( DIMENSIONS: LDA X N)
C
C    LDA       : LEADING DIMENSION OF MATRIX A, HAS TO BE >= MAX(1, NCLIN)
C
C    LDCJ      : LEADING DIMENSION OF THE JACOBIAN OF NONLINEAR
C                CONSTRAINTS, AS COMPUTED BY THE USER ROUTINE 'FUNCON'
C    XNAMES    : ARRAY OF STRINGS OF CHARACTERS, CONTAINING
C                NAMES OF THE VARIABLES
C
C    PNAME     : STRING OF CHARACTERS CONTAINING THE NAME OF THE PROBLEMS
C
C    GNAMES    : ARRAY OF STRINGS OF CHARACTERS, CONTAINING NAMES
C                OF CONSTRAINTS
C
C    WRK       : REAL WORKSPACE ARRAY OF SIZE LWRK
C                THE REQUIRED LENGTH OF LWRK IS APPROXIMATED FROM ABOVE  BY:
C                (2N+24)(N+NCLIN+NCNLN)+(2NCNLN+6)*N+(MAXNF+NX)(N+2)+
C                [(N+1)**2 (N+2)(N+14)]/4    
C 
C 
C    IWRK      : INTEGER WORKSPACE ARRAY OF SIZE LIWRK
C                THE REQUIRED LENGTH OF INTEGER SPACE IS
C                4((N+NCLIN+NCNLN) + (MAXNF+NX) + (N+1)(N+2)/2
C
C    (OUTPUT)
C
C     X        : THE OPTIMAL (OR BEST FOUND)  POINT 
C
C     FX       : THE VALUE OF OBJECTIVE FUNCTION AT X
C
C     INFO     : THE EXIT INFORMATION:
C                0     SUCCESSFUL MINIMIZATION
C                1     TOO MANY FUNCTION EVALUATIONS
C                2     TOO MANY ITERATIONS
C               -1     ILLEGAL VALUE OF N, NCLIN OR NCNLN
C               -2     ILLEGAL VALUE OF LDA OR LDX
C               -3     REAL WORKSPACE IS TOO SMALL
C               -4     INTEGER WORKSPACE IS TOO SMALL
C               -5     INCONSISTENT BOUNDS
C               -6     FUNCTION VALUE CANNOT BE COMPUTED
C                      AT INITIAL POINT
C               -7     MINIMIZATION OVER TRUST REGION FAILED
C               -8     MAXNF IS TOO SMALL, LESS THAN 2
C               -9     CANNOT BUILD A 2 POINT MODEL
C     IT       : THE NUMBER OF ITERATIONS
C
C     NF       : THE NUMBER OF FUNCTION EVALUATIONS
C
C  ********************************************************************
C
C     Programming:  K. Scheinberg, Spring 1998.
C
C  ********************************************************************
C



C
C  SUBROUTINE PARAMETERS
C
      INTEGER          N , NX, LDX , NCLIN, NCNLN, LDA, LDCJ,
     +                 IT, NF, INFO, LWRK , LIWRK, IWRK(LIWRK)

      DOUBLE PRECISION X(LDX*NX), FX(NX), LB(N+NCLIN+NCNLN),
     +                 UB(N+NCLIN+NCNLN), A(LDA*N), WRK(LWRK)         

      LOGICAL          IFINIV

      CHARACTER*10     XNAMES(N), PNAME, GNAMES(NCLIN+NCNLN)
C
C  COMMON VARIABLES
C

C
C  PROCESS CONTROL PARAMETERS
C
      INTEGER          IOUT  , IPRINT
      DOUBLE PRECISION MCHEPS, CNSTOL
      COMMON / DFOCM / IOUT  , IPRINT, MCHEPS, CNSTOL

C
C  INTERPOLATION CONTROL PARAMETERS
C      
      INTEGER          NPMIN, LAYER, EFFORT
      COMMON / OPTI /  NPMIN, LAYER, EFFORT

C
C  EXTERNAL ROUTINES
C

      DOUBLE PRECISION DDOT

      EXTERNAL         DDOT
C
C  LOCAL VARIABLES
C


      INTEGER          MAXNF , MAXIT , VARNT , IOSPEC, ISCAL,
     +                 IP(6) , BADBND, SCALE , I     , J    ,
     +                 ICURW , LENW  , LSCAL
     
      DOUBLE PRECISION DELMIN  , LOWBND, ANOISE, RNOISE, PIVTHR, 
     +                 ADDTHR  , XCHTHR, RP(7) , DELTA , DELMAX,
     +                 NOISE(2), AVGSC , VAL
      CHARACTER*64     SPECS
  
      LOGICAL          IFERR 
C
C  PARAMETERS
C   
      DOUBLE PRECISION ONE, ZERO
      PARAMETER      ( ONE = 1.0D0, ZERO = 0.0D0 )
      INTEGER          INSPEC
      PARAMETER      ( INSPEC = 45 )

C
C  SUBROUTINES CALLED: 
C
C  APPLICATIONS: DFOSLV, FUN, SCL, UNSCL, FUNCON
C  BLAS        : DDOT
C

C  ----------------------------------
C  CHECK IF THE INITIAL DATA IS VALID
C  ----------------------------------
 
     

C
C  CHECK IF THE DIMENSIONS ARE POSITIVE
C
      IF ( N .LE. 0  ) THEN
         IF (IPRINT .GE. 0 ) WRITE( IOUT, 1000 ) N
         INFO = -1
         RETURN
      ENDIF

      IF ( NCLIN .LT. 0  ) THEN
         IF (IPRINT .GE. 0 ) WRITE( IOUT, 1010 ) NCLIN
         INFO = -1
         RETURN
      ENDIF

      IF (  NCNLN .LT. 0 ) THEN
         IF (IPRINT .GE. 0 ) WRITE( IOUT, 1020 ) NCNLN
         INFO = -1
         RETURN
      ENDIF

C
C  CHECK IF THE  BOUNDS ARE CONSISTENT
C
      BADBND=0
      DO 10 I=1,N+NCLIN+NCNLN
        IF (LB( I ) .GT. UB( I )+MCHEPS) BADBND=1
 10   CONTINUE
      IF (BADBND.EQ.1) THEN
        IF (IPRINT .GE. 0 ) WRITE(IOUT, 1030)
        INFO=-5
        RETURN
      ENDIF
    


C
C  CHECK IF DIMENSION OF LINEAR CONSTRAINT MATRIX IS SATISFACTORY
C

      IF ( LDA .LT. MAX(NCLIN,1) ) THEN 
         IF (IPRINT .GE. 0 ) WRITE(IOUT, 1060) MAX(NCLIN,1)
         INFO = -2              
         RETURN
      ENDIF

C
C  CHECK IF DIMENSION OF NONLINEAR CONSTRAINT JACOBIAN IS SATISFACTORY
C

      IF ( LDCJ .LT. MAX(NCNLN,1) ) THEN 
         IF (IPRINT .GE. 0 ) WRITE(IOUT, 1065) MAX(NCNLN,1)
         INFO = -2              
         RETURN
      ENDIF

C
C  CHECK IF LEADING DIMENSION OF INITIAL SET 'X' IS SATISFACTORY
C

      IF ( LDX .LT. N ) THEN 
         IF (IPRINT .GE. 0 ) WRITE(IOUT, 1080) N
         INFO = -2              
         RETURN
      ENDIF

C
C  CHECK IF OTHER DIMENSION OF INITIAL SET 'X' IS POSITIVE
C

      IF ( NX .LE. 0 ) THEN 
         IF (IPRINT .GE. 0 ) WRITE(IOUT, 1090) 
         INFO = -2              
         RETURN
      ENDIF

C
C  READ THE SPECIFICATION FILE
C      

      SPECS = 'DFO.SPC'
      OPEN( INSPEC , FILE = SPECS, FORM = 'FORMATTED',
     +      STATUS = 'OLD', IOSTAT = IOSPEC )
      REWIND INSPEC
      READ ( INSPEC, 3000 ) DELMIN, DELMAX, DELTA , LOWBND, MAXNF , 
     +                      MAXIT , ANOISE, RNOISE, PIVTHR, 
     +                      ADDTHR, XCHTHR, CNSTOL, NPMIN ,
     +                      LAYER , EFFORT, VARNT , SCALE ,
     +                      IPRINT, IOUT

      CLOSE ( INSPEC )
C
C  STORE THE MINIMIZATION PARAMETERS, OBTAINED FROM SPECIFICATION FILE
C

      MCHEPS     = 1.5D-16
      IP( 1 )    = MAXIT
      IP( 2 )    = MAXNF
      IP( 3 )    = NPMIN
      IP( 4 )    = LAYER
      IP( 5 )    = EFFORT
      IP( 6 )    = VARNT
      RP( 1 )    = DELTA
      RP( 2 )    = DELMAX
      RP( 3 )    = DELMIN
      RP( 4 )    = PIVTHR
      RP( 5 )    = ADDTHR
      RP( 6 )    = XCHTHR
      RP( 7 )    = LOWBND
      NOISE( 1 ) = ANOISE
      NOISE( 2 ) = RNOISE

C
C  CHECK IF THE INITIAL NUMBER OF INTERPOLATION POINTS
C  IS NOT LARGER THAN MAXIMUM NUMBER OF FUNCTION EVALUATIONS
C

      IF ( MAXNF .LT. 2 ) THEN
         IF (IPRINT .GE. 0 ) WRITE( IOUT, 1050 ) 2
         INFO = -8
         RETURN
      ENDIF

C
C  PRINT OUT SOME SPECIFICATIONS INFO
C


      IF ( IPRINT .GE. 2 ) THEN
        IF ( VARNT .EQ.1 ) THEN
          WRITE ( IOUT, 4000 )
        ELSE
          WRITE ( IOUT, 4020 )
        ENDIF
      ENDIF
C
C  IF THERE ARE NO NONLINEAR CONSTRAINTS SET THE CONSTRAINT 
C  TOLERANCE, USED BY NPSOL, TO A DEFAULT VALUE
C
      IF ( NCNLN .EQ. 0 ) CNSTOL = 1.0D-8


C
C  SET THE POINTERS TO SPACE FOR SCALING COEFFICIENTS
C
      ISCAL = 1
      LSCAL = ISCAL - 1
C
C  CHECK IF REAL SPACE IS SUFFICIENT
C
      ICURW = ISCAL + N
      LENW  = LWRK - ICURW + 1
      IF ( LENW .LT. 0 ) THEN
        WRITE( IOUT, 1070 ) LWRK
        INFO = -3
        RETURN
      ENDIF
C
C INITIALIZE NF TO ZERO.
C IF NO INITIAL POINT WITH FUNCTION VALUE IS PROVIDED, COMPUTE
C THE FUNCTION VALUE AT THE FIRST POINT AND SET NF = 1
C
      NF = 0
      IF ( .NOT. IFINIV ) THEN
        CALL FUN( N, X, FX, IFERR )
        IF ( IFERR ) THEN
          IF (IPRINT .GE. 0 ) WRITE(IOUT, 1040)
          INFO = -6
          RETURN
        ENDIF
        NF = 1
        NX = 1
      ENDIF

C       
C  CHECK IF SCALING AND NONLINEAR CONSTRAINTS ARE NOT USED AT
C  THE SAME TIME (NOT SUPPORTED BY CURRENT VERSION)
C
      IF ( SCALE .NE. 0 .AND. NCNLN .GT. 0 ) THEN
         IF (IPRINT .GE. 0 ) WRITE(IOUT, 5000) 
         SCALE = 0
      ENDIF

C       
C  SCALE THE PROBLEM IF REQUIRED
C

      IF ( SCALE .NE. 0 ) THEN
        AVGSC=ONE
        DO 20 I=1, N
          IF ( SCALE .EQ. 1 ) THEN
            WRK( LSCAL + I ) = ONE + ABS(X(I))
            AVGSC   = AVGSC * WRK( LSCAL + I )
          ELSE 
            WRK( LSCAL + I ) = UB(I) - LB(I)
            IF ( WRK( LSCAL + I). LT. CNSTOL .OR. WRK( LSCAL + I ) 
     +                                       .GT. 1.0D+20 ) THEN
              IF (IPRINT .GE. 0 ) WRITE( IOUT, 5200 )  I
              WRK( LSCAL + I ) = ONE
              AVGSC   = AVGSC * WRK( LSCAL + I )
            ENDIF
          ENDIF
 20     CONTINUE
      ENDIF
C
C  SCALE THE POINT AND THE BOUNDS
C
      IF ( SCALE .NE. 0 ) THEN
        DO 25 I=1,NX
          CALL SCL( N, X((I-1)*N+1), WRK( ISCAL ) )
 25     CONTINUE  
        CALL SCL( N, LB, WRK( ISCAL ) )
        CALL SCL( N, UB,  WRK( ISCAL ) )
C
C  SHOULD SCALE THE TRUST REGION RADIUS AS WELL. RIGHT NOW ONLY SCALE
C  THE LOWER BOUND ON THE TRUST REGIONS RADIUS
C

C        RP( 1 ) = RP( 1 )*(N/AVGSC)
C        RP( 2 ) = RP( 2 )*(N/AVGSC)
        RP( 3 ) = RP( 3 )/(EXP(LOG(AVGSC)/N))
        DO 40 I=1,N
          DO 30 J=1,NCLIN
            A(LDA*(I-1)+J)=A(LDA*(I-1)+J)*WRK( LSCAL + I )
 30       CONTINUE  
 40     CONTINUE  
        IF ( IPRINT .GE. 2 ) THEN
          WRITE( IOUT, 8000 )
          DO 45 I = 1, N
            WRITE( IOUT, 8010 ) WRK( LSCAL + I )
 45       CONTINUE
        ENDIF  
      ENDIF

C
C  CALL THE DFO SOLVER
C  

      CALL DFOSLV( N     , NX        , NCLIN, NCNLN, X    , LDX  ,   
     +             FX    , LB        , UB   , A    , LDA  , LDCJ , 
     +             SCALE , WRK(ISCAL), INFO , IT   , NF   , NOISE,   
     +             IP, RP, WRK(ICURW), LENW , IWRK , LIWRK )



C
C  UNSCALE THE PROBLEM IF SCALED
C

      IF (SCALE .NE. 0) THEN
        CALL UNSCL( N, X, WRK(ISCAL) )
        CALL UNSCL( N, LB, WRK(ISCAL) )
        CALL UNSCL( N, UB, WRK(ISCAL) )
        DO 60 I=1,N
          DO 50 J=1,NCLIN
            A(LDA*(J-1)+I)=A(LDA*(J-1)+I)/WRK(LSCAL + I)
 50       CONTINUE  
 60     CONTINUE  
      ENDIF

C
C  IF DEMANDED, PRINT OUT THE FINAL OUTPUT
C
      IF ( IPRINT .GE. 0 ) THEN
        WRITE ( IOUT, 6000 ) 
        DO 70 I = 1, N
          WRITE( IOUT, 6200 ) XNAMES(I), X( I )
 70     CONTINUE
        IF ( NCLIN .GT. 0 ) THEN
          WRITE ( IOUT, 6100 ) 
          DO 80 I = 1, NCLIN
            VAL=DDOT(N, A(I), LDA, X, 1)
            WRITE( IOUT, 6200 ) GNAMES(I), VAL
 80       CONTINUE
        ENDIF
        IF ( NCNLN .GT. 0 ) THEN
          WRITE ( IOUT, 6300 ) 
          DO 85 I = 1, NCNLN
            IWRK( I ) = 1
 85       CONTINUE  
          CALL FUNCON(0, NCNLN  , N, LDCJ  , IWRK   ,
     *                X, WRK(ICURW), WRK(ICURW+LDCJ), 1)
          DO 90 I = 1, NCNLN
            WRITE( IOUT, 6200 ) GNAMES(NCLIN+I), WRK( ICURW+I-1 )
 90       CONTINUE
        ENDIF
        WRITE( IOUT, 6400 )  PNAME, N, NCLIN+NCNLN, NF, IT, FX(1), INFO
C       WRITE( IOUT, 6600 )  PNAME, N, NCLIN+NCNLN, NF, IT, FX(1), INFO 
      ENDIF

      RETURN


C
C  NON-EXECUTABLE STATEMENTS
C

 

 1000 FORMAT(/' DFO: *** ERROR: NUMBER OF VARIABLES',/  
     +        '                 HAS ILLEGAL VALUE:', I6,/ )
 1010 FORMAT(/' DFO: *** ERROR: NUMBER OF LINEAR CONSTRAINTS',/  
     +        '                 HAS ILLEGAL VALUE:', I6,/)
 1020 FORMAT(/' DFO: *** ERROR: NUMBER OF NONLINEAR CONSTRAINTS',/  
     +        '                 HAS ILLEGAL VALUE:', I6,/ )
 1030 FORMAT(/' DFO: *** ERROR: THE UPPER AND LOWER BOUNDS ARE',/, 
     +        '                 INCONSISTENT', /)
 1040 FORMAT(/' DFO: *** ERROR: FUNCTION VALUE CANNOT BE OBTAINED',/,
     +        '                 FOR INITIAL POINT!')
 1050 FORMAT(/' DFO: *** ERROR: MAXNF IS TOO SMALL!' /
     +        '                 IT SHOULD BE AT LEAST ', I5 )
 1060 FORMAT(/' DFO: *** ERROR: THE LEADING DIMENSION OF ',/, 
     +        '                 LINEAR CONSTRAINTS SHOULD BE AT LEAST',
     +                          I6,/)
 1065 FORMAT(/' DFO: *** ERROR: THE LEADING DIMENSION OF ',/, 
     +        '                 NONLINEAR JACOBIAN SHOULD BE AT LEAST',
     +                          I6,/)
 1070 FORMAT('  DFO: *** ERROR: LWRK IS TOO SMALL EVEN TO START!' /
     +        '                 IT IS: ',I10 )
 1080 FORMAT(/' DFO: *** ERROR: THE LEADING DIMENSION OF ',/, 
     +        '                 INITIAL SET "X" SHOULD BE AT LEAST',
     +                          I6,/)
 1090 FORMAT(/' DFO: *** ERROR: THE NUMBER  OF INITIAL ',/, 
     +        '                 POINTS "X" SHOULD BE POSITIVE')
 3000 FORMAT( 4( D12.4, /), 2( I12, /), 6( D12.4, /), 6(I12, /), I12 )
 5000 FORMAT(/' DFO: *** WARNING: SCALING IS NOT SUPPORTED ',/, 
     +        '          FOR PROBLEMS WITH NONLINEAR CONSTRAINTS',/,
     +        '          NO SCALING WILL BE PERFORMED',/)
 5200 FORMAT( ' DFO: *** WARNING:',I4,'-th variable cannot be scaled,'
     +                   ,/, 
     +        '          scaling coefficient is too small or',/,
     +        '          too large',/)
 4000 FORMAT( ' Minimum frobenius norm models will be used', / )
 4020 FORMAT( ' Models based on incomplete Newton Polynomial basis',/
     +        ' will be used',/ )

 6000 FORMAT( 'OPTIMAL POINT:', / ) 
 6100 FORMAT( 'LINEAR CONSTRAINTS:', / ) 
 6200 FORMAT( A10, D12.6, / )
 6300 FORMAT( 'NONLINEAR CONSTRAINTS:', / ) 
 6400 FORMAT( /, 24('*'), 'DFO: FINAL OUTPUT ', 24('*') //
     +    ' problem                 =      ', A10,   /,
     +    ' # variables             =      ', I10,   /,
     +    ' # constraints           =      ', I10,   /,
     +    ' # objective functions   =      ', I10,   /,
     +    ' # iterations            =      ', I10,   /,
     +    ' Final f                 =      ', D15.7, /,
     +    ' Exit code               =      ', I10,   /,
     +     65('*') / )
 6600 FORMAT( A8, I10, 1X, I10, 1X, I10, 1X, I10, 1X,  D14.7, 1X, I10 )
 8000 FORMAT( 'SCALING COEFFICIENTS:',/)
 8010 FORMAT( D14.7, 1X )


      END
*


