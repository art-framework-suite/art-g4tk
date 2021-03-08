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
// ClassName:   G4HadronPhysicsQGSP_BERT_HP
//
// Author: 2002 J.P. Wellisch
//
// Modified:
// 15.12.2005 G.Folger: migration to non static particles
// 25.04.2007 G.Folger: Add quasielastic as option, use quasielastic by default
// 31.10.2012 A.Ribon: Use G4MiscBuilder
// 19.03.2013 A.Ribon: Replace LEP with FTFP
// 19.01.2021 D.Rivera: imported into artg4tk based on :
//            geant4.10.06.p01/source/physics_lists/constructors/hadron_inelastic/include/G4HadronPhysicsQGSP_BERT_HP.hh
//
//----------------------------------------------------------------------------
//
#ifndef MyG4HadronPhysicsQGSP_BERT_ArHP_h
#define MyG4HadronPhysicsQGSP_BERT_ArHP_h 1

#include "Geant4/G4HadronPhysicsQGSP_BERT.hh"

//#include "globals.hh"
//#include "G4ios.hh"
//
//#include "G4VPhysicsConstructor.hh"
//
//#include "G4PiKBuilder.hh"
//#include "G4FTFPPiKBuilder.hh"
//#include "G4QGSPPiKBuilder.hh"
//#include "G4BertiniPiKBuilder.hh"
//
//#include "G4ProtonBuilder.hh"
//#include "G4FTFPProtonBuilder.hh"
//#include "G4QGSPProtonBuilder.hh"
//#include "G4BertiniProtonBuilder.hh"
//
//#include "G4NeutronBuilder.hh"
//#include "G4FTFPNeutronBuilder.hh"
//#include "G4QGSPNeutronBuilder.hh"
//#include "G4BertiniNeutronBuilder.hh"
//#include "G4NeutronPHPBuilder.hh"
//
//#include "G4HyperonFTFPBuilder.hh"
//#include "G4AntiBarionBuilder.hh"
//#include "G4FTFPAntiBarionBuilder.hh"
//
//class G4ComponentGGHadronNucleusXsc;
//

class MyG4HadronPhysicsQGSP_BERT_ArHP : public G4HadronPhysicsQGSP_BERT 
{
  public: 
    MyG4HadronPhysicsQGSP_BERT_ArHP(G4int verbose =1);
    MyG4HadronPhysicsQGSP_BERT_ArHP(const G4String& name, G4bool quasiElastic=true);
    virtual ~MyG4HadronPhysicsQGSP_BERT_ArHP() {}

  protected:
    virtual void Neutron() override;
    virtual void ExtraConfiguration() override;
};

// 2002 by J.P. Wellisch

#endif

