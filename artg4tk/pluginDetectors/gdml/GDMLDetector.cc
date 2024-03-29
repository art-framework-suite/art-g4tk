//
//               __        __ __  __  __
//   ____ ______/ /_____ _/ // / / /_/ /__
//  / __ `/ ___/ __/ __ `/ // /_/ __/ //_/
// / /_/ / /  / /_/ /_/ /__  __/ /_/ ,<
// \__,_/_/   \__/\__, /  /_/  \__/_/|_|
//               /____/
//
// artg4tk: art based Geant 4 Toolkit
//
//=============================================================================
// GDMLDetector_service.hh:
// GDMLDetectorService is the service that constructs the Geant 4 Geometry
// as specified in a gdml file.
// To use this service, all you need to do is put it in the services section
// of the fcl configuration file, like this (Just change the name of the gdml file):
//
// <pre>
// services: {
//   ...
//     ...
// GDMLDetector :
//    {
//    category: "world"
//    gdmlFileName_ : "ta_target.gdml"
//    }
//   }
// </pre>
// Author: Hans Wenzel (Fermilab)
//=============================================================================

// framework includes:
#include "art/Framework/Core/ProducesCollector.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Services/Registry/ServiceHandle.h"

// artg4tk includes:
#include "artg4tk/DataProducts/G4DetectorHits/ArtG4tkVtx.hh"
#include "artg4tk/pluginDetectors/gdml/ByParticle.hh"
#include "artg4tk/pluginDetectors/gdml/CalorimeterHit.hh"
#include "artg4tk/pluginDetectors/gdml/CalorimeterSD.hh"
#include "artg4tk/pluginDetectors/gdml/ColorReader.hh"
#include "artg4tk/pluginDetectors/gdml/DRCalorimeterHit.hh"
#include "artg4tk/pluginDetectors/gdml/DRCalorimeterSD.hh"
#include "artg4tk/pluginDetectors/gdml/GDMLDetector_service.hh"
#include "artg4tk/pluginDetectors/gdml/HadIntAndEdepTrkSD.hh"
#include "artg4tk/pluginDetectors/gdml/HadInteractionSD.hh"
#include "artg4tk/pluginDetectors/gdml/PhotonHit.hh"
#include "artg4tk/pluginDetectors/gdml/PhotonSD.hh"
#include "artg4tk/pluginDetectors/gdml/TrackerHit.hh"
#include "artg4tk/pluginDetectors/gdml/TrackerSD.hh"
#include "artg4tk/services/DetectorHolder_service.hh"

//
// Geant 4 includes:
#include "Geant4/G4GDMLParser.hh"
#include "Geant4/G4LogicalVolume.hh"
#include "Geant4/G4LogicalVolumeStore.hh"
#include "Geant4/G4PhysicalVolumeStore.hh"
#include "Geant4/G4SDManager.hh"
#include "Geant4/G4StepLimiter.hh"
#include "Geant4/G4UnitsTable.hh"
#include "Geant4/G4UserLimits.hh"
#include "Geant4/G4VPhysicalVolume.hh"
// C++ includes
#include <map>
#include <memory>
#include <unordered_map>
#include <vector>
#ifdef WITH_G4CXOPTICKS
#include "G4CXOpticks.hh"
#include <cuda_runtime.h>
#endif
using std::string;

