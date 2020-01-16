########### What I did ###########
1) To enable LET calculations I disabled the following line in the main file:

        //kp: if(let -> doCalculation)

2) To have Let.out produced, above wasn't enough. I had to change the value 
of doCalculation to 'true' at the top of HadrontherapyLet.cc

     G4bool HadrontherapyLet::doCalculation = true; //false

This was because, previously AnalysisMessenger class was where the value was
set to true.

I tested that if the value is false, it doesn't produce the Let.out file.

3) In "inline void SetDetectorPosition()" of the file PassiveProtonBeamLine.hh, 
I changed the (X,Y,Z) values of DetectorPosition to zeros as follows:

    inline void SetDetectorPosition()
    {
		/*
        // Adjust detector position
        detectorPosition.setX(detectorToPhantomPosition.getX() - phantomSizeX/2 + detectorSizeX/2);
        detectorPosition.setY(detectorToPhantomPosition.getY() - phantomSizeY/2 + detectorSizeY/2);
        detectorPosition.setZ(detectorToPhantomPosition.getZ() - phantomSizeZ/2 + detectorSizeZ/2);
        */
        //kp:
        detectorPosition.setX(0.0);
        detectorPosition.setY(0.0);
        detectorPosition.setZ(0.0);        
    }
    
4) Changed the material of the world vaolume from "G4_AIR" to "Galactic". See the following code in
PassiveProtonBeamLine::ConstructPassiveProtonBeamLine():

    G4Material* airNist =  G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR", isotopes);
    auto galacticMaterial = G4Material::GetMaterial("Galactic"); //kp:

    G4Box* treatmentRoom = new G4Box("TreatmentRoom",worldX_half,worldY_half,worldZ_half);
    G4LogicalVolume* logicTreatmentRoom = new G4LogicalVolume(treatmentRoom,
                                                              //airNist,
                                                              galacticMaterial, //kp:
                                                              "logicTreatmentRoom",
                                                              0,0,0);

Actually, it didn't work. So, reverted back to the original code.
