#include "simNE213PrimaryGeneratorAction.hh"
#include "simNE213PrimaryGeneratorMessenger.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"
#include "Randomize.hh"

simNE213PrimaryGeneratorAction* simNE213PrimaryGeneratorAction::fgInstance = 0;

simNE213PrimaryGeneratorAction::simNE213PrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(),fParticleGun(0),volumeKind("Point"),beamKind("default"),
  energyDistributionKind("default")

{
	G4int n_particle = 1;
	fParticleGun  = new G4ParticleGun(n_particle);

	messenger = new simNE213PrimaryGeneratorMessenger(this);

	SetDefaultPrimaryParticle();

	fgInstance = this;

}

void simNE213PrimaryGeneratorAction::SetDefaultPrimaryParticle()
{
		// Default Primary Particle

	// Define primary particles : neutrons
	G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
	G4String particleName;
	G4ParticleDefinition* particle = particleTable->FindParticle(particleName="neutron");
	fParticleGun->SetParticleDefinition(particle);

	// Define energy characteristics (mean, standard deviation)
	G4double defaultMeanKineticEnergy = 14.0*MeV;
	meanKineticEnergy = defaultMeanKineticEnergy;

	G4double defaultSigmaKineticEnergy = 0.*MeV;
	sigmaKineticEnergy = defaultSigmaKineticEnergy;

	// Gun initial position
	G4double defaultX0 = 0.;
	X0 = defaultX0;

	G4double defaultY0 = 0.;
	Y0 = defaultY0;

	G4double defaultZ0 = -1.*m;
	Z0 = defaultZ0;

	G4double defaultRadius = 2.54*cm;
	radius = defaultRadius;

	// Gun default angle (for cone beam only)
	G4double defaultAngle = 3.14159265/6;
	angle = defaultAngle;
}


simNE213PrimaryGeneratorAction::~simNE213PrimaryGeneratorAction()
{
	delete fParticleGun;
	delete messenger;
	fgInstance = 0;
}

void simNE213PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
	// Spatial distribution commands
	if (volumeKind == "Point") GenerateDefaultPoint();
	else if (volumeKind == "Disc") GenerateDisc();
	else if (volumeKind == "Circle") GenerateCircle();
	else if (volumeKind == "GaussianDisc") GenerateGaussianDisc();

	// Gun direction commands
	if (beamKind == "default") GenerateDefaultBeam();
	else if (beamKind == "Isotropic") GenerateIsotropicFlux();
	else if (beamKind == "ConeBeam") GenerateConeBeam();

	// Energy distribution commands
	if (energyDistributionKind == "default") GenerateDefaultEnergyDistribution();
	else if (energyDistributionKind == "gaussian") GenerateGaussianEnergyDistribution();
	else if (energyDistributionKind == "AmBe") GenerateAmBeEnergyDistribution();
	else if (energyDistributionKind == "PuC") GeneratePuCEnergyDistribution();
	else if (energyDistributionKind == "Cf252") GenerateCf252EnergyDistribution();

	fParticleGun->GeneratePrimaryVertex(anEvent);
}

const simNE213PrimaryGeneratorAction* simNE213PrimaryGeneratorAction::Instance()
{
	return fgInstance;
}

// Position of the particle gun

void simNE213PrimaryGeneratorAction::SetXposition (G4double val )
{ X0 = val;}

void simNE213PrimaryGeneratorAction::SetYposition (G4double val )
{ Y0 = val;}

void simNE213PrimaryGeneratorAction::SetZposition (G4double val )
{ Z0 = val;}


// Volume repartition of the particle gun

void simNE213PrimaryGeneratorAction::GenerateDefaultPoint()
{
	G4double x = X0;
	G4double y = Y0;
	G4double z = Z0;
	fParticleGun->SetParticlePosition(G4ThreeVector(x,y,z));
}

