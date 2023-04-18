//
// ********************************************************************
// This is a variation of the file provided by the
// Geant4 collaboration (https://geant4.web.cern.ch/):
// G4HadronPhysicsQGSP_BERT_ArHP.cc
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

// -- artg4tk includes
#include "artg4tk/lists/MyG4HadronPhysicsQGSP_BERT_ArHP.hh"
#include "artg4tk/lists/MyG4NeutronPHPBuilder.hh"

//#include "Geant4/G4HadronPhysicsQGSP_BERT_HP.hh"

#include "Geant4/G4ParticleDefinition.hh"
#include "Geant4/G4ParticleTable.hh"
#include "Geant4/G4SystemOfUnits.hh"
#include "Geant4/G4ios.hh"
#include "Geant4/globals.hh"

#include "Geant4/G4BertiniNeutronBuilder.hh"
#include "Geant4/G4FTFPNeutronBuilder.hh"
#include "Geant4/G4NeutronBuilder.hh"
#include "Geant4/G4QGSPNeutronBuilder.hh"
//#include "Geant4/G4NeutronPHPBuilder.hh"

#include "Geant4/G4HadronCaptureProcess.hh"
#include "Geant4/G4LFission.hh"
#include "Geant4/G4NeutronCaptureXS.hh"
#include "Geant4/G4NeutronRadCapture.hh"
#include "Geant4/G4ParticleHPCaptureData.hh"
#include "Geant4/G4ProcessManager.hh"
#include "Geant4/G4ProcessVector.hh"

#include "Geant4/G4CrossSectionDataSetRegistry.hh"

#include "Geant4/G4PhysListUtil.hh"

#include "Geant4/G4HadronicParameters.hh"

// factoryGeant4/
#include "Geant4/G4PhysicsConstructorFactory.hh"
//
G4_DECLARE_PHYSCONSTR_FACTORY(MyG4HadronPhysicsQGSP_BERT_ArHP);

MyG4HadronPhysicsQGSP_BERT_ArHP::MyG4HadronPhysicsQGSP_BERT_ArHP(G4int)
  : MyG4HadronPhysicsQGSP_BERT_ArHP("hInelastic MyQGSP_BERT_ArHP")
{}

MyG4HadronPhysicsQGSP_BERT_ArHP::MyG4HadronPhysicsQGSP_BERT_ArHP(const G4String& name,
                                                                 G4bool /*quasiElastic */)
  : G4HadronPhysicsQGSP_BERT(name)
{
  minBERT_neutron = 19.9 * MeV;
}

void
MyG4HadronPhysicsQGSP_BERT_ArHP::Neutron()
{
  auto neu = new G4NeutronBuilder(true); // Fission on
  AddBuilder(neu);
  auto qgs = new G4QGSPNeutronBuilder(QuasiElasticQGS);
  AddBuilder(qgs);
  qgs->SetMinEnergy(minQGSP_neutron);
  neu->RegisterMe(qgs);
  auto ftf = new G4FTFPNeutronBuilder(QuasiElasticFTF);
  AddBuilder(ftf);
  ftf->SetMinEnergy(minFTFP_neutron);
  ftf->SetMaxEnergy(maxFTFP_neutron);
  neu->RegisterMe(ftf);
  auto bert = new G4BertiniNeutronBuilder;
  AddBuilder(bert);
  bert->SetMinEnergy(minBERT_neutron);
  bert->SetMaxEnergy(maxBERT_neutron);
  neu->RegisterMe(bert);
  auto hp = new MyG4NeutronPHPBuilder;
  AddBuilder(hp);
  neu->RegisterMe(hp);
  neu->Build();
}

void
MyG4HadronPhysicsQGSP_BERT_ArHP::ExtraConfiguration()
{
  // --- Neutrons ---
  const G4ParticleDefinition* neutron = G4Neutron::Neutron();
  G4HadronicProcess* capture = G4PhysListUtil::FindCaptureProcess(neutron);
  if (capture) {
    G4NeutronRadCapture* theNeutronRadCapture = new G4NeutronRadCapture();
    theNeutronRadCapture->SetMinEnergy(minBERT_neutron);
    capture->RegisterMe(theNeutronRadCapture);
  }
  G4HadronicProcess* fission = G4PhysListUtil::FindFissionProcess(neutron);
  if (fission) {
    G4LFission* theNeutronLEPFission = new G4LFission();
    theNeutronLEPFission->SetMinEnergy(minBERT_neutron);
    theNeutronLEPFission->SetMaxEnergy(G4HadronicParameters::Instance()->GetMaxEnergy());
    fission->RegisterMe(theNeutronLEPFission);
  }
}
