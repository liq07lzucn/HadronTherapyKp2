//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// Hadrontherapy advanced example for Geant4
// See more at: https://twiki.cern.ch/twiki/bin/view/Geant4/AdvancedExamplesHadrontherapy

#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4RunManager.hh"
#include "G4GeometryManager.hh"
#include "G4SolidStore.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"

#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4NistManager.hh"
#include "G4NistElementBuilder.hh"
//#include "HadrontherapyDetectorConstruction.hh"
//#include "HadrontherapyModulator.hh"
#include "PassiveProtonBeamLine.hh"
//#include "PassiveProtonBeamLineMessenger.hh"
#include "HadrontherapyDetectorMessenger.hh"
#include "HadrontherapyDetectorROGeometry.hh"
#include "HadrontherapyMatrix.hh"
#include "HadrontherapyLet.hh"


//
//kp: Following is static member initialization, just like int Box::objectCount = 0, where objectCount is
//    the static member of the Box class. (see https://www.tutorialspoint.com/cplusplus/cpp_static_members.htm)
//    Here, the first 'HadrontherapyDetectorConstruction' along with * defines the variable type. The second
//    HadrontherapyDetectorConstruction defines the scope that is which class the variable 'instance' belongs to.
//    For example, another variable of the same type could belong to other class of the same program.
//    Remember static data members are initialized outside of the class definition (after declaring them inside the class)
//
PassiveProtonBeamLine* PassiveProtonBeamLine::instance = 0;

//G4bool PassiveProtonBeamLine::doCalculation = false;
/////////////////////////////////////////////////////////////////////////////
PassiveProtonBeamLine::PassiveProtonBeamLine():
physicalTreatmentRoom(0),//hadrontherapyDetectorConstruction(0)
motherPhys(physicalTreatmentRoom), // pointer to WORLD volume
detectorSD(0), detectorROGeometry(0), matrix(0),
phantom(0), detector(0),
phantomLogicalVolume(0), detectorLogicalVolume(0),
phantomPhysicalVolume(0), detectorPhysicalVolume(0),
aRegion(0)
{
    // Messenger to change parameters of the passiveProtonBeamLine geometry
    //passiveMessenger = new PassiveProtonBeamLineMessenger(this);
    detectorMessenger = new HadrontherapyDetectorMessenger(this); //kp

    
    //***************************** PW ***************************************
    static G4String ROGeometryName = "DetectorROGeometry";
    RO = new HadrontherapyDetectorROGeometry(ROGeometryName);
    
    G4cout << "Going to register Parallel world...";
    RegisterParallelWorld(RO);
    G4cout << "... done" << G4endl;

}
/////////////////////////////////////////////////////////////////////////////
PassiveProtonBeamLine::~PassiveProtonBeamLine()
{
    //delete passiveMessenger;
    //delete hadrontherapyDetectorConstruction;
    delete detectorROGeometry;
    delete matrix;
    delete detectorMessenger;
}

/////////////////////////////////////////////////////////////////////////////
PassiveProtonBeamLine* PassiveProtonBeamLine::GetInstance()
{
    return instance;
}

/////////////////////////////////////////////////////////////////////////////
G4VPhysicalVolume* PassiveProtonBeamLine::Construct()
{
    // Sets default geometry and materials
    SetDefaultDimensions();
    SetDimensionsOfPhantomAndDetector(); //kp
 
    
    // Construct the whole Passive Beam Line
    ConstructPassiveProtonBeamLine();
    motherPhys = physicalTreatmentRoom; //kp: Without this phantom & detector were not constructed.
    
    /*
    // ***************************** PW ***************************************
    if (!hadrontherapyDetectorConstruction)        
        // HadrontherapyDetectorConstruction builds ONLY the phantom and the detector with its associated ROGeometry
        hadrontherapyDetectorConstruction = new HadrontherapyDetectorConstruction(physicalTreatmentRoom);
      */
    ConstructPhantomAndDetector();
       
    //hadrontherapyDetectorConstruction-> InitializeDetectorROGeometry(RO, hadrontherapyDetectorConstruction->GetDetectorToWorldPosition());    
    //InitializeDetectorROGeometry(RO, this->GetDetectorToWorldPosition()); //kp
    InitializeDetectorROGeometry(RO, GetDetectorToWorldPosition()); //kp
    // ***************************** PW ***************************************

    return physicalTreatmentRoom;
}

