#ifndef simNE213DetectorConstruction_h
#define simNE213DetectorConstruction_h 1

/// Detector construction class to define materials and geometry.

#include "G4VUserDetectorConstruction.hh"
#include "G4Material.hh"
#include "globals.hh"

class G4VPhysicalVolume;

class simNE213DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    simNE213DetectorConstruction();
    virtual ~simNE213DetectorConstruction();
    G4VPhysicalVolume* Construct();

  private:
    void DefineMaterials();
    G4VPhysicalVolume* ConstructDetector();

    G4Element* elHydrogen;
    G4Element* elCarbon;
    G4Element* elAluminium;
    G4Element* elOxygen;
    G4Element* elSilicium;

    G4Material* NE213;
    G4Material* matContainerNE213;
    G4Material* Glass;
    G4Material* world_mat;

    G4MaterialPropertiesTable* NE213_mt;
};

#endif

