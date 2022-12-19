#include "MyG4BiasedBertiniNeutronBuilder.hh"
//#include "Geant4/G4BertiniNeutronBuilder.hh"
#include "Geant4/G4SystemOfUnits.hh"
#include "Geant4/G4ParticleDefinition.hh"
#include "Geant4/G4ParticleTable.hh"
#include "Geant4/G4ProcessManager.hh"

MyG4BiasedBertiniNeutronBuilder::
MyG4BiasedBertiniNeutronBuilder()
{
  theMin = 0;
  theMax = 9.9*GeV;
  theXSMultiplier = 1.0; // -- no bias
  theModel = new G4CascadeInterface;
}

MyG4BiasedBertiniNeutronBuilder::
MyG4BiasedBertiniNeutronBuilder( G4double XSMultiplier )
{
  theMin = 0;
  theMax = 9.9*GeV;
  G4cout << "(MyG4BiasedBertiniNeutronBuilder) theBertiniNeutronBuilder was passed XSMultiplier =  " << XSMultiplier << G4endl;
  theXSMultiplier = XSMultiplier;
  theModel = new G4CascadeInterface;
}

void MyG4BiasedBertiniNeutronBuilder::
Build(G4NeutronInelasticProcess * aP )
{
  theModel->SetMinEnergy(theMin);
  theModel->SetMaxEnergy(theMax);
  G4cout << "(MyG4BiasedBertiniNeutronBuilder) Multiplying the G4NeutronInelastic XS by : " << theXSMultiplier << G4endl;
  aP->MultiplyCrossSectionBy(theXSMultiplier);
  aP->RegisterMe(theModel);
}

MyG4BiasedBertiniNeutronBuilder::
~MyG4BiasedBertiniNeutronBuilder()
{
}

void MyG4BiasedBertiniNeutronBuilder::
Build(G4HadronElasticProcess * )
{
}

void MyG4BiasedBertiniNeutronBuilder::
Build(G4HadronFissionProcess * )
{
}

void MyG4BiasedBertiniNeutronBuilder::
Build(G4HadronCaptureProcess * )
{
}

// 2002 by J.P. Wellisch
