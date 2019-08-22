#source /Users/kpadhikari/KpInstalls/Geant4/geant4.10.05.p01-install/bin/geant4.sh

cmake -DGeant4_DIR=/Users/kpadhikari/KpInstalls/Geant4/geant4.10.05.p01-install/lib/Geant4-10.5.1 Source/hadrontherapy/
echo " "
echo " Now running 'make'"

make
