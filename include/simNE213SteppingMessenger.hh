#ifndef simNE213SteppingMessenger_h
#define simNE213SteppingMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class simNE213SteppingAction;

class G4UIcmdWithABool;

class simNE213SteppingMessenger: public G4UImessenger
{
  public:
    simNE213SteppingMessenger(simNE213SteppingAction*);
    ~simNE213SteppingMessenger();

   void SetNewValue(G4UIcommand*, G4String);

  private:
    simNE213SteppingAction*    sAct;
    G4UIcmdWithABool*  		 printSteppingCmd;
    G4UIcmdWithABool*		 calcSteppingCmd;
    G4UIcmdWithABool*		 spectrumSteppingCmd;
};

#endif
