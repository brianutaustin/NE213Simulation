#include "simNE213PrimaryGeneratorMessenger.hh"
#include "simNE213PrimaryGeneratorAction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

simNE213PrimaryGeneratorMessenger::simNE213PrimaryGeneratorMessenger(simNE213PrimaryGeneratorAction* primaryGun)
  :primary(primaryGun)
{ 
  XpositionCmd = new G4UIcmdWithADoubleAndUnit("/beam/position/Xposition",this);
  XpositionCmd -> SetGuidance("set x coordinate of particle");
  XpositionCmd -> SetParameterName("position",false);
  XpositionCmd -> SetDefaultUnit("mm");
  XpositionCmd -> SetUnitCandidates("mm cm m");
  XpositionCmd -> AvailableForStates(G4State_PreInit,G4State_Idle);   

  YpositionCmd = new G4UIcmdWithADoubleAndUnit("/beam/position/Yposition",this);
  YpositionCmd -> SetGuidance("set y coordinate of particle");
  YpositionCmd -> SetParameterName("position",false);
  YpositionCmd -> SetDefaultUnit("mm");
  YpositionCmd -> SetUnitCandidates("mm cm m");
  YpositionCmd -> AvailableForStates(G4State_PreInit,G4State_Idle);     

  ZpositionCmd = new G4UIcmdWithADoubleAndUnit("/beam/position/Zposition",this);
  ZpositionCmd -> SetGuidance("set z coordinate of particle");
  ZpositionCmd -> SetParameterName("position",false);
  ZpositionCmd -> SetDefaultUnit("mm");
  ZpositionCmd -> SetUnitCandidates("mm cm m");
  ZpositionCmd -> AvailableForStates(G4State_PreInit,G4State_Idle); 

  volumeCmd = new G4UIcmdWithAString("/beam/position/distribution",this);
  volumeCmd->SetGuidance("Choose the type of volume for the source");
  volumeCmd->SetGuidance("Choice : Point, Disc, Circle, GaussianDisc ");
  volumeCmd->SetParameterName("choice",true);
  volumeCmd->SetCandidates("Point Disc Circle GaussianDisc");
  volumeCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  radiusCmd = new G4UIcmdWithADoubleAndUnit("/beam/position/radius",this);
  radiusCmd -> SetGuidance("set radius for circle and disc distribution");
  radiusCmd -> SetParameterName("position",false);
  radiusCmd -> SetDefaultUnit("mm");
  radiusCmd -> SetUnitCandidates("mm cm m");
  radiusCmd -> AvailableForStates(G4State_PreInit,G4State_Idle);

  beamCmd = new G4UIcmdWithAString("/beam/direction",this);
  beamCmd->SetGuidance("Choose the type of beam");
  beamCmd->SetGuidance("Choice : default, Isotropic, ConeBeam ");
  beamCmd->SetParameterName("choice",true);
  beamCmd->SetCandidates("default Isotropic ConeBeam");
  beamCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  angleCmd = new G4UIcmdWithADouble("/beam/direction/angle",this);
  angleCmd -> SetGuidance("Choose the angle for cone beam");
  angleCmd -> SetParameterName("Angle",false);
  angleCmd -> AvailableForStates(G4State_PreInit,G4State_Idle);

  energyDistributionCmd = new G4UIcmdWithAString("/beam/energy/distribution",this);
  energyDistributionCmd->SetGuidance("Choose the type of Energy distribution");
  energyDistributionCmd->SetGuidance("Choice : default, gaussian, AmBe, PuC, Cf252");
  energyDistributionCmd->SetParameterName("choice",true);
  energyDistributionCmd->SetCandidates("default gaussian AmBe PuC Cf252");
  energyDistributionCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  meanKineticEnergyCmd = new G4UIcmdWithADoubleAndUnit("/beam/energy/meanEnergy",this);
  meanKineticEnergyCmd -> SetGuidance("set mean Kinetic energy");
  meanKineticEnergyCmd -> SetParameterName("Energy",false);
  meanKineticEnergyCmd -> SetDefaultUnit("MeV");
  meanKineticEnergyCmd -> SetUnitCandidates("eV keV MeV GeV TeV");
  meanKineticEnergyCmd -> AvailableForStates(G4State_PreInit,G4State_Idle);

  sigmaKineticEnergyCmd = new G4UIcmdWithADoubleAndUnit("/beam/energy/sigmaEnergy",this);
  sigmaKineticEnergyCmd -> SetGuidance("set Kinetic energy standard deviation");
  sigmaKineticEnergyCmd -> SetParameterName("Energy",false);
  sigmaKineticEnergyCmd -> SetDefaultUnit("MeV");
  sigmaKineticEnergyCmd -> SetUnitCandidates("eV keV MeV GeV TeV");
  sigmaKineticEnergyCmd -> AvailableForStates(G4State_PreInit,G4State_Idle);

}

simNE213PrimaryGeneratorMessenger::~simNE213PrimaryGeneratorMessenger()
{
  delete XpositionCmd; 
  delete YpositionCmd; 
  delete ZpositionCmd;
  delete volumeCmd;
  delete radiusCmd;
  delete beamCmd;
  delete angleCmd;
  delete energyDistributionCmd;
  delete meanKineticEnergyCmd;
  delete sigmaKineticEnergyCmd;
}

void simNE213PrimaryGeneratorMessenger::SetNewValue(G4UIcommand * command,G4String newValue)
{ 
  if ( command == XpositionCmd )                                                                        
    { primary -> SetXposition(XpositionCmd -> GetNewDoubleValue(newValue));}

  else if ( command == YpositionCmd )                                                                        
    { primary -> SetYposition(YpositionCmd -> GetNewDoubleValue(newValue));}

  else if ( command == ZpositionCmd )                                                                        
    { primary -> SetZposition(ZpositionCmd -> GetNewDoubleValue(newValue));}

  else if( command == volumeCmd)
   { primary->SetVolume(newValue);}

  else if( command == radiusCmd)
   { primary->SetRadius(radiusCmd -> GetNewDoubleValue(newValue));}

  else if( command == beamCmd )
   { primary->SetBeam(newValue); }

  else if( command == angleCmd)
    { primary->SetAngle(angleCmd -> GetNewDoubleValue(newValue));}

  else if( command == energyDistributionCmd)
   { primary->SetEnergyDistribution(newValue);}

  else if (command == meanKineticEnergyCmd)
   { primary-> SetMeanKineticEnergy(meanKineticEnergyCmd -> GetNewDoubleValue(newValue));}

  else if (command == sigmaKineticEnergyCmd)
    {primary -> SetSigmaKineticEnergy(sigmaKineticEnergyCmd -> GetNewDoubleValue(newValue));}

}




