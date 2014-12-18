MODULE printMod

IMPLICIT NONE

CONTAINS

	SUBROUTINE skrivTilfil(rotasjon, Mtot, Mmidt, Q, spenninger, materiale, nElement, AugM)
		
		REAL ,ALLOCATABLE ,DIMENSION(:) :: rotasjon
		REAL ,ALLOCATABLE ,DIMENSION(:,:) :: Mtot
		REAL ,ALLOCATABLE ,DIMENSION(:,:) :: Mmidt
		REAL ,ALLOCATABLE ,DIMENSION(:,:) :: Q
		REAL ,ALLOCATABLE ,DIMENSION(:,:) :: spenninger
		REAL ,ALLOCATABLE ,DIMENSION(:,:) :: materiale
		
		REAL ,ALLOCATABLE ,DIMENSION(:,:) :: AugM
		
		INTEGER, INTENT(in) :: nElement
		
		REAL :: flytespenning
		REAL, ALLOCATABLE, DIMENSION(:,:) :: prosentforhold
		INTEGER :: i, unit_int_out
		
		!! Skriver til output.dat-------------------------------------------------------------
		unit_int_out=11
		OPEN(unit=unit_int_out,file='resultatFil.dat', status="unknown", action="write")
			
			!-------Skriver ut hva rotasjonen ble i de forskjellige nodene-------------
			WRITE(unit_int_out,*)("Rotasjonene til knutepunktene")
			WRITE(unit_int_out,*)("     Theta")
			WRITE(unit_int_out,*) (rotasjon(:))
			
			!------Skriver ut hva endemomentene ble for alle elementene----------------
			WRITE(unit_int_out,*)("Endemomenter [kNm]")
			WRITE(unit_int_out,*)("   [KP1        KP2]")
			
			DO i=1,nElement
				WRITE(unit_int_out,*) (Mtot(i,:))
			ENDDO
			
			!------Skriver ut moment på midten (eventuelt der det virker punktLast)----
			WRITE(unit_int_out,*)("Momenter ved utsatte knutePunkt")
			WRITE(unit_int_out,*)("[avstandKP1 (Moment ved utsatt punkt)]")
			
			DO i=1,nElement
				WRITE(unit_int_out,*) (Mmidt(i,:))
			ENDDO
			!------Skriver ut skjærkreftene--------------------------------------------
			WRITE(unit_int_out,*)("Skjærkrefter [kNm]")
			WRITE(unit_int_out,*)("   [Q12        Q21]")
			
			DO i=1,nElement
				WRITE(unit_int_out,*) (Q(i,:))
			ENDDO
			!------Skriver ut spenningene på hvert element-----------------------------
			WRITE(unit_int_out,*)("Boeyespenninger[MPa]")
			WRITE(unit_int_out,*)("  [KP1        Midten      KP2]")
			
			DO i=1,nElement
			WRITE(unit_int_out,*) (spenninger(i,:))
			ENDDO
			
			!----------- prosentvis spenning----------------------------------------------
			! henter ut flytespenning fra den inleste filen. Kan for eksempel vaere 355GPa
			flytespenning= materiale(1,2)
			prosentForhold = (spenninger/flytespenning)*100
			! skriver til filen
			WRITE(unit_int_out,*)("Boyespenninger relativt til flytspenning i prosent")
			WRITE(unit_int_out,*)("  [KP1  Midten  KP2]")
			
			DO i=1,nElement
				WRITE(unit_int_out,*)(abs(prosentForhold(i,:)))
			ENDDO	
			
		CLOSE(unit_int_out)
	END SUBROUTINE skrivTilfil
END MODULE printMod