// In the following method the DEFAULTS used in the geometry of
// passive beam line are provided
// HERE THE USER CAN CHANGE THE GEOMETRY CHARACTERISTICS OF BEAM
// LINE ELEMENTS, ALTERNATIVELY HE/SHE CAN USE THE MACRO FILE (IF A
// MESSENGER IS PROVIDED)
//
// DEFAULT MATERIAL ARE ALSO PROVIDED and COLOURS ARE ALSO DEFINED
// ----------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////
void PassiveProtonBeamLine::SetDefaultDimensions()
{
    // Set of coulors that can be used
    white = new G4VisAttributes( G4Colour());
    white -> SetVisibility(true);
    white -> SetForceSolid(true);
	
    blue = new G4VisAttributes(G4Colour(0. ,0. ,1.));
    blue -> SetVisibility(true);
    blue -> SetForceSolid(true);
	
    gray = new G4VisAttributes( G4Colour(0.5, 0.5, 0.5 ));
    gray-> SetVisibility(true);
    gray-> SetForceSolid(true);
	
    red = new G4VisAttributes(G4Colour(1. ,0. ,0.));
    red-> SetVisibility(true);
    red-> SetForceSolid(true);
	
    yellow = new G4VisAttributes(G4Colour(1., 1., 0. ));
    yellow-> SetVisibility(true);
    yellow-> SetForceSolid(true);
	
    green = new G4VisAttributes( G4Colour(25/255. , 255/255. ,  25/255. ));
    green -> SetVisibility(true);
    green -> SetForceSolid(true);
	
    darkGreen = new G4VisAttributes( G4Colour(0/255. , 100/255. ,  0/255. ));
    darkGreen -> SetVisibility(true);
    darkGreen -> SetForceSolid(true);
    
    darkOrange3 = new G4VisAttributes( G4Colour(205/255. , 102/255. ,  000/255. ));
    darkOrange3 -> SetVisibility(true);
    darkOrange3 -> SetForceSolid(true);
	
    skyBlue = new G4VisAttributes( G4Colour(135/255. , 206/255. ,  235/255. ));
    skyBlue -> SetVisibility(true);
    skyBlue -> SetForceSolid(true);
    

    
    // DEFAULT DEFINITION OF THE MATERIALS
    // All elements and compound definition follows the NIST database
    
    // ELEMENTS
    G4Element* zincNist = G4NistManager::Instance()->FindOrBuildElement("Zn");
    G4Element* copperNist = G4NistManager::Instance()->FindOrBuildElement("Cu");
        
    G4double d; // Density
    G4int nComponents;// Number of components
    G4double fractionmass; // Fraction in mass of an element in a material
    
    d = 8.40*g/cm3;
    nComponents = 2;
    G4Material* brass = new G4Material("Brass", d, nComponents);
    brass -> AddElement(zincNist, fractionmass = 30 *perCent);
    brass -> AddElement(copperNist, fractionmass = 70 *perCent); 
}

