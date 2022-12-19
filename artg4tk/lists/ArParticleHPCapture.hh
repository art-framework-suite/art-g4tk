//
// ********************************************************************
// This is a variation of the file provided by the
// Geant4 collaboration (https://geant4.web.cern.ch/):
// G4ParticleHPCapture.hh
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
#ifndef ArParticleHPCapture_h
#define ArParticleHPCapture_h 1

#include "Geant4/globals.hh"
#include "Geant4/G4ParticleHPChannel.hh"
#include "Geant4/G4HadronicInteraction.hh"

class ArParticleHPCapture : public G4HadronicInteraction
{
  public:

  ArParticleHPCapture();

  ~ArParticleHPCapture();

  G4HadFinalState * ApplyYourself(const G4HadProjectile& aTrack, G4Nucleus& aTargetNucleus);

  virtual const std::pair<G4double, G4double> GetFatalEnergyCheckLevels() const;

   public:
      G4int GetVerboseLevel() const;
      void SetVerboseLevel( G4int );
      void BuildPhysicsTable(const G4ParticleDefinition&);
      virtual void ModelDescription(std::ostream& outFile) const;

  private:

      std::vector<G4ParticleHPChannel*>* theCapture;
  G4String dirName;
  G4int numEle;

  G4HadFinalState theResult;
};

#endif
