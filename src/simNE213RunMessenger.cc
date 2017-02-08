#include "simNE213RunMessenger.hh"
#include "simNE213RunAction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAnInteger.hh"

simNE213RunMessenger::simNE213RunMessenger(simNE213RunAction* primaryAction):
rAct(primaryAction)
{
  minLightGridCmd = new G4UIcmdWithADouble("/grid/minLightGrid", this);
  minLightGridCmd -> SetGuidance("Set minimum energy for the light grid");
  minLightGridCmd -> SetParameterName("Energy",false);
  minLightGridCmd -> AvailableForStates(G4State_PreInit,G4State_Idle);

  maxLightGridCmd = new G4UIcmdWithADouble("/grid/maxLightGrid", this);
  maxLightGridCmd -> SetGuidance("Set maximum energy for the light grid");
  maxLightGridCmd -> SetParameterName("Energy",false);
  maxLightGridCmd -> AvailableForStates(G4State_PreInit,G4State_Idle);

  channelNbLightGridCmd = new G4UIcmdWithAnInteger("/grid/channelNbLightGrid", this);
  channelNbLightGridCmd -> SetGuidance("Set number of channel for the light grid");
  channelNbLightGridCmd -> SetParameterName("Channel",false);
  channelNbLightGridCmd -> AvailableForStates(G4State_PreInit,G4State_Idle);
}

simNE213RunMessenger::~simNE213RunMessenger()
{
  delete minLightGridCmd;
  delete maxLightGridCmd;
  delete channelNbLightGridCmd;
}

void simNE213RunMessenger::SetNewValue(G4UIcommand * command,G4String newValue)
{
  if (command == minLightGridCmd)
    {rAct -> SetMinLightGrid(minLightGridCmd -> GetNewDoubleValue(newValue));}

  else if (command == maxLightGridCmd)
    {rAct -> SetMaxLightGrid(maxLightGridCmd -> GetNewDoubleValue(newValue));}

  else if(command == channelNbLightGridCmd)
    {rAct -> SetChannelNbLightGrid(channelNbLightGridCmd -> GetNewIntValue(newValue));}
}
