MODULE lesinputModule

IMPLICIT NONE

CONTAINS

	SUBROUTINE lesinput(nKnutePunkt, knutePunkt, nElement, element, nPunktLast, punktLast, nFordeltLast, fordeltLast, &
	nYmoment, Ymoment, nPipeProfil, pipeProfil, nIPE_beam, IPE_beam, nMateriale, materiale)
	
	REAL ,ALLOCATABLE, INTENT(inout), DIMENSION(:,:) :: knutePunkt
	INTEGER ,ALLOCATABLE, INTENT(inout), DIMENSION(:,:) :: element 
	REAL ,ALLOCATABLE ,INTENT(inout), DIMENSION(:,:) :: punktLast
	REAL ,ALLOCATABLE ,INTENT(inout), DIMENSION(:,:) :: fordeltLast
	REAL ,ALLOCATABLE, INTENT(inout), DIMENSION(:,:) :: Ymoment
	REAL ,ALLOCATABLE, INTENT(inout), DIMENSION(:,:) :: pipeProfil 
	REAL ,ALLOCATABLE ,INTENT(inout), DIMENSION(:,:) :: IPE_beam
	REAL ,ALLOCATABLE ,INTENT(inout), DIMENSION(:,:) :: materiale
	
	INTEGER, INTENT(out) :: nKnutePunkt, nElement, nPunktLast, nFordeltLast, nYmoment, nPipeProfil, nIPE_beam, nMateriale
	
	!-------------------------(indre variabler)------------
		INTEGER :: i, unit_int_inn
		
		!-----------------------Åpner inputfila-------------------
		unit_int_inn=5
		OPEN(unit_int_inn, file="inputFil.dat",status="old") 
			
			!------------------Leser hvor mange punkt det er-----------
			READ(unit_int_inn,*) ! hopper over beskrivende rad
			READ(unit_int_inn,*) nKnutePunkt, nElement, nPunktLast, nFordeltLast, nYmoment, nPipeProfil, &
			 nIPE_beam, nMateriale ! Leser in antall
			
			!------------------Allocate mine til matrisene----------------------
			ALLOCATE (knutePunkt(nKnutePunkt,3)) 
			ALLOCATE (element(nElement,5)) 
			ALLOCATE (punktLast(nPunktLast,3)) 
			ALLOCATE (fordeltLast(nFordeltLast,3))
			ALLOCATE (Ymoment(nYmoment,2))
			ALLOCATE (pipeProfil(nPipeProfil,2))
			ALLOCATE (IPE_beam(nIPE_beam,4))
			ALLOCATE (materiale(nMateriale,2))
!################BEGYNER INNLESING##########################################################			 
			! ------Leser inn x og y koordinater til knutepunktene og grensebetingelser----
			! x-koordinat lagres i første kolonne, y-koordinat i 2.kolonne
			! Grensebetingelse lagres i kolonne 3, fast innspent=1 og fri rotasjon=0
	
			READ(unit_int_inn,*) ! hopper over beskrivende rad
			DO i=1,nknutePunkt
				READ(unit_int_inn,*) knutePunkt(i,1),knutePunkt(i,2), knutePunkt(i,3) !knutePunkt = fscanf(fileID,'!f !f !i',[3 nKnutePunkt])
			ENDDO

!-------------element---------------------------------------

			READ(unit_int_inn,*) ! hopper over beskrivende rad
			
			!Leser konnektivitet: sammenheng elementender og knutepunktnummer.
			!Leser Og EI for elementene
			! Elementnummer tilsvarer radnummer i "Elem-variabel"
			! Knutepunktnummer for lokal ende 1 lagres i kolonne 1
			! Knutepunktnummer for lokal ende 2 lagres i kolonne 2
			! E-modul= 3 !(samme som tverrsnitsstype) 1= I-Profil og 2= rørprofil
			! Tverrsnitstype = 4
			! Material-ID =5

				! leser inn i elemententMatrise
			DO i=1,nElement
				READ(unit_int_inn,*) element(i,1),element(i,2), element(i,3), & ! element = fscanf(fileID,'!i !i !i !i !i',[5 nElement])
				element(i,4), element(i,5)
			ENDDO

			!--------------Leser laster som virker------------- 