/////////////////////////////////////////////////////////////////////////////
void PassiveProtonBeamLine::ConstructPassiveProtonBeamLine()
{
    // -----------------------------
    // Treatment room - World volume
    //------------------------------
    // Treatment room sizes
    const G4double worldX_half = 30.0 *cm; //400.0 *cm;
    const G4double worldY_half = 30.0 *cm; //400.0 *cm;
    const G4double worldZ_half = 30.0 *cm; //400.0 *cm;
    G4bool isotopes = false;
    
    G4Material* airNist =  G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR", isotopes);
    //auto galacticMaterial = G4Material::GetMaterial("Galactic"); //kp:
    //airNist =  G4NistManager::Instance()->FindOrBuildMaterial("Galactic", isotopes);//kp:

    G4Box* treatmentRoom = new G4Box("TreatmentRoom",worldX_half,worldY_half,worldZ_half);
    G4LogicalVolume* logicTreatmentRoom = new G4LogicalVolume(treatmentRoom,
                                                              airNist,
                                                              //galacticMaterial, //kp:
                                                              "logicTreatmentRoom",
                                                              0,0,0);
    physicalTreatmentRoom = new G4PVPlacement(0,
                                              G4ThreeVector(),
                                              "physicalTreatmentRoom",
                                              logicTreatmentRoom,
                                              0,false,0);
    
     /*
    // The treatment room is invisible in the Visualisation
    //logicTreatmentRoom -> SetVisAttributes(G4VisAttributes::Invisible);
    
    // Components of the Passive Proton Beam Line
    HadrontherapyBeamLineSupport();
   
    HadrontherapyBeamScatteringFoils();
    HadrontherapyRangeShifter();
    HadrontherapyBeamCollimators();
    HadrontherapyBeamMonitoring();
    HadrontherapyMOPIDetector();
    HadrontherapyBeamNozzle();
    HadrontherapyBeamFinalCollimator();
    
    // The following lines construc a typical modulator wheel inside the Passive Beam line.
    // Please remember to set the nodulator material (default is air, i.e. no modulator!)
    // in the HadrontherapyModulator.cc file
    modulator = new HadrontherapyModulator();
    modulator -> BuildModulator(physicalTreatmentRoom);
    *     */

}

//kp: All the following code copied from HadrontherapyDetectorConstruction.cc and modified some
/*
HadrontherapyDetectorConstruction::HadrontherapyDetectorConstruction(G4VPhysicalVolume* physicalTreatmentRoom)
: motherPhys(physicalTreatmentRoom), // pointer to WORLD volume
detectorSD(0), detectorROGeometry(0), matrix(0),
phantom(0), detector(0),
phantomLogicalVolume(0), detectorLogicalVolume(0),
phantomPhysicalVolume(0), detectorPhysicalVolume(0),
aRegion(0) */
void PassiveProtonBeamLine::ConstructPhantomAndDetector()
{
    /* NOTE! that the HadrontherapyDetectorConstruction class
     * does NOT inherit from G4VUserDetectorConstruction G4 class
     * So the Construct() mandatory virtual method is inside another geometric class
     * like the passiveProtonBeamLIne, ...
     */
    
    // Messenger to change parameters of the phantom/detector geometry
    //detectorMessenger = new HadrontherapyDetectorMessenger(this);
    
    //SetDimensionsOfPhantomAndDetector();
    
    
    // Write virtual parameters to the real ones and check for consistency
    UpdateGeometry();   
}


void PassiveProtonBeamLine::SetDimensionsOfPhantomAndDetector()
{
    //kp: m, cm, mm, um, nm, pm, k, M, G - meter, centi, milli, micro, nano, pico, kilo, Mega, Giga
    //    so 200 slabs of 200 um thickess means a total of 4 cm along X- or beam direction. So, imagine
    //    the volum to be a stack of 200 slabs with the X-axis or beam line passing perpendicularly through
    //    the center of each of these slabs. In other words, the voxel is a thin rectangular plate of sides
    //    4 cm * 4 cm and with a thickness of 0.02 cm or 0.2 mm.
    //
    // Default detector voxels size
    // 200 slabs along the beam direction (X)
    sizeOfVoxelAlongX = 200 *um; //40. cm/ 200 um   = 200 bins = 1 mm slice
    sizeOfVoxelAlongY = 4 *cm;   //40. cm / 4 cm    = 10 bins  = 40 mm slice
    sizeOfVoxelAlongZ = 4 *cm;   //40. cm / 4 cm    = 10 bins  = 40 mm slice
    //kp: I was changing these numbers from macro, using lines such as follows:
    //     /changeDetector/voxelSize 2 400 400 mm
    //   where I am using 100 x-bins of size 2 mm, 1 y- and z- bins of size 40 cm or 400 mm
    
    // Define here the material of the water phantom and of the detector
    SetPhantomMaterial("G4_WATER");
    // Construct geometry (messenger commands)
    //SetDetectorSize(4.*cm, 4.*cm, 4.*cm);
    SetDetectorSize(40. *cm, 40. *cm, 40. *cm);
    SetPhantomSize(40. *cm, 40. *cm, 40. *cm);
    
    //SetPhantomPosition(G4ThreeVector(20. *cm, 0. *cm, 0. *cm));
    SetPhantomPosition(G4ThreeVector(0. *cm, 0. *cm, 0. *cm));
    SetDetectorToPhantomPosition(G4ThreeVector(0. *cm, 0. *cm, 0. *cm));
    SetDetectorPosition();
    //GetDetectorToWorldPosition();	
}

