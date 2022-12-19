//
// ********************************************************************
// This is a variation of the file provided by the
// Geant4 collaboration (https://geant4.web.cern.ch/):
// QGSP_BERT_HP.cc
// (see https://gitlab.cern.ch/geant4/geant4/)
//
// The file was adjusted to work with artg4tk and/or to implement
// features relevant to the simulation of liquid Argon TPC's. 
// 
// For the original Geant4 License and Disclaimer see:
// https://gitlab.cern.ch/geant4/geant4/-/blob/master/LICENSE
//
// ********************************************************************
//
#include <iomanip>

#include <CLHEP/Units/SystemOfUnits.h>

// -- artg4tk includes
#include "artg4tk/lists/MyG4HadronPhysicsQGSP_BERT_ArHP.hh"

#include "Geant4/globals.hh"
#include "Geant4/G4ios.hh"
#include "Geant4/G4ProcessManager.hh"
#include "Geant4/G4ProcessVector.hh"
#include "Geant4/G4ParticleTypes.hh"
#include "Geant4/G4ParticleTable.hh"

#include "Geant4/G4Material.hh"
#include "Geant4/G4MaterialTable.hh"

#include "Geant4/G4DecayPhysics.hh"
#include "Geant4/G4RadioactiveDecayPhysics.hh"
#include "Geant4/G4EmStandardPhysics.hh"
#include "Geant4/G4EmExtraPhysics.hh"
#include "Geant4/G4IonPhysics.hh"
#include "Geant4/G4StoppingPhysics.hh"
#include "Geant4/G4HadronElasticPhysicsHP.hh"


/////////////////////////////////////////////////////////////////////////////
// The following change is the _only_ required change to move from
// the non-extensible factory to the exensible factory.  All other changes
// relative to the "factory" example are there to demonstrate new features.
/////////////////////////////////////////////////////////////////////////////
//non-extensible:  #include "G4PhysListFactory.hh"
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

#include "Geant4/G4PhysListStamper.hh"  // defines macro for factory registration
#include "MyQGSP_BERT_ArHP.hh"

// -- Register the physics list
G4_DECLARE_PHYSLIST_FACTORY(MyQGSP_BERT_ArHP);

MyQGSP_BERT_ArHP::MyQGSP_BERT_ArHP(G4int ver)
{

  G4cout << "<<< Geant4 Physics List simulation engine: MyQGSP_BERT_ArHP"<<G4endl;
  G4cout <<G4endl<<G4endl;

  defaultCutValue = 0.7*CLHEP::mm;
  SetVerboseLevel(ver);

  // EM Physics
  RegisterPhysics( new G4EmStandardPhysics(ver) );

  // Synchroton Radiation & GN Physics
  RegisterPhysics( new G4EmExtraPhysics(ver) );

  // Decays
  RegisterPhysics( new G4DecayPhysics(ver) );
  RegisterPhysics( new G4RadioactiveDecayPhysics(ver) );

  // Hadron Elastic scattering
  RegisterPhysics( new G4HadronElasticPhysicsHP(ver) );

  // Hadron Physics
  RegisterPhysics( new MyG4HadronPhysicsQGSP_BERT_ArHP(ver));

  // Stopping Physics
  RegisterPhysics( new G4StoppingPhysics(ver));

  // Ion Physics
  RegisterPhysics( new G4IonPhysics(ver));

}

void MyQGSP_BERT_ArHP::SetCuts()
{
  if (verboseLevel >1){
    G4cout << "MyQGSP_BERT_ArHP::SetCuts:";
  }
  //  " G4VUserPhysicsList::SetCutsWithDefault" method sets
  //   the default cut value for all particle types

  SetCutsWithDefault();

  //Set proton cut value to 0 for producing low energy recoil nucleus
  SetCutValue(0, "proton");

}
