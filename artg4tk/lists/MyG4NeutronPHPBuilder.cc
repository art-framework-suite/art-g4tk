//
// ********************************************************************
// This is a variation of the file provided by the
// Geant4 collaboration (https://geant4.web.cern.ch/):
// G4NeutronPHPBuilder.cc
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
//
// -- artg4tk includes
#include "artg4tk/lists/MyG4NeutronPHPBuilder.hh"

//#include "Geant4/G4NeutronPHPBuilder.hh"
#include "Geant4/G4ParticleDefinition.hh"
#include "Geant4/G4ParticleTable.hh"
#include "Geant4/G4ProcessManager.hh"
#include "Geant4/G4SystemOfUnits.hh"

MyG4NeutronPHPBuilder::MyG4NeutronPHPBuilder()
{
  theHPElastic = 0;
  theHPElasticData = 0;

  theHPFission = 0;
  theHPFissionData = 0;

  theHPCapture = 0;
  theHPCaptureData = 0;

  theHPInelastic = 0;
  theHPInelasticData = 0;

  theMin = 0;
  theIMin = theMin;
  theMax = 20 * MeV;
  theIMax = theMax;
}

void
MyG4NeutronPHPBuilder::Build(G4HadronElasticProcess* aP)
{
  if (theHPElastic == 0)
    theHPElastic = new G4ParticleHPElastic;
  theHPElastic->SetMinEnergy(theMin);
  theHPElastic->SetMaxEnergy(theMax);
  if (theHPElasticData == 0)
    theHPElasticData = new G4ParticleHPElasticData;
  aP->AddDataSet(theHPElasticData);
  aP->RegisterMe(theHPElastic);
}

void
MyG4NeutronPHPBuilder::Build(G4HadronFissionProcess* aP)
{
  if (theHPFission == 0)
    theHPFission = new G4ParticleHPFission;
  theHPFission->SetMinEnergy(theMin);
  theHPFission->SetMaxEnergy(theMax);
  if (theHPFissionData == 0)
    theHPFissionData = new G4ParticleHPFissionData;
  aP->AddDataSet(theHPFissionData);
  aP->RegisterMe(theHPFission);
}

void
MyG4NeutronPHPBuilder::Build(G4HadronCaptureProcess* aP)
{
  if (theHPCapture == 0)
    theHPCapture = new ArParticleHPCapture;
  theHPCapture->SetMinEnergy(theMin);
  theHPCapture->SetMaxEnergy(theMax);
  if (theHPCaptureData == 0)
    theHPCaptureData = new G4ParticleHPCaptureData;
  aP->AddDataSet(theHPCaptureData);
  aP->RegisterMe(theHPCapture);
}

void
MyG4NeutronPHPBuilder::Build(G4NeutronInelasticProcess* aP)
{
  if (theHPInelastic == 0)
    theHPInelastic = new G4ParticleHPInelastic(G4Neutron::Neutron(), "NeutronHPInelastic");
  theHPInelastic->SetMinEnergy(theIMin);
  theHPInelastic->SetMaxEnergy(theIMax);
  if (theHPInelasticData == 0)
    theHPInelasticData = new G4ParticleHPInelasticData(G4Neutron::Neutron());
  aP->AddDataSet(theHPInelasticData);
  aP->RegisterMe(theHPInelastic);
}
