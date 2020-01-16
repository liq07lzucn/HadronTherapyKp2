/**********************
*  This code started from https://www.jlab.org/Hall-B//secure/eg4/adhikari/myHomeLinked/MyHm/RadCorr/RCSLACPOL_STEG/SingleProg2Stage/TestExe/createNsubJobs4mRipaniSimlns.C
*  which, in turn, was
*  started from a copy of ~adhikari/secure/Testfiles/Fortran/WdMake/RadLCppRootFortran/createNsubJobs4rcslacpolStegPassVarsFrmMain.C 2/11/13
*   in order to make scripts to submit simulation jobs using XS-maps & other related stuff from M. Ripani
*   
*   
*   How to compile?
*      g++ -o createMacroAndRunSim createMacroAndRunSim.C
*
***********/

/* system example : DIR */
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream> //this and the following line required for cout, cin etc. (otherwise, it's basically C)
//#include "/u/home/adhikari/LinkedFiles/myOwnHeaderFiles.h"
#include <dirent.h> //Needed for the following function which is fully defined at the bottom

#define MAKE_MULT_PLOTS

using namespace std;


void makeCmdLineInpFileOldNotUsed(char inpFl[], double EbMeV, int NumOfEvents);

void makeCmdLineInpFileCurrent(char inpFl[], double EbMeV, int NumOfEvents);

int main(int argc, char **argv)
{  
  //int forWhat, testOrAna; 
  double Eb_init = 0., EbeamMeV=0., Eb_step = 0.;
  int NumOfEvents = 1, HowMany = 1;
  if(argc<2) 
    {
      cout<<endl<<"\t Usage: \n \t Mode 1:"<<endl;
      cout<<"./exe  EbeamMeV"<<endl;
      cout<<"\tOptional:      ./exe EbeamMeV  NumOfEvents "<<endl;
      cout<<"\t Default value of NumOfEvents is 1.\n";
      cout<<"\t Mode 2: "<<endl;
      cout<<"\t ./exe Eb_init   NumOfEvents     Eb_step     HowMany"<<endl;
      return -1;
    }
  
  Eb_init=atof(argv[1]); //atoi(argv[1]);
  if(argc > 2) NumOfEvents = atoi(argv[2]);
  if(argc > 3) Eb_step = atof(argv[3]);
  if(argc > 4) HowMany = atoi(argv[4]);
               
  //if(!(forWhat==2)) {cout<<"Invalid value for 'forWhat'!"<<endl; return -1;}
  //char cstr[200]; ext = argv[argc-2]; 
  //sprintf(cstr," Just for test.%s.%c.%c.%c ",ext,ext[0],ext[1],ext[2]); cout<<argc<<" "<<cstr<<ext<<endl;     
 
  ofstream kpout; 
  char macroFile[50] = "codeCreated.mac";
  EbeamMeV = Eb_init;
  makeCmdLineInpFileCurrent( macroFile, EbeamMeV, NumOfEvents);

/*
  for(int i=firstJobNum; i<(firstJobNum+HowManyJobs); i++)
    {
      
      if(SubJobNowOrNot==11) 
		{sprintf(command,"jsub %s",jsubFl);  
	  cout<<"command: "<<command<<endl; system(command);}  //#jsub job_sub_batch
    }    
  cout<<"The program ends at this point. See you!!!"<<endl;  
  */   
  char command[100];
  if(argc > 4)
	{
		for(int i=0; i<HowMany; i++)
			{
				EbeamMeV = Eb_init + i * Eb_step;
				system("sleep 1s"); //Make sure macroFile is updated with correct Ebeam and # of events.
				makeCmdLineInpFileCurrent( macroFile, EbeamMeV, NumOfEvents);
			  
				system("sleep 1s");
				
				sprintf(command, "./hadrontherapy %s", macroFile);
				system(command);
				//sprintf(command, "mv -f LetDoseEtc.out  Results/V1/Let_gpsPr%dMeV.out", int(EbeamMeV));
				sprintf(command, "mv -f Let.out  Results/V1/Let_gpsPr%06.2fMeV.out", EbeamMeV);   system(command);

				sprintf(command, "mv -f Dose.out  Results/V1/Dose_gpsPr%06.2fMeV.out", EbeamMeV);   system(command);
			}
	}
  else 
	{
	  //sprintf(command, "./exampleB4c -m %s", macroFile);
	  sprintf(command, "./hadrontherapy %s", macroFile);
	  system(command);
	  //sprintf(command, "mv -f LetDoseEtc.out  Results/V1/Let_gpsPr%dMeV.out", int(EbeamMeV));
	  sprintf(command, "mv -f Let.out  Results/V1/Let_gpsPr%06.2fMeV.out", EbeamMeV);   system(command);

	  sprintf(command, "mv -f Dose.out  Results/V1/Dose_gpsPr%06.2fMeV.out", EbeamMeV);   system(command);
	}
  
  return 0;
}

