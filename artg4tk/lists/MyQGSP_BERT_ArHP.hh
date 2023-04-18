//
// ********************************************************************
// This is a variation of the file provided by the
// Geant4 collaboration (https://geant4.web.cern.ch/):
// QGSP_BERT_HP.hh
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
#ifndef MyQGSP_BERT_ArHP_h
#define MyQGSP_BERT_ArHP_h 1

#include "Geant4/G4VModularPhysicsList.hh"
#include "Geant4/globals.hh"

class MyQGSP_BERT_ArHP : public G4VModularPhysicsList {
public:
  MyQGSP_BERT_ArHP(G4int ver = 1);

  MyQGSP_BERT_ArHP(const MyQGSP_BERT_ArHP&) = delete;
  MyQGSP_BERT_ArHP& operator=(const MyQGSP_BERT_ArHP&) = delete;

  void SetCuts() override;
};

#endif /* artg4tk_lists_MyQGSP_BERT_ArHP_hh */
