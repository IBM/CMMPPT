C Copyright (C) 2000, International Business Machines
C Corporation and others.  All Rights Reserved.
       SUBROUTINE NBUILD(POLY  , POINTS, VALUE, PNTINT, VALINT, SP2IN  ,  
     +                   IN2SP , NIND  , N    , BASE  , DISTP , DELTA, 
     +                   PIVTHR, PIVVAL, NEQCON)                


C
C  **********************************************************************
C  THIS FUNCTION BUILDS A SET OF NEWTON FUNDAMENTAL POLYNOMIALS (NFPs) (AS
C  COMPLETE AS POSSIBLE WITH HIGHEST DEGREE 2) FOR THE SUPPLIED Q POINTS.
C
C  THE NFPs ARE BUILT BY BLOCKS IN THE FOLLOWING WAY:
C  THE ZERO-TH (CONSTANT) BLOCK CONSIST OF ONE ELEMENT. THIS ELEMENT IS
C  A POLYNOMIAL OF DEGREE ZERO; I.E., A CONSTANT (WE CHOOSE THE CONSTANT
C  TO BE 1). WE CHOOSE THE BASE TO CORRESPOND TO THIS  FIRST NFP.
C
C  THE FIRST BLOCK (ELEMENTS OF DEGREE UP TO ONE) CONSISTS OF 'N' NFPs.
C  TO COMPUTE AN K-TH ELEMENT IN THIS BLOCK WE CHOOSE A POINT FROM
C  THE GIVEN ARRAY OF POINTS, WHICH WAS NOT CHOSEN BEFORE, WE PUT THE
C  POINT IN THE ARRAY 'PNTINT' OF "CHOSEN" POINTS. THEN WE CONSTRUCT THE
C  K-TH POLYNOMIAL SO THAT ITS VALUE AT THAT K-TH POINT IS 1 AND
C  ITS VALUE AT ALL PREVIOUSLY CHOSEN POINTS IS 0. WE ALSO
C  UPDATE THE POLYNOMIAL IN THE SAME BLOCK SO THAT THEIR VALUE IS 0
C  AT THE K-TH POINT. WE STORE THE N+1 COEFFICIENTS OF THE K-TH NFP
C  IN ARRAY POLY (AFTER THE PREVIOUSLY COMPUTED POLYNOMIALS)
C
C  FOR THE QUADRATIC BLOCK THE PROCEDURE REPEATS IN A SIMILAR WAY, EXCEPT
C  THE NUMBER OF POLYNOMIALS IN THE QUADRATIC BLOCK CAN GO UP TO
C  N(N+1)/2 AND THEIR LENGTH IS ALWAYS (N+1)(N+2)/2.
C
C  WE STOP WHEN WE EITHER RUN OUT OF POINTS, WHICH SATISFY OUR
C  REQUIREMENTS (SEE BELOW) OR COMPLETE THE BASIS;
C  I.E., BUILD (N+1)(N+2)/2 POLYNOMIALS.
C
C  WE CHOOSE THE POINT FROM THE POOL BY TWO CRITERIA - PROXIMITY
C  TO THE BASE AND "INDEPENDENCE" (GEOMETRY, WELL-POISEDNESS). 
C  WE REQUIRE POINTS TO BE SUFFICIENTLY INDEPENDENT (WELL-POISED)
C  AND TO BE WITHIN CERTAIN DISTANCE FROM THE BASE.
C  WHEN WE HAVE A FEW ELIGIBLE POINTS WE CHOSE AMONG POINT CLOSER TO THE
C  BASE A POINT WITH THE BEST 'INDEPENDENCE' PROPERTY.
C
C  PARAMETERS
C
C  POINTS (INPUT)  ARRAY (N,Q) WITH THE POOL OF POTENTIAL INTERPOLATION POINTS
C
C  PIVTHR (INPUT)  PIVOT THRESHOLD VALUE
C
C  VALUE  (INPUT)  ARRAY (Q) OF VALUES AT THE POINTS
C
C  BASE   (INPUT)  INDEX OF THE BASE POINT
C
C  DISTP  (INPUT)  ARRAY (LVALUE) OF DISTANCES OF POINTS TO THE BASE POINTS
C
C  N      (INPUT)  PROBLEM DIMENSION
C
C  DELTA  (INPUT)  TRUST REGION RADIUS
C
C  POLY   (OUTPUT) LONG ARRAY CONTAINING ALL NEWTON FUNDAMENTAL  POLYNOMIALS
C
C  NIND   (OUTPUT) NUMBER OF POINTS INCLUDED IN THE INTERPOLATION
C
C  PNTINT (OUTPUT) ARRAY (N*NIND) OF POINTS INCLUDED IN  THE  INTERPOLATION
C                  IN THE ORDER OF INCLUSION
C  VALINT (OUTPUT) ARRAY (NIND) OF VALUES OF POINTS IN PNTINT
C
C  PIVVAL (OUTPUT) ARRAY (NIND) OF VALUES OF THE PIVOTS PRODUCED IN NBUILD
C
C  SP2IN  (OUTPUT) ARRAY OF 0/1 WHICH INDICATES WHICH POINTS IN "POINTS"
C                  ARE INCLUDED IN THE INTERPOLATION
C  IN2SP  (OUTPUT) ARRAY (NIND) OF INDICES WHICH FOR EVERY POINT IN "PNTINT"
C                  INDICATES ITS POSITION IN "POINTS"
C  NEQCON (INPUT)  NUMBER OF LINEARLY INDEPENDENT EQUALITY CONSTRAINTS
C
C  ****************************************************************************
C

       DOUBLE PRECISION  POLY(LPOLY)   , POINTS(LPNTS) , VALUE(LVALUE),
     +                   PIVTHR        , DISTP(LVALUE) , DELTA        ,
     +                   PNTINT(LPTINT), VALINT(LVLINT), PIVVAL(LVLINT)
 
                 
       INTEGER           NIND, N, BASE , SP2IN(LVALUE) , IN2SP(LVLINT),
     +                   NEQCON        
       
     
