#ifndef TMyQGSP_BERT_HP_NeutronXSBias_h
#define TMyQGSP_BERT_HP_NeutronXSBias_h 1

#include <CLHEP/Units/SystemOfUnits.h>

#include "Geant4/globals.hh"
#include "Geant4/G4VModularPhysicsList.hh"
#include "Geant4/CompileTimeConstraints.hh"

template<class T>
class TMyQGSP_BERT_HP_NeutronXSBias: public T
{
public:
  TMyQGSP_BERT_HP_NeutronXSBias(G4int ver=1);
  virtual ~TMyQGSP_BERT_HP_NeutronXSBias();

public:
  // SetCuts()
  virtual void SetCuts();

private:
  enum {ok = CompileTimeConstraints::IsA<T, G4VModularPhysicsList>::ok };
};

#include "artg4tk/lists/MyQGSP_BERT_HP_NeutronXSBias.icc"
typedef TMyQGSP_BERT_HP_NeutronXSBias<G4VModularPhysicsList> MyQGSP_BERT_HP_NeutronXSBias;

// 2019 by D. Rivera

#endif



