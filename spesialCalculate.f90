MODULE spesialCalculate

IMPLICIT NONE

CONTAINS

	FUNCTION midtmoment(Mtot, punktLast, fordeltLast, element, elementlengder, nElement ,nPunktLast , nFordeltLast) RESULT (midtM)   
	
		REAL ,ALLOCATABLE, INTENT(in), DIMENSION(:,:) :: Mtot
		REAL ,ALLOCATABLE, INTENT(in), DIMENSION(:,:) :: punktLast
		REAL ,ALLOCATABLE, INTENT(in), DIMENSION(:,:) :: fordeltLast
		INTEGER ,ALLOCATABLE, INTENT(in), DIMENSION(:,:) :: element
		REAL ,ALLOCATABLE, INTENT(in), DIMENSION(:) :: elementlengder
		INTEGER, INTENT(in) :: nElement ,nPunktLast , nFordeltLast 
		
		REAL , DIMENSION(nElement,2) :: midtM ! midtmom er matrisen som blir returnert
		
		INTEGER :: i, elementNr
		REAL ::  L, M12, M21, q1, q2, MF1, dQ , MlinearLast, avstandKP1, avstandKP2, momentP, P
		
		midtM=0.0
		
	!! -------- finer midtmoment for bjelken uten laster for hver element i --  
		DO i = 1,nElement
			L = elementlengder(i);
			M12 = Mtot(i,1);        ! ENDemoment ved 1
			M21 = Mtot(i,2);        ! ENDemoment ved 2
			midtM(i,2) = -M12 + (M21 + M12)/L * 0.5*L;
			midtM(i,1) = 0.5*L;
		ENDDO   
	!! ---------- Finner midtMomentet fra fordeltLast-------------------------   
		DO i = 1,nFordeltLast
		   
			elementNr = fordeltLast(i,1);  
			L = elementlengder(elementNr);      
			
			q1 = fordeltLast(i, 2);      
			q2 = fordeltLast(i, 3);     
			
			M12 = Mtot(elementNr,1);     
			M21 = Mtot(elementNr,2);      
			
			IF (q1 < q2) THEN 
				MF1=-(q1*L**2)/8;        
			else ! q1 > q2 eller de er like store( tar også med q1=q2 slik at den også blir regnet ut) 
			   
				MF1=-(q2*L**2)/8;
			ENDIF
	!!          Finner momentet fra LinærFordeltLast

				!regner bidraget fra den linære lasten. Hvis det ikke er noe linær
				!last --> dQ=0 det gir da 0 bidrag til moment med dQ=0.
				!Så trenger ikke se på tilfellet hvor det ikke er linær last.
				 dQ = abs(q1 - q2);

				!Max moment vil treffe ved L/sqrt(3)=0,57735*L, men siden vi ønsker og
				!superponere lasten ved L/2 bruker vi Momentet ved L/2.

				!Formel for Mtot(x) av linær fordelt last.
				!Mtot(x)=((dQ*L^2)/6)*((x)/L-((x)^3)/L)-->Mtot(L/2)=((dQ*L^2)/6)*((1/2)-(1/8))=(dQ*L^2)/16
				MlinearLast = -(dQ*L**2)/16 ; 
		   
	!       Superponerer bidragene fra ENDemomenter og moment fra last = totalt midtmoment
			midtM(elementNr,2) = -M12 + (M21 + M12)/L * 0.5*L + (MF1 + MlinearLast);
			midtM(elementNr,1) = 0.5*L;    ! Kolonne 1 forteller hvor momentet er beregnet
		ENDDO     
	!!--------- Finner midtmoment for alle bjelker med punktLast-------------
		 !Moment der punktLast virker.(rett under punktLasten)
		DO i = 1,nPunktLast
		   
			elementNr = punktLast(i,1);    
			L = elementlengder(elementNr);     
			
			P = punktLast(i, 2); 
			avstandKP1 = punktLast(i, 3);        
			avstandKP2 = L - avstandKP1;                  
				 
			M12 = Mtot(elementNr,1);      ! Endemoment 12
			M21 = Mtot(elementNr,2);      ! Endemoment 21
			momentP = -P*avstandKP1*avstandKP2/L; 

			midtM(elementNr,2) = -M12 + (M21 + M12)/L * avstandKP1 + momentP;
			midtM(elementNr,1) = avstandKP1;   ! avstand til KP1 fra der momentet er beregnet  
		ENDDO
	END FUNCTION midtmoment
	
	
!################################################################################################	
	FUNCTION finnSkjaerKraft(Mtot, punktLast, fordeltLast, elementlengder, nElement, npunktlast, nfordeltlast) RESULT(skjaerKraft)
    ! Q=Q_fastinnspenning +Q_enderotasjoner
	    REAL ,ALLOCATABLE, INTENT(in), DIMENSION(:,:) :: Mtot
		REAL ,ALLOCATABLE, INTENT(in), DIMENSION(:,:) :: punktLast
		REAL ,ALLOCATABLE, INTENT(in), DIMENSION(:,:) :: fordeltLast
		REAL ,ALLOCATABLE, INTENT(in), DIMENSION(:) :: elementlengder
		INTEGER, INTENT(in) :: nElement, npunktlast, nfordeltlast
		
		REAL , DIMENSION(nElement,2) :: skjaerKraft
		! lokale variable
		INTEGER :: i, elementNr
		REAL ::  L, M12, M21, q1, q2, avstandKP1, avstandKP2, P
		
		skjaerKraft=0.0
		  
		!! Skjærkraftbidrag fra endemomenter
		DO i = 1,nElement
			
			!Deklarer nye variabler
			L=elementlengder(i)
			! Skjærkraftbidrag fra endemomenter
			M12 = -Mtot(i,1)
			M21 = -Mtot(i,2)
			skjaerKraft(i,1) = (M21 + M12)/L  !Q12
			skjaerKraft(i,2) = -(M21 + M12)/L !Q21
		ENDDO
		
		!! Skjærkraftbidrag fra fordelte laster
		DO i = 1,nfordeltlast
			elementNr = fordeltLast(i,1)            
			q1 = fordeltLast(i,2)                          
			q2 = fordeltLast(i,3)                          
			L = elementlengder(elementNr)                    
			skjaerKraft(elementNr,1) = skjaerKraft(elementNr,1) + 2/6 * q1*L + 1/6 * q2*L! Skjærkraft ved 1
			skjaerKraft(elementNr,2) = skjaerKraft(elementNr,2) + 1/6 * q1*L + 2/6 * q2*L! Skjærkraft ved 2
		ENDDO
		
		!! Skjærkraftbidrag fra punktlaster
		DO i = 1,npunktlast
			elementNr = punktLast(i,1)                     
			P = punktLast(i,2)                             
			avstandKP1 = punktLast(i,3)                          
			L = elementlengder(elementNr)                                                        
			  IF ( avstandKP1 < L .AND. avstandKP1 > 0) THEN ! punktLast på knutepunkt gir ikke bidrag til skjær
				avstandKP2 = L-avstandKP1 
				
				!Q=
				skjaerKraft(elementNr,1) = skjaerKraft(elementNr,1) + P*avstandKP2/L ! Q12
				skjaerKraft(elementNr,2) = skjaerKraft(elementNr,2) + P*avstandKP1/L ! Q21
			  ENDIF
		ENDDO  
		! Angir positiv retning
		skjaerKraft(:,2) = skjaerKraft(:,2)*(-1)                      
	END FUNCTION finnSkjaerKraft
END MODULE spesialCalculate
