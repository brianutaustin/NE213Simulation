#include "simNE213EventAction.hh"

#include "simNE213RunAction.hh"
#include "simNE213SteppingAction.hh"
  // use of stepping action to get and reset accumulated energy  

#include "G4RunManager.hh"
#include "G4Event.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

simNE213EventAction* simNE213EventAction::fgInstance = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

simNE213EventAction* simNE213EventAction::Instance()
{
// Static acces function via G4RunManager 

  return fgInstance;
}      

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

simNE213EventAction::simNE213EventAction()
: G4UserEventAction(),
  fPrintModulo(1000),
  fEnergySum(0.),
  fEnergy2Sum(0.),
  fNeutronEnteringSum(0.),
  fNeutronNoInteractionSum(0.),
  fInteractionsNPSum(0.),
  fInteractionsNCSum(0.),
  fInteractionsNBe9Sum(0.),
  fTripleAlphaSum(0.),
  fProtonLeavingSum(0.),
  fCarbonLeavingSum(0.),
  fTransferedEnergyCarbonSum(0.),
  fTransferedEnergyProtonSum(0.),
  fLostEnergyProtonSum(0.),
  fGammaProductionSum(0.),
  fTransferedEnergyGammaSum(0.),
  fLostEnergyGammaSum(0.)
{ 
  fgInstance = this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

simNE213EventAction::~simNE213EventAction()
{ 
  fgInstance = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void simNE213EventAction::BeginOfEventAction(const G4Event* event)
{  
  G4int eventNb = event->GetEventID();
  if (eventNb%fPrintModulo == 0) { 
    G4cout << "\n---> Begin of event: " << eventNb << G4endl;
  }
 
  // Reset accounted energy in stepping action
  simNE213SteppingAction::Instance()->Reset();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void simNE213EventAction::EndOfEventAction(const G4Event* event)
{
  // accumulate statistics
  G4double energy = simNE213SteppingAction::Instance()->GetEnergy();
  fEnergySum  += energy;
  fEnergy2Sum += energy*energy;

  G4double light = simNE213SteppingAction::Instance()->GetLight();
  fLightDistribution->AddValue(light,1.);
  G4int eventNb = event->GetEventID();
  if (eventNb%fPrintModulo == 0) { 
    G4cout << "\n---> End of event: " << eventNb <<", light produced : "<<light<<" MeVee"<<G4endl; 
  }

  G4double photonLight = simNE213SteppingAction::Instance()->GetPhotonLight();
  fPhotonLightDistribution->AddValue(photonLight,1.);

  G4double neutronEntering = simNE213SteppingAction::Instance()->GetNeutronEntering();
  fNeutronEnteringSum += neutronEntering;
  G4double neutronNoInteraction = simNE213SteppingAction::Instance()->GetNeutronNoInteraction();
  fNeutronNoInteractionSum += neutronNoInteraction;

  G4double interactionsNP = simNE213SteppingAction::Instance()->GetInteractionsNP();
  fInteractionsNPSum += interactionsNP;
  G4double interactionsNC = simNE213SteppingAction::Instance()->GetInteractionsNC();
  fInteractionsNCSum += interactionsNC;
  G4double interactionsNBe9 = simNE213SteppingAction::Instance()->GetInteractionsNBe9();
  fInteractionsNBe9Sum += interactionsNBe9;
  G4double tripleAlpha = simNE213SteppingAction::Instance()->GetTripleAlpha();
  fTripleAlphaSum += tripleAlpha;

  G4double protonLeaving = simNE213SteppingAction::Instance()->GetProtonLeaving();
  fProtonLeavingSum += protonLeaving;
  G4double carbonLeaving = simNE213SteppingAction::Instance()->GetCarbonLeaving();
  fCarbonLeavingSum += carbonLeaving;

  G4double transferedEnergyCarbon = simNE213SteppingAction::Instance()->GetTransferedEnergyCarbon();
  fTransferedEnergyCarbonSum += transferedEnergyCarbon;
  G4double transferedEnergyProton = simNE213SteppingAction::Instance()->GetTransferedEnergyProton();
  fTransferedEnergyProtonSum += transferedEnergyProton;
  G4double lostEnergyProton = simNE213SteppingAction::Instance()->GetLostEnergyProton();
  fLostEnergyProtonSum += lostEnergyProton;

  G4double gammaProduction = simNE213SteppingAction::Instance()->GetGammaProduction();
  fGammaProductionSum += gammaProduction;
  G4double transferedEnergyGamma = simNE213SteppingAction::Instance()->GetTransferedEnergyGamma();
  fTransferedEnergyGammaSum += transferedEnergyGamma;
  G4double lostEnergyGamma = simNE213SteppingAction::Instance()->GetLostEnergyGamma();
  fLostEnergyGammaSum += lostEnergyGamma;


}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void simNE213EventAction::Reset()
{
  //reset cumulative quantities
  //
  fEnergySum = 0.;
  fEnergy2Sum = 0.;
  fNeutronEnteringSum = 0.;
  fNeutronNoInteractionSum = 0.;
  fInteractionsNPSum = 0.;
  fInteractionsNCSum = 0.;
  fInteractionsNBe9Sum = 0.;
  fTripleAlphaSum = 0.;
  fProtonLeavingSum = 0.;
  fCarbonLeavingSum = 0.;
  fTransferedEnergyCarbonSum = 0.;
  fTransferedEnergyProtonSum = 0.;
  fLostEnergyProtonSum = 0.;
  fGammaProductionSum = 0.;
  fTransferedEnergyGammaSum = 0.;
  fLostEnergyGammaSum = 0.;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
