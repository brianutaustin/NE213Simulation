#include "simNE213DetectorConstruction.hh"
#include "simNE213SteppingAction.hh"
#include "G4SystemOfUnits.hh"
#include "G4NistManager.hh"
#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4Element.hh"
#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

simNE213DetectorConstruction::simNE213DetectorConstruction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

simNE213DetectorConstruction::~simNE213DetectorConstruction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void simNE213DetectorConstruction::DefineMaterials()
{
	// définition des matériaux utilisés  -------------------------------------

	// air
	G4NistManager* nist = G4NistManager::Instance();
	world_mat = nist->FindOrBuildMaterial("G4_AIR");

	// NE213 ou BC501-A (St Gobain)
	// NATCM3_H	4.82E-2
	// NATCM3_C	3.98E-2
	// RHO_NE213 0.874

	G4double a,density,fracMass,z;
	G4int ncomp;
	G4String symbol;

	// NE213
	a = 1.00794*g/mole;
	elHydrogen = new G4Element("Hydrogen", symbol="H",z=1.,a);

	a = 12.0107*g/mole;
	elCarbon = new G4Element("Carbon", symbol="C",z=6.,a);

	density = 0.874*g/cm3;
	NE213 = new G4Material("NE213",density,ncomp=2);
	NE213->AddElement(elHydrogen,fracMass=9.2*perCent);
	NE213->AddElement(elCarbon,fracMass=90.8*perCent);

	// Coque (Aluminium)
	a = 26.9815*g/mole;
	elAluminium = new G4Element("Aluminium", symbol="Al",z=13.,a);

	density = 2.6989*g/cm3;
	matContainerNE213 = new G4Material("Container NE213", density,ncomp=1);
	matContainerNE213->AddElement(elAluminium,fracMass=100.*perCent);

	// Verre (coque arrière)
	// SiO2 : a = 60.0843*g/mole;
	a = 28.0855*g/mole;
	elSilicium = new G4Element("Silicium", symbol="Si",z=14.,a);
	a = 15.9994*g/mole;
	elOxygen = new G4Element("Oxygen", symbol="O",z=8.,a);

	density = 2.648*g/cm3;
	Glass = new G4Material("Glass",density,ncomp=2);
	Glass->AddElement(elOxygen,2);
	Glass->AddElement(elSilicium,1);


	// Propriétés optiques des matériaux

	const G4int NE213_NUMENTRIES = 8;
	G4double NE213_Energy[NE213_NUMENTRIES] = { 2.48*eV, 2.61*eV, 2.76*eV, 2.84*eV, 2.92*eV, 3.10*eV, 3.20*eV, 3.31*eV };
	G4double NE213_SCINT[NE213_NUMENTRIES] = { 0.18, 0.36, 0.7, 0.85, 1.0, 0.65, 0.35, 0.05 };
	G4double NE213_RIND[NE213_NUMENTRIES]  = { 1.53, 1.53, 1.53, 1.53, 1.53, 1.53, 1.53, 1.53 };
	G4double NE213_ABSL[NE213_NUMENTRIES]  = { 7.0*m, 7.0*m, 7.0*m, 7.0*m, 7.0*m, 7.0*m, 7.0*m, 7.0*m }; // (lower value)

	NE213_mt = new G4MaterialPropertiesTable();
	NE213_mt->AddProperty("FASTCOMPONENT", NE213_Energy, NE213_SCINT, NE213_NUMENTRIES);
	NE213_mt->AddProperty("SLOWCOMPONENT", NE213_Energy, NE213_SCINT, NE213_NUMENTRIES);
	NE213_mt->AddProperty("RINDEX",        NE213_Energy, NE213_RIND,  NE213_NUMENTRIES);
	NE213_mt->AddProperty("ABSLENGTH",     NE213_Energy, NE213_ABSL,  NE213_NUMENTRIES);
	NE213_mt->AddConstProperty("SCINTILLATIONYIELD",0./MeV); // Set to zero if scintillation is not desired
	NE213_mt->AddConstProperty("RESOLUTIONSCALE",1.0);
	NE213_mt->AddConstProperty("FASTTIMECONSTANT",3.16*ns); // Saint Gobain
	NE213_mt->AddConstProperty("SLOWTIMECONSTANT",32.3*ns); // To be checked
	NE213_mt->AddConstProperty("YIELDRATIO",1.0);
	NE213->SetMaterialPropertiesTable(NE213_mt);

	NE213->GetIonisation()->SetBirksConstant(0.137*mm/MeV); // from

}

G4VPhysicalVolume* simNE213DetectorConstruction::Construct()
{
	DefineMaterials();
	return ConstructDetector();
}

G4VPhysicalVolume* simNE213DetectorConstruction::ConstructDetector()
{
	// définition de la géométrie -------------------------------------

	// World
	G4double world_sizeXY,world_sizeZ;

	world_sizeXY = world_sizeZ = 5.0*m;
	G4Box* worldBox = new G4Box("G4Box World",0.5*world_sizeXY,0.5*world_sizeXY,0.5*world_sizeZ);
	G4LogicalVolume* logicWorld = new G4LogicalVolume(worldBox,world_mat,"Logical World");
	G4VPhysicalVolume* physWorld = new G4PVPlacement(0, //no rotation
													G4ThreeVector(),       //at (0,0,0)
													logicWorld,            //its logical volume
													"Physical World",               //its name
													0,                     //its mother  volume
													false,                 //no boolean operation
													0,                     //copy number
													0);			//overlaps checking

	// scintillateur NE213 2"x2"
	G4Tubs* scintillatorTub = new G4Tubs("G4Tubs Scintillator", 0.0*2.54*cm, 1.*2.54*cm, 1.*2.54*cm, 0.*deg,360.*deg);
	G4LogicalVolume* logicScintillator = new G4LogicalVolume(scintillatorTub,NE213,"Logical Scintillator",0,0,0);
	G4VPhysicalVolume* physScintillator = new G4PVPlacement(0,G4ThreeVector(),logicScintillator,"Physical Scintillator",logicWorld,false,0);

	// Coque "Tour" Scintillateur NE213
	G4Tubs* scintillatorContainer = new G4Tubs("G4Tubs Container", 1.*2.54*cm, 1.*2.74*cm, 1.*2.74*cm, 0.*deg, 360.*deg);
	G4LogicalVolume* logicContainer = new G4LogicalVolume(scintillatorContainer,matContainerNE213, "Logical Container",0,0,0);
	G4VPhysicalVolume* physContainer = new G4PVPlacement(0,G4ThreeVector(),logicContainer,"Physical Container",logicWorld,false,0);

	// Coque "Avant" Scintillateur NE213
	G4Tubs* scintillatorContainerFront = new G4Tubs("G4Tubs Front Container", 0.*1.54*cm, 1.*2.54*cm, 1.*0.1*cm, 0.*deg, 360.*deg);
	G4LogicalVolume* logicContainerFront = new G4LogicalVolume(scintillatorContainerFront,matContainerNE213, "Logical Front Container",0,0,0);
	G4VPhysicalVolume* physContainerFront = new G4PVPlacement(0,G4ThreeVector(0,0,-1*2.64*cm),logicContainerFront,"Physical Front Container", logicWorld,false,0);

	// Coque "Arrière" Scintillateur NE213
	G4Tubs* scintillatorContainerBack = new G4Tubs("G4Tubs Back Container", 0.*2.54*cm, 1.*2.54*cm, 1.*0.1*cm, 0.*deg, 360.*deg);
	G4LogicalVolume* logicContainerBack = new G4LogicalVolume(scintillatorContainerBack,Glass, "Logical Back Container",0,0,0);
	G4VPhysicalVolume* physContainerBack = new G4PVPlacement(0,G4ThreeVector(0,0,1*2.64*cm),logicContainerBack,"Physical Back Container", logicWorld,false,0);

	// Photomultiplicateur
	G4Tubs* PMTub = new G4Tubs("G4Tubs PM Tub", 1.*2.54*cm, 1.*2.74*cm, 1.*5.0*cm, 0.*deg, 360.*deg); // Vérifier les dimensions
	G4LogicalVolume* logicPMTub = new G4LogicalVolume(PMTub,Glass, "logical PMTub",0,0,0);
	G4VPhysicalVolume* physPMTub = new G4PVPlacement(0,G4ThreeVector(0.,0.,1*7.74*cm),logicPMTub,"Physical PM Tub",logicWorld,false,0);

	// Vitre avant PM
	G4Tubs* PMTubFront = new G4Tubs("G4Tubs PM Tub Front", 0.*2.54*cm, 1.*2.54*cm, 1.*0.1*cm, 0.*deg, 360.*deg);
	G4LogicalVolume* logicPMTubFront = new G4LogicalVolume(PMTubFront,Glass, "Logical PM Tub Front",0,0,0);
	G4VPhysicalVolume* physPMTubFront = new G4PVPlacement(0,G4ThreeVector(0,0,1*2.84*cm),logicPMTubFront,"Physical PM Tub Front", logicWorld,false,0);


	// connexion avec l'objet stepping action
	// A partir de
	//
	simNE213SteppingAction* steppingAction = simNE213SteppingAction::Instance();
	steppingAction->SetVolume(logicScintillator);

	return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
