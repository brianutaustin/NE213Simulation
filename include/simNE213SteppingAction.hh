#ifndef simNE213SteppingAction_h
#define simNE213SteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "globals.hh"
#include "grid.hh"

class G4LogicalVolume;
class simNE213SteppingMessenger;

/// Stepping action class
/// 
/// It holds data member fEnergy for accumulating the energy deposit
/// in a selected volume step by step.
/// The selected volume is set from  the detector construction via the  
/// SetVolume() function. The accumulated energy deposit is reset for each 
/// new event via the Reset() function from the event action.

class simNE213SteppingAction : public G4UserSteppingAction
{
  public:
    simNE213SteppingAction();
    virtual ~simNE213SteppingAction();

    // static access method
    static simNE213SteppingAction* Instance();

    // method from the base class
    virtual void UserSteppingAction(const G4Step*);

    // reset accumulated energy
    void Reset();

    // set methods
    void SetVolume(G4LogicalVolume* volume) { fVolume = volume; }
    void SetPrintStepping(G4bool);
    void SetCalcStepping(G4bool);
    void SetSpectrumStepping(G4bool);
  
    // get methods
    G4LogicalVolume* GetVolume() const { return fVolume; }
    G4double GetEnergy() const { return fEnergy; }
    G4double GetNeutronEntering() const { return fNeutronEntering; }
    G4double GetNeutronNoInteraction() const {return fNeutronNoInteraction; }
    G4double GetInteractionsNP() const { return fInteractionsNP; }
    G4double GetInteractionsNC() const { return fInteractionsNC; }
    G4double GetInteractionsNBe9() const { return fInteractionsNBe9; }
    G4double GetTripleAlpha() const { return fTripleAlpha; }
    G4double GetProtonLeaving() const { return fProtonLeaving; }
    G4double GetCarbonLeaving() const { return fCarbonLeaving; }
    G4double GetTransferedEnergyCarbon() const { return fTransferedEnergyCarbon; }
    G4double GetTransferedEnergyProton() const { return fTransferedEnergyProton; }
    G4double GetLostEnergyProton() const { return fLostEnergyProton; }
    G4double GetGammaProduction() const { return fGammaProduction; }
    G4double GetTransferedEnergyGamma() const { return fTransferedEnergyGamma; }
    G4double GetLostEnergyGamma() const { return fLostEnergyGamma; }

    G4double GetLight() const { return fLight; }
    G4double GetPhotonLight() const { return fPhotonLight; }

    G4bool GetCalcStepping() const {return bCalc; }
    G4bool GetSpectrumStepping() const { return bSpectrum; }
   
  private:
    static simNE213SteppingAction* fgInstance;  

    simNE213SteppingMessenger* steppingMessenger;
    G4bool bPrint;
    G4bool bCalc;
    G4bool bSpectrum;
  
    G4LogicalVolume* fVolume;
    G4double  fEnergy;
    G4double  fLight;
    G4double  fPhotonLight;
    G4double  fNeutronEntering;
    G4double  fNeutronNoInteraction;
    G4double  fInteractionsNP;
    G4double  fInteractionsNC;
    G4double  fInteractionsNBe9;
    G4double  fTripleAlpha;
    G4double  fProtonLeaving;
    G4double  fCarbonLeaving;
    G4double  fTransferedEnergyCarbon;
    G4double  fTransferedEnergyProton;
    G4double  fLostEnergyProton;
    G4double  fGammaProduction;
    G4double  fTransferedEnergyGamma;
    G4double  fLostEnergyGamma;
};


#endif
