#ifndef simNE213ScintHit_h
#define simNE213ScintHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"
#include "G4VPhysicalVolume.hh"

class simNE213ScintHit : public G4VHit
{
public:
  
  simNE213ScintHit();
  simNE213ScintHit(G4VPhysicalVolume* pVol);
  ~simNE213ScintHit();
  simNE213ScintHit(const simNE213ScintHit &right);
  const simNE213ScintHit& operator=(const simNE213ScintHit &right);
  G4int operator==(const simNE213ScintHit &right) const;

  inline void *operator new(size_t);
  inline void operator delete(void *aHit);
  
  void Draw();
  void Print();

  inline void SetEdep(G4double de) { edep = de; }  
  inline void AddEdep(G4double de) { edep += de; }
  inline G4double GetEdep() { return edep; }
  
  inline void SetPos(G4ThreeVector xyz) { pos = xyz; }
  inline G4ThreeVector GetPos() { return pos; }

  inline const G4VPhysicalVolume * GetPhysV() { return physVol; }

private:
  G4double edep;
  G4ThreeVector pos;
  const G4VPhysicalVolume* physVol;

};

typedef G4THitsCollection<simNE213ScintHit> simNE213ScintHitsCollection;

extern G4Allocator<simNE213ScintHit> simNE213ScintHitAllocator;

inline void* simNE213ScintHit::operator new(size_t)
{
  void *aHit;
  aHit = (void *) simNE213ScintHitAllocator.MallocSingle();
  return aHit;
}

inline void simNE213ScintHit::operator delete(void *aHit)
{
  simNE213ScintHitAllocator.FreeSingle((simNE213ScintHit*) aHit);
}

#endif