//Code to produce something like the following file:
//    file:///Users/kpadhikari/KpInstalls/Geant4/ExamplesBld/HadronTherapyKp2/defaultMacro10000Evts.mac
void makeCmdLineInpFileCurrent(char inpFl[], double EbMeV, int NumOfEvents)
{
    char beamEnergyInMeV[150];
    ofstream kpout;
    kpout.open(inpFl,ios_base::out); 
    if(! kpout.is_open()) cout<<"Cannot open the output file File# : "<<endl;

    //kpout<<"#kp: It works (execute this code such as follows: './exampleB4c -m gpg_pars.inc')"<<endl;
    kpout<<"#kp: It works (execute this code such as follows: './hadrontherapy -m gps_pars.inc')"<<endl;
    kpout<<""<<endl;
    kpout<<""<<endl;
    kpout<<""<<endl;
    kpout<<"# G.A.P.Cirrone,"<<endl;
    kpout<<"# pablo.cirrone@lns.infn.it"<<endl;
    kpout<<"#"<<endl;
    kpout<<"# Default macro file. It is called if no argument is provided at run"<<endl;
    kpout<<"# and when the PHYSLIST enviroment variable is OFF"<<endl;
    kpout<<"# i.e. simply typing $G4WORKDIR/bin/Linux-++/Hadrontherapy <no argument here!>"<<endl;
    kpout<<""<<endl;
    kpout<<"#########################"<<endl;
    kpout<<"# Set of the verboses"<<endl;
    kpout<<"#"<<endl;
    kpout<<"/control/verbose 0"<<endl;
    kpout<<"/tracking/verbose 0"<<endl;
    kpout<<"/run/verbose 0"<<endl;
    kpout<<"/event/verbose 0"<<endl;
    kpout<<""<<endl;
    kpout<<"##########################"<<endl;
    kpout<<"# Set of the physic models"<<endl;
    kpout<<"#"<<endl;
    kpout<<"#/Physics/addPhysics QGSP_BIC_EMY"<<endl;
    kpout<<""<<endl;
    kpout<<"#### /Physics/addPhysics HADRONTHERAPY_1"<<endl;
    kpout<<"/run/numberOfThreads 1"<<endl;
    kpout<<""<<endl;
    kpout<<"##########################"<<endl;
    kpout<<"# Initialisation procedure"<<endl;
    kpout<<"#"<<endl;
    kpout<<"/run/initialize"<<endl;
    kpout<<""<<endl;
    kpout<<"####################################################"<<endl;
    kpout<<"# Set here the CUT and the STEP MAX for the tracking."<<endl;
    kpout<<"# Suggested values of cut and step"<<endl;
    kpout<<"#"<<endl;
    kpout<<"# Set cuts OUTSIDE the detector"<<endl;
    kpout<<"#"<<endl;
    kpout<<"/run/setCut 1 m"<<endl;
    kpout<<""<<endl;
    kpout<<"# Set cuts ONLY inside the detector"<<endl;
    kpout<<"#"<<endl;
    kpout<<"/run/setCutForRegion DetectorLog 0.1 mm"<<endl;
    kpout<<""<<endl;
    kpout<<""<<endl;
    kpout<<"/run/setCutForAGivenParticle e- 0.1 mm"<<endl;
    kpout<<"/run/setCutForAGivenParticle e+ 0.1 mm"<<endl;
    kpout<<"/run/setCutForAGivenParticle proton 0.1 mm"<<endl;
    kpout<<"#/Step/waterPhantomStepMax 0.1 mm"<<endl;
    kpout<<""<<endl;
    kpout<<"#########################"<<endl;
    kpout<<"# Set the primary particle type,"<<endl;
    kpout<<"# energy and position along the X direction"<<endl;
    kpout<<"#"<<endl;
    kpout<<""<<endl;
    kpout<<"#---------------------------gps-----------------"<<endl;
    kpout<<"/gps/pos/shape Circle"<<endl;
    kpout<<"#/gps/pos/centre -310. 0. 0. cm"<<endl;
    kpout<<"/gps/pos/centre -40. 0. 0. cm"<<endl;
    kpout<<"/gps/pos/radius 0. mm"<<endl;
    kpout<<"/gps/pos/sigma_r 2. mm"<<endl;
    kpout<<"/gps/particle proton"<<endl;
    kpout<<"#/gps/particle ion"<<endl;
    kpout<<"#/gps/ion 6 12 6"<<endl;
    kpout<<"/gps/pos/type Beam"<<endl;
    kpout<<""<<endl;
    kpout<<"# the incident surface is in the y-z plane"<<endl;
    kpout<<"#"<<endl;
    kpout<<"/gps/pos/rot1 0 1 0"<<endl;
    kpout<<"/gps/pos/rot2 0 0 1"<<endl;
    kpout<<""<<endl;
    kpout<<"# the beam is travelling along the x-axis without any angular dispersion (angular despersion set to 0.0)"<<endl;
    kpout<<"#"<<endl;
    kpout<<""<<endl;
    kpout<<"/gps/ang/rot1 0 0 1"<<endl;
    kpout<<"/gps/ang/rot2 0 1 0"<<endl;
    kpout<<"/gps/ang/type beam1d"<<endl;
    kpout<<"/gps/ang/sigma_r 0.028 deg"<<endl;
    kpout<<""<<endl;
    kpout<<"# the beam energy is in gaussian profile"<<endl;
    kpout<<"#"<<endl;
    kpout<<"/gps/ene/type Gauss"<<endl;
    kpout<<"#/gps/ene/mono 60.0 MeV"<<endl;
    kpout<<"#/gps/ene/mono 100.0 MeV"<<endl;


    kpout<<"#/gps/ene/mono 205.0 MeV"<<endl;
    sprintf(beamEnergyInMeV, "/gps/ene/mono   %03.2f MeV", EbMeV);
    kpout<<beamEnergyInMeV<<endl;
     
    kpout<<"#/gps/ene/sigma 0.25 MeV"<<endl;
    kpout<<"#/gps/ene/mono 744 MeV"<<endl;
    kpout<<"#/gps/ene/sigma 0.740 MeV"<<endl;
    kpout<<""<<endl;
    kpout<<"###################################################"<<endl;
    kpout<<"#		DETECTOR DESCRIPTION"<<endl;
    kpout<<"########################################################"<<endl;
    kpout<<"# USE THE FOLLOWING FOUR LINES FOR DEFINITION OF PHANTOM"<<endl;
    kpout<<"########################################################"<<endl;
    kpout<<"#"<<endl;
    kpout<<"#/changePhantom/position 20. 0. 0. cm"<<endl;
    kpout<<"#/changePhantom/update"<<endl;
    kpout<<""<<endl;
    kpout<<"/changePhantom/size 40 40 40  cm"<<endl;
    kpout<<"#/changePhantom/position 20 0 0 cm"<<endl;
    kpout<<"/changePhantom/position 0 0 0 cm"<<endl;
    kpout<<""<<endl;
    kpout<<"###################################################"<<endl;
    kpout<<"# USE THE FOLLOWING FOR A VOXELLIZED DETECTOR"<<endl;
    kpout<<"###################################################"<<endl;
    kpout<<"#"<<endl;
    kpout<<"####/changeDetector/size 4 4 4 cm"<<endl;
    kpout<<"/changeDetector/size 40 40 40 cm"<<endl;
    kpout<<"####/changeDetector/voxelSize .1 40 40 mm"<<endl;
    kpout<<"####/changeDetector/voxelSize 2 80 80 mm"<<endl;
    kpout<<"/changeDetector/voxelSize 2 400 400 mm"<<endl;
    kpout<<""<<endl;
    kpout<<"# Put the detector in the lower left corner of the phantom"<<endl;
    kpout<<"#"<<endl;
    kpout<<"####/changeDetector/displacement 0 18 18 cm"<<endl;
    kpout<<"#kp: Following causes the calle of UpdateGeometry() which"<<endl;
    kpout<<"#    will update geom of both Phantom and Detector."<<endl;
    kpout<<"/changePhantom/update"<<endl;
    kpout<<""<<endl;
    kpout<<"#########################"<<endl;
    kpout<<"# Display the event number"<<endl;
    kpout<<"# during the run"<<endl;
    kpout<<"#"<<endl;
    kpout<<"#### /event/printEventNumber 100"<<endl;
    kpout<<""<<endl;
    kpout<<"#########################"<<endl;
    kpout<<"# Start of the run"<<endl;
    kpout<<"#"<<endl;
    kpout<<"# If secondary particles dose & fluence are needed"<<endl;
    kpout<<"#### /analysis/secondary true"<<endl;
    kpout<<"#### /analysis/computeLet"<<endl;

    kpout<<"# Default material is water liquid"<<endl;
    kpout<<"/changePhantom/material G4_WATER"<<endl;

    kpout<<"/run/printProgress 1000"<<endl;
    kpout<<"#/run/printProgress 10000"<<endl;
    kpout<<"#/run/beamOn 1000"<<endl;
    kpout<<"#/run/beamOn 3000"<<endl;    

    //kpout<<"/run/beamOn 1000"<<endl;
    kpout<<"/run/beamOn "<<NumOfEvents<<endl;
    kpout<<"/tracking/verbose 0"<<endl;
  //=================

  kpout.close();
}

