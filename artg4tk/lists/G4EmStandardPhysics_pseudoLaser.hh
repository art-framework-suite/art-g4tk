#ifndef G4EmStandardPhysics_pseudoLaser_h
#define G4EmStandardPhysics_pseudoLaser_h 1

#include "Geant4/G4VPhysicsConstructor.hh"
#include "Geant4/G4EmParticleList.hh"
#include "Geant4/globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4EmStandardPhysics_pseudoLaser : public G4VPhysicsConstructor
{
public:

  explicit G4EmStandardPhysics_pseudoLaser(G4int ver=1, const G4String& name="");

  virtual ~G4EmStandardPhysics_pseudoLaser();

  virtual void ConstructParticle();
  virtual void ConstructProcess();

private:
  G4int  verbose;
  G4EmParticleList partList;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif






