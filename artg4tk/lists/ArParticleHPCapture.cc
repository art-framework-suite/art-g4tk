//
// ********************************************************************
// This is a variation of the file provided by the
// Geant4 collaboration (https://geant4.web.cern.ch/):
// G4ParticleHPCapture.cc
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
// -- artg4tk includes
#include "artg4tk/lists/ArParticleHPCapture.hh"
#include "artg4tk/lists/ArParticleHPCaptureFS.hh"

#include "Geant4/G4IonTable.hh"
#include "Geant4/G4ParticleHPCapture.hh"
#include "Geant4/G4ParticleHPDeExGammas.hh"
#include "Geant4/G4ParticleHPManager.hh"
#include "Geant4/G4ParticleTable.hh"
#include "Geant4/G4SystemOfUnits.hh"
#include "Geant4/G4Threading.hh"

ArParticleHPCapture::ArParticleHPCapture()
  : G4HadronicInteraction("NeutronHPCapture"), theCapture(NULL), numEle(0)
{
  SetMinEnergy(0.0);
  SetMaxEnergy(20. * MeV);
}

ArParticleHPCapture::~ArParticleHPCapture()
{
  if (!G4Threading::IsWorkerThread()) {
    if (theCapture != NULL) {
      for (std::vector<G4ParticleHPChannel*>::iterator ite = theCapture->begin();
           ite != theCapture->end();
           ite++) {
        delete *ite;
      }
      theCapture->clear();
    }
  }
}

#include "Geant4/G4ParticleHPThermalBoost.hh"
G4HadFinalState*
ArParticleHPCapture::ApplyYourself(const G4HadProjectile& aTrack, G4Nucleus& aNucleus)
{
  G4ParticleHPManager::GetInstance()->OpenReactionWhiteBoard();
  if (std::getenv("NeutronHPCapture"))
    G4cout << " ####### ArParticleHPCapture called" << G4endl;
  const G4Material* theMaterial = aTrack.GetMaterial();
  G4int n = theMaterial->GetNumberOfElements();
  G4int index = theMaterial->GetElement(0)->GetIndex();
  if (n != 1) {
    G4double* xSec = new G4double[n];
    G4double sum = 0;
    G4int i;
    const G4double* NumAtomsPerVolume = theMaterial->GetVecNbOfAtomsPerVolume();
    G4double rWeight;
    G4ParticleHPThermalBoost aThermalE;
    for (i = 0; i < n; i++) {
      index = theMaterial->GetElement(i)->GetIndex();
      rWeight = NumAtomsPerVolume[i];
      xSec[i] = ((*theCapture)[index])
                  ->GetXsec(aThermalE.GetThermalEnergy(
                    aTrack, theMaterial->GetElement(i), theMaterial->GetTemperature()));
      xSec[i] *= rWeight;
      sum += xSec[i];
    }
    G4double random = G4UniformRand();
    G4double running = 0;
    for (i = 0; i < n; i++) {
      running += xSec[i];
      index = theMaterial->GetElement(i)->GetIndex();
      if (sum == 0 || random <= running / sum)
        break;
    }
    if (i == n)
      i = std::max(0, n - 1);
    delete[] xSec;
  }

  G4HadFinalState* result = ((*theCapture)[index])->ApplyYourself(aTrack);

  // Overwrite target parameters
  aNucleus.SetParameters(G4ParticleHPManager::GetInstance()->GetReactionWhiteBoard()->GetTargA(),
                         G4ParticleHPManager::GetInstance()->GetReactionWhiteBoard()->GetTargZ());
  const G4Element* target_element = (*G4Element::GetElementTable())[index];
  const G4Isotope* target_isotope = NULL;
  G4int iele = target_element->GetNumberOfIsotopes();
  for (G4int j = 0; j != iele; j++) {
    target_isotope = target_element->GetIsotope(j);
    if (target_isotope->GetN() ==
        G4ParticleHPManager::GetInstance()->GetReactionWhiteBoard()->GetTargA())
      break;
  }
  aNucleus.SetIsotope(target_isotope);

  G4ParticleHPManager::GetInstance()->CloseReactionWhiteBoard();
  return result;
}

const std::pair<G4double, G4double>
ArParticleHPCapture::GetFatalEnergyCheckLevels() const
{
  return std::pair<G4double, G4double>(10 * perCent, DBL_MAX);
}

G4int
ArParticleHPCapture::GetVerboseLevel() const
{
  return G4ParticleHPManager::GetInstance()->GetVerboseLevel();
}
void
ArParticleHPCapture::SetVerboseLevel(G4int newValue)
{
  G4ParticleHPManager::GetInstance()->SetVerboseLevel(newValue);
}

void
ArParticleHPCapture::BuildPhysicsTable(const G4ParticleDefinition&)
{

  G4ParticleHPManager* hpmanager = G4ParticleHPManager::GetInstance();

  theCapture = hpmanager->GetCaptureFinalStates();

  if (G4Threading::IsMasterThread()) {

    if (theCapture == NULL)
      theCapture = new std::vector<G4ParticleHPChannel*>;

    if (numEle == (G4int)G4Element::GetNumberOfElements())
      return;

    if (theCapture->size() == G4Element::GetNumberOfElements()) {
      numEle = G4Element::GetNumberOfElements();
      return;
    }

    if (!std::getenv("G4NEUTRONHPDATA"))
      throw G4HadronicException(
        __FILE__,
        __LINE__,
        "Please setenv G4NEUTRONHPDATA to point to the neutron cross-section files.");
    dirName = std::getenv("G4NEUTRONHPDATA");
    G4String tString = "/Capture";
    dirName = dirName + tString;

    G4ParticleHPCaptureFS* theFS = new G4ParticleHPCaptureFS;
    ArParticleHPCaptureFS* theArFS = new ArParticleHPCaptureFS;
    for (G4int i = numEle; i < (G4int)G4Element::GetNumberOfElements(); i++) {
      theCapture->push_back(new G4ParticleHPChannel);
      ((*theCapture)[i])->Init((*(G4Element::GetElementTable()))[i], dirName);
      if ((*(G4Element::GetElementTable()))[i]->GetZ() == 18) {
        ((*theCapture)[i])->Register(theArFS);
        std::cout << "======= use new Argon Capture =======" << std::endl; // Jingbo Wang
      } else
        ((*theCapture)[i])->Register(theFS);
    }
    delete theFS;
    delete theArFS;
    hpmanager->RegisterCaptureFinalStates(theCapture);
  }
  numEle = G4Element::GetNumberOfElements();
}

void
ArParticleHPCapture::ModelDescription(std::ostream& outFile) const
{
  outFile << "High Precision model based on Evaluated Nuclear Data Files (ENDF) for radiative "
             "capture reaction of neutrons below 20MeV\n";
}
