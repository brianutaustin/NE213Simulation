#include "simNE213SteppingMessenger.hh"
#include "simNE213SteppingAction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithABool.hh"

simNE213SteppingMessenger::simNE213SteppingMessenger(simNE213SteppingAction* steppingAction):
sAct(steppingAction)
{
  printSteppingCmd = new G4UIcmdWithABool("/stepping/print", this);
  printSteppingCmd -> SetGuidance("Activation of printing sequence in SteppingAction");
  printSteppingCmd -> SetParameterName("Print",false);
  printSteppingCmd -> SetDefaultValue(false);
  printSteppingCmd -> AvailableForStates(G4State_PreInit,G4State_Idle);

  calcSteppingCmd = new G4UIcmdWithABool("/stepping/calculation", this);
  calcSteppingCmd -> SetGuidance("Activation of calculation sequence in SteppingAction");
  calcSteppingCmd -> SetParameterName("Calc",false);
  calcSteppingCmd -> SetDefaultValue(true);
  calcSteppingCmd -> AvailableForStates(G4State_PreInit,G4State_Idle);

  spectrumSteppingCmd = new G4UIcmdWithABool("/stepping/spectrum", this);
  spectrumSteppingCmd -> SetGuidance("Activation of spectrum construction in SteppingAction");
  spectrumSteppingCmd -> SetParameterName("Spectrum",false);
  spectrumSteppingCmd -> SetDefaultValue(true);
  spectrumSteppingCmd -> AvailableForStates(G4State_PreInit,G4State_Idle);
}

simNE213SteppingMessenger::~simNE213SteppingMessenger()
{
  delete printSteppingCmd;
  delete calcSteppingCmd;
  delete spectrumSteppingCmd;
}

void simNE213SteppingMessenger::SetNewValue(G4UIcommand * command,G4String newValue)
{
  if (command == printSteppingCmd)
    {sAct -> SetPrintStepping(printSteppingCmd -> GetNewBoolValue(newValue));}
  else if (command == calcSteppingCmd)
    {sAct -> SetCalcStepping(calcSteppingCmd -> GetNewBoolValue(newValue));}
  else if (command == spectrumSteppingCmd)
    {sAct -> SetSpectrumStepping(spectrumSteppingCmd -> GetNewBoolValue(newValue));}
}
