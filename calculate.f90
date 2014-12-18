MODULE calculate

IMPLICIT NONE

CONTAINS

!   --------------------------------------------------------------------
!   FUNCTION      lengder             No.:
!   --------------------------------------------------------------------
!
!   Hensikt :
!   Beregne alle lengdene til alle elementene
!   Metode :
!   Beregne lengdene ved formelen SQRT(dx**2 + dy**2)
!
!   Kall sekvens .......................................................
!
!           lengder(knutePunkt, element, nElement)
!
!   Parametre:
!   Navn    I/O  Type     Innhold/Beskrivelse
!   .................................................................
!   elementlengder      O    R       liste som inneholder lengden til alle elementene 
!   knutePunkt    		I    R       knutepunkt matrise
!   element       		I    I       element matrise
!   nElement       		I    I       antall elementer
!   
!     F U K S J O N E R :
!     SQRT      Regner ut kvadratroten til et tall.
!
!   Programmert av: Lars Brusletto
!   Date/Version  : 23.11.13 / 1.0
!
! **********************************************************************

FUNCTION lengder(knutePunkt, element, nElement) RESULT(elementlengder)
    
    REAL ,ALLOCATABLE, INTENT(in), DIMENSION(:,:) :: knutePunkt 
    INTEGER ,ALLOCATABLE, INTENT(in), DIMENSION(:,:) :: element
	INTEGER, INTENT(in) :: nElement 
		
	REAL , DIMENSION(nElement) :: elementlengder
	REAL ::dx,dy
	INTEGER :: i, KP1, KP2
		
	!Opretter tom svar matrise på ønsket størrelse for output 
	elementlengder=0.0
    
	! Beregner elementlengder
	DO i=1,nElement
		KP1 = element(i, 1)      ! Lokal ende 1
		KP2 = element(i, 2)      ! Lokal ende 2

		dx = knutePunkt(KP1,1)-knutePunkt(KP2,1) ! knutePunkt(KP1,1) 1 står for x kordinatet
		dy = knutePunkt(KP1,2)-knutePunkt(KP2,2) ! knutePunkt(KP1,2) 2 står for y kordinatet

		elementlengder(i) = SQRT(dx**2 + dy**2)
	ENDDO
END FUNCTION lengder

