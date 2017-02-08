#include "simNE213ScintHit.hh"
#include "G4ios.hh"
#include "G4VVisManager.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"



G4Allocator<simNE213ScintHit> simNE213ScintHitAllocator;

simNE213ScintHit::simNE213ScintHit():physVol(0) {}

simNE213ScintHit::simNE213ScintHit(G4VPhysicalVolume* pVol):physVol(pVol) {}

simNE213ScintHit::~simNE213ScintHit() {}


simNE213ScintHit::simNE213ScintHit(const simNE213ScintHit &right)
  : G4VHit()
{
  edep = right.edep;
  pos = right.pos;
  physVol = right.physVol;
}



const simNE213ScintHit& simNE213ScintHit::operator=(const simNE213ScintHit &right){
  edep = right.edep;
  pos = right.pos;
  physVol = right.physVol;
  return *this;
}

G4int simNE213ScintHit::operator==(const simNE213ScintHit&) const{return false;}
  //returns false because there currently isnt need to check for equality yet

void simNE213ScintHit::Draw(){}

void simNE213ScintHit::Print(){}
