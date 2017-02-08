#ifndef simNE213PrimaryGeneratorMessenger_h
#define simNE213PrimaryGeneratorMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"



class simNE213PrimaryGeneratorAction;

class G4UIcmdWithAString;
class G4UIcmdWithADouble;
class G4UIcmdWithADoubleAndUnit;

class simNE213PrimaryGeneratorMessenger: public G4UImessenger
{
  public:
  simNE213PrimaryGeneratorMessenger(simNE213PrimaryGeneratorAction*);
  ~simNE213PrimaryGeneratorMessenger();
  
  void SetNewValue(G4UIcommand*, G4String);
  
private:
  simNE213PrimaryGeneratorAction*    primary;
  G4UIcmdWithADoubleAndUnit*	    XpositionCmd;
  G4UIcmdWithADoubleAndUnit*	    YpositionCmd;
  G4UIcmdWithADoubleAndUnit* 	    ZpositionCmd;
  G4UIcmdWithAString*		    volumeCmd;
  G4UIcmdWithADoubleAndUnit*	    radiusCmd;
  G4UIcmdWithAString*               beamCmd;
  G4UIcmdWithADouble*		    angleCmd;
  G4UIcmdWithAString*		    energyDistributionCmd;
  G4UIcmdWithADoubleAndUnit*	    meanKineticEnergyCmd;
  G4UIcmdWithADoubleAndUnit*	    sigmaKineticEnergyCmd;
};

#endif