!   --------------------------------------------------------------------
!   FUNCTION      finnFiMoment             No.:
!   --------------------------------------------------------------------
!   Hensikt : Denne funksjonen regner ut FastInnspeningsMomentet ved å bruke formelene fra
!   side 281 i kompendiet "Del 1: Konstruksjonsanalyse". Fra faget "TMR 4167". 
!
!   Beregner: FastInnspeningsMomentet for : "PunktLast", "Jevnt Fordelt Last" og "linær fordelt Last".
!
!   Kall sekvens .......................................................
!
!           finnFiMoment(element, nElement, punktLast, npunktlast, fordeltLast, nfordeltlast, elementlengder)
!
!   Parametre:
!   Navn    I/O  Type     Innhold/Beskrivelse
!   .................................................................
!	nElement			I	 R
!	npunktlast			I	 R
!	nfordeltlast		I	 R
!   element       		I    I       element matrise
!   punktLast       	I    I       punktLast matrise
!   fordeltLast       	I    I       fordeltLast matrise
!   elementlengder      I    R       liste som inneholder lengden til alle elementene
!	fiMoment			O	 R
!   nElement       		I    I       antall elementer
!
!Lokale variable brukt i funksjonen:
!	REAL :: P, L, avstandKP1, avstandKP2, q1, q2
!	INTEGER :: i, elementNr	
!	
!     F U K S J O N E R :
!   
!   Programmert av: Lars Brusletto
!   Date/Version  : 23.11.13 / 1.0
!
! **********************************************************************
FUNCTION finnFiMoment(element, nElement, punktLast, npunktlast, fordeltLast, nfordeltlast, elementlengder) RESULT(fiMoment)

	INTEGER, INTENT(in) :: nElement, npunktlast, nfordeltlast 
	
	INTEGER , INTENT(in), DIMENSION(nElement,5) :: element
	REAL , INTENT(in), DIMENSION(npunktlast,3) :: punktLast
	REAL , INTENT(in), DIMENSION(nfordeltlast,3) :: fordeltLast
	REAL , INTENT(in), DIMENSION(nElement) :: elementlengder 
	
	
	REAL , DIMENSION(nElement,2) :: fiMoment !returneres
	!---lokale variable brukt i funksjonen----------
	REAL :: P, L, avstandKP1, avstandKP2, q1, q2
	INTEGER :: i, elementNr	
	
	fiMoment=0.0 ! Lager resultatMatrisen fiMoment og fyller den med 0'er

    ! Går gjenom alle elementene i konstruksjonen 
    DO i=1, nElement
        !! Punktlast
        IF(i <= npunktlast) THEN

            elementNr  = punktLast(i,1)
            P = punktLast(i,2)
            L=elementlengder(elementNr)
            avstandKP1 = punktLast(i,3)
            avstandKP2= L-avstandKP1
            
            ! fiMoment= FastInnspeningsMoment
            fiMoment(elementNr,1)= fiMoment(elementNr,1)-(P*avstandKP1*(avstandKP2)**2)/(L**2) ! fiMoment12
            fiMoment(elementNr,2)= fiMoment(elementNr,2) + (P*avstandKP1**2*(avstandKP2))/(L**2) ! fiMoment21

        ENDIF
        !! Fordelte Laster
        IF (i <= nfordeltlast) THEN
            elementNr  = fordeltLast(i,1)
            q1         = fordeltLast(i,2)
            q2         = fordeltLast(i,3)
            L          = elementlengder(elementNr)
            
            fiMoment(elementNr,1)= fiMoment(elementNr,1) - (q1*L**2)/12.0 ! fiMoment12
            fiMoment(elementNr,2)= fiMoment(elementNr,2) + (q1*L**2)/12.0 ! fiMoment21
            
            !! Linær Fordelt Last(TrekantLast)
            IF(q1 /= q2) THEN  
                fiMoment(elementNr,1)= fiMoment(elementNr,1) - ((q2-q1)*L**2)/30.0 ! fiMoment12
                fiMoment(elementNr,2)= fiMoment(elementNr,2) + ((q2-q1)*L**2)/20.0 ! fiMoment21
            ENDIF
        ENDIF
    ENDDO
END FUNCTION finnFiMoment

!##################################################################################################
FUNCTION lastvektor(fiMoment, Ymoment, nYmoment, element, nElement, knutePunkt, nKnutePunkt) RESULT(R)
	
	REAL ,ALLOCATABLE, INTENT(in), DIMENSION(:,:) :: fiMoment
	REAL ,ALLOCATABLE, INTENT(in), DIMENSION(:,:) :: Ymoment
	INTEGER ,ALLOCATABLE, INTENT(in), DIMENSION(:,:) :: element
	REAL ,ALLOCATABLE, INTENT(in), DIMENSION(:,:) :: knutePunkt
	 
	INTEGER, INTENT(in) :: nElement, nKnutePunkt, nYmoment 
	
	REAL , DIMENSION(nKnutePunkt) :: R !returneres
	INTEGER :: nfiMoment, kp1, kp2, i
	REAL :: m12, m21
	R=0.0
	nfiMoment=nElement ! antall fastInnspeningsMoment--> vil være like mange som det er elementer
	 
    !!
    !Plasserer fastinnspenningsmoment til riktig bjelke
    DO i=1,nfiMoment
 
        kp1 = element(i,1)
        kp2 = element(i,2)
        m12 = fiMoment(i,1)
        m21 = fiMoment(i,2)
        
        R(kp1) = R(kp1) + m12
        R(kp2) = R(kp2) + m21
        
    ENDDO
    !snur fortegn
    R = R*(-1) 
    !!
    !legger til ytre moment på bjelke som blir påvirket
    IF (nYmoment>=1) THEN ! om man ikke har ytre moment på konstruksjonen vil man ikke renge ut bidrag fra ytre moment
        DO i=1,nYmoment
			kp1=Ymoment(i,1)
            R(kp1)= R(kp1) + Ymoment(i,2)
        ENDDO
    ENDIF 
