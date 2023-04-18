#include <iomanip>   
#include <CLHEP/Units/SystemOfUnits.h>

// -- artg4tk includes
#include "artg4tk/lists/G4EmStandardPhysics_pseudoLaser.hh"

#include "Geant4/globals.hh"
#include "Geant4/G4ios.hh"
#include "Geant4/G4ProcessManager.hh"
#include "Geant4/G4ProcessVector.hh"
#include "Geant4/G4ParticleTypes.hh"
#include "Geant4/G4ParticleTable.hh"

#include "Geant4/G4Material.hh"
#include "Geant4/G4MaterialTable.hh"

//#include "Geant4/G4DecayPhysics.hh"
#include "MyG4DecayPhysics.hh"
#include "Geant4/G4EmExtraPhysics.hh"
#include "Geant4/G4IonPhysics.hh"
#include "Geant4/G4StoppingPhysics.hh"
#include "Geant4/G4HadronElasticPhysics.hh"
#include "Geant4/G4NeutronTrackingCut.hh"

#include "Geant4/G4HadronPhysicsQGSP_BERT.hh"

/////////////////////////////////////////////////////////////////////////////
// The following change is the _only_ required changed to move from
// the non-extensible factory to the exensible factory.  All other changes
// relative to the "factory" example are there to demonstrate new features.
/////////////////////////////////////////////////////////////////////////////
// non-extensible:  #include "G4PhysListFactory.hh"
#include "Geant4/G4PhysListFactoryAlt.hh"
/////////////////////////////////////////////////////////////////////////////
// headers needed to demonstrate new features
/////////////////////////////////////////////////////////////////////////////

// allow ourselves to extend the short names for physics ctor addition/replace
// along the same lines as EMX, EMY, etc
#include "Geant4/G4PhysListRegistry.hh"

// allow ourselves to give the user extra info about available physics ctors
#include "Geant4/G4PhysicsConstructorFactory.hh"

/////////////////////////////////////////////////////////////////////////////
// pull in a user defined physics list definition into the main program
// and register it with the factory (doesn't have to be the main program
// but the .o containing the declaration _must_ get linked/loaded)

#include "Geant4/G4VModularPhysicsList.hh"

#include "Geant4/G4PhysListStamper.hh" // defines macro for factory registration
#include "MyQGSP_BERT_LASER.hh"

// -- Register the physics list
G4_DECLARE_PHYSLIST_FACTORY(MyQGSP_BERT_LASER);

MyQGSP_BERT_LASER::MyQGSP_BERT_LASER(G4int ver)
{

  G4cout << "<<< Geant4 Physics List simulation engine: MyQGSP_BERT_LASER"<<G4endl;
  G4cout <<G4endl;

  defaultCutValue = 0.7*CLHEP::mm;  
  SetVerboseLevel(ver);

  // EM Physics
  //RegisterPhysics( new G4EmStandardPhysics(ver) );
  RegisterPhysics( new G4EmStandardPhysics_pseudoLaser(ver) ); // -- replace with laser version

  // Synchroton Radiation & GN Physics
  //<--RegisterPhysics( new G4EmExtraPhysics(ver) );
  //auto MyG4EmExtraPhysics = new G4EmExtraPhysics(ver);
  //MyG4EmExtraPhysics->MuonNuclear(false); // -- deactivate muonNuclear process
  //RegisterPhysics( MyG4EmExtraPhysics );

  // Decays
  //RegisterPhysics( new MyG4DecayPhysics(ver) );

   // Hadron Elastic scattering
  RegisterPhysics( new G4HadronElasticPhysics(ver) );

  // Hadron Physics
  RegisterPhysics( new G4HadronPhysicsQGSP_BERT(ver));

  // Stopping Physics
  RegisterPhysics( new G4StoppingPhysics(ver) );

  // Ion Physics
  RegisterPhysics( new G4IonPhysics(ver));
  
  // Neutron tracking cut
  RegisterPhysics( new G4NeutronTrackingCut(ver));

}
