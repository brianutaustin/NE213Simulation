#include "simNE213SteppingAction.hh"
#include "simNE213SteppingMessenger.hh"

#include "simNE213DetectorConstruction.hh"
#include "simNE213RunAction.hh"

#include "G4Step.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"

#include "Randomize.hh"


/* ---------------------------- Fonctions de transformation en lumiere ---------------------------- */

G4double pulseHeightResolution(G4double L)
{
/*	G4double A = 2.25;
	G4double B = 9.31;	// valeurs PTB
	G4double C = 0.6;
*/
	G4double A = 0.082659587;
	G4double B = 0.267349531;	// valeurs Francois
	G4double C = 0.054844220;

	G4double value = L*std::sqrt(A*A+B*B/L+C*C/(L*L));
	return value;
}

G4double genericLightOutput(G4double a1, G4double a2, G4double a3, G4double a4,
							G4double E)
{
  G4double value;
  value = a1*E-a2*(1-exp(-a3*pow(E,a4)));
  return value;
}

G4double carbonLightOutput(G4double Ec)
{
	G4double light, resolution;
	G4double value(0.);
	light = genericLightOutput(0.071,0.,0.,0.,Ec);
	resolution = pulseHeightResolution(light);
	value = G4RandGauss::shoot(light,resolution/2.355);
	return value;
}

G4double hydrogenLightOutput(G4double Ep)
{
	G4double light, resolution;
	G4double value(0.);
	light = genericLightOutput(0.83,2.82,0.25,0.93,Ep);
	resolution = pulseHeightResolution(light);
	value = G4RandGauss::shoot(light,resolution/2.355);
	return value;
}

G4double alphaLightOutput(G4double Ea)
{
	G4double light, resolution;
	G4double value(0.);
	light = genericLightOutput(0.41,5.9,0.065,1.01,Ea);
	resolution = pulseHeightResolution(light);
	value = G4RandGauss::shoot(light,resolution/2.355);
	return value;
}

G4double electronLightOutput(G4double Ee)
{
	G4double light, resolution;
	G4double value(0.);
	light = Ee;
	resolution = pulseHeightResolution(light);
	value = G4RandGauss::shoot(light,resolution/2.355);
	return value;
}

/* ------------------------------------------------------------------------------------------- */

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

simNE213SteppingAction* simNE213SteppingAction::fgInstance = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