END FUNCTION lastvektor
!#####################################################

FUNCTION stivhetsmatrise(element, nElement, knutePunkt, nKnutePunkt, elementLengder, elementStivhet) RESULT(K)
	
	INTEGER, INTENT(in) :: nElement, nKnutePunkt
	INTEGER , INTENT(in), DIMENSION(nElement,5) :: element
	REAL , INTENT(in), DIMENSION(nKnutePunkt,3) :: knutePunkt
	REAL , INTENT(in), DIMENSION(nElement) :: elementLengder
	REAL , INTENT(in), DIMENSION(nElement,3) :: elementStivhet

	! K er størelsen nKnutepunkt, nKnutePunkt
	REAL , DIMENSION(nKnutePunkt, nKnutePunkt) :: K !returneres
	INTEGER :: kp1, kp2, i
	REAL :: EI, L
	K=0.0 ! prealokerer matrise slik at for løkka går fortere

	!! Går gjennom hvert element og finner knutepunkt 1 og knutepunkt 2 til det lokale systemet. 
	!  Setter det så inn en den globale stivhetsmatrisen K.  
    DO i=1,nElement
    
        kp1 = element(i,1) ! kp1 = knutepunkt1
        kp2 = element(i,2) ! kp2 = knutepunkt2
        
        EI  = elementStivhet(i,1)   
        L   = elementLengder(i)
        
        K(kp1, kp1)=K(kp1, kp1)+ (4.0*EI)/L
        K(kp1, kp2)=K(kp1, kp2)+ (2.0*EI)/L
        K(kp2, kp1)=K(kp2, kp1)+ (2.0*EI)/L
        K(kp2, kp2)=K(kp2, kp2)+ (4.0*EI)/L
    ENDDO
     
END FUNCTION stivhetsmatrise

!! finnEndemoment regner ut rotasjonsmoment for hvert element.
! Raden representerer hvilket element som har det kalkulerte rotasjonsmomentet
! i henholdsvis knutepunkt 1 og knutepunkt 2 på formen [KP1 KP2]
FUNCTION  finnEndemoment(rotasjon, elementStivhet, elementlengder, element, nElement) RESULT(Mrot)
	REAL ,ALLOCATABLE, INTENT(in), DIMENSION(:) :: rotasjon
	REAL ,ALLOCATABLE, INTENT(in), DIMENSION(:,:) :: elementStivhet
	REAL ,ALLOCATABLE, INTENT(in), DIMENSION(:) :: elementLengder
	INTEGER ,ALLOCATABLE, INTENT(in), DIMENSION(:,:) :: element

	INTEGER, INTENT(in) :: nElement
	
	REAL , DIMENSION(nElement, 2) :: Mrot !returneres
	INTEGER :: kp1, kp2, i
	REAL :: EI, L , theta1, theta2
	Mrot=0.0
    
    DO i=1,nElement
        
        !lokale knutepunkter
        kp1=element(i,1)
        kp2=element(i,2)
        
        !Finne elementdata
        EI = elementStivhet(i,1)
        L  = elementlengder(i)
        theta1 = rotasjon(kp1)
        theta2 = rotasjon(kp2)
        
        !Regner ut endemoment og plasserer svaret på riktig posisjon
        Mrot(i,1) = 4.0*EI/L*(theta1+0.5*theta2) 
        Mrot(i,2) = 4.0*EI/L*(theta2+0.5*theta1)
    ENDDO
END FUNCTION finnEndemoment

