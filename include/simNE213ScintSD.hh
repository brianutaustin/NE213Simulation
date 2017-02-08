#ifndef simNE213ScintSD_h
#define simNE213ScintSD_h 1

#include "simNE213ScintHit.hh"

#include "G4VSensitiveDetector.hh"

class G4Step;
class G4HCofThisEvent;

class simNE213ScintSD : public G4VSensitiveDetector
{
public:
  simNE213ScintSD(G4String name);
  ~simNE213ScintSD();
  
  void Initialize(G4HCofThisEvent* HCE);
  G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist);
  void EndOfEvent(G4HCofThisEvent* HCE);
  void clear();
  void DrawAll();
  void PrintAll();
  
private:
  simNE213ScintHitsCollection* scintCollection;
  
};

#endif