void makeCmdLineInpFileOldNotUsed(char inpFl[], double EbMeV, int NumOfEvents)
{
    char beamEnergyInMeV[150];
    ofstream kpout;
    kpout.open(inpFl,ios_base::out); 
    if(! kpout.is_open()) cout<<"Cannot open the output file File# : "<<endl;

    kpout<<"#kp: It works (execute this code such as follows: './exampleB4c -m gpg_pars.inc')"<<endl;
    kpout<<"/run/initialize"<<endl;
    kpout<<"#https://indico.cern.ch/event/294651/sessions/55918/attachments/552022/760639/PrimaryParticles.pdf"<<endl;
    kpout<<"#/gun/List    #List available particles"<<endl;        
    kpout<<"##Set    particle    to    be    generated"<<endl;
    kpout<<"/gun/particle proton"<<endl;
    kpout<<"#Set    momentum    direction"<<endl;
    kpout<<"/gun/direction  0. 0. 1. "<<endl; 
    kpout<<"#Set    kinetic    energy"<<endl;
    //kpout<<"/gun/energy     220 MeV"<<endl;
    sprintf(beamEnergyInMeV, "/gun/energy     %3.2f MeV", EbMeV);
    kpout<<beamEnergyInMeV<<endl;
    kpout<<"#/gun/momentum     #Set momentum"<<endl;    
    kpout<<"#/gun/momentumAmp  #Set absolute  value of momentum"<<endl;    
    kpout<<"#/gun/position     #Set starting  position of the particle"<<endl;
    kpout<<"#/gun/time         #Set initial time of he particle"<<endl;    
    kpout<<"#/gun/polarization #Set polarization"<<endl;
    kpout<<"#Set number of particles to be generated (per event)"<<endl;  
    kpout<<"/gun/number      1"<<endl;  
    kpout<<"#/gun/ion          #Set properties of ion to be generated [usage]"<<endl;    
    kpout<<"#/gun/ion          #Z A Q"<<endl;
    kpout<<"#/run/beamOn 1"<<endl;
    //kpout<<"/run/beamOn 1000"<<endl;
    kpout<<"/run/beamOn "<<NumOfEvents<<endl;
  //=================

  kpout.close();
}