/////////////////////////////////////////////////////////////////////////////
// ConstructPhantom() is the method that construct a water box (called phantom
// (or water phantom) in the usual Medical physicists slang).
// A water phantom can be considered a good approximation of a an human body.
void PassiveProtonBeamLine::ConstructPhantom()
{
    // Definition of the solid volume of the Phantom
    phantom = new G4Box("Phantom", phantomSizeX/2, phantomSizeY/2, phantomSizeZ/2);
    
    // Definition of the logical volume of the Phantom
    phantomLogicalVolume = new G4LogicalVolume(phantom, phantomMaterial,
                                               "phantomLog", 0, 0, 0);
    
    // Definition of the physics volume of the Phantom
    phantomPhysicalVolume 
       = new G4PVPlacement(0, phantomPosition, "phantomPhys",
                           phantomLogicalVolume, motherPhys, false,0);
    
    // Visualisation attributes of the phantom
    red = new G4VisAttributes(G4Colour(255/255., 0/255. ,0/255.));
    red -> SetVisibility(true);
    red -> SetForceSolid(true);
    red -> SetForceWireframe(true);
    phantomLogicalVolume -> SetVisAttributes(red);
}

/////////////////////////////////////////////////////////////////////////////
// ConstructDetector() is the method the reconstruct a detector region
// inside the water phantom. It is a volume, located inside the water phantom.
//
//           **************************
//           *    water phantom       *
//           *                        *
//           *                        *
//           *---------------         *
//  Beam     *              -         *
//  ----->   * detector     -         *
//           *              -         *
//           *---------------         *
//           *                        *
//           *                        *
//           *                        *
//           **************************
//
// The detector can be dived in slices or voxelized and inside it different 
// quantities (dose distribution, fluence distribution, LET, etc) can be stored.
void PassiveProtonBeamLine::ConstructDetector()

{
    // Definition of the solid volume of the Detector
    detector = new G4Box("Detector", phantomSizeX/2, phantomSizeY/2, phantomSizeZ/2);
    
    // Definition of the logic volume of the Phantom
    detectorLogicalVolume = new G4LogicalVolume(detector, detectorMaterial,
                                                "DetectorLog",0,0,0);
    // Definition of the physical volume of the Phantom
    detectorPhysicalVolume = new G4PVPlacement(0,
                                               detectorPosition, // Setted by displacement
                                               "DetectorPhys",
                                               detectorLogicalVolume,
                                               phantomPhysicalVolume,
                                               false,0);
    
    // Visualisation attributes of the detector
    skyBlue = new G4VisAttributes( G4Colour(135/255. , 206/255. ,  235/255. ));
    skyBlue -> SetVisibility(true);
    skyBlue -> SetForceSolid(true);
    //skyBlue -> SetForceWireframe(true); //This makes only boundaries/edges visible
    detectorLogicalVolume -> SetVisAttributes(skyBlue);
    
    // **************
    // **************
    // Cut per Region
    // **************
    //
    // A smaller cut is fixed in the phantom to calculate the energy deposit with the
    // required accuracy
    if (!aRegion)
    {
        aRegion = new G4Region("DetectorLog");
        detectorLogicalVolume -> SetRegion(aRegion);
        aRegion->AddRootLogicalVolume( detectorLogicalVolume );
    }
}