void simNE213PrimaryGeneratorAction::GenerateDisc() // Uniform distribution on a disc
{
	G4double theta = G4UniformRand()*2*3.14159265;
	G4double r = std::sqrt(G4UniformRand())*radius;
	G4double x = X0 + std::cos(theta)*r;
	G4double y = Y0 + std::sin(theta)*r;
 	G4double z = Z0 ;

	fParticleGun->SetParticlePosition(G4ThreeVector(x,y,z));
}

void simNE213PrimaryGeneratorAction::GenerateCircle()
{

	G4double theta = G4UniformRand()*2*3.14159265;
	G4double x = X0 + std::cos(theta)*radius;
	G4double y = Y0 + std::sin(theta)*radius;
 	G4double z = Z0 ;

	fParticleGun->SetParticlePosition(G4ThreeVector(x,y,z));
}

void simNE213PrimaryGeneratorAction::GenerateGaussianDisc()
{


	G4double theta = G4UniformRand()*2*3.14159265;
	G4double r = G4RandGauss::shoot(0,0.5*radius);
	r = std::sqrt(r);
	G4double x = X0 + std::cos(theta)*r;
	G4double y = Y0 + std::sin(theta)*r;
 	G4double z = Z0 ;
	fParticleGun->SetParticlePosition(G4ThreeVector(x,y,z));

}

void simNE213PrimaryGeneratorAction::SetVolume(G4String sVolume)
{
	if((sVolume == "Point" || sVolume == "Disc" || sVolume == "Circle" || sVolume == "GaussianDisc")) {volumeKind = sVolume; }
	else G4cout<<"This option is not valid "<<
               "---> Point"
             <<G4endl;
}

// Changing radius (for circle and disc geometries)
void simNE213PrimaryGeneratorAction::SetRadius(G4double val)
{ radius = val; }



// Beam of the particle gun

void simNE213PrimaryGeneratorAction::GenerateDefaultBeam()
{
	fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
}

void simNE213PrimaryGeneratorAction::GenerateIsotropicFlux()
{
  G4double a,b,c,mu,phi;

  mu = 2*G4UniformRand()-1; // cosinus de la colatitude ( mu = cos(theta) )
  phi = G4UniformRand()*2*3.14159265;  // longitude
  a = std::cos(phi)*std::sqrt(1-mu*mu);
  b = std::sin(phi)*std::sqrt(1-mu*mu);
  c = mu;

  G4ThreeVector direction(a,b,c);
  fParticleGun->SetParticleMomentumDirection(direction);
}

void simNE213PrimaryGeneratorAction::GenerateConeBeam()
{
	G4double theta = angle;
	G4double phi = G4UniformRand()*2*3.14159265;
	G4double r = std::sqrt(G4UniformRand())*tan(theta);
	G4double a = std::cos(phi)*r;
	G4double b = std::sin(phi)*r;
	G4double c = 1.;
	G4double n = a*a+b*b+c*c;
	a /= n;
	b /= n;
	c /= n;
	fParticleGun->SetParticleMomentumDirection(G4ThreeVector(a,b,c));
}

void simNE213PrimaryGeneratorAction::SetBeam(G4String beam)
{
  if((beam == "Isotropic" || beam == "default" || beam == "ConeBeam")) {beamKind = beam;}

  else G4cout<<"This option is not valid "<<
               "---> Isotropic"
             <<G4endl;
}

void simNE213PrimaryGeneratorAction::SetAngle(G4double val)
{angle = val;}



// Energy Distribution of the particle

void simNE213PrimaryGeneratorAction::GenerateDefaultEnergyDistribution()
{
	fParticleGun->SetParticleEnergy(meanKineticEnergy);
}

void simNE213PrimaryGeneratorAction::GenerateGaussianEnergyDistribution()
{
	G4double muE = meanKineticEnergy;
	G4double sigmaE = sigmaKineticEnergy;
	G4double kinE = G4RandGauss::shoot(muE,sigmaE);
	fParticleGun->SetParticleEnergy(kinE);
}

// Due to their length the other energy distibutions are implemented at the end of the file

