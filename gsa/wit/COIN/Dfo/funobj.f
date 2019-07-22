C Copyright (C) 2000, International Business Machines
C Corporation and others.  All Rights Reserved.
      SUBROUTINE FUNOBJ(MODE, N, X, OBJF, OBJGRD, NSTATE)
          
      DOUBLE PRECISION OBJF, X(N), OBJGRD(N)
      
      INTEGER          MODE, N, NSTATE, I, J

      DOUBLE PRECISION GMOD, HMOD
 
      COMMON / MDLPAR / GMOD(200), HMOD(200,200)

      DOUBLE PRECISION  HALF

      PARAMETER        (HALF=0.5D0)
      IF (MODE.EQ.1 .OR. MODE .EQ. 2) THEN   
        DO 30 I=1,N
          OBJGRD(I)=GMOD(I)
          DO 40 J=1,N
            OBJGRD(I)=OBJGRD(I)+HMOD(I,J)*X(J)
 40       CONTINUE
 30     CONTINUE
      ENDIF
      IF (MODE.EQ.0 .OR. MODE .EQ. 2 ) THEN
        OBJF=0
        DO 50 I=1,N
          OBJF=OBJF+GMOD(I)*X(I)
          DO 60 J=1,N
            OBJF=OBJF+HALF*HMOD(I,J)*X(J)*X(I)
 60       CONTINUE
 50     CONTINUE
      ENDIF
      RETURN
      END
