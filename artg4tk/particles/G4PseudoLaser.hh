#ifndef G4PseudoLaser_h
#define G4PseudoLaser_h 1

#include "globals.hh"
#include "G4ios.hh"
#include "G4ParticleDefinition.hh"

// ######################################################################
// ###                         MUONPLUS                               ###
// ######################################################################

class G4PseudoLaser : public G4ParticleDefinition
{
 private:
   static G4PseudoLaser* theInstance;
   G4PseudoLaser(){}
   ~G4PseudoLaser(){}

 public:
   static G4PseudoLaser* Definition();
   static G4PseudoLaser* PseudoLaserDefinition();
   static G4PseudoLaser* PseudoLaser();
};

#endif








