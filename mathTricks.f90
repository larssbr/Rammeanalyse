MODULE mathTricks

IMPLICIT NONE

CONTAINS
	!! useRandBetingelser er en SUBROUTINE som innfører randbetingelser
	!
	!Når et knutepunkt er fastholdt mot rotasjon skal tilhørende kolonne og
	!linje nulles ut i systemstivhetsmatrisen. Diagonalelementet settes lik
	!et vvilkårlig tall. Tilhørende element i lastvektoren må også nulles ut.	
	SUBROUTINE useRandBetingelser(K,R, knutePunkt, nknutePunkt)
	
		INTEGER, INTENT(in) :: nknutePunkt	
		REAL , INTENT(inout), DIMENSION(nknutePunkt,nknutePunkt) :: K 
		REAL , INTENT(inout), DIMENSION(nknutePunkt) :: R
		REAL , INTENT(inout), DIMENSION(nknutePunkt,3) :: knutePunkt
	
		INTEGER :: i
		
		!!   Innfører randbetingelser   
		DO i=1,nKnutePunkt ! går gjennom alle radene i både K og R
			IF (knutepunkt(i,3) == 1) THEN !Finner de knutepunktene som har fast innspenning
			
				K(i,:)=0 !Nuller ut den i'te raden i systemStivhetsMatrisen
				R(i)=0   !nuller ut den i'te raden i LastVektoren 
				K(:,i)=0 !Nuller ut den i'te kolonnen i systemStivhetsMatrisen
				K(i,i)=1.0 ! Gir diagonalen en verdi
			ENDIF
		ENDDO
	END SUBROUTINE useRandBetingelser
	
END MODULE mathTricks