C
C  COMMON VARIABLES
C   
        
       INTEGER           LPOLY, LPNTS, LVALUE, LPTINT, LVLINT
       COMMON /RPART/    LPOLY, LPNTS, LVALUE, LPTINT, LVLINT
       
       INTEGER           IOUT  , IPRINT
       DOUBLE PRECISION  MCHEPS, CNSTOL
       COMMON / DFOCM /  IOUT  , IPRINT, MCHEPS, CNSTOL

       INTEGER           NPMIN, LAYER, EFFORT
       COMMON / OPTI  /  NPMIN, LAYER, EFFORT


C
C  LOCAL VARIABLES
C

       DOUBLE PRECISION VAL, VMAX, DELU, DELL

       INTEGER          Q, QP1 , DD   , DDM1 , NDD   , NP1   , NP2 , 
     +                  I, JBEG, J    , JMAX , NLAYER, LV    , JEND,
     +                  K, KI  , LL   , KK   , PBASE , IPBASE, 
     +                  L, IPOL, IPEND, IBEG , IEND


       DOUBLE PRECISION ONE, ZERO, THOUS, HUGE
       PARAMETER       (ONE=1.0D0, ZERO=0.0D0, THOUS=10.0D3,
     +                  HUGE=1.0D20)
       INTRINSIC        ABS
C
C     SUBROUTINES AND FUNCTIONS CALLED:
C
C       APPLICATION:       NEXTNP, EVALNP,  SWAPNP, IZRVEC
C       FORTRAN SUPPLIED:  ABS
C       BLAS:              DCOPY 
C


C
C COMPUTE VALUES ASSOCIATED WITH N: DD - NUMBER OF TERMS IN A COMPLETE 
C QUADRATIC POLYNOMIAL, ALSO NUMBER OF NFP IN A COMPLETE QUADRATIC BASIS.
C NDD - NUMBER OF QUADRATIC TERMS IN A QUADRATIC POLYNOMIAL, ALSO NUMBER
C OF NFP IN A COMPLETE QUADRATIC BLOCK OF THE BASIS.
C

       NP1  = N + 1
       NP2  = N + 2
       DD   = NP1*NP2/2
       DDM1 = DD-1
       NDD  = DD-NP1
       QP1  = LVALUE+1
       Q    = LVALUE


       CALL IZRVEC(SP2IN, LVALUE)

