#ifndef MyG4BiasedBertiniNeutronBuilder_h
#define MyG4BiasedBertiniNeutronBuilder_h 1

#include "Geant4/globals.hh"

#include "Geant4/G4HadronElasticProcess.hh"
#include "Geant4/G4HadronFissionProcess.hh"
#include "Geant4/G4HadronCaptureProcess.hh"
#include "Geant4/G4NeutronInelasticProcess.hh"
#include "Geant4/G4VNeutronBuilder.hh"

#include "Geant4/G4CascadeInterface.hh"

class MyG4BiasedBertiniNeutronBuilder : public G4VNeutronBuilder
{
  public:
    MyG4BiasedBertiniNeutronBuilder();
    MyG4BiasedBertiniNeutronBuilder( G4double XSMultiplier );
    virtual ~MyG4BiasedBertiniNeutronBuilder();

  public:
    virtual void Build(G4HadronElasticProcess * aP);
    virtual void Build(G4HadronFissionProcess * aP);
    virtual void Build(G4HadronCaptureProcess * aP);
    virtual void Build(G4NeutronInelasticProcess * aP);

    void SetMinEnergy(G4double aM) {theMin = aM;}
    void SetMaxEnergy(G4double aM) {theMax = aM;}

  private:
    G4CascadeInterface * theModel;
    G4double theXSMultiplier;
    G4double theMin;
    G4double theMax;

};


#endif

