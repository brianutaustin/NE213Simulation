#include "simNE213ScintSD.hh"
#include "simNE213ScintHit.hh"

#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4ParticleDefinition.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4ios.hh"
#include "G4VProcess.hh"




simNE213ScintSD::simNE213ScintSD(G4String name):G4VSensitiveDetector(name)
{
	collectionName.insert("scintCollection");
}

simNE213ScintSD::~simNE213ScintSD() {}

void simNE213ScintSD::Initialize(G4HCofThisEvent* HCE)
{
	scintCollection = new simNE213ScintHitsCollection(SensitiveDetectorName,collectionName[0]); 
	//A way to keep all the hits of this event in one place if needed
	static G4int HCID = -1;
	if(HCID<0)
	{ 
		HCID = GetCollectionID(0); 
	}
  	HCE->AddHitsCollection( HCID, scintCollection );
}

G4bool simNE213ScintSD::ProcessHits(G4Step* aStep,G4TouchableHistory* )
{ 
	G4double edep = aStep->GetTotalEnergyDeposit();
	if(edep == 0.) return false; //No edep so not counted as hit

	G4StepPoint* thePrePoint = aStep->GetPreStepPoint();
	G4TouchableHistory* theTouchable = (G4TouchableHistory*)(aStep->GetPreStepPoint()->GetTouchable());
	G4VPhysicalVolume* thePrePV = theTouchable->GetVolume();
  
	G4StepPoint* thePostPoint = aStep->GetPostStepPoint();

	//Get the average position of the hit
	G4ThreeVector pos = thePrePoint->GetPosition() + thePostPoint->GetPosition();
	pos/=2.;

	simNE213ScintHit* scintHit = new simNE213ScintHit(thePrePV);

	scintHit->SetEdep(edep);
	scintHit->SetPos(pos);

	scintCollection->insert(scintHit);

	return true;
}
void simNE213ScintSD::EndOfEvent(G4HCofThisEvent* HCE) 
{}
void simNE213ScintSD::clear() 
{}
void simNE213ScintSD::DrawAll() 
{}
void simNE213ScintSD::PrintAll() 
{}