void simNE213PrimaryGeneratorAction::SetEnergyDistribution(G4String sEnergyDistribution)
{
	if((sEnergyDistribution == "default" || sEnergyDistribution == "gaussian"
	|| sEnergyDistribution == "AmBe" || sEnergyDistribution == "PuC" || sEnergyDistribution == "Cf252"))
	   {energyDistributionKind = sEnergyDistribution;}

	else {
		G4cout<<"This option is not valid "<<"---> default"<<G4endl;
		energyDistributionKind = "default";
	     }
}

void simNE213PrimaryGeneratorAction::SetMeanKineticEnergy(G4double val)
{
	meanKineticEnergy = val;
}

void simNE213PrimaryGeneratorAction::SetSigmaKineticEnergy(G4double val)
{
	sigmaKineticEnergy = val;
}

void simNE213PrimaryGeneratorAction::GenerateAmBeEnergyDistribution(void)
{
	const G4int AmBe_n = 200;
	G4double AmBe_p[AmBe_n] = {
	1.210e-005, 	3.122e-005, 	4.296e-005, 	5.197e-005, 	7.260e-005, 	1.980e-004, 	5.212e-004, 	9.382e-004, 	1.343e-003, 	1.687e-003,
	1.978e-003, 	2.223e-003, 	2.448e-003, 	2.683e-003, 	2.878e-003, 	3.041e-003, 	3.212e-003, 	3.374e-003, 	3.424e-003, 	3.421e-003,
	3.418e-003, 	3.414e-003, 	3.407e-003, 	3.390e-003, 	3.354e-003, 	3.290e-003, 	3.192e-003, 	3.059e-003, 	2.907e-003, 	2.745e-003,
	2.580e-003, 	2.420e-003, 	2.267e-003, 	2.158e-003, 	2.221e-003, 	2.638e-003, 	3.055e-003, 	3.234e-003, 	3.334e-003, 	3.396e-003,
	3.478e-003, 	3.634e-003, 	3.806e-003, 	3.926e-003, 	3.990e-003, 	4.035e-003, 	4.094e-003, 	4.166e-003, 	4.251e-003, 	4.351e-003,
	4.469e-003, 	4.592e-003, 	4.862e-003, 	5.387e-003, 	5.804e-003, 	6.261e-003, 	6.800e-003, 	7.341e-003, 	7.831e-003, 	8.269e-003,
	8.701e-003, 	9.140e-003, 	9.558e-003, 	1.002e-002, 	1.051e-002, 	1.060e-002, 	1.049e-002, 	1.039e-002, 	1.031e-002, 	1.023e-002,
	1.016e-002, 	1.009e-002, 	1.001e-002, 	9.917e-003, 	9.806e-003, 	9.684e-003, 	9.561e-003, 	9.444e-003, 	9.337e-003, 	9.242e-003,
	9.158e-003, 	9.084e-003, 	9.017e-003, 	8.953e-003, 	8.887e-003, 	8.820e-003, 	8.754e-003, 	8.687e-003, 	8.618e-003, 	8.548e-003,
	8.476e-003, 	8.404e-003, 	8.327e-003, 	8.245e-003, 	8.159e-003, 	8.070e-003, 	7.980e-003, 	7.890e-003, 	7.793e-003, 	7.643e-003,
	7.351e-003, 	7.064e-003, 	6.820e-003, 	6.601e-003, 	6.388e-003, 	6.163e-003, 	5.922e-003, 	5.681e-003, 	5.472e-003, 	5.327e-003,
	5.224e-003, 	5.188e-003, 	5.185e-003, 	5.199e-003, 	5.205e-003, 	5.171e-003, 	5.135e-003, 	5.101e-003, 	5.091e-003, 	4.985e-003,
	4.737e-003, 	4.555e-003, 	4.415e-003, 	4.309e-003, 	4.232e-003, 	4.169e-003, 	4.091e-003, 	4.101e-003, 	4.252e-003, 	4.374e-003,
	4.489e-003, 	4.607e-003, 	4.711e-003, 	4.812e-003, 	4.902e-003, 	4.958e-003, 	4.960e-003, 	4.960e-003, 	4.960e-003, 	4.959e-003,
	4.958e-003, 	4.957e-003, 	4.956e-003, 	4.955e-003, 	4.954e-003, 	4.952e-003, 	4.949e-003, 	4.945e-003, 	4.939e-003, 	4.928e-003,
	4.909e-003, 	4.880e-003, 	4.844e-003, 	4.801e-003, 	4.754e-003, 	4.702e-003, 	4.647e-003, 	4.590e-003, 	4.523e-003, 	4.446e-003,
	4.368e-003, 	4.294e-003, 	4.223e-003, 	4.150e-003, 	4.077e-003, 	4.005e-003, 	3.938e-003, 	3.882e-003, 	3.835e-003, 	3.795e-003,
	3.756e-003, 	3.716e-003, 	3.677e-003, 	3.642e-003, 	3.611e-003, 	3.583e-003, 	3.556e-003, 	3.532e-003, 	3.511e-003, 	3.490e-003,
	3.463e-003, 	3.430e-003, 	3.391e-003, 	3.344e-003, 	3.286e-003, 	3.214e-003, 	3.127e-003, 	3.028e-003, 	2.916e-003, 	2.792e-003,
	2.664e-003, 	2.526e-003, 	2.373e-003, 	2.213e-003, 	2.040e-003, 	1.856e-003, 	1.665e-003, 	1.469e-003, 	1.269e-003, 	1.073e-003};

	G4int AmBe_k = 0;
	G4double tmp = 0.;
	G4double AmBe_r = G4UniformRand();
	while(AmBe_r > tmp)
	{
		tmp += AmBe_p[AmBe_k];
		AmBe_k += 1;
	}
	G4double kinE = (0.05*AmBe_k+0.025) + (G4UniformRand()-0.5)*0.05;
	fParticleGun->SetParticleEnergy(kinE);
}