C
C  INITIALIZE THE POLY , THE NEWTON POLYNOMIALS WITH THE MONOMIALS
C

C
C  CONSTANT BLOCK
C
       POLY(1) = 1

C
C  LINEAR BLOCK
C
       DO 20 I = 1,N
         JBEG  = (I-1)*NP1+2
         JEND  = JBEG + N
         DO 10 J = JBEG, JEND
           POLY(J) = ZERO
 10      CONTINUE
         POLY(JBEG+I) = ONE
 20    CONTINUE

C
C  QUADRATIC BLOCK
C
       DO 50 I = 1,NDD
         JBEG = (I-1)*DD+N*NP1+2
         JEND = JBEG+N
         DO 30 J = JBEG, JEND
           POLY(J) = ZERO
 30      CONTINUE
         JBEG = JEND+1
         JEND = I*DD+N*NP1+1
         DO 40 J = JBEG, JEND
           POLY(J) = ZERO
 40      CONTINUE
         POLY(I+JBEG-1) = ONE
 50    CONTINUE


C
C  *************************
C  START THE MAJOR ITERATION
C  *************************
C

C
C PUT THE BASE POINT TO BE THE FIRST IN THE NEW INTERPOLATION SET,
C UPDATE ALL RELATED VALUES APPROPRIATELY
C

       IF (BASE.NE.1) THEN
         PBASE     = IN2SP(BASE)
         IPBASE    = (PBASE-1)*N+1
         CALL DCOPY(N, POINTS(IPBASE), 1, PNTINT, 1)
         VALINT(1)     = VALUE(PBASE)
         SP2IN(PBASE)= 1
         IN2SP(1)  = PBASE
         BASE      = 1
       ELSE
         SP2IN(IN2SP(1))=1
       ENDIF

       PIVVAL(1) = ONE

       CALL IZRVEC(IN2SP(2), LVLINT-1)


       DO 180 I=2,DD
C
C  IF THE WE ARE IN THE LINEAR BLOCK BUT THE NUMBER OF LINEARLY
C  INDEPENDENT POINTS HAS REACHED IT'S MAXIMUM THEN COMPLETE
C  THE THE LINEAR BLOCK TILL THE END WITH DUMMY ELEMENTS
C  (SEE COMMENTS IN SUBROUTINE COMPL)
C
         IF ( I .GT. N + 1 - NEQCON .AND. I .LE. N + 1) THEN
            IBEG = 2 + (N+1)*(I-2) 
            IEND = 1 + (N+1)*(I-1)
            DO 53 J=IBEG, IEND
              POLY(J)=ZERO
 53         CONTINUE  
            VALINT(I)=HUGE
            PIVVAL(I)=HUGE
            NIND = N+1    
            GOTO 180  
         ENDIF     
C        
C  FIND THE POINT THAT GIVES THE LARGEST PIVOT (DOING PARTIAL PIVOTING)
C  WITHIN DIFFERENT LAYERS OF PROXIMITY TO  THE BASE:
C  FIRST TRY ALL POINTS WITHIN DELTA RADIUS, IF THERE ARE NONE
C  WITH SUFFICIENTLY LARGE PIVOT VALUE, SEARCH WITHIN 2*DELTA, ETC.
C  THE LARGEST NEIGHBORHOOD OF SEARCH HAS RADIUS NLAYER*DELTA
C  

         VMAX =ZERO
         JMAX = I
C
C  IF ALL  THE POINTS ARE ALREADY USED, THEN QUIT
C
         IF ( I .GT. Q ) GO TO 85