///////////////////////////////////////////////////////////////////////
void PassiveProtonBeamLine::InitializeDetectorROGeometry(HadrontherapyDetectorROGeometry* ROloc,
                                                         G4ThreeVector detectorToWorldPosition)
{
    ROloc->Initialize(detectorToWorldPosition,
                   detectorSizeX/2, detectorSizeY/2, detectorSizeZ/2,
                   numberOfVoxelsAlongX, numberOfVoxelsAlongY, numberOfVoxelsAlongZ);
}
void PassiveProtonBeamLine::VirtualLayer(G4bool Varbool)
{
   
    //Virtual  plane
    VirtualLayerPosition = G4ThreeVector(0*cm,0*cm,0*cm);
    NewSource= Varbool;
    if(NewSource == true)
    {
       // std::cout<<"trr"<<std::endl;
        G4Material* airNist =  G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");
        
        solidVirtualLayer = new G4Box("VirtualLayer", 1.*um, 20.*cm, 40.*cm);
        
        logicVirtualLayer = new G4LogicalVolume(solidVirtualLayer,
                                                airNist, "VirtualLayer");
        
        physVirtualLayer= new G4PVPlacement(0,VirtualLayerPosition,
                                            "VirtualLayer", logicVirtualLayer,
                                            motherPhys, false, 0);
        
        logicVirtualLayer -> SetVisAttributes(skyBlue);
    }
}


///////////////////////////////////////////////////////////////////////
void  PassiveProtonBeamLine::ParametersCheck()
{
    // Check phantom/detector sizes & relative position
    if (!IsInside(detectorSizeX, detectorSizeY, detectorSizeZ,
                  phantomSizeX, phantomSizeY, phantomSizeZ,
                  detectorToPhantomPosition) )
        G4Exception("HadrontherapyDetectorConstruction::ParametersCheck()", "Hadrontherapy0001", FatalException, "Error: Detector is not fully inside Phantom!");
    
    // Check Detector sizes respect to the voxel ones
    
    if ( detectorSizeX < sizeOfVoxelAlongX) {
        G4Exception("HadrontherapyDetectorConstruction::ParametersCheck()", "Hadrontherapy0002", FatalException, "Error:  Detector X size must be bigger or equal than that of Voxel X!");
    }
    if ( detectorSizeY < sizeOfVoxelAlongY) {
        G4Exception(" HadrontherapyDetectorConstruction::ParametersCheck()", "Hadrontherapy0003", FatalException, "Error:  Detector Y size must be bigger or equal than that of Voxel Y!");
    }
    if ( detectorSizeZ < sizeOfVoxelAlongZ) {
        G4Exception(" HadrontherapyDetectorConstruction::ParametersCheck()", "Hadrontherapy0004", FatalException, "Error:  Detector Z size must be bigger or equal than that of Voxel Z!");
    }
}

///////////////////////////////////////////////////////////////////////
G4bool PassiveProtonBeamLine::SetPhantomMaterial(G4String material)
{
    //kp: From http://www.apc.univ-paris7.fr/~franco/g4doxy/html/classG4NistManager.html
    //   G4Element * 	FindOrBuildElement (const G4String &symb, G4bool isotopes=true)
    if (G4Material* pMat = G4NistManager::Instance()->FindOrBuildMaterial(material, false) )
    {
        phantomMaterial  = pMat;
        detectorMaterial = pMat;
        if (detectorLogicalVolume && phantomLogicalVolume)
        {
            detectorLogicalVolume -> SetMaterial(pMat);
            phantomLogicalVolume ->  SetMaterial(pMat);
            
            G4RunManager::GetRunManager() -> PhysicsHasBeenModified();
            G4RunManager::GetRunManager() -> GeometryHasBeenModified();
            G4cout << "The material of Phantom/Detector has been changed to " << material << G4endl;
        }
    }
    else
    {
        G4cout << "WARNING: material \"" << material << "\" doesn't exist in NIST elements/materials"
        " table [located in $G4INSTALL/source/materials/src/G4NistMaterialBuilder.cc]" << G4endl;
        G4cout << "Use command \"/parameter/nist\" to see full materials list!" << G4endl;
        return false;
    }
    
    return true;
}
/////////////////////////////////////////////////////////////////////////////
void PassiveProtonBeamLine::SetPhantomSize(G4double sizeX, G4double sizeY, G4double sizeZ)
{
    if (sizeX > 0.) phantomSizeX = sizeX;
    if (sizeY > 0.) phantomSizeY = sizeY;
    if (sizeZ > 0.) phantomSizeZ = sizeZ;
}

