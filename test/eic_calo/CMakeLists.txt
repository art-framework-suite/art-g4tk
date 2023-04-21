cet_test(eiccalo_test HANDBUILT
  TEST_EXEC art
  TEST_ARGS --rethrow-all -c testeic_calo.fcl
  DATAFILES   testeic_calo.fcl teststandard_physicslist.fcl testeic_CrystalCal_PbF2.gdml
  )
