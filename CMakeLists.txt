########################################################################
# ArtG4tk
#
# NOTE: Build may be steered for internal development purposes by
#       setting the following CMake variables in cache (e.g. with cmake
#       -DXXXX:BOOL=ON):
#       
#       * WITH_G4CXOPTICKS "Build with OPTICKS"
#
# Updated for cetmodules 3.10.00 and bitrot in internal code resolved -
# as far as possible without tests - by CG 2022-04-06.
########################################################################
cmake_minimum_required (VERSION 3.20 FATAL_ERROR)

find_package(cetmodules 3.10.00 REQUIRED) # For FindGeant4.cmake
project(artg4tk VERSION 12.00.00 LANGUAGES CXX)
option(WITH_G4CXOPTICKS "Build example with OPTICKS" OFF)

include(CetCMakeEnv)
cet_cmake_env()

cet_set_compiler_flags(DIAGS CAUTIOUS WERROR NO_UNDEFINED EXTRA_FLAGS -pedantic)

cet_report_compiler_flags(REPORT_THRESHOLD VERBOSE)

find_package(CLHEP COMPONENTS Vector REQUIRED EXPORT)
find_package(Geant4 REQUIRED COMPONENTS gdml EXPORT)
find_package(art_root_io REQUIRED EXPORT)
find_package(art REQUIRED EXPORT)
find_package(canvas_root_io REQUIRED)
find_package(canvas REQUIRED EXPORT)
find_package(messagefacility REQUIRED EXPORT)
find_package(fhiclcpp REQUIRED EXPORT)
find_package(cetlib_except REQUIRED EXPORT)
find_package(Range-v3 REQUIRED EXPORT)
find_package(ROOT COMPONENTS Core Hist Tree REQUIRED EXPORT)
 message(STATUS "CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")
 message(STATUS "WITH_G4CXOPTICKS: ${WITH_G4CXOPTICKS}")
if(WITH_G4CXOPTICKS)
  if(CMAKE_PREFIX_PATH)
    message(STATUS "CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")
  endif()
  if(OPTICKS_PREFIX)
    message(STATUS "OPTICKS_PREFIX: ${OPTICKS_PREFIX}")
  endif()
  if (CMAKE_MODULE_PATH)
    message(STATUS "CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH}")
  endif()
  message(STATUS "WITH_G4CXOPTICKS is set")
  list(APPEND CMAKE_MODULE_PATH "$ENV{OPTICKS_HOME}/cmake/Modules")
  message(STATUS "*************************************************************")
  message(STATUS "CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH}")
  message(STATUS "OPTICKS_HOME:  $ENV{OPTICKS_HOME}")
  message(STATUS "*************************************************************")
  include(OpticksOptionsAsExternal)
  find_package(Opticks REQUIRED)
  include_directories(${OPTICKS_PREFIX}/include/G4CX
                      ${OPTICKS_PREFIX}/include/SysRap
                      $ENV{CUDA_INSTALL_DIR}/include
  )

  find_package(U4 REQUIRED CONFIG)
endif()


cet_cmake_module_directories(Modules BINARY)

add_subdirectory(artg4tk)
add_subdirectory(examples)
add_subdirectory(fcl)
add_subdirectory(test)

# packaging utility
# this has to be last
cet_cmake_config()
