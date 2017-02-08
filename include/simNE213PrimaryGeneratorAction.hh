#ifndef simNE213PrimaryGeneratorAction_h
#define simNE213PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"

class G4ParticleGun;
class G4Event;
class simNE213DetectorConstruction;
class simNE213PrimaryGeneratorMessenger;

/// The primary generator action class with particle gun.
///
/// The default kinematic is a 14 MeV neutron, point source (-2,0,0) 

class simNE213PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  private:
    static simNE213PrimaryGeneratorAction* fgInstance;
    G4ParticleGun*  fParticleGun; // pointer a to G4 gun class
    
    void SetDefaultPrimaryParticle();	

    G4double X0;
    G4double Y0;
    G4double Z0;
    G4double radius;
    G4double angle;

    G4String volumeKind;
    G4String beamKind;
    G4String energyDistributionKind;
    G4double meanKineticEnergy;
    G4double sigmaKineticEnergy;

    simNE213PrimaryGeneratorMessenger* messenger;


  public:
    simNE213PrimaryGeneratorAction();    
    virtual ~simNE213PrimaryGeneratorAction();
    static const simNE213PrimaryGeneratorAction* Instance();
    void GeneratePrimaries(G4Event*);         
    const G4ParticleGun* GetParticleGun() const { return fParticleGun; }


    void SetXposition(G4double);
    void SetYposition(G4double);
    void SetZposition(G4double);

    void GenerateDefaultPoint();
    void GenerateDisc();
    void GenerateCircle();
    void GenerateGaussianDisc();
    void SetVolume(G4String);
    void SetRadius(G4double);

    void GenerateDefaultBeam();
    void GenerateIsotropicFlux();
    void GenerateConeBeam();
    void SetBeam(G4String);
    void SetAngle(G4double);

    void GenerateDefaultEnergyDistribution();
    void GenerateGaussianEnergyDistribution();
    void GenerateAmBeEnergyDistribution();
    void GeneratePuCEnergyDistribution();
    void GenerateCf252EnergyDistribution();
    void GenerateAlternateGamma();
    void SetEnergyDistribution(G4String);
    void SetMeanKineticEnergy(G4double);
    void SetSigmaKineticEnergy(G4double);
    const G4String GetEnergyDistribution(void) const { return energyDistributionKind; }
    const G4double GetMeanKineticEnergy(void) const { return meanKineticEnergy; }
    const G4double GetSigmaKineticEnergy(void) const { return sigmaKineticEnergy; }

};

#endif


