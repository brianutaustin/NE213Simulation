#include "simNE213RunAction.hh"
#include "simNE213RunMessenger.hh"
#include "simNE213PrimaryGeneratorAction.hh"
#include "simNE213EventAction.hh"
#include "simNE213SteppingAction.hh"
#include "grid.hh"
#include "G4SystemOfUnits.hh"

  // use of other actions
  // - primary generator: to get info for printing about the primary
  // - event action: to get and reset accumulated energy sums
  // - stepping action: to get info about accounting volume

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

simNE213RunAction::simNE213RunAction()
: G4UserRunAction()
{
  // add new units for dose
  //
  const G4double milligray = 1.e-3*gray;
  const G4double microgray = 1.e-6*gray;
  const G4double nanogray  = 1.e-9*gray;
  const G4double picogray  = 1.e-12*gray;

  new G4UnitDefinition("milligray", "milliGy" , "Dose", milligray);
  new G4UnitDefinition("microgray", "microGy" , "Dose", microgray);
  new G4UnitDefinition("nanogray" , "nanoGy"  , "Dose", nanogray);
  new G4UnitDefinition("picogray" , "picoGy"  , "Dose", picogray);

  runMessenger = new simNE213RunMessenger(this);

  SetDefaultLightGrid();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

simNE213RunAction::~simNE213RunAction()
{
  delete runMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....


void simNE213RunAction::SetDefaultLightGrid()
{
  // grille lumière [0, 20 MeVee] en 1000 canaux

  G4double defaultMinLightGrid = 0.;
  minLightGrid = defaultMinLightGrid;

  G4double defaultMaxLightGrid = 20.;
  maxLightGrid = defaultMaxLightGrid;

  G4int defaultChannelNbLightGrid = 1000;
  channelNbLightGrid = defaultChannelNbLightGrid;
}

void simNE213RunAction::SetMinLightGrid(G4double val)
{minLightGrid = val;}

void simNE213RunAction::SetMaxLightGrid(G4double val)
{maxLightGrid = val;}

void simNE213RunAction::SetChannelNbLightGrid(G4int val)
{channelNbLightGrid = val;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void simNE213RunAction::BeginOfRunAction(const G4Run* aRun)
{
  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;

  //inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);

  //initialize event cumulative quantities
  simNE213EventAction::Instance()->Reset();

  // light grid
  lightDistribution = new Grid1D(minLightGrid, maxLightGrid, channelNbLightGrid);
  simNE213EventAction::Instance()->SetLightDistribution(lightDistribution);
  G4cout << "### Set light grid distribution to  ["
  <<minLightGrid<<", "
  <<maxLightGrid<<" MeVee] in "
  <<channelNbLightGrid<<" channels."
  << G4endl;

  // photon light grid
  photonLightDistribution = new Grid1D(minLightGrid, maxLightGrid, channelNbLightGrid);
  simNE213EventAction::Instance()->SetPhotonLightDistribution(photonLightDistribution);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void simNE213RunAction::EndOfRunAction(const G4Run* aRun)
{
  G4int nofEvents = aRun->GetNumberOfEvent();
  if (nofEvents == 0) return;

  // Compute dose
  //
  G4double energySum  = simNE213EventAction::Instance()->GetEnergySum();
  G4double energy2Sum = simNE213EventAction::Instance()->GetEnergy2Sum();
  G4double rms = energy2Sum - energySum*energySum/nofEvents;
  if (rms > 0.) rms = std::sqrt(rms); else rms = 0.;

  G4double mass = simNE213SteppingAction::Instance()->GetVolume()->GetMass();
  G4double dose = energySum/mass;
  G4double rmsDose = rms/mass;

  // Compute number of secondary particles - interactions
  //
  G4double neutronEnteringSum = simNE213EventAction::Instance()->GetNeutronEnteringSum();
  G4double neutronNoInteractionSum = simNE213EventAction::Instance()->GetNeutronNoInteractionSum();

  G4double interactionsNPSum = simNE213EventAction::Instance()->GetInteractionsNPSum();
  G4double interactionsNCSum = simNE213EventAction::Instance()->GetInteractionsNCSum();
  G4double interactionsNBe9Sum = simNE213EventAction::Instance()->GetInteractionsNBe9Sum();
  G4double tripleAlphaSum = simNE213EventAction::Instance()->GetTripleAlpha();

  G4double protonLeavingSum = simNE213EventAction::Instance()->GetProtonLeavingSum();
  G4double carbonLeavingSum = simNE213EventAction::Instance()->GetCarbonLeavingSum();

  G4double transferedEnergyCarbonSum = simNE213EventAction::Instance()->GetTransferedEnergyCarbonSum();
  G4double transferedEnergyProtonSum = simNE213EventAction::Instance()->GetTransferedEnergyProtonSum();
  G4double lostEnergyProtonSum = simNE213EventAction::Instance()->GetLostEnergyProtonSum();

  G4double gammaProductionSum = simNE213EventAction::Instance()->GetGammaProductionSum();
  G4double transferedEnergyGammaSum = simNE213EventAction::Instance()->GetTransferedEnergyGammaSum();
  G4double lostEnergyGammaSum = simNE213EventAction::Instance()->GetLostEnergyGammaSum();

  // Run conditions

  const G4ParticleGun* particleGun
    = simNE213PrimaryGeneratorAction::Instance()->GetParticleGun();
  G4String particleName
    = particleGun->GetParticleDefinition()->GetParticleName();
  G4double particleEnergy = simNE213PrimaryGeneratorAction::Instance()->GetMeanKineticEnergy();
  G4double particleSigmaEnergy = simNE213PrimaryGeneratorAction::Instance()->GetSigmaKineticEnergy();
  G4String particleEnergyDistribution = simNE213PrimaryGeneratorAction::Instance()->GetEnergyDistribution();

  G4bool calc = simNE213SteppingAction::Instance()->GetCalcStepping();
  G4bool spectrum = simNE213SteppingAction::Instance()->GetSpectrumStepping();


  // Print
  //
  char filename[256];
  FILE *oFile = 0;
  sprintf(filename,"Output-%s-%fMeV.txt",particleName.data(),particleEnergy);
  oFile = fopen(filename,"w");

  char filename2[256];
  FILE *oFile2 = 0;
  sprintf(filename2,"Output[photon]-%s-%fMeV.txt",particleName.data(),particleEnergy);
  oFile2 = fopen(filename2,"w");



  G4cout
     << "\n--------------------End of Run------------------------------\n";
  if (particleEnergyDistribution == "default")
    {G4cout
     << " The run consists of " << nofEvents << " "<< particleName << " of "
     <<   G4BestUnit(particleEnergy,"Energy") << ".";
    }
  else if (particleEnergyDistribution == "gaussian")
    {G4cout
     << " The run consists of " << nofEvents << " "<< particleName << " of "
     <<   G4BestUnit(particleEnergy,"Energy") << " with a standard deviation of "
     <<   G4BestUnit(particleSigmaEnergy, "Energy") << " under a gaussian distribution.";
    }
  else if (particleEnergyDistribution == "AmBe")
    {G4cout
     << " The run consists of " << nofEvents << " "<< particleName << " from an Am-Be neutron source."
     <<G4endl;
    }
  else if (particleEnergyDistribution == "PuC")
    {G4cout
     << " The run consists of " << nofEvents << " "<< particleName << " from a Pu-C neutron source."
     <<G4endl;
    }
  else if (particleEnergyDistribution == "Cf252")
    {G4cout
     << " The run consists of " << nofEvents << " "<< particleName << " from a Cf252 neutron source."
     <<G4endl;
    }

  G4cout
     << "\n Dose in scoring volume "
     << simNE213SteppingAction::Instance()->GetVolume()->GetName() << " : "
     << G4BestUnit(dose,"Dose")
     << " +- "                   << G4BestUnit(rmsDose,"Dose")<< G4endl
     << G4endl
     << neutronEnteringSum << " neutrons have entered the scintillator and "
     << neutronNoInteractionSum  << " left without interacting." << G4endl
     << G4endl;
  if (calc == false) {G4cout << "Warning : Caculation sequence is deactivated!" << G4endl
			<<"	---> Activation by /stepping/calculation true."<< G4endl;}
  if (spectrum == false) {G4cout << "Warning : spectrum edition is deactivated!" << G4endl
			<<"	---> Activation by /stepping/spectrum true."<< G4endl;}
  G4cout
     << G4endl
     << "Have been registered : "<< G4endl
     << interactionsNPSum << " interactions n-p "<< G4endl
     << interactionsNCSum << " interactions n-C "<< G4endl
     << interactionsNBe9Sum << " interactions n + C -> Be + alpha" << G4endl
     << tripleAlphaSum << " interaction n + C -> n' + 3 alphas" << G4endl
     << G4endl
     << protonLeavingSum << " protons and " << carbonLeavingSum << " carbons left the detector with a positive kinetic energy" << G4endl
     << G4BestUnit(transferedEnergyCarbonSum,"Energy") << " have been transfered from neutrons to C12 and "
     << G4BestUnit(transferedEnergyProtonSum,"Energy") << " to protons whose "
     << G4BestUnit(lostEnergyProtonSum,"Energy") << " have been lost" << G4endl
     << G4endl
     << gammaProductionSum << " gammas have been created with a total energy of " << G4BestUnit(transferedEnergyGammaSum,"Energy")
     << " whose " << G4BestUnit(lostEnergyGammaSum,"Energy") << " have left the scintillator." << G4endl
     << "\n------------------------------------------------------------\n"
     << G4endl;



  if (oFile) {
	lightDistribution->Normalise();
	lightDistribution->Dump(oFile);
	fflush(oFile);
  } else {
	G4cout<< "\n--------------------Light Distribution------------------------------\n"<< G4endl;
	lightDistribution->Normalise();
	lightDistribution->Dump();
	G4cout<< "\n------------------------------------------------------------\n"<< G4endl;
  }
  delete lightDistribution;

  if (oFile2) {
	photonLightDistribution->Normalise();
	photonLightDistribution->Dump(oFile2);
	fflush(oFile2);
  } else {
	G4cout<< "\n--------------------Light Distribution------------------------------\n"<< G4endl;
	photonLightDistribution->Normalise();
	photonLightDistribution->Dump();
	G4cout<< "\n------------------------------------------------------------\n"<< G4endl;
  }
  delete photonLightDistribution;


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