C
C SET THE NUMBER OF LAYERS. BY SETTING IT TO A LARGER VALUE WE ALLOW
C TO INCLUDE (IN THE INTERPOLATION) POINTS THAT ARE FURTHER AWAY FROM THE
C BASE. THUS WE CAN ACHIEVE A MORE COMPLETE MODEL BUT ON A MORE RELAXED
C REGION
C
C
C  IF WE WANT TO BUILD AT LEAST 'NPMIN' ELEMENTS OF THE BASIS (NPMIN>=2)
C  THEN UNTIL WE DO SO WE ALLOW TO LOOK FOR POINTS IN A WIDER NEIGHBORHOOD
C  DEFINE THE WIDER NEIGHBORHOOD AS THE DISTANCE TO THE FURTHEST POINT
C
         VAL = ZERO
         DO 55 K = 1, Q
           IF ( DISTP(K) .GT. VAL ) VAL = DISTP(K)
 55      CONTINUE  
         IF ( I .LE. NPMIN ) THEN
           NLAYER = VAL/DELTA + 1
         ELSE
           NLAYER = LAYER   
         ENDIF 

C
C  CYCLE FOR DIFFERENT LAYERS OF PROXIMITY
C  
         DO 80 LV=1,NLAYER

           IF ( I .LE. NP1 ) THEN
             IPEND = NP1
           ELSE
             IPEND = DD
           ENDIF
C
C  CYCLE OVER ALL POLYNOMIALS IN THE SAME BLOCK FROM I-TH AND ABOVE
C  AS SOON  AS WE FIND A POLYNOMIAL WHICH GIVES US A ACCEPTABLE PIVOT
C  WE QUIT THE LOOP. (WE DO NOT DO TOTAL PIVOTING HERE, TO DO IT, WE
C  WOULD CYCLE OVER POLYNOMIAL UNTIL WE CHOOSE THE ONE WITH BEST PIVOT)
C

           DO 70 IPOL=I, IPEND  

C
C   UPDATE THE IPOLY-TH POLYNOMIAL SO THAT IT IS ZERO AT PREVIOUS POINTS
C
 
             CALL NEXTNP(IPOL, POLY, PNTINT, I-1 ,N, LPOLY, LPTINT)
C
C  SET THE BOUNDARIES OF CURRENT LAYER, DELU - OUTER, DELL - INNER BOUND
C        
             DELU = LV*DELTA + 100*MCHEPS
             DELL = (LV-1)*DELTA + 100*MCHEPS
             DO 60 J=1,Q
C
C  IF THE J-TH POINT SATISFIES DISTANCE REQUIREMENTS THEN PICK IT AS
C  A CANDIDATE
C
               IF ( DISTP(J) .LE. DELU .AND. DISTP(J) .GT. DELL
     +              .AND. SP2IN(J).EQ.0 ) THEN
C
C  EVALUATE THE I-TH POLYNOMIAL AT THE PICKED POINT
C
                 CALL EVALNP( VAL  , POINTS, J, POLY, IPOL, N, 
     +                        LPNTS, LPOLY )
C
C  IF THE VALUE OF THE PIVOT IS LARGER THAN THE LARGEST PIVOT SO FAR
C  THEN ACCEPT THIS POINT AS THE BEST CANDIDATE
C

                 IF ( ABS(VAL).GT.ABS(VMAX) + 100*MCHEPS ) THEN
                   VMAX=VAL
                   JMAX=J
                 ENDIF

               ENDIF

 60          CONTINUE

C
C  IF WITHIN CURRENT LAYER A SUFFICIENTLY LARGE PIVOT IS FOUND
C  THEN STOP LOOKING FOR MORE POINT AND MOVE ON TO INCLUDING THE
C  CURRENT BEST.
C
             IF (ABS(VMAX).GE.PIVTHR) GO TO 90

C
C  OTHERWISE MOVE TO THE  NEXT LAYER
C

 70        CONTINUE

 80      CONTINUE
C
C  ALL ACCEPTABLE LAYERS ARE SEARCHED  AND STILL
C  CANNOT FIND PIVOT LARGE ENOUGH
C

 85      IF (ABS(VMAX).LT.PIVTHR  ) THEN
           NIND=I-1

