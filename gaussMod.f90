MODULE gaussMod

IMPLICIT NONE

CONTAINS

    ! AGMENTED MATRIX er en matrise slått sammen av 2 matriser.
	! brukes til å slå sammen stivhetsmatrisen inv(k) med lastvektoren R
	! metoden returnerer den Augmentede matrisen
    FUNCTION makeAugmentedMatrix(K, R, nKnutePunkt) RESULT(AugM)
		! nKnutePunkt er nrader
		INTEGER, INTENT(in) :: nKnutePunkt ! er antall rader i K matrisen
		REAL, INTENT(in),  DIMENSION(nKnutePunkt,nKnutePunkt) :: K
		REAL, INTENT(in),  DIMENSION(nKnutePunkt) :: R

		REAL, DIMENSION(nKnutePunkt,nKnutePunkt+1) :: AugM
		INTEGER :: i

		AugM(1:nKnutePunkt,1:nKnutePunkt) = K
		AugM(1:nKnutePunkt,nKnutePunkt+1) = R ! legger til vectorB i AUGM

		DO i=1,nKnutePunkt
			IF (AugM(i,i)==0) THEN 
				write(*,*) ' error -->Matrix is singular! fra makeAugmented matrix ,at linje i=', i
			ENDIF
		ENDDO               
    END FUNCTION makeAugmentedMatrix
	
	SUBROUTINE rowReduce(AugM, nKnutePunkt)
		! nKnutePunkt er nrader
		REAL, INTENT(inout), DIMENSION(:,:) :: AugM
		INTEGER, INTENT(in) :: nKnutePunkt
		
		REAL, DIMENSION(nKnutePunkt) :: r
		INTEGER :: counter, ncolum, j, i, k
	
		DO k = 1, nKnutePunkt	
			DO i = k+1, nKnutePunkt
				DO j= k+1 , nKnutePunkt+1
					AugM(i,j)=AugM(i,j)- AugM(k,j)*(AugM(i,k)/AugM(k,k))
				ENDDO
				AugM(i,k)=0
			ENDDO
		ENDDO	  	  
	END SUBROUTINE rowReduce
	
	
	FUNCTION backSub(AugM,nKnutePunkt) RESULT(solve)
		! nKnutePunkt er nrader
		INTEGER, INTENT(in) :: nKnutePunkt
		REAL, INTENT(inout), DIMENSION(nKnutePunkt,nKnutePunkt) :: AugM
		REAL, DIMENSION(nKnutePunkt) :: solve
		
		INTEGER :: i, j,k
		DOUBLE PRECISION :: tmp
		
		solve=0.0
		
		!i er rader, starter nederst og går opppover
		DO i=nKnutePunkt,1,-1
			!j er koloner uten diagonalen og b
			DO j=i+1, nKnutePunkt
			
				AugM(i,nKnutePunkt+1) = AugM(i,nKnutePunkt+1) -AugM(i,j)*AugM(j,nKnutePunkt+1)				
				AugM(i,j)=0
			ENDDO
			
			AugM(i,nKnutePunkt+1)=AugM(i,nKnutePunkt+1)/AugM(i,i)	
			AugM(i,i)=1	
		ENDDO
		solve=AugM(:,nKnutePunkt+1)				
	END FUNCTION backSub
END MODULE gaussMod
