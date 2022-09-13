//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
//
//---------------------------------------------------------------------------
//
// ClassName:   G4EmStandardPhysics_pseudoLaser
//
// Author:      V.Ivanchenko 09.11.2005
//
// Modified:
// 05.12.2005 V.Ivanchenko add controlled verbosity
// 23.11.2006 V.Ivanchenko remove mscStepLimit option and improve cout
// 05.01.2022 D.Rivera: imported into artg4tk based on :
//            geant4.10.06.p01/source/physics_lists/constructors/electromagnetic/include/G4EmStandardPhysics.hh
//            Rebranded as G4EmStandardPhysics_pseudoLaser for use with larg4
//            and to implement an "effective" laser simulation
//
//----------------------------------------------------------------------------
//
// This class provides construction of default EM standard physics
//

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






