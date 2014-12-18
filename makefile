OBJECTS = oglprogram.o  lesInn.o tegnInfo.o skalering.o tegnYtreKrefter.o tegnDiagram.o

CFLAGS = -lglut -lGLU -lGL -lm

# Sett compilator som skal brukes C = gcc , FORTRAN = gfortran .
# Må inkludere $(CC) før det som skal compillers.
CC = gcc

# Mappenavnnet
all: oglprogram

# Output navnet er oglprogram. For å kjøre filen: ./oglprogram
oglprogram: ${OBJECTS}
		$(CC) ${OBJECTS} -o oglprogram ${CFLAGS} 

# Vask opp etter seg.
# rm = remove
clean:
		rm  -f oglprogram ${OBJECTS} *.o 

# Vaske grundig
veryclean: clean
		rm  -f oglprogram ${OBJECTS} *~ *.o