void simNE213PrimaryGeneratorAction::GeneratePuCEnergyDistribution(void)
{
	const G4int PuC_n = 200;
	G4double PuC_p[PuC_n] = {
	5.212e-03, 	8.415e-03, 	8.661e-03, 	8.457e-03, 	8.046e-03, 	7.381e-03, 	6.488e-03, 	5.539e-03, 	4.581e-03, 	3.414e-03,
	2.473e-03, 	1.464e-03, 	6.020e-04, 	2.457e-04, 	8.261e-05, 	9.453e-06, 	9.342e-06, 	9.333e-06, 	9.306e-06, 	9.263e-06,
	9.206e-06, 	9.137e-06, 	9.055e-06, 	8.964e-06, 	8.863e-06, 	8.754e-06, 	8.638e-06, 	8.516e-06, 	8.388e-06, 	8.255e-06,
	8.119e-06, 	7.978e-06, 	7.835e-06, 	7.689e-06, 	7.541e-06, 	7.391e-06, 	7.240e-06, 	7.089e-06, 	6.937e-06, 	6.785e-06,
	6.728e-06, 	1.014e-05, 	1.297e-05, 	1.525e-05, 	1.738e-05, 	1.948e-05, 	2.163e-05, 	2.387e-05, 	2.634e-05, 	3.914e-05,
	5.123e-05, 	7.026e-05, 	1.125e-04, 	1.376e-04, 	1.703e-04, 	2.144e-04, 	2.844e-04, 	4.156e-04, 	6.527e-04, 	1.156e-03,
	1.853e-03, 	2.864e-03, 	4.041e-03, 	4.992e-03, 	5.471e-03, 	6.056e-03, 	6.634e-03, 	7.321e-03, 	7.624e-03, 	8.022e-03,
	8.614e-03, 	9.275e-03, 	1.050e-02, 	1.171e-02, 	1.221e-02, 	1.230e-02, 	1.248e-02, 	1.301e-02, 	1.314e-02, 	1.315e-02,
	1.321e-02, 	1.327e-02, 	1.327e-02, 	1.344e-02, 	1.374e-02, 	1.358e-02, 	1.357e-02, 	1.352e-02, 	1.330e-02, 	1.313e-02,
	1.364e-02, 	1.420e-02, 	1.508e-02, 	1.606e-02, 	1.711e-02, 	1.751e-02, 	1.723e-02, 	1.688e-02, 	1.656e-02, 	1.610e-02,
	1.587e-02, 	1.572e-02, 	1.556e-02, 	1.531e-02, 	1.495e-02, 	1.463e-02, 	1.429e-02, 	1.381e-02, 	1.311e-02, 	1.231e-02,
	1.166e-02, 	1.115e-02, 	1.100e-02, 	1.094e-02, 	1.089e-02, 	1.083e-02, 	1.060e-02, 	1.023e-02, 	1.004e-02, 	9.985e-03,
	9.963e-03, 	9.933e-03, 	9.888e-03, 	9.796e-03, 	9.708e-03, 	9.651e-03, 	9.597e-03, 	9.535e-03, 	9.348e-03, 	8.875e-03,
	8.713e-03, 	8.648e-03, 	8.500e-03, 	8.254e-03, 	8.093e-03, 	7.891e-03, 	7.689e-03, 	7.506e-03, 	7.310e-03, 	6.952e-03,
	6.242e-03, 	5.600e-03, 	4.947e-03, 	4.193e-03, 	3.394e-03, 	2.691e-03, 	1.954e-03, 	1.216e-03, 	5.532e-04, 	6.522e-05,
	1.501e-07, 	1.442e-07, 	1.384e-07, 	1.329e-07, 	1.277e-07, 	1.226e-07, 	1.177e-07, 	1.130e-07, 	1.085e-07, 	1.041e-07,
	9.994e-08, 	9.595e-08, 	9.208e-08, 	8.837e-08, 	8.481e-08, 	8.138e-08, 	7.809e-08, 	7.494e-08, 	7.191e-08, 	6.898e-08,
	6.618e-08, 	6.349e-08, 	6.091e-08, 	5.842e-08, 	5.604e-08, 	5.375e-08, 	5.154e-08, 	4.944e-08, 	4.741e-08, 	4.545e-08,
	4.361e-08, 	4.180e-08, 	4.007e-08, 	3.844e-08, 	3.685e-08, 	3.532e-08, 	3.386e-08, 	3.245e-08, 	3.112e-08, 	2.983e-08,
	2.859e-08, 	2.739e-08, 	2.627e-08, 	2.516e-08, 	2.411e-08, 	2.311e-08, 	2.214e-08, 	2.122e-08, 	2.032e-08, 	1.949e-08};

	G4int PuC_k = 0;
	G4double tmp = 0.;
	G4double PuC_r = G4UniformRand();
	while(PuC_r > tmp)
	{
		tmp += PuC_p[PuC_k];
		PuC_k += 1;
	}
	G4double kinE = (0.05*PuC_k+0.025) + (G4UniformRand()-0.5)*0.05;
	fParticleGun->SetParticleEnergy(kinE);
}