FUNCTION  boyespenning(Mtot,Mmidt,elementStivhet, nElement) RESULT(sigma)
    
    REAL ,ALLOCATABLE, INTENT(in), DIMENSION(:,:) :: Mtot
	REAL ,ALLOCATABLE, INTENT(in), DIMENSION(:,:) :: Mmidt
	REAL ,ALLOCATABLE, INTENT(in), DIMENSION(:,:) :: elementStivhet

	INTEGER, INTENT(in) :: nElement 

	!MRot er nElement mange rader og 3 kolone bred
	REAL , DIMENSION(nElement,3) :: sigma !returneres
	REAL :: I, yc, Mmidten, M12, M21
	INTEGER :: j
	
	sigma = 0.0

    DO j=1,nElement

        !Finner nødvendig material-data
        I=elementStivhet(j,2)
        yc=elementStivhet(j,3)

        !henter moment i maksimumpunkter
        Mmidten=Mmidt(j,2) !moment på midten/(der hvor punktlasten virker) av bjelken
        M12=Mtot(j,1)        !moment ved lokal ende 1
        M21=Mtot(j,2)        !moment ved lokal ende 2

        !regner ut bøyespenningen og legger de inn i Matrisen
        sigma(j,1)= (M12*yc)/I
        sigma(j,2)= (Mmidten*yc)/I
        sigma(j,3)= (M21*yc)/I

    ENDDO

    !Konverterer fra Pa[N/m^2] til Mpa=10^6*pa[N/mm^2]
    sigma =sigma/10e6 ! sigma*10^-6
END FUNCTION
!####################################################################################

FUNCTION finnElementStivhet(element, nElement, pipeProfil, IPE_beam, materialer) RESULT(EI)   
	
	INTEGER, INTENT(in) :: nElement 
	
	INTEGER , INTENT(in), DIMENSION(nElement,5) :: element
	REAL ,ALLOCATABLE, INTENT(in), DIMENSION(:,:) :: pipeProfil ! 2 koloner bred
	REAL ,ALLOCATABLE, INTENT(in), DIMENSION(:,:) :: IPE_beam ! 4 koliner bred
	REAL ,ALLOCATABLE, INTENT(in), DIMENSION(:,:) :: materialer ! 2 koloner bred

	!EI er nElement mange rader og 3 kolone bred
	REAL , DIMENSION(nElement, 3) :: EI !returneres
	INTEGER :: tverrsnittType, profil_ID, material_ID, j
	REAL ::  D, r, t,  fl, ft, sl, st, I_flens, I_steg , E, I, yc
	
	REAL(8), PARAMETER :: PI = 3.141592653589793239
	EI=0.0

!! For-løkke går gjennom alle elementer
    DO j=1,nElement
        
       tverrsnittType = element(j,3)
       profil_ID = element(j,4)
       material_ID = element(j,5)
				
       E = materialer( material_ID , 1)*10e9  ! 10^9= giga
    
       IF (tverrsnittType==2) THEN ! if tverrsnittType=="pipe"
           
           D =       pipeProfil(profil_ID,1)  ! D= diameter
           r =       D/2.0                      ! r = radius
           t =       pipeProfil(profil_ID,2 ) ! t = tykkelse
           yc=       D/2.0
           
            ! Regner ut "annet arealmomentet" for rørProfilet 
            I = (PI*(r**4-(r-t)**4))/4.0
            
       ELSE ! tverrsnittType= IPE(beam)="bjelke"
            fl = IPE_beam(profil_ID,1) ! fl= flens lengde
            ft = IPE_beam(profil_ID,2) ! ft= flens tykkelse
            sl = IPE_beam(profil_ID,3) ! sl= stag lengde
            st = IPE_beam(profil_ID,4) ! st = stag tykkelse
!           Regner ut nøytralaksen
            yc = (2*ft+sl)/2 ! senteret av snittet
!           Finner arealtreghetsmoment( annet arealmoment) med steiner's teorem
            I_flens = ((fl*1*ft**3)/12.0) + fl*ft*(yc-0.5*ft)**2
            I_steg = (1.0/12.0)*sl**3*st

            I = 2*I_flens+I_steg ! 2 flenser og et steg i en IPE beam
            
       ENDIF
       ! Plassere vikitg info om stivhet i en tabell, EI
        
        I = I/(10e12)  !(mm^4)*10^(-12) =m^4
        
        EI(j,1) = E*I        ! stivheten E*I
        EI(j,2) = I          ! "Annet arealMoment"
        EI(j,3) = yc         ! "center_of_cross_section"(yc)
         
    ENDDO
END FUNCTION

END MODULE calculate
