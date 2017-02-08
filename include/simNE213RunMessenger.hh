#ifndef simNE213RunMessenger_h
#define simNE213RunMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class simNE213RunAction;

class G4UIcmdWithADouble;
class G4UIcmdWithAnInteger;

class simNE213RunMessenger: public G4UImessenger
{
  public:
    simNE213RunMessenger(simNE213RunAction*);
    ~simNE213RunMessenger();

   void SetNewValue(G4UIcommand*, G4String);

  private:
    simNE213RunAction*    rAct;
    G4UIcmdWithADouble*    minLightGridCmd;
    G4UIcmdWithADouble*    maxLightGridCmd;
    G4UIcmdWithAnInteger*  channelNbLightGridCmd;
};

#endif
