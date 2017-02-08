#ifndef simNE213RunAction_h
#define simNE213RunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"
#include "grid.hh"

class G4Run;
class simNE213RunMessenger;

/// Run action class
///
/// In EndOfRunAction(), it calculates the dose in the selected volume 
/// from the energy deposit accumulated via stepping and event actions.
/// The computed dose is then printed on the screen.

class simNE213RunAction : public G4UserRunAction
{
  public:
    simNE213RunAction();
    virtual ~simNE213RunAction();
    virtual void BeginOfRunAction(const G4Run*);
    virtual void   EndOfRunAction(const G4Run*);

    void SetMinLightGrid(G4double);
    void SetMaxLightGrid(G4double);
    void SetChannelNbLightGrid(G4int);
  protected:
    Grid1D *lightDistribution;
    Grid1D *photonLightDistribution;

  private:
    simNE213RunMessenger* runMessenger;
    G4double minLightGrid;
    G4double maxLightGrid;
    G4int    channelNbLightGrid;

    void SetDefaultLightGrid();
};


#endif