/////////////////////////////////////////////////////////////////////////////
void PassiveProtonBeamLine::SetDetectorSize(G4double sizeX, G4double sizeY, G4double sizeZ)
{
    if (sizeX > 0.) {detectorSizeX = sizeX;}
    if (sizeY > 0.) {detectorSizeY = sizeY;}
    if (sizeZ > 0.) {detectorSizeZ = sizeZ;}
    SetVoxelSize(sizeOfVoxelAlongX, sizeOfVoxelAlongY, sizeOfVoxelAlongZ);
}

/////////////////////////////////////////////////////////////////////////////
void PassiveProtonBeamLine::SetVoxelSize(G4double sizeX, G4double sizeY, G4double sizeZ)
{
    if (sizeX > 0.) {sizeOfVoxelAlongX = sizeX;}
    if (sizeY > 0.) {sizeOfVoxelAlongY = sizeY;}
    if (sizeZ > 0.) {sizeOfVoxelAlongZ = sizeZ;}
}

///////////////////////////////////////////////////////////////////////
void PassiveProtonBeamLine::SetPhantomPosition(G4ThreeVector pos)
{
    phantomPosition = pos;
}

/////////////////////////////////////////////////////////////////////////////
void PassiveProtonBeamLine::SetDetectorToPhantomPosition(G4ThreeVector displ)
{
    detectorToPhantomPosition = displ;
}

void PassiveProtonBeamLine::SetVirtualLayerPosition(G4ThreeVector position)
{
    
    VirtualLayerPosition = position;
    physVirtualLayer->SetTranslation(VirtualLayerPosition);
    
}
/////////////////////////////////////////////////////////////////////////////
void PassiveProtonBeamLine::UpdateGeometry()
{
    /*
     * Check parameters consistency
     */
    ParametersCheck();
    
    G4GeometryManager::GetInstance() -> OpenGeometry();
    if (phantom)
    {
        phantom -> SetXHalfLength(phantomSizeX/2);
        phantom -> SetYHalfLength(phantomSizeY/2);
        phantom -> SetZHalfLength(phantomSizeZ/2);
        
        phantomPhysicalVolume -> SetTranslation(phantomPosition);
    }
    else   ConstructPhantom();
    
    
    // Get the center of the detector
    SetDetectorPosition();
    if (detector)
    {
        
        detector -> SetXHalfLength(detectorSizeX/2);
        detector -> SetYHalfLength(detectorSizeY/2);
        detector -> SetZHalfLength(detectorSizeZ/2);
        
        detectorPhysicalVolume -> SetTranslation(detectorPosition);
    }
    else    ConstructDetector();    
    
    
    
    // Round to nearest integer number of voxel
    numberOfVoxelsAlongX = G4lrint(detectorSizeX / sizeOfVoxelAlongX);
    sizeOfVoxelAlongX = ( detectorSizeX / numberOfVoxelsAlongX );
    numberOfVoxelsAlongY = G4lrint(detectorSizeY / sizeOfVoxelAlongY);
    sizeOfVoxelAlongY = ( detectorSizeY / numberOfVoxelsAlongY );
    numberOfVoxelsAlongZ = G4lrint(detectorSizeZ / sizeOfVoxelAlongZ);
    sizeOfVoxelAlongZ = ( detectorSizeZ / numberOfVoxelsAlongZ );
    PassiveProtonBeamLine *ppbl= (PassiveProtonBeamLine*) 
           G4RunManager::GetRunManager()->GetUserDetectorConstruction();
    
    HadrontherapyDetectorROGeometry* ROloc = (HadrontherapyDetectorROGeometry*) ppbl->GetParallelWorld(0);
    
    //Set parameters, either for the Construct() or for the UpdateROGeometry()
    ROloc->Initialize(GetDetectorToWorldPosition(),
                   detectorSizeX/2, detectorSizeY/2, detectorSizeZ/2,
                   numberOfVoxelsAlongX, numberOfVoxelsAlongY, numberOfVoxelsAlongZ);
    
    //This method below has an effect only if the RO geometry is already built.
    ROloc->UpdateROGeometry();
    
    
    
    volumeOfVoxel = sizeOfVoxelAlongX * sizeOfVoxelAlongY * sizeOfVoxelAlongZ;
    massOfVoxel = detectorMaterial -> GetDensity() * volumeOfVoxel;
    //  This will clear the existing matrix (together with all data inside it)!
    matrix = HadrontherapyMatrix::GetInstance(numberOfVoxelsAlongX,
                                              numberOfVoxelsAlongY,
                                              numberOfVoxelsAlongZ,
                                              massOfVoxel);
    
    
    // Initialize RBE
    //HadrontherapyRBE::CreateInstance(numberOfVoxelsAlongX, numberOfVoxelsAlongY, numberOfVoxelsAlongZ, massOfVoxel);
    
    // Comment out the line below if let calculation is not needed!
    // Initialize LET with energy of primaries and clear data inside
    if ( (let = HadrontherapyLet::GetInstance(this)) )
    {
        HadrontherapyLet::GetInstance() -> Initialize();
    }
    
    
    // Initialize analysis
    // Inform the kernel about the new geometry
    G4RunManager::GetRunManager() -> GeometryHasBeenModified();
    G4RunManager::GetRunManager() -> PhysicsHasBeenModified();
    
    PrintParameters();
    
    // CheckOverlaps();
}

