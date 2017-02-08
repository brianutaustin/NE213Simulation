#ifndef simNE213EventAction_h
#define simNE213EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include "grid.hh"

/// Event action class
///
/// It holds data member fEnergySum and fEnergy2Sum for accumulating 
/// the event energy deposit its square event by event.
/// These data are then used in the run action to compute the dose.
/// The accumulated energy and enrgy square sums are reset for each 
/// new run via the Reset() function from the run action.

class simNE213EventAction : public G4UserEventAction
{
  public:
    simNE213EventAction();
    virtual ~simNE213EventAction();
    
    // static access method
    static simNE213EventAction* Instance();

    virtual void BeginOfEventAction(const G4Event* event);
    virtual void EndOfEventAction(const G4Event* event);

    void Reset();

    // get methods
    G4double GetEnergySum() const { return fEnergySum; }
    G4double GetEnergy2Sum() const { return fEnergy2Sum; }
    G4double GetNeutronEnteringSum() const { return fNeutronEnteringSum; }
    G4double GetNeutronNoInteractionSum() const { return fNeutronNoInteractionSum; }
    G4double GetInteractionsNPSum() const { return fInteractionsNPSum; }
    G4double GetInteractionsNCSum() const { return fInteractionsNCSum; }
    G4double GetInteractionsNBe9Sum() const { return fInteractionsNBe9Sum; }
    G4double GetTripleAlpha() const { return fTripleAlphaSum; }
    G4double GetProtonLeavingSum() const { return fProtonLeavingSum; }
    G4double GetCarbonLeavingSum() const { return fCarbonLeavingSum; }
    G4double GetTransferedEnergyCarbonSum() const { return fTransferedEnergyCarbonSum; }
    G4double GetTransferedEnergyProtonSum() const { return fTransferedEnergyProtonSum; }
    G4double GetLostEnergyProtonSum() const { return fLostEnergyProtonSum; }
    G4double GetGammaProductionSum() const { return fGammaProductionSum; }
    G4double GetTransferedEnergyGammaSum() const { return fTransferedEnergyGammaSum; }
    G4double GetLostEnergyGammaSum() const { return fLostEnergyGammaSum; }

    // set methods
    void SetLightDistribution(Grid1D* aDistribution){ fLightDistribution = aDistribution; }
    void SetPhotonLightDistribution(Grid1D* aDistribution) {fPhotonLightDistribution = aDistribution; }

  private:
    static simNE213EventAction* fgInstance;  

    G4int     fPrintModulo;
    G4double  fEnergySum;
    G4double  fEnergy2Sum;
    G4double  fNeutronEnteringSum;
    G4double  fNeutronNoInteractionSum;
    G4double  fInteractionsNPSum;
    G4double  fInteractionsNCSum;
    G4double  fInteractionsNBe9Sum;
    G4double  fTripleAlphaSum;
    G4double  fProtonLeavingSum;
    G4double  fCarbonLeavingSum;
    G4double  fTransferedEnergyCarbonSum;
    G4double  fTransferedEnergyProtonSum;
    G4double  fLostEnergyProtonSum;
    G4double  fGammaProductionSum;
    G4double  fTransferedEnergyGammaSum;
    G4double  fLostEnergyGammaSum;

    Grid1D*   fLightDistribution;
    Grid1D*   fPhotonLightDistribution;
};

#endif
