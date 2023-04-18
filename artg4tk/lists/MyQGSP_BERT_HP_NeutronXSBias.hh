#ifndef MyQGSP_BERT_HP_NeutronXSBias_hh
#define MyQGSP_BERT_HP_NeutronXSBias_hh 1

#include <CLHEP/Units/SystemOfUnits.h>

#include "Geant4/CompileTimeConstraints.hh"
#include "Geant4/G4VModularPhysicsList.hh"
#include "Geant4/globals.hh"

template <class T>
class TMyQGSP_BERT_HP_NeutronXSBias : public T {
public:
  TMyQGSP_BERT_HP_NeutronXSBias(G4int ver = 1);

  G4double xsecScale() { return this->INXS_SCALE; }

private:
  enum {ok = CompileTimeConstraints::IsA<T, G4VModularPhysicsList>::ok };

  const char* NEUTRON_INXS_SCALE;

  G4double INXS_SCALE;
  // -- added in v10_04...
  void SetCuts() override;
};

#include "artg4tk/lists/MyQGSP_BERT_HP_NeutronXSBias.icc"
typedef TMyQGSP_BERT_HP_NeutronXSBias<G4VModularPhysicsList> MyQGSP_BERT_HP_NeutronXSBias;

// 2019 by D. Rivera

#endif /* MyQGSP_BERT_HP_NeutronXSBias_hh*/