simNE213SteppingAction* simNE213SteppingAction::Instance()
{
// Static acces function via G4RunManager

  return fgInstance;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

simNE213SteppingAction::simNE213SteppingAction()
: G4UserSteppingAction(),
  fVolume(0),
  fEnergy(0.),
  fLight(0.),
  fPhotonLight(0.),
  fNeutronEntering(0.),
  fNeutronNoInteraction(0.),
  fInteractionsNP(0.),
  fInteractionsNC(0.),
  fInteractionsNBe9(0.),
  fProtonLeaving(0.),
  fCarbonLeaving(0.),
  fTransferedEnergyCarbon(0.),
  fTransferedEnergyProton(0.),
  fLostEnergyProton(0.),
  fGammaProduction(0.),
  fTransferedEnergyGamma(0.),
  fLostEnergyGamma(0.)
{
  steppingMessenger = new simNE213SteppingMessenger(this);

  fgInstance = this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

simNE213SteppingAction::~simNE213SteppingAction()
{
  delete steppingMessenger;

  fgInstance = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// Variables Locales

  // Comptabilité - Interactions

  G4int nPrimaryNeutrons = 0;			// Nombre de neutrons entrant dans le détecteur
  G4double nNoEvent = 0;			// Nombre de neutrons sans interactions dans le scintillateur

  G4double nEvent_nc = 0;			// Nombre d'interactions n-C
  G4double nEvent_np = 0;			// Nombre d'interactions n-p
  G4double nTripleAlpha = 0;			// Nombre de réactions n-3*alpha
  G4double nEvent_Be8 = 0;
  G4double nEvent_B12 = 0;

  G4double nProton_out = 0;			// Nombre de protons sortant du détecteur
  G4double nCarbon_out = 0;			// Nombre de carbones sortant du détecteur

  // Mémorisations temporaires
  G4ThreeVector firstInteractionPosition;	// Vecteur pour mémorisation du premier PostStepPoint du neutron primaire
  G4ThreeVector secondaryPosition;		// Vecteur pour mémorisation des PreStepPoint des particules secondaires
  G4ThreeVector firstDisplacement;		// Trajectoire d'entrée du neutron primaire
  G4ThreeVector lastDisplacement;		// Trajectoire de sortie du neutron primaire

  G4int alphaID1(0);				// Stockage de l'ID du premier alpha
  G4int alphaID2(0);				// Stockage de l'ID du deuxième alpha
  G4int alpha_tmp(0);				// Nombre d'alphas rencontrés lors d'un cycle

  G4int gammaID(0);

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void simNE213SteppingAction::UserSteppingAction(const G4Step* step)
{
  // get volume of the current step
  G4LogicalVolume* volume
    = step->GetPreStepPoint()->GetTouchableHandle()
      ->GetVolume()->GetLogicalVolume();

  // check if we are in scoring volume
  if (volume != fVolume ) return;

  // collect energy and track length step by step
  G4double edep = step->GetTotalEnergyDeposit();
  fEnergy += edep;


// Nicolas DESPLAN 11/10/2012

  G4StepPoint* point1 = step->GetPreStepPoint(); // pointeur sur PreStepPoint
  G4StepPoint* point2 = step->GetPostStepPoint(); // pointeur sur PostStepPoint
  G4Track* track = step->GetTrack();
  G4int trackID = track->GetTrackID();
  G4String particleName = track->GetParticleDefinition()->GetParticleName();

  //G4double particleVelocity = track->GetVelocity();
  G4double particleKineticEnergy = track->GetKineticEnergy();
  G4double particleMass = point2->GetMass();
  G4ThreeVector particleMomentum = track->GetMomentum();

  // Séquence de calcul
  if (bCalc == true)
  {
    // Stockage temporaire des positions d'intérêt
    if (point1->GetStepStatus() == fGeomBoundary && trackID == 1) // Si le neutron est bien primaire et si c'est son premier chemin
    {
	firstInteractionPosition = point2->GetPosition(); // On stocke la position de la première interaction du neutron primaire
    }
    else
    {
	secondaryPosition = point1->GetPosition(); // Dans les autres cas, on prend la position de création de la particule pour la comparer
    }								 // avec celle du neutron primaire.


    // Comptage des neutrons sans interaction
    if (point1->GetStepStatus() == fGeomBoundary && trackID == 1) // Si le neutron primaire vient tout juste d'entrer,
    {
	firstDisplacement = step->GetDeltaPosition(); // on stocke sa trajectoire !
	nPrimaryNeutrons += 1; // comptage local des neutrons primaires
	fNeutronEntering = 1; // comptage global
    }
    if (point2->GetStepStatus() == fGeomBoundary && trackID == 1) //Si le neutron primaire vient tout juste de sortir,
    {
	lastDisplacement = step->GetDeltaPosition(); // on stocke aussi sa trajectoire !
    }
    if (firstDisplacement == lastDisplacement && trackID == 1) // et si les deux sont égales, alors il ne s'est rien passé
    {
	fNeutronNoInteraction = 1; // global
	nNoEvent += 1; // local
    }


    // Comptage des différentes interactions avec les neutrons primaires

  if (firstInteractionPosition == secondaryPosition) // si on a concordance des positions
    {
	// 1 - Protons
	if (particleName == "proton") // et que c'est un proton, on le compte
	{
		fInteractionsNP = 1.; // globalement via Run et EventAction
		nEvent_np += 1; // localement via SteppingAction
		fTransferedEnergyProton += track->GetVertexKineticEnergy(); // et on prend son énergie
	}
	// 2 - Carbones
	else if (particleName == "C12[0.0]") // Idem pour le carbone,
	{
		fInteractionsNC = 1.;
		nEvent_nc += 1;
		fTransferedEnergyCarbon += track->GetVertexKineticEnergy();
	}
	// 3 - Production de Be9
	else if (particleName == "Be9[0.0]") // le béryllium 9
	{
		fInteractionsNBe9 = 1.;
	}
     // 4 - Comptage des réactions triple alpha
    if (trackID == 1) // Remise à zéro avec le nouveau neutron primaire
    {
	alphaID1 = 0;
	alphaID2 = 0;
	alpha_tmp = 0;
    }
    if (particleName == "alpha" && alphaID1 == 0 && trackID != alphaID1) // Pour le 1er alpha
    {
	alphaID1 = trackID; // Stockage de son ID pour éviter les doubles comptes
	alpha_tmp += 1; // 1 alpha dans cette séquence
    }
    if (particleName == "alpha" && alphaID1 != 0 && alphaID2 == 0 && alphaID1 != trackID) // Pour le 2e (s'il existe)
    {
	alphaID2 = trackID;
	alpha_tmp += 1;
    }
    if (particleName == "alpha" && alphaID1 != 0 && alphaID2 != 0 && alphaID1 != trackID && alphaID2 != trackID) // Pour le 3e (s'il existe)
    {
	alphaID2 = trackID;
	alpha_tmp += 1;
    }
    if (alpha_tmp == 3) // Si 3 alphas ont été aperçus, on compte une réaction n + C -> n' + 3 alphas
    {
	nTripleAlpha += 1; // local
	fTripleAlpha += 1; // global
    }
    }



    // Comptage des carbones et protons qui sortent du détecteur.
    if (point2->GetStepStatus() == fGeomBoundary && particleName == "proton") // Si la particule quitte le détecteur et que c'est un proton, on la compte
    {
	fProtonLeaving = 1; // global
	nProton_out += 1;  // local
	fLostEnergyProton += particleKineticEnergy; // énergie qui quitte le détecteur par les protons
    }
    if (point2->GetStepStatus() == fGeomBoundary && particleName == "C12[0.0]") // Idem pour le carbone
    {
	fCarbonLeaving = 1; // global
	nCarbon_out += 1; // local
    }

    // Comptage des gammas apparaissant et quittant le détecteur
    if (trackID == 1){ gammaID = 0; }
    if (particleName == "gamma" and trackID != gammaID)
    {
	gammaID = trackID;
	fGammaProduction = 1;
	fTransferedEnergyGamma += track->GetVertexKineticEnergy();
    }
    if (particleName == "gamma" and point2->GetStepStatus() == fGeomBoundary)
    {
	fLostEnergyGamma += particleKineticEnergy;
    }
  }

  // calc light emitted by n-H and n-C collisions
  if (bSpectrum ==  true)
  {
  static G4int trackID2 = 0;
  G4int currentTrackID = track->GetTrackID();

  if (trackID2!=currentTrackID) {
	trackID2 = currentTrackID;
	G4double initialEnergy = track->GetVertexKineticEnergy(); // energie incidente
	G4double light = 0.;

  if (particleName=="proton"){
		light=hydrogenLightOutput(initialEnergy);
		fLight+=light;
	}
	else if (particleName=="alpha"){
		light=alphaLightOutput(initialEnergy);
		fLight+=light;
	}
	else if (particleName=="C12[0.0]"){
		light=carbonLightOutput(initialEnergy);
		fLight+=light;
	}
	else if (particleName=="C13[0.0]"){
		light=carbonLightOutput(initialEnergy);
		fLight+=light;
	}
	else if (particleName=="e-"){
		light=electronLightOutput(initialEnergy);
		fLight+=light;
	}
	else if (particleName=="opticalphoton"){
		fPhotonLight+= 4250.0*initialEnergy;
	}
	else
	{
		//G4cout<<"Particle : "<<particleName<<", unknowed reaction that could create that!"<<G4endl;
	}
  }
  }

  //G4VProcess* particleProcess = point2->GetProcessDefinedStep();

  // Séquence d'affichage (A modifier en fonction de ce que l'on souhaite afficher !)
  if (bPrint == true)
  {
  G4cout<<"Track ID:"<<trackID
  <<", Particle name : "<<particleName
  <<", PreStepPoint : "<<point1->GetPosition()
  <<" PostStepPoint : "<<point2->GetPosition()<<G4endl
  <<", Kinetic Energy (Post) = "<<particleKineticEnergy
  <<", Energy Deposit = "<<edep
  <<", Mass = "<<particleMass<<G4endl;
  /*<<"Number of primary neutron : "<<nPrimaryNeutrons<<G4endl
  <<"Number of neutron passing through : "<<nNoEvent<<G4endl
  <<"Number of (n,p) interactions: "<<nEvent_np<<G4endl
  <<"Number of (n,C) interactions: "<<nEvent_nc<<G4endl
  <<"Number of triple alpha reactions : "<<nTripleAlpha<<G4endl
  <<"Number of protons leaving detector : "<<nProton_out<<G4endl
  <<"Number of carbons leaving detector : "<<nCarbon_out<<G4endl;*/
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void simNE213SteppingAction::SetPrintStepping(G4bool value)
{bPrint = value;}

void simNE213SteppingAction::SetCalcStepping(G4bool value)
{bCalc = value;}

void simNE213SteppingAction::SetSpectrumStepping(G4bool value)
{bSpectrum = value;}

void simNE213SteppingAction::Reset()
{
  fEnergy = 0.;
  fLight = 0.;
  fPhotonLight = 0.;
  fNeutronEntering = 0.;
  fNeutronNoInteraction = 0.;
  fInteractionsNP = 0.;
  fInteractionsNC = 0.;
  fInteractionsNBe9 = 0.;
  fTripleAlpha = 0;
  fProtonLeaving = 0.;
  fCarbonLeaving = 0.;
  fTransferedEnergyCarbon = 0.;
  fTransferedEnergyProton = 0.;
  fLostEnergyProton = 0.;
  fGammaProduction = 0.;
  fTransferedEnergyGamma = 0.;
  fLostEnergyGamma = 0.;
}