/////////////////////////////////////////////////////////////////////////////
//Check of the geometry
/////////////////////////////////////////////////////////////////////////////
void PassiveProtonBeamLine::CheckOverlaps()
{
    G4PhysicalVolumeStore* thePVStore = G4PhysicalVolumeStore::GetInstance();
    G4cout << thePVStore->size() << " physical volumes are defined" << G4endl;
    G4bool overlapFlag = false;
    G4int res=1000;
    G4double tol=0.; //tolerance
    for (size_t i=0;i<thePVStore->size();i++)
    {
        //overlapFlag = (*thePVStore)[i]->CheckOverlaps(res,tol,fCheckOverlapsVerbosity) | overlapFlag;
        overlapFlag = (*thePVStore)[i]->CheckOverlaps(res,tol,true) | overlapFlag;    }
    if (overlapFlag)
        G4cout << "Check: there are overlapping volumes" << G4endl;
}

/////////////////////////////////////////////////////////////////////////////
void PassiveProtonBeamLine::PrintParameters()
{
    
    G4cout << "The (X,Y,Z) dimensions of the phantom are : (" <<
    G4BestUnit( phantom -> GetXHalfLength()*2., "Length") << ',' <<
    G4BestUnit( phantom -> GetYHalfLength()*2., "Length") << ',' <<
    G4BestUnit( phantom -> GetZHalfLength()*2., "Length") << ')' << G4endl;
    
    G4cout << "The (X,Y,Z) dimensions of the detector are : (" <<
    G4BestUnit( detector -> GetXHalfLength()*2., "Length") << ',' <<
    G4BestUnit( detector -> GetYHalfLength()*2., "Length") << ',' <<
    G4BestUnit( detector -> GetZHalfLength()*2., "Length") << ')' << G4endl;
    
    G4cout << "Displacement between Phantom and World is: ";
    G4cout << "DX= "<< G4BestUnit(phantomPosition.getX(),"Length") <<
    "DY= "<< G4BestUnit(phantomPosition.getY(),"Length") <<
    "DZ= "<< G4BestUnit(phantomPosition.getZ(),"Length") << G4endl;
    
    G4cout << "The (X,Y,Z) sizes of the Voxels are: (" <<
    G4BestUnit(sizeOfVoxelAlongX, "Length")  << ',' <<
    G4BestUnit(sizeOfVoxelAlongY, "Length")  << ',' <<
    G4BestUnit(sizeOfVoxelAlongZ, "Length") << ')' << G4endl;
    
    G4cout << "The number of Voxels along (X,Y,Z) is: (" <<
    numberOfVoxelsAlongX  << ',' <<
    numberOfVoxelsAlongY  <<','  <<
    numberOfVoxelsAlongZ  << ')' << G4endl;
}
