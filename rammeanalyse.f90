!!---------RAMMEANALYSE-------------------------------------------------------
program rammeanalyse

	! USE statement
	USE lesinputModule
	USE calculate
	USE spesialCalculate
	USE mathTricks
	USE gaussMod
	USE printMod
		
	IMPLICIT NONE
  
    ! verdier som kommer fra inputfilen
    REAL ,ALLOCATABLE, DIMENSION(:,:) :: knutePunkt 
	INTEGER ,ALLOCATABLE, DIMENSION(:,:) :: element 
	REAL ,ALLOCATABLE ,DIMENSION(:,:) :: punktLast 
	REAL ,ALLOCATABLE ,DIMENSION(:,:) :: fordeltLast
	REAL ,ALLOCATABLE, DIMENSION(:,:) :: Ymoment 
	REAL ,ALLOCATABLE, DIMENSION(:,:) :: pipeProfil 
	REAL ,ALLOCATABLE ,DIMENSION(:,:) :: IPE_beam 
	REAL ,ALLOCATABLE ,DIMENSION(:,:) :: materiale
	INTEGER :: nKnutePunkt, nElement, nPunktLast, nFordeltLast, nYmoment, nPipeProfil, nIPE_beam, nMateriale
	
	! verdier som kommer fra utregninger med funksjoner
	REAL ,ALLOCATABLE ,DIMENSION(:) :: elementlengder
	REAL ,ALLOCATABLE ,DIMENSION(:,:) :: elementStivhet
	REAL ,ALLOCATABLE ,DIMENSION(:,:) :: K
	REAL ,ALLOCATABLE ,DIMENSION(:,:) :: fiMoment
	REAL ,ALLOCATABLE ,DIMENSION(:) :: R
	REAL, ALLOCATABLE, DIMENSION(:,:) :: AugM
	REAL ,ALLOCATABLE ,DIMENSION(:) :: rotasjon
	REAL ,ALLOCATABLE ,DIMENSION(:,:) :: Mrot
	REAL ,ALLOCATABLE ,DIMENSION(:,:) :: Mtot
	REAL ,ALLOCATABLE ,DIMENSION(:,:) :: Q
	REAL ,ALLOCATABLE ,DIMENSION(:,:) :: Mmidt
	REAL ,ALLOCATABLE ,DIMENSION(:,:) :: spenninger
	
!! ------Leser input-data----------------------------------------------------
    Call lesinput(nKnutePunkt, knutePunkt, nElement, element, nPunktLast, punktLast, nFordeltLast, fordeltLast, &
	nYmoment, Ymoment, nPipeProfil, pipeProfil, nIPE_beam, IPE_beam, nMateriale, materiale)

!! ------Regner lengder til elementene---------------------------------------   
    elementlengder = lengder(knutePunkt,element, nElement)

!! ------Regner stivhet EI til elementene------------------------------------
    elementStivhet = finnElementStivhet(element, nElement, pipeProfil, IPE_beam, materiale)
 
    !       Testing av oppgave c
    !   elementStivhet(2,1:2) = 2*elementStivhet(1,1:2)        

!!  -----Setter opp systemstivhetsmatrisen----------------------------------
    K = stivhetsmatrise(element, nElement, knutePunkt, nKnutePunkt, elementLengder, elementStivhet)
    
!!  -------Fastinnspenningsmomentene------------------------------------------
    fiMoment = finnFiMoment(element, nElement, punktLast, npunktlast, fordeltLast, nfordeltlast, elementlengder)

!!  -------Setter opp lastvektor----------------------------------------------

    R = lastvektor(fiMoment, Ymoment, nYmoment, element, nElement, knutePunkt, nKnutePunkt)
       
!! ---------Finner Rotasjoner---------------------------------------
!   Innfører randbetingelser og løser ligningen K*rot=R

	! --------Innfører randbetingelser-------
	call useRandBetingelser(K,R, knutePunkt, nknutePunkt)
		 
	!----------Lager Augmented Matrix---------
	!(slår sammen k(systemStivhetsmatrisen)og R(lastvektoren). slik at jeg av denne kan regne ut  rot( rotasjons"vektoren")
	AugM = makeAugmentedMatrix(K, R, nKnutePunkt) ! K(:,:) og b(:)
	
	! -------Løser ligningssytemet--------
	
	call rowReduce(AugM, nKnutePunkt)
		
	rotasjon = backSub(AugM,nKnutePunkt)

!! -------Løser Ligningen --------------------------
    Mrot = finnEndemoment(rotasjon, elementStivhet, elementlengder, element, nElement) ! Mrot(nElement,2)

!! -------Finner total endemoment for hver bjelke---------------------------
    Mtot = Mrot + fiMoment

!! -------Finner skjærkrefter ved bjelkeender--------------------------------

   Q = finnSkjaerKraft(Mtot, punktLast, fordeltLast, elementlengder, nElement, npunktlast, nfordeltlast)

!! -------Finner midtmoment for belastede element----------------------------
    Mmidt = midtmoment(Mtot, punktLast, fordeltLast, element, elementlengder, nElement ,nPunktLast , nFordeltLast)

!! -------Finner bøyespenninger ved alle ender og ved utsatte knutePunkt
    spenninger = boyespenning(Mtot,Mmidt,elementStivhet, nElement)
    
!! -------Print til fil-------------------------------------------------------------
	call skrivTilfil(rotasjon, Mtot, Mmidt, Q, spenninger, materiale, nElement, AugM)
		
END PROGRAM      