artg4tk::GDMLDetectorService::GDMLDetectorService(fhicl::ParameterSet const& p)
  : artg4tk::DetectorBase(p,
                          p.get<string>("name", "GDMLDetectorService"),
                          p.get<string>("category", "World"),
                          p.get<string>("mother_category", ""))
  , gdmlFileName_(p.get<std::string>("gdmlFileName_", ""))
  , checkoverlaps_(p.get<bool>("CheckOverlaps", false))
  , volumeNames_(p.get<std::vector<std::string>>("volumeNames", {}))
  , stepLimits_(p.get<std::vector<float>>("stepLimits", {}))
  , dumpMP_(p.get<bool>("DumpMaterialProperties", false))
  , logInfo_("GDMLDetectorService")
{
  // Make sure units are defined.
  G4UnitDefinition::GetUnitsTable();
  // -- D.R. : Check for valid volume, steplimit pairs
  if (volumeNames_.size() != stepLimits_.size()) {
    throw cet::exception("LArG4DetectorService") << "Configuration error: volumeNames:[] and"
                                                 << " stepLimits:[] have different sizes!"
                                                 << "\n";
  }

  inputVolumes_ = volumeNames_.size();

  //-- define commonly used units, that we might need
  new G4UnitDefinition("volt/cm", "V/cm", "Electric field", CLHEP::volt / CLHEP::cm);

  if (inputVolumes_ > 0) {
    mf::LogInfo("LArG4DetectorService::Ctr")
      << "Reading stepLimit(s) from the configuration file, for volume(s):";
  }
  for (size_t i = 0; i < inputVolumes_; ++i) {
    if (stepLimits_.at(i) < 0) {
      throw cet::exception("LArG4DetectorService")
        << "Invalid stepLimits found. Step limits must be"
        << " positive! Bad value : stepLimits[" << i << "] = " << stepLimits_.at(i) << " [mm]\n";
    }
    selectedVolumes_.push_back(std::make_pair(volumeNames_.at(i), stepLimits_.at(i)));
    mf::LogInfo("LArG4DetectorService::Ctr")
      << "Volume: " << volumeNames_.at(i) << ", stepLimit: " << stepLimits_.at(i);
  } //--loop over inputVolumes
} //--Ctor

// Destructor

artg4tk::GDMLDetectorService::~GDMLDetectorService() = default;

