#ifndef MyG4HadronPhysicsQGSP_BERT_HP_NeutronXSBias_h
#define MyG4HadronPhysicsQGSP_BERT_HP_NeutronXSBias_h 1

#include "Geant4/G4ios.hh"
#include "Geant4/globals.hh"

#include "Geant4/G4VPhysicsConstructor.hh"

// -- Pion and Kaon builders
#include "Geant4/G4BertiniPiKBuilder.hh"
#include "Geant4/G4FTFPPiKBuilder.hh"
#include "Geant4/G4PiKBuilder.hh"
#include "Geant4/G4QGSPPiKBuilder.hh"

// -- Proton builders
#include "Geant4/G4BertiniProtonBuilder.hh"
#include "Geant4/G4FTFPProtonBuilder.hh"
#include "Geant4/G4ProtonBuilder.hh"
#include "Geant4/G4QGSPProtonBuilder.hh"

// -- Neutron builders
#include "Geant4/G4BertiniNeutronBuilder.hh"
#include "Geant4/G4FTFPNeutronBuilder.hh"
#include "Geant4/G4NeutronBuilder.hh"
#include "Geant4/G4QGSPNeutronBuilder.hh"
//#include "MyG4BiasedBertiniNeutronBuilder.hh"
#include "Geant4/G4NeutronPHPBuilder.hh"

// -- Other builders
#include "Geant4/G4AntiBarionBuilder.hh"
#include "Geant4/G4FTFPAntiBarionBuilder.hh"
#include "Geant4/G4HyperonFTFPBuilder.hh"

class G4ComponentGGHadronNucleusXsc;

class MyG4HadronPhysicsQGSP_BERT_HP_NeutronXSBias : public G4VPhysicsConstructor {
public:
  MyG4HadronPhysicsQGSP_BERT_HP_NeutronXSBias(G4int verbose = 2);
  MyG4HadronPhysicsQGSP_BERT_HP_NeutronXSBias(const G4String& name, G4bool quasiElastic = true);
  MyG4HadronPhysicsQGSP_BERT_HP_NeutronXSBias(const G4String& name,
                                              G4bool quasiElastic = true,
                                              G4double XSMultiplier = 1.0);
  virtual ~MyG4HadronPhysicsQGSP_BERT_HP_NeutronXSBias();

  void ConstructParticle() override;
  void ConstructProcess() override;

private:
  void CreateModels();

  struct ThreadPrivate {
    G4NeutronBuilder* theNeutrons;
    G4FTFPNeutronBuilder* theFTFPNeutron;
    G4QGSPNeutronBuilder* theQGSPNeutron;
    G4BertiniNeutronBuilder* theBertiniNeutron;
    G4NeutronPHPBuilder* theHPNeutron;

    G4PiKBuilder* thePiK;
    G4FTFPPiKBuilder* theFTFPPiK;
    G4QGSPPiKBuilder* theQGSPPiK;
    G4BertiniPiKBuilder* theBertiniPiK;

    G4ProtonBuilder* thePro;
    G4FTFPProtonBuilder* theFTFPPro;
    G4QGSPProtonBuilder* theQGSPPro;
    G4BertiniProtonBuilder* theBertiniPro;

    G4HyperonFTFPBuilder* theHyperon;

    G4AntiBarionBuilder* theAntiBaryon;
    G4FTFPAntiBarionBuilder* theFTFPAntiBaryon;

    G4ComponentGGHadronNucleusXsc* xsKaon;
    G4VCrossSectionDataSet* xsNeutronCaptureXS;
  };
  static G4ThreadLocal ThreadPrivate* tpdata;

  G4double theBertiniNeutronXSMultiplier;
};

// 2019 by D. Rivera

#endif /* artg4tk_lists_MyG4HadronPhysicsQGSP_BERT_HP_NeutronXSBias_hh */
