#!/bin/bash

#./exampleB4c -m gpg_pars.inc
# ./createMacroAndRunSim 200 1000   1
# i.e.  ./createMacroAndRunSim EbMeV   NumOfEvents   1=FileTagWithEbMeV
# Last par is to tag output file name with the beam energy
# 
 
#./createMacroAndRunSim 191 1000   1
#./createMacroAndRunSim 193 1000   1
#./createMacroAndRunSim 195 1000   1
#./createMacroAndRunSim 197 1000   1
#./createMacroAndRunSim 199 1000   1

#./createMacroAndRunSim 170 5000   1
#./createMacroAndRunSim 175 5000   1

echo "Printing text with newline"
echo -n "Printing text without newline"
echo -e "\nRemoving \t backslash \t characters\n"

for element in Hydrogen Helium Lithium Beryllium
do
  echo "Element: $element"
  sleep 0.5s # sleep for 0.5 sec
done

sleep 2s

# Eb and dE in MeV units
Ebi=170
Eb=0
dE=5 
EvNum=5000
for i in {0..10}
do
  rm -f codeCreated.mac
  #echo "Number: $i"
  let Eb=Ebi+i*dE  #Works without spaces
  #Eb=Ebi+i*dE #without 'let' it 
  #echo "./createMacroAndRunSim 170 5000   1"
  echo "Executing "
  echo "./createMacroAndRunSim $Eb $EvNum   1"
  echo "....."
  sleep 2s
  ./createMacroAndRunSim $Eb $EvNum   1
 
done