std::vector<G4LogicalVolume*>
artg4tk::GDMLDetectorService::doBuildLVs()
{
  ColorReader* fReader = new ColorReader;
  G4GDMLParser* parser = new G4GDMLParser(fReader);
  parser->SetOverlapCheck(checkoverlaps_);

  parser->Read(gdmlFileName_,false);
  G4VPhysicalVolume* World = parser->GetWorldVolume();
  std::cout << World->GetTranslation() << std::endl << std::endl;
  std::cout << "Found World:  " << World->GetName() << std::endl;
#ifdef WITH_G4CXOPTICK
  //@@@ Integration
  G4CXOpticks::SetGeometry(World);
#endif

  std::cout << "World LV:  " << World->GetLogicalVolume()->GetName() << std::endl;
  G4LogicalVolumeStore* pLVStore = G4LogicalVolumeStore::GetInstance();
  std::cout << "Found " << pLVStore->size() << " logical volumes." << std::endl << std::endl;
  G4PhysicalVolumeStore* pPVStore = G4PhysicalVolumeStore::GetInstance();
  std::cout << "Found " << pPVStore->size() << " physical volumes." << std::endl << std::endl;
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  const G4GDMLAuxMapType* auxmap = parser->GetAuxMap();
  std::cout << "Found " << auxmap->size() << " volume(s) with auxiliary information." << std::endl
            << std::endl;
  std::cout << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"
            << std::endl;
  for (G4GDMLAuxMapType::const_iterator iter = auxmap->begin(); iter != auxmap->end(); iter++) {
    G4cout << "Volume " << ((*iter).first)->GetName()
           << " has the following list of auxiliary information: " << G4endl;
    for (G4GDMLAuxListType::const_iterator vit = (*iter).second.begin();
         vit != (*iter).second.end();
         vit++) {
      std::cout << "--> Type: " << (*vit).type << " Value: " << (*vit).value << std::endl;

      G4double value = atof((*vit).value);
      G4double val_unit = 1; //--no unit
      G4String provided_category = "NONE";
      if (((*vit).unit) && ((*vit).unit != "")) { // -- if provided and non-NULL
        val_unit = G4UnitDefinition::GetValueOf((*vit).unit);
        mf::LogInfo("AuxUnit") << " Unit parsed = " << (*vit).unit;
        value *=
          val_unit; //-- Now do something with the value, making sure that the unit is appropriate
        provided_category = G4UnitDefinition::GetCategory((*vit).unit);
      }
      if ((*vit).type == "StepLimit") {
        G4UserLimits* fStepLimit = NULL;

        //-- check that steplimit has valid length unit category
        G4String steplimit_category = "Length";
        if (provided_category == steplimit_category) {
          mf::LogInfo("AuxUnit") << "Valid StepLimit unit category obtained: "
                                 << provided_category.c_str();
          fStepLimit = new G4UserLimits(value);
          G4cout << "fStepLimit:  " << value << "  " << value / CLHEP::cm << " cm" << std::endl;
        } else if (provided_category ==
                   "NONE") { //--no unit category provided, use the default CLHEP::mm
          MF_LOG_WARNING("StepLimitUnit") << "StepLimit in geometry file does not have a unit!"
                                          << " Defaulting to mm...";
          value *= CLHEP::mm;
          fStepLimit = new G4UserLimits(value);
          G4cout << "fStepLimit:  " << value << "  " << value / CLHEP::cm << " cm" << std::endl;
        } else { //--wrong unit category provided
          throw cet::exception("StepLimitUnit")
            << "StepLimit does not have a valid length unit!\n"
            << " Category of unit provided = " << provided_category << ".\n";
        }

        ((*iter).first)->SetUserLimits(fStepLimit);
        // -- D.R. insert into map <volName,stepLimit> to cross-check later
        MF_LOG_DEBUG("LArG4DetectorService::")
          << "Set stepLimit for volume: " << ((*iter).first)->GetName() << " from the GDML file.";
        setGDMLVolumes_.insert(std::make_pair(((*iter).first)->GetName(), atof((*vit).value)));
      }
      if((*vit).type == "ExcitationEnergy")
      {
        G4String ExcitationEnergy_category = "Energy";
        if(provided_category == ExcitationEnergy_category)
        {
          G4cout << "Valid ExcitationEnergy unit category obtained: " << provided_category.c_str()
                 << G4endl;
          G4cout << " unit Value:" << val_unit << "  Value:  " << value << G4endl;
          G4cout << " unit Value:" << val_unit << "  Value:  " << value / CLHEP::eV << " eV"
                 << G4endl;
          ((*iter).first)->GetMaterial()->GetIonisation()->SetMeanExcitationEnergy(value);
          G4cout << " Mean Ionization energy:  "
                 <<((*iter).first) ->GetMaterial()->GetIonisation()->GetMeanExcitationEnergy() << G4endl;
        }
      }
      if ((*vit).type == "SensDet") {
        if ((*vit).value == "DRCalorimeter") {
          G4String name = ((*iter).first)->GetName() + "_DRCalorimeter";
          DRCalorimeterSD* aDRCalorimeterSD = new DRCalorimeterSD(name);
          SDman->AddNewDetector(aDRCalorimeterSD);
          ((*iter).first)->SetSensitiveDetector(aDRCalorimeterSD);
          std::cout << "Attaching sensitive Detector: " << (*vit).value
                    << " to Volume:  " << ((*iter).first)->GetName() << std::endl;
          DetectorList.push_back(std::make_pair((*iter).first->GetName(), (*vit).value));
        } else if ((*vit).value == "Calorimeter") {
          G4String name = ((*iter).first)->GetName() + "_Calorimeter";
          CalorimeterSD* aCalorimeterSD = new CalorimeterSD(name);
          SDman->AddNewDetector(aCalorimeterSD);
          ((*iter).first)->SetSensitiveDetector(aCalorimeterSD);
          std::cout << "Attaching sensitive Detector: " << (*vit).value
                    << " to Volume:  " << ((*iter).first)->GetName() << std::endl;
          DetectorList.push_back(std::make_pair((*iter).first->GetName(), (*vit).value));
        } else if ((*vit).value == "PhotonDetector") {
          G4String name = ((*iter).first)->GetName() + "_PhotonDetector";
          PhotonSD* aPhotonSD = new PhotonSD(name);
          SDman->AddNewDetector(aPhotonSD);
          ((*iter).first)->SetSensitiveDetector(aPhotonSD);
          std::cout << "Attaching sensitive Detector: " << (*vit).value
                    << " to Volume:  " << ((*iter).first)->GetName() << std::endl;
          DetectorList.push_back(std::make_pair((*iter).first->GetName(), (*vit).value));
        } else if ((*vit).value == "Tracker") {
          G4String name = ((*iter).first)->GetName() + "_Tracker";
          TrackerSD* aTrackerSD = new TrackerSD(name);
          SDman->AddNewDetector(aTrackerSD);
          ((*iter).first)->SetSensitiveDetector(aTrackerSD);
          std::cout << "Attaching sensitive Detector: " << (*vit).value
                    << " to Volume:  " << ((*iter).first)->GetName() << std::endl;
          DetectorList.push_back(std::make_pair((*iter).first->GetName(), (*vit).value));
        } else if ((*vit).value == "HadInteraction") {
          G4String name = ((*iter).first)->GetName() + "_HadInteraction";
          HadInteractionSD* aHadInteractionSD = new HadInteractionSD(name);
          // NOTE: This will be done in the HadInteractionSD ctor
          // SDman->AddNewDetector(aHadInteractionSD);
          ((*iter).first)->SetSensitiveDetector(aHadInteractionSD);
          std::cout << "Attaching sensitive Detector: " << (*vit).value
                    << " to Volume:  " << ((*iter).first)->GetName() << std::endl;
          DetectorList.push_back(std::make_pair((*iter).first->GetName(), (*vit).value));
        } else if ((*vit).value == "HadIntAndEdepTrk") {
          G4String name = ((*iter).first)->GetName() + "_HadIntAndEdepTrk";
          HadIntAndEdepTrkSD* aHadIntAndEdepTrkSD = new HadIntAndEdepTrkSD(name);
          // NOTE: This will be done in the HadIntAndEdepTrkSD ctor
          // SDman->AddNewDetector(aHadIntAndEdepTrkSD);
          ((*iter).first)->SetSensitiveDetector(aHadIntAndEdepTrkSD);
          std::cout << "Attaching sensitive Detector: " << (*vit).value
                    << " to Volume:  " << ((*iter).first)->GetName() << std::endl;
          DetectorList.push_back(std::make_pair((*iter).first->GetName(), (*vit).value));
        }
      }
    }
    std::cout << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"
              << std::endl;
  }
  if (dumpMP_) {
    G4cout << *(G4Material::GetMaterialTable());
    /*
      G4Region* region = G4RegionStore::GetInstance()->GetRegion("DefaultRegionForTheWorld", false);
      std::vector<G4Material*>::const_iterator mItr = region->GetMaterialIterator();
      size_t nMaterial = region->GetNumberOfMaterials();
      G4cout << nMaterial << G4endl;
      for (size_t iMate = 0; iMate < nMaterial; iMate++) {
        G4cout << (*mItr)->GetName() << G4endl;
        G4MaterialPropertiesTable* mt = (*mItr)->GetMaterialPropertiesTable();
        if (mt != nullptr) {
          mt->DumpTable();
          //            if (mt->GetProperty("SLOWCOMPONENT", true) != nullptr) {
          //  mt->GetProperty("SLOWCOMPONENT", true)->SetSpline(true);
          //    std::cout << "Scint " << mt->GetProperty("SLOWCOMPONENT", true)->GetVectorLength()<<
    std::endl;
          //}
      }
      mItr++;
    }
    G4cout << G4endl;
    */
  }
  if (inputVolumes_ > 0) {
    setStepLimits(parser);
  }
  std::cout << "List SD Tree: " << std::endl;
  SDman->ListTree();
  std::cout << " Collection Capacity:  " << SDman->GetCollectionCapacity() << std::endl;
  G4HCtable* hctable = SDman->GetHCtable();
  for (G4int j = 0; j < SDman->GetCollectionCapacity(); j++) {
    std::cout << "HC Name: " << hctable->GetHCname(j) << "   SD Name:  " << hctable->GetSDname(j)
              << std::endl;
  }
  std::cout << "==================================================" << std::endl;
  // Return our logical volumes.
  std::vector<G4LogicalVolume*> myLVvec;
  myLVvec.push_back(pLVStore->at(0)); // only need to return the LV of the world
  std::cout << "nr of LV ======================:  " << myLVvec.size() << std::endl;
  return myLVvec;
}

