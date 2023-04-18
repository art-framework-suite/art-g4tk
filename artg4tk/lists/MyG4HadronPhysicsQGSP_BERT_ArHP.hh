//
// ********************************************************************
// This is a variation of the file provided by the
// Geant4 collaboration (https://geant4.web.cern.ch/):
// G4HadronPhysicsQGSP_BERT_ArHP.hh
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
#ifndef MyG4HadronPhysicsQGSP_BERT_ArHP_h
#define MyG4HadronPhysicsQGSP_BERT_ArHP_h 1

#include "Geant4/G4HadronPhysicsQGSP_BERT.hh"


class MyG4HadronPhysicsQGSP_BERT_ArHP : public G4HadronPhysicsQGSP_BERT {
public:
  MyG4HadronPhysicsQGSP_BERT_ArHP(G4int verbose = 1);
  MyG4HadronPhysicsQGSP_BERT_ArHP(const G4String& name, G4bool quasiElastic = true);
  virtual ~MyG4HadronPhysicsQGSP_BERT_ArHP() {}

protected:
  void Neutron() override;
  void ExtraConfiguration() override;
};


#endif /* artg4tk_lists_MyG4HadronPhysicsQGSP_BERT_ArHP_hh */
