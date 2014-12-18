# Fortran delen

echo ====================
echo Rammeanalyse  start
echo ====================

#compile
echo "gfortran lesinputModule.f90 mathTricks.f90 calculate.f90 gaussMod.f90 spesialCalculate.f90 printMod.f90  rammeanalyse.f90 -o program" 
gfortran lesinputModule.f90 mathTricks.f90 calculate.f90 gaussMod.f90 spesialCalculate.f90 printMod.f90  rammeanalyse.f90 -o program 

#Run the program
echo "./program"
./program


echo ====================
echo Rammeanalyse  compilated
echo ====================
# C delen
echo ====================
echo Visual Program
echo ====================

#Compile 
echo "make"
make

# #Run the program
echo "./oglprogram"
./oglprogram

echo ====================
echo Visual Program compilated
echo ====================