std::vector<G4VPhysicalVolume*>
artg4tk::GDMLDetectorService::doPlaceToPVs(std::vector<G4LogicalVolume*>)
{
  // Note we don't use our input.
  std::vector<G4VPhysicalVolume*> myPVvec;
  G4PhysicalVolumeStore* pPVStore = G4PhysicalVolumeStore::GetInstance();
  myPVvec.push_back(pPVStore->at(
    pPVStore->size() - 1)); // only need to return the PV of the world  (last entry in Volume Store)
  return myPVvec;
}

void
artg4tk::GDMLDetectorService::setStepLimits(G4GDMLParser* parser)
{
  // -- D. Rivera : This function sets step limits for volumes provided in the configuration file
  //                and overrides the step limit (if any) set for the same volumes but from the GMDL
  //                geometry file. The GDML step limit (if provided in the gdml file) is set first
  //                and later overriden by this method if a valid volumeName,setStepLimit is
  //                provided.
  MF_LOG_WARNING("LArG4DetectorService::setStepLimits")
    << "Setting step limits from configuration"
    << " file. This will OVERRIDE redundant stepLimit(s) set in the GDML file. Note"
    << " that stepLimits are only active if enabled in the physicsListService via the"
    << " appropriate parameter.";

  std::string volumeName = "";
  float previousStepLimit = 0.;
  float newStepLimit = 0.;
  for (size_t i = 0; i < inputVolumes_; ++i) {
    // -- Check whether the volumeName provided corresponds to a valid volumeName in the geometry
    G4LogicalVolume* setVol = parser->GetVolume(selectedVolumes_[i].first);

    // -- get the G4LogicalVolume corresponding to the selectedVolume
    volumeName = setVol->GetName();
    newStepLimit = selectedVolumes_[i].second;
    MF_LOG_DEBUG("LArG4DetectorService::setStepLimits")
      << "Got logical volume with name: " << volumeName;

    // -- check if a stepLimit for this volume has been set before:
    auto search = setGDMLVolumes_.find(volumeName);
    if (search != setGDMLVolumes_.end()) {
      previousStepLimit = search->second;
      if (newStepLimit != previousStepLimit) {
        MF_LOG_WARNING("LArG4DetectorService::setStepLimits")
          << "OVERRIDING PREVIOUSLY SET"
          << " STEPLIMIT FOR VOLUME : " << volumeName << " FROM " << previousStepLimit << " mm TO "
          << newStepLimit << " mm";
      } else {
        MF_LOG_WARNING("LArG4DetectorService::setStepLimits")
          << "New stepLimit matches previously"
          << " set stepLimit from the GDML file for volume : " << volumeName
          << " stepLimit : " << newStepLimit << " mm. Nothing will be changed.";
        continue;
      }
    } //--check if new steplimit differs from a previously set value

    G4UserLimits* fStepLimitOverride = new G4UserLimits(selectedVolumes_[i].second);
    mf::LogInfo("LArG4DetectorService::setStepLimits")
      << "fStepLimitOverride:  " << selectedVolumes_[i].second << "  "
      << (selectedVolumes_[i].second * CLHEP::mm) / CLHEP::cm << " cm"
      << " for volume: " << selectedVolumes_[i].first;
    setVol->SetUserLimits(fStepLimitOverride);
  } //--loop over input volumes
} //--end of setStepLimit()