! punkt last
			READ(unit_int_inn,*) ! hopper over beskrivende rad
			!   kolonne 1= bjelke-element-id
			!   kolonne 2= størelse på last
			!   kolonne 3= avstand fra lokal ende(1 av (1--p---2)bjelke her ville avstanden vært 2 strekker)
			!   altså --> = alfa*lengde
			!   [bjelke-element-id, størelse på last, avstand fra lokal ende 1]    
		
			DO i=1,nPunktLast
				READ(unit_int_inn,*) punktLast(i,1),punktLast(i,2),punktLast(i,3) !punktLast= fscanf(fileID, '!i !f !f', [3 nPunktLast])
			ENDDO
! fordelt last
			
			READ(unit_int_inn,*) ! hopper over beskrivende rad
			!   kolonne 1 = bjelke-element-id
			!   kolonne 2 = størelse på last i lokal ende 1 
			!   kolonne 3 = størelse på last i lokal ende 2
			!   [bjelke-element-id, størelse på last i lokal ende 1, størelse på last i lokal ende 2]
			
			
			!   Antar at en ikke trenger å ta hensyn til om en fordelt last har avstand
			!   fra en lokal ende.
			
			DO i=1,nFordeltLast
				READ(unit_int_inn,*) fordeltLast(i,1),fordeltLast(i,2),fordeltLast(i,3) !fordeltLast= fscanf(fileID, '!i !f !f', [3 nFordeltLast])
			ENDDO
			
! ytre moment	
			READ(unit_int_inn,*) ! hopper over beskrivende rad
			!   kolonne 1= bjelke-element-id hvor momentet er satt på lokal ende 1
			!   Kolonne 2= størrelse på moment ved lokal ende 1
			![bjelke-element-id , størrelse på moment]    

			DO i=1,nYmoment
				READ(unit_int_inn,*) Ymoment(i,1),Ymoment(i,2) !Ymoment= fscanf(fileID, '!i !f', [2 nYmoment])
			ENDDO	

			!-------Leser inn material data------------------------------------------------------------------------
			
! rør profil
			READ(unit_int_inn,*) ! hopper over beskrivende rad
			! Rørprofil-id blir radnummeret i rørdatamatrisen "pipeProfil"
			! [ytre Diameter, rørtykelse]

			DO i=1,nPipeProfil
				READ(unit_int_inn,*) pipeProfil(i,1),pipeProfil(i,2) !pipeProfil = fscanf(fileID, '!f !f', [2 nPipeProfile])
			ENDDO
! IPE_beam
			READ(unit_int_inn,*) ! hopper over beskrivende rad
			!     IPE_beam-id blir radnummeret i IPE_beam-matrisen "IPE_beam"
			!     Flenslengde i kolonne 1
			!     Flenstykkelse i kolonne 2
			!     Steglengde i kolonne 3
			!     Stegtykkelse i kolonne 4
			!     [Flenslengde,Flenstykkelse,Steglengde,Stegtykkelse]

			DO i=1,nIPE_beam
				READ(unit_int_inn,*) IPE_beam(i,1),IPE_beam(i,2), IPE_beam(i,3),IPE_beam(i,4) !IPE_beam = fscanf(fileID, '!f !f !f !f', [4 nIPE_beam])
			ENDDO
! matriale	
			READ(unit_int_inn,*) ! hopper over beskrivende rad
			!     Materialnummer tilsvarer radnummer i material-matrisen "materilale"
			!     E-modul lagres i kolonne 1
			!     Flytespenning lagres i kolonne 2
			!     [E-modul, Flytespenning]

			DO i=1,nMateriale
				READ(unit_int_inn,*) materiale(i,1),materiale(i,2) !materiale = fscanf(fileID, '!f !f', [2 nMateriale])
			ENDDO
			
		! ------------Luker input-filen--------------------------
		CLOSE(unit_int_inn) ! lukker documentet input.dat
	

	END SUBROUTINE lesinput
END MODULE lesinputModule
