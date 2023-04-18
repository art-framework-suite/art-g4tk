//
// ********************************************************************
// This is a variation of the file provided by the
// Geant4 collaboration (https://geant4.web.cern.ch/):
// G4NeutronPHPBuilder.hh
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
#ifndef MyG4NeutronPHPBuilder_h
#define MyG4NeutronPHPBuilder_h 1

// -- artg4tk includes
#include "artg4tk/lists/ArParticleHPCapture.hh"
#include "G4Version.hh"
#include "Geant4/globals.hh"
#if G4VERSION_NUMBER < 110
#include "Geant4/G4HadronCaptureProcess.hh"
#include "Geant4/G4HadronFissionProcess.hh"
#include "Geant4/G4NeutronInelasticProcess.hh"
#else
#include "Geant4/G4NeutronCaptureProcess.hh"
#include "Geant4/G4NeutronFissionProcess.hh"
#include "Geant4/G4HadronInelasticProcess.hh"
#endif
#include "Geant4/G4HadronElasticProcess.hh"
#include "Geant4/G4VNeutronBuilder.hh"
#include "Geant4/G4ParticleHPElastic.hh"
#include "Geant4/G4ParticleHPElasticData.hh"
#include "Geant4/G4ParticleHPFission.hh"
#include "Geant4/G4ParticleHPFissionData.hh"
#include "Geant4/G4ParticleHPInelastic.hh"
#include "Geant4/G4ParticleHPInelasticData.hh"
#include "Geant4/G4ParticleHPCaptureData.hh"

class MyG4NeutronPHPBuilder : public G4VNeutronBuilder {
public:
  MyG4NeutronPHPBuilder();
  virtual ~MyG4NeutronPHPBuilder() {}

public:
  void Build(G4HadronElasticProcess* aP) final override;
#if G4VERSION_NUMBER < 110
  void Build(G4HadronFissionProcess* aP) final override;
  void Build(G4HadronCaptureProcess* aP) final override;
  void Build(G4NeutronInelasticProcess* aP) final override;
#else
  void Build(G4NeutronFissionProcess* aP) final override;
  void Build(G4NeutronCaptureProcess* aP) final override;
  void Build(G4HadronInelasticProcess* aP) final override;
#endif
  virtual void
  SetMinEnergy(G4double aM) final override
  {
    theMin = aM;
    theIMin = theMin;
  }
  void
  SetMinInelasticEnergy(G4double aM)
  {
    theIMin = aM;
  }
  virtual void
  SetMaxEnergy(G4double aM) final override
  {
    theIMax = aM;
    theMax = aM;
  }
  void
  SetMaxInelasticEnergy(G4double aM)
  {
    theIMax = aM;
  }

  using G4VNeutronBuilder::Build; // Prevent compiler warning

private:
  G4double theMin;
  G4double theIMin;
  G4double theMax;
  G4double theIMax;

  G4ParticleHPElastic* theHPElastic;
  G4ParticleHPElasticData* theHPElasticData;
  G4ParticleHPInelastic* theHPInelastic;
  G4ParticleHPInelasticData* theHPInelasticData;
  G4ParticleHPFission* theHPFission;
  G4ParticleHPFissionData* theHPFissionData;
  ArParticleHPCapture* theHPCapture;
  G4ParticleHPCaptureData* theHPCaptureData;
};

#endif /* artg4tk_lists_MyG4NeutronPHPBuilder_hh */