C
C  COMPLETE OR INCOMPLETE BASIS IS BUILT, EXIT
C
           RETURN
         ENDIF



C
C   WE GET HERE ONLY WHEN THE CURRENT PIVOT IS FOUND AND IS LARGE ENOUGH
C

 90      J   = JMAX
         VAL = VMAX
         IF ( IPRINT .GE. 3 ) WRITE(IOUT, 8000) J, IPOL, VAL

C
C  PLACE POLYNOMIAL WITH INDEX 'IPOLY' ON THE I-TH PLACE
C  (BY SWAPPING I-TH AND IPOLY-TH POLYNOMIALS)
C  THIS IS DONE BECAUSE OF SOME SPECIFICS OF THE CODE
C  (BY EVENTUALLY MODIFYING THE CODE WE CAN AVOID SUCH SWAPPING )
C
         IF ( IPOL .NE. I )  CALL SWAPNP( N, I, IPOL, POLY, LPOLY )   
C
C ASSOCIATE INTERPOLATION POINT J WITH THE I-TH POLYNOMIAL
C WRITE THE POINT INTO Y AND THE CORRESPONDING VALUE INTO VALINT
C

         CALL DCOPY(N, POINTS((J-1)*N+1), 1, PNTINT((I-1)*N+1), 1)
         VALINT(I) = VALUE(J)
         SP2IN(J)  = 1
         IN2SP(I)  = J
         PIVVAL(I) = VAL/DELU

C
C  NORMALIZE THE I-TH POLYNOMIAL
C
 
         IF (I.EQ.1) THEN
           POLY(I) = ONE 
           LL = 1

         ELSE IF (I.LE.NP1) THEN
           K  = (I-2)*(NP1)+2
           KK = K + N 
           DO 100 J = K,KK
             POLY(J) = POLY(J)/VAL
 100       CONTINUE

         ELSE 
           K  = (I-NP2)*DD + N*NP1 + 2
           KK = K + DD - 1
           DO 110 J = K,KK
             POLY(J) = POLY(J)/VAL
 110       CONTINUE         
         ENDIF    

C
C  UPDATE THE I-TH POLYNOMIAL AGAIN SO THAT IT HAS VALUE 0 AT
C  ALL PREVIOUS POINT. IN EXACT ARITHMETIC THIS IS REDUNDANT,
C  BUT WE ARE DOING IT FOR STABILITY, SINCE IF VAL IS RATHER
C  SMALL THEN AN ERROR IS INTRODUCED (THIS IS SIMILAR TO 
C  A MODIFIED GRAMM-SCHMIDT PROCEDURE) 
C

         CALL NEXTNP(I, POLY, PNTINT, I-1 ,N, LPOLY, LPTINT)

C
C  RENORMALIZE THE I-TH POLYNOMIAL
C

         CALL EVALNP(VAL, PNTINT, I,  POLY, I, N, LPTINT, LPOLY)


         IF (I.EQ.1) THEN
           POLY(I) = ONE 
           LL = 1

         ELSE IF (I.LE.NP1) THEN
           K  = (I-2)*(NP1)+2
           KK = K + N
           DO 120 J = K,KK
             POLY(J) = POLY(J)/VAL
 120       CONTINUE

         ELSE 
           K  = (I-NP2)*DD + N*NP1 + 2
           KK = K + DD - 1
           DO 130 J = K,KK
             POLY(J) = POLY(J)/VAL
 130       CONTINUE         
         ENDIF    



 
C
C  FINALLY, UPDATE ALL THE POLYNOMIALS IN THE SAME BLOCK SO
C  THAT THEY HAVE VALUE 0 AT THE NEW I-TH POINT
C   


         IF (I.EQ.1) THEN
C
C   POLYNOMIAL IS A CONSTANT, DO NOTING
C
           CONTINUE

         ELSE IF (I.LE.NP1) THEN

