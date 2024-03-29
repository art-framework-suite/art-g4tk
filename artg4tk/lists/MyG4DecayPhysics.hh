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
//---------------------------------------------------------------------------
//
// ClassName:   G4DecayPhysics
//
// Author: 2002 J.P. Wellisch
//
// Modified:
// 10.11.2005 V.Ivanchenko edit to provide a standard
// 05.12.2005 V.Ivanchenko add controlled verbosity
// 07.03.2022 D.Rivera: imported into artg4tk based on :
//            geant4.10.06.p01/source/physics_lists/constructors/decay/include/G4DecayPhysics.cc
//
//----------------------------------------------------------------------------
//

#ifndef MyG4DecayPhysics_h
#define MyG4DecayPhysics_h 1

#include "Geant4/globals.hh"
#include "Geant4/G4VPhysicsConstructor.hh"

#include "Geant4/G4Decay.hh"

class MyG4DecayPhysics : public G4VPhysicsConstructor
{
  public: 
    MyG4DecayPhysics(G4int ver = 1);
    MyG4DecayPhysics(const G4String& name, G4int ver = 1);
    virtual ~MyG4DecayPhysics();

  public: 
    // This method will be invoked in the Construct() method. 
    // each particle type will be instantiated
  virtual void ConstructParticle();
 
    // This method will be invoked in the Construct() method.
    // each physics process will be instantiated and
    // registered to the process manager of each particle type 
  virtual void ConstructProcess();

  virtual G4Decay* GetDecayProcess() { return fDecayProcess; }

private:
  static G4ThreadLocal G4Decay* fDecayProcess;
  G4int    verbose;
  static G4ThreadLocal G4bool   wasActivated;
};

#endif