void
artg4tk::GDMLDetectorService::doCallArtProduces(art::ProducesCollector& collector)
{
  // Tell Art what we produce, and label the entries
  std::vector<std::pair<std::string, std::string>>::const_iterator cii;
  for (cii = DetectorList.begin(); cii != DetectorList.end(); cii++) {
    if ((*cii).second == "DRCalorimeter") {
      std::string identifier = myName() + (*cii).first;
      collector.produces<DRCalorimeterHitCollection>(identifier);
      std::string EdepID = identifier + "Edep";
      collector.produces<ByParticle>(EdepID);
      std::string NCerenID = identifier + "NCeren";
      collector.produces<ByParticle>(NCerenID);
    } else if ((*cii).second == "Calorimeter") {
      std::string identifier = myName() + (*cii).first;
      collector.produces<CalorimeterHitCollection>(identifier);
    } else if ((*cii).second == "PhotonDetector") {
      std::string identifier = myName() + (*cii).first;
      collector.produces<PhotonHitCollection>(identifier);
    } else if ((*cii).second == "Tracker") {
      std::string identifier = myName() + (*cii).first;
      collector.produces<TrackerHitCollection>(identifier);
    } else if ((*cii).second == "HadInteraction") {
      // std::string identifier = myName() + (*cii).first;
      collector.produces<ArtG4tkVtx>(); // do NOT use product instance name (for now)
    } else if ((*cii).second == "HadIntAndEdepTrk") {
      collector.produces<ArtG4tkVtx>();
      collector.produces<TrackerHitCollection>();
    }
  }
}