C 
C  WE ARE IN LINEAR BLOCK, ALL PREVIOUS POLYNOMIALS IN LINEAR BLOCK ARE UPDATED
C
           KI  = (I-2)*(NP1) + 2
           DO 150 L = 2, I-1
             CALL EVALNP(VAL, PNTINT, I, POLY, L, N, LPTINT, LPOLY)
             K  = (L-2)*(NP1) + 2
             DO 140 J = 0,N
               POLY(K+J)  =  POLY(K+J) - POLY(KI+J)*VAL
 140         CONTINUE
 150       CONTINUE
          
         ELSE
               

C
C  WE ARE IN QUADRATIC BLOCK, UPDATE POLYNOMIALS IN QUADRATIC BLOCK
C

           KI = (I-NP2)*DD + N*NP1 + 2
           DO 170 L = NP2,I-1
             CALL EVALNP(VAL, PNTINT, I, POLY, L, N, LPTINT, LPOLY)
             K  =  (L-NP2)*DD + N*NP1 + 2
             DO 160 J = 0,DD-1
               POLY(K+J) = POLY(K+J) - POLY(KI+J)*VAL
 160         CONTINUE
 170       CONTINUE         

                 

  
         ENDIF

 180   CONTINUE

       NIND=DD
       RETURN
 8000  FORMAT(' NBUILD: Pivoting: point: ',I5, ', polynomial: ',I5, 
     +        ', pivot value: ', D14.7 )
       END





      SUBROUTINE SWAPNP( N, I, J, POLY, LPOLY )

C
C  *********************************************************
C  THIS SUBROUTINE SWAPS I-TH NEWTON POLYNOMIAL WITH THE J-TH 
C  NEWTON POLYNOMIAL. BOTH POLYNOMIAL ARE STORED IN ARRAY
C  'POLY'. BOTH POLYNOMIALS SHOULD BE OF THE SAME DEGREE.
C  *********************************************************
C

C
C  SUBROUTINE PARAMETERS
C

      INTEGER           N, I, J, LPOLY
      DOUBLE PRECISION  POLY(LPOLY)
C
C  GLOBAL VARIABLES
C
      INTEGER           IOUT  , IPRINT
      DOUBLE PRECISION  MCHEPS, CNSTOL
      COMMON / DFOCM /  IOUT  , IPRINT, MCHEPS, CNSTOL
C
C  LOCAL VARIABLES
C
      INTEGER           NP1, NP2, DD, KEND, K, KI, KJ 
      DOUBLE PRECISION  VAL

C
C  PARAMETERS RELATED TO LENGTH AND NUMBER OF  POLYNOMIALS
C

      NP1 = N + 1
      NP2 = N + 2
      DD  = NP1*NP2/2
C 
C  SET BEGINNING AND END POINTERS
C

C
C  IF WE THE SWAP IS PERFORMED IN THE LINEAR BLOCK
C
      IF (I .LE. NP1 ) THEN
        IF ( J .GT. NP1 ) THEN
          IF ( IPRINT .GE. 0 ) WRITE( IOUT, 1000 ) 
          STOP
        ENDIF
        KEND = NP1
        KI   = 1+(I-2)*NP1
        KJ   = 1+(J-2)*NP1
      ELSE
C
C  IF WE THE SWAP IS PERFORMED IN THE QUADRATIC BLOCK
C
        IF ( J .LE. NP1 ) THEN
          WRITE( IOUT, 1000 ) 
          STOP
        ENDIF
        KEND = DD
        KI   = 1+N*NP1+(I-NP2)*DD
        KJ   = 1+N*NP1+(J-NP2)*DD
      ENDIF
C
C  PERFORM THE SWAP
C
      IF ( J .NE. I ) THEN
        DO 60 K=1,KEND
          VAL        = POLY(KJ+K)
          POLY(KJ+K) = POLY(KI+K)
          POLY(KI+K) = VAL
 60     CONTINUE
      ENDIF
      RETURN

 1000 FORMAT( ' SWAPNP:  ERROR! TRYING TO SWAP POLYNOMIALS',/
     +        '          OF DIFFERENT DEGREE. MUST BE A BUG!',/)
      END  





