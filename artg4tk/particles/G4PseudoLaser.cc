#include "G4PseudoLaser.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"

#include "G4MuonDecayChannel.hh"
#include "G4DecayTable.hh"

// ######################################################################
// ###                          MUONPLUS                              ###
// ######################################################################
G4PseudoLaser* G4PseudoLaser::theInstance = 0;

G4PseudoLaser* G4PseudoLaser::Definition()
{
  if (theInstance !=0) return theInstance;
  const G4String name = "mulaser";
  // search in particle table]
  G4ParticleTable* pTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* anInstance = pTable->FindParticle(name);
  if (anInstance ==0)
  {
    std::cout << "Creating " << name << std::endl;
  // create particle
  //
  //    Arguments for constructor are as follows
  //               name             mass          width         charge
  //             2*spin           parity  C-conjugation
  //          2*Isospin       2*Isospin3       G-parity
  //               type    lepton number  baryon number   PDG encoding
  //             stable         lifetime    decay table
  //             shortlived      subType    anti_encoding
  anInstance = new G4ParticleDefinition(
                 name, 0.1056583715*GeV, 2.99598e-16*MeV,  +1.*eplus, 
		    1,               0,                0,          
		    0,               0,                0,             
	     "lepton",              -1,                0,        66613, /////need to change this 
		true/*false*/,      2196.98*ns,             NULL,
             false,           "mu"
              );
   // Bohr Magnetron
   G4double muB =  0.5*eplus*hbar_Planck/(anInstance->GetPDGMass()/c_squared) ;
   
   anInstance->SetPDGMagneticMoment( muB * 1.0011659209);

  //create Decay Table 
  /*
  G4DecayTable* table = new G4DecayTable();
  // create a decay channel
  G4VDecayChannel* mode = new G4MuonDecayChannel("mulaser",1.00);
  table->Insert(mode);
  anInstance->SetDecayTable(table);
  */
  }

  theInstance = reinterpret_cast<G4PseudoLaser*>(anInstance);
  std::cout << "Returning " << theInstance << std::endl;
  return theInstance;
}

G4PseudoLaser*  G4PseudoLaser::PseudoLaserDefinition()
{
  return Definition();
}

G4PseudoLaser*  G4PseudoLaser::PseudoLaser()
{
  return Definition();
}

