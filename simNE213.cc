#include "simNE213DetectorConstruction.hh"
#include "simNE213PrimaryGeneratorAction.hh"
#include "simNE213RunAction.hh"
#include "simNE213EventAction.hh"
#include "simNE213SteppingAction.hh"

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4VModularPhysicsList.hh"
#include "G4OpticalPhysics.hh"
#include "QGSP_BIC_HP.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

#include "Randomize.hh"


int main(int argc,char** argv)
{
  // Choose the Random engine
  //
  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);

  // Construct the default run manager
  //
  G4RunManager * runManager = new G4RunManager;

  // Set mandatory initialization classes
  //
  // Detector construction
  runManager->SetUserInitialization(new simNE213DetectorConstruction());

  // Physics list
  G4VModularPhysicsList* physics = new QGSP_BIC_HP;
  runManager->SetUserInitialization(physics);

    // Optical Physics
    G4OpticalPhysics* simNE213OpticalPhysics = new G4OpticalPhysics();
    physics->RegisterPhysics( simNE213OpticalPhysics );

    simNE213OpticalPhysics->SetWLSTimeProfile("delta");

    simNE213OpticalPhysics->SetScintillationYieldFactor(1.0);
    simNE213OpticalPhysics->SetScintillationExcitationRatio(0.0);

    simNE213OpticalPhysics->SetMaxNumPhotonsPerStep(100);
    simNE213OpticalPhysics->SetMaxBetaChangePerStep(10.0);

    //simNE213OpticalPhysics->SetTrackSecondariesFirst(kCerenkov,true);
    simNE213OpticalPhysics->SetTrackSecondariesFirst(kScintillation,true);

  // Primary generator action
  runManager->SetUserAction(new simNE213PrimaryGeneratorAction());

  // Set user action classes
  //
  // Stepping action
  runManager->SetUserAction(new simNE213SteppingAction());

  // Event action
  runManager->SetUserAction(new simNE213EventAction());

  // Run action
  runManager->SetUserAction(new simNE213RunAction());

  // Initialize G4 kernel
  //
  runManager->Initialize();

#ifdef G4VIS_USE
  // Initialize visualization
  G4VisManager* visManager = new G4VisExecutive;
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  // G4VisManager* visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();
#endif

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  if (argc!=1) {
    // batch mode
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command+fileName);
  }
  else {
    // interactive mode : define UI session
#ifdef G4UI_USE
    G4UIExecutive* ui = new G4UIExecutive(argc, argv);
#ifdef G4VIS_USE
    UImanager->ApplyCommand("/control/execute init_vis.mac");
#else
    UImanager->ApplyCommand("/control/execute init.mac");
#endif
    UImanager->ApplyCommand("/control/execute ./Defaults.mac");
    ui->SessionStart();
    delete ui;
#endif
  }

  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  // owned and deleted by the run manager, so they should not be deleted
  // in the main() program !

#ifdef G4VIS_USE
  delete visManager;
#endif
  delete runManager;

  return 0;
}
