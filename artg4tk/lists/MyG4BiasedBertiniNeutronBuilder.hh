#ifndef MyG4BiasedBertiniNeutronBuilder_h
#define MyG4BiasedBertiniNeutronBuilder_h 1

#include "Geant4/globals.hh"
#include "G4Version.hh"
#if G4VERSION_NUMBER < 110
#include "Geant4/G4HadronCaptureProcess.hh"
#else
#include "Geant4/G4NeutronCaptureProcess.hh"
#endif
#include "Geant4/G4HadronElasticProcess.hh"
#if G4VERSION_NUMBER < 110
#include "Geant4/G4HadronFissionProcess.hh"
#include "Geant4/G4NeutronInelasticProcess.hh"
#else
#include "Geant4/G4NeutronFissionProcess.hh"
#include "Geant4/G4HadronInelasticProcess.hh"
#endif

#include "Geant4/G4VNeutronBuilder.hh"

#include "Geant4/G4CascadeInterface.hh"

class MyG4BiasedBertiniNeutronBuilder : public G4VNeutronBuilder {
public:
  MyG4BiasedBertiniNeutronBuilder();
  MyG4BiasedBertiniNeutronBuilder(G4double XSMultiplier);

  void Build(G4HadronElasticProcess* aP) override;
#if G4VERSION_NUMBER < 110
  void Build(G4HadronFissionProcess* aP) override;
  void Build(G4HadronCaptureProcess* aP) override;
  void Build(G4NeutronInelasticProcess* aP) override;
#else
  void Build(G4NeutronFissionProcess* aP) override;
  void Build(G4NeutronCaptureProcess* aP) override;
  void Build(G4HadronInelasticProcess* aP) override;
#endif


  void
  SetMinEnergy(G4double aM) override
  {
    theMin = aM;
  }
  void
  SetMaxEnergy(G4double aM) override
  {
    theMax = aM;
  }

private:
  G4CascadeInterface* theModel;
  G4double theXSMultiplier;
  G4double theMin;
  G4double theMax;
};

#endif /* artg4tk_lists_MyG4BiasedBertiniNeutronBuilder_hh */