void simNE213PrimaryGeneratorAction::GenerateCf252EnergyDistribution(void)
{
	const G4int Cf252_n = 200;
	G4double Cf252_p[Cf252_n] = {
	3.773e-03, 	6.743e-03, 	8.520e-03, 	9.841e-03, 	1.088e-02, 	1.173e-02, 	1.243e-02, 	1.301e-02, 	1.350e-02, 	1.390e-02,
	1.423e-02, 	1.450e-02, 	1.472e-02, 	1.489e-02, 	1.502e-02, 	1.511e-02, 	1.517e-02, 	1.519e-02, 	1.519e-02, 	1.517e-02,
	1.512e-02, 	1.505e-02, 	1.497e-02, 	1.486e-02, 	1.475e-02, 	1.462e-02, 	1.448e-02, 	1.433e-02, 	1.416e-02, 	1.399e-02,
	1.382e-02, 	1.363e-02, 	1.344e-02, 	1.325e-02, 	1.305e-02, 	1.284e-02, 	1.263e-02, 	1.243e-02, 	1.221e-02, 	1.200e-02,
	1.179e-02, 	1.157e-02, 	1.135e-02, 	1.114e-02, 	1.092e-02, 	1.071e-02, 	1.049e-02, 	1.028e-02, 	1.006e-02, 	9.854e-03,
	9.644e-03, 	9.436e-03, 	9.231e-03, 	9.027e-03, 	8.826e-03, 	8.627e-03, 	8.430e-03, 	8.236e-03, 	8.045e-03, 	7.856e-03,
	7.670e-03, 	7.487e-03, 	7.306e-03, 	7.129e-03, 	6.955e-03, 	6.783e-03, 	6.615e-03, 	6.449e-03, 	6.287e-03, 	6.128e-03,
	5.971e-03, 	5.818e-03, 	5.668e-03, 	5.520e-03, 	5.376e-03, 	5.235e-03, 	5.096e-03, 	4.961e-03, 	4.828e-03, 	4.699e-03,
	4.572e-03, 	4.448e-03, 	4.327e-03, 	4.208e-03, 	4.093e-03, 	3.980e-03, 	3.869e-03, 	3.762e-03, 	3.657e-03, 	3.554e-03,
	3.454e-03, 	3.356e-03, 	3.261e-03, 	3.168e-03, 	3.077e-03, 	2.989e-03, 	2.903e-03, 	2.819e-03, 	2.737e-03, 	2.657e-03,
	2.580e-03, 	2.504e-03, 	2.430e-03, 	2.359e-03, 	2.289e-03, 	2.221e-03, 	2.155e-03, 	2.091e-03, 	2.028e-03, 	1.967e-03,
	1.908e-03, 	1.851e-03, 	1.795e-03, 	1.740e-03, 	1.688e-03, 	1.636e-03, 	1.586e-03, 	1.538e-03, 	1.490e-03, 	1.445e-03,
	1.400e-03, 	1.357e-03, 	1.315e-03, 	1.274e-03, 	1.234e-03, 	1.196e-03, 	1.159e-03, 	1.122e-03, 	1.087e-03, 	1.053e-03,
	1.020e-03, 	9.875e-04, 	9.563e-04, 	9.260e-04, 	8.966e-04, 	8.681e-04, 	8.404e-04, 	8.136e-04, 	7.876e-04, 	7.624e-04,
	7.379e-04, 	7.142e-04, 	6.912e-04, 	6.689e-04, 	6.473e-04, 	6.263e-04, 	6.061e-04, 	5.864e-04, 	5.673e-04, 	5.488e-04,
	5.309e-04, 	5.136e-04, 	4.968e-04, 	4.805e-04, 	4.648e-04, 	4.495e-04, 	4.346e-04, 	4.204e-04, 	4.064e-04, 	3.930e-04,
	3.800e-04, 	3.674e-04, 	3.552e-04, 	3.434e-04, 	3.320e-04, 	3.209e-04, 	3.102e-04, 	2.999e-04, 	2.898e-04, 	2.801e-04,
	2.707e-04, 	2.616e-04, 	2.528e-04, 	2.443e-04, 	2.361e-04, 	2.281e-04, 	2.204e-04, 	2.130e-04, 	2.058e-04, 	1.988e-04,
	1.920e-04, 	1.855e-04, 	1.792e-04, 	1.731e-04, 	1.672e-04, 	1.615e-04, 	1.560e-04, 	1.506e-04, 	1.455e-04, 	1.405e-04,
	1.357e-04, 	1.311e-04, 	1.265e-04, 	1.222e-04, 	1.180e-04, 	1.139e-04, 	1.100e-04, 	1.062e-04, 	1.025e-04, 	9.896e-05};

	G4int Cf252_k = 0;
	G4double tmp = 0.;
	G4double Cf252_r = G4UniformRand();
	while(Cf252_r > tmp)
	{
		tmp += Cf252_p[Cf252_k];
		Cf252_k += 1;
	}
	G4double kinE = (0.05*Cf252_k+0.025) + (G4UniformRand()-0.5)*0.05;
	fParticleGun->SetParticleEnergy(kinE);
}