void
artg4tk::GDMLDetectorService::doFillEventWithArtHits(G4HCofThisEvent* myHC)
{
  //
  // NOTE(JVY): 1st hadronic interaction will be fetched as-is from HadInteractionSD
  //            a copy (via copy ctor) will be placed directly into art::Event
  //
  std::vector<std::pair<std::string, std::string>>::const_iterator cii;
  // std::cout << "****************Detectorlist size:  " << DetectorList.size() << std::endl;
  for (cii = DetectorList.begin(); cii != DetectorList.end(); cii++) {
    std::string sdname = (*cii).first + "_" + (*cii).second;
    //        std::cout << "****************SDNAME:" << sdname << std::endl;
    if ((*cii).second == "HadInteraction") {
      G4SDManager* sdman = G4SDManager::GetSDMpointer();
      HadInteractionSD* hisd =
        dynamic_cast<HadInteractionSD*>(sdman->FindSensitiveDetector(sdname));
      if (hisd) {
        const ArtG4tkVtx& inter = hisd->Get1stInteraction();
        if (inter.GetNumOutcoming() > 0) {
          std::unique_ptr<ArtG4tkVtx> firstint(new ArtG4tkVtx(inter));
          art::ServiceHandle<artg4tk::DetectorHolderService> detectorHolder;
          art::Event& e = detectorHolder->getCurrArtEvent();
          e.put(std::move(firstint)); // note that there's NO product instance name (for now, at
                                      // least) (part of) the is that the name is encoded into the
                                      // "collection" which is NOT used in this specific case
        }
        hisd->clear(); // clear out after movind info to EDM; no need to clea out in the producer !
      }
    } else if ((*cii).second == "HadIntAndEdepTrk") {
      G4SDManager* sdman = G4SDManager::GetSDMpointer();
      HadIntAndEdepTrkSD* sd =
        dynamic_cast<HadIntAndEdepTrkSD*>(sdman->FindSensitiveDetector(sdname));
      if (sd) {
        art::ServiceHandle<artg4tk::DetectorHolderService> detectorHolder;
        art::Event& e = detectorHolder->getCurrArtEvent();
        const ArtG4tkVtx& inter = sd->Get1stInteraction();
        if (inter.GetNumOutcoming() > 0) {
          std::unique_ptr<ArtG4tkVtx> firstint(new ArtG4tkVtx(inter));
          e.put(std::move(firstint)); // note that there's NO product instance name (for now, at
                                      // least) (part of) the is that the name is encoded into the
                                      // "collection" which is NOT used in this specific case
        }
        const TrackerHitCollection& trkhits = sd->GetEdepTrkHits();
        if (!trkhits.empty()) {
          std::unique_ptr<TrackerHitCollection> hits(new TrackerHitCollection(trkhits));
          e.put(std::move(hits));
        }
        sd->clear(); // clear out after moving info to EDM; no need to clea out in the producer !
      }
    } else if ((*cii).second == "Tracker") {
      G4SDManager* sdman = G4SDManager::GetSDMpointer();
      TrackerSD* trsd = dynamic_cast<TrackerSD*>(sdman->FindSensitiveDetector(sdname));
      art::ServiceHandle<artg4tk::DetectorHolderService> detectorHolder;
      art::Event& e = detectorHolder->getCurrArtEvent();
      const TrackerHitCollection& trkhits = trsd->GetHits();
      std::unique_ptr<TrackerHitCollection> hits(new TrackerHitCollection(trkhits));
      std::string identifier = myName() + (*cii).first;
      e.put(std::move(hits), identifier);
    } else if ((*cii).second == "Calorimeter") {
      G4SDManager* sdman = G4SDManager::GetSDMpointer();
      CalorimeterSD* calsd = dynamic_cast<CalorimeterSD*>(sdman->FindSensitiveDetector(sdname));
      art::ServiceHandle<artg4tk::DetectorHolderService> detectorHolder;
      art::Event& e = detectorHolder->getCurrArtEvent();
      const CalorimeterHitCollection& calhits = calsd->GetHits();
      std::unique_ptr<CalorimeterHitCollection> hits(new CalorimeterHitCollection(calhits));
      std::string identifier = myName() + (*cii).first;
      e.put(std::move(hits), identifier);
    } else if ((*cii).second == "DRCalorimeter") {
      G4SDManager* sdman = G4SDManager::GetSDMpointer();
      DRCalorimeterSD* drcalsd =
        dynamic_cast<DRCalorimeterSD*>(sdman->FindSensitiveDetector(sdname));
      art::ServiceHandle<artg4tk::DetectorHolderService> detectorHolder;
      art::Event& e = detectorHolder->getCurrArtEvent();
      const DRCalorimeterHitCollection& drcalhits = drcalsd->GetHits();
      std::unique_ptr<DRCalorimeterHitCollection> hits(new DRCalorimeterHitCollection(drcalhits));
      std::string identifier = myName() + (*cii).first;
      e.put(std::move(hits), identifier);
      //
      const ByParticle& edeps = drcalsd->GetEbyParticle();
      std::unique_ptr<ByParticle> edepsptr(new ByParticle(edeps));
      std::string edidentifier = myName() + (*cii).first + "Edep";
      e.put(std::move(edepsptr), edidentifier);
      //
      const ByParticle& nceren = drcalsd->GetNCerenbyParticle();
      std::unique_ptr<ByParticle> ncerenptr(new ByParticle(nceren));
      std::string ncidentifier = myName() + (*cii).first + "NCeren";
      e.put(std::move(ncerenptr), ncidentifier);
    } else if ((*cii).second == "PhotonDetector") {
      G4SDManager* sdman = G4SDManager::GetSDMpointer();
      PhotonSD* phsd = dynamic_cast<PhotonSD*>(sdman->FindSensitiveDetector(sdname));
      art::ServiceHandle<artg4tk::DetectorHolderService> detectorHolder;
      art::Event& e = detectorHolder->getCurrArtEvent();
      const PhotonHitCollection& phhits = phsd->GetHits();
      std::unique_ptr<PhotonHitCollection> hits(new PhotonHitCollection(phhits));
      std::string identifier = myName() + (*cii).first;
      e.put(std::move(hits), identifier);
    }
  }
}
