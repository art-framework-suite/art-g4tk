#ifndef MyQGSP_BERT_LASER_h
#define MyQGSP_BERT_LASER_h 1

#include "Geant4/globals.hh"
#include "Geant4/G4VModularPhysicsList.hh"

class MyQGSP_BERT_LASER: public G4VModularPhysicsList
{
public:
  MyQGSP_BERT_LASER(G4int ver = 1);
  virtual ~MyQGSP_BERT_LASER()=default;

  MyQGSP_BERT_LASER(const MyQGSP_BERT_LASER &) = delete;
  MyQGSP_BERT_LASER & operator=(const MyQGSP_BERT_LASER &)=delete;
  
};

#endif



