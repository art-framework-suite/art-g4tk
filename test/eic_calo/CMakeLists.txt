cet_test(eiccalo_test HANDBUILT
  TEST_EXEC art
  TEST_ARGS --rethrow-all -c eic_calo.fcl
  DATAFILES   eic_calo.fcl standard_physicslist.fcl eic_CrystalCal_PbF2.gdml
  )
