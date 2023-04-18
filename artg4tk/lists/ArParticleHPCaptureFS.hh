//
// ********************************************************************
// This is a variation of the file provided by the
// Geant4 collaboration (https://geant4.web.cern.ch/):
// G4ParticleHPCaptureFS.hh
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
#ifndef ArParticleHPCaptureFS_h
#define ArParticleHPCaptureFS_h 1

// -- artg4tk includes
#include "artg4tk/lists/ArCaptureGammas.hh"

#include "Geant4/G4HadFinalState.hh"
#include "Geant4/G4HadProjectile.hh"
#include "Geant4/G4ParticleHPEnAngCorrelation.hh"
#include "Geant4/G4ParticleHPFinalState.hh"
#include "Geant4/G4ParticleHPNames.hh"
#include "Geant4/G4ParticleHPPhotonDist.hh"
#include "Geant4/G4ReactionProductVector.hh"
#include "Geant4/globals.hh"

class ArParticleHPCaptureFS : public G4ParticleHPFinalState {
public:
  bool useArCapGamma = true;

  ArParticleHPCaptureFS()
  {
    hasXsec = false;
  }

  void Init(G4double A,
            G4double Z,
            G4int M,
            G4String& dirName,
            G4String& aFSType,
            G4ParticleDefinition*) override;
  G4HadFinalState* ApplyYourself(const G4HadProjectile& theTrack) override;
  G4ParticleHPFinalState*
  New() override
  {
    return new ArParticleHPCaptureFS;
  }

private:
  G4double targetMass{0};

  G4ParticleHPPhotonDist theFinalStatePhotons;
  ArCaptureGammas theFinalgammas;

  G4ParticleHPEnAngCorrelation theMF6FinalState;
  G4bool hasExactMF6{false};

  G4ParticleHPNames theNames;
};

#endif /* artg4tk_lists_ArParticleHPCaptureFS_hh */