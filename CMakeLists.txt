#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(Simulation)

set(mytarget    MuONe           )  # equivalent to "name := myexample" in G4 GNUmakefiles
set(useROOT     true            )  # use true or false (or comment to set to false)

list(APPEND CMAKE_PREFIX_PATH /opt/Geant4)

SET(GCC_DEB_COMPILE_FLAGS "-g")
add_definitions(${GCC_DEB_COMPILE_FLAGS})
#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_G4VIS_BUILD_OPENGLX_DRIVER ON)
option(WITH_G4VIS_BUILD_OPENGLXM_DRIVER ON)
option(WITH_G4VIS_USE_OPENGLX ON)
option(WITH_G4VIS_USE_OPENGLXM ON)

option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#
include(${Geant4_USE_FILE})

include_directories(${PROJECT_SOURCE_DIR}/include)


#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(${mytarget} ${mytarget}.cc ${sources} ${headers})
target_link_libraries(${mytarget} ${Geant4_LIBRARIES})

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build CompositeCalorimeter. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(EXAMPLECOMPOSITECALORIMETER_SCRIPTS
#  gui.mac
#  test.g4mac
#  vis.mac
#  dataconf/g4testbeamhcal96.conf
#  dataconf/testbeamhcal96.conf
#  datageom/tbhcal96.geom
#  datageom/tbhcal96hcal.geom
#  datageom/tbhcal96xtal.geom
#  dataglobal/fmap.tb96
#  dataglobal/material.cms
#  dataglobal/rotation.cms
#  datavis/tbhcal96hcal.vis
#  datavis/tbhcal96.vis
#  datavis/tbhcal96xtal.vis
  )

list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
#---Locate the ROOT package and defines a number of variables (e.g. ROOT_INCLUDE_DIRS)
find_package(ROOT REQUIRED COMPONENTS RIO NET NETX XROOTD)
if(ROOT_FOUND)
  message(STATUS "G4 Examples: ROOT found. --> ROOT enabled.")
  # Uncomment this line if suitable (some customization may be needed)
  add_definitions(-DG4ANALYSIS_USE_ROOT)
  #---Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
#   include(${ROOT_USE_FILE})
  include_directories(${ROOT_INCLUDE_DIRS})
  include_directories(${PROJECT_SOURCE_DIR}/include
                    ${Geant4_INCLUDE_DIR}
                    ${ROOT_INCLUDE_DIR})
  target_link_libraries(${mytarget} ${ROOT_LIBRARIES})
  EXECUTE_PROCESS(COMMAND rootcint -f RootDictionary.cc -I./include RootEvent.hh RootStep.hh )
  EXECUTE_PROCESS(COMMAND root-config --cflags OUTPUT_VARIABLE ROOT_CXX_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ROOT_CXX_FLAGS}")
else()
  message(STATUS "G4 Examples: ROOT not found. --> iort_therapy example with ROOT plotter disabled.")
  include_directories(${PROJECT_SOURCE_DIR}/include
                    ${Geant4_INCLUDE_DIR})
endif()

foreach(_script ${EXAMPLECOMPOSITECALORIMETER_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

#----------------------------------------------------------------------------
# For internal Geant4 use - but has no effect if you build this
# example standalone
#
##add_custom_target(ExampleCompositeCalorimeter DEPENDS CompositeCalorimeter)
# add_executable(${myexe} ${mymain} ${sources} ${headers})
if(ROOT_FOUND)
  EXECUTE_PROCESS(COMMAND root-config --libs OUTPUT_VARIABLE ROOT_LD_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
  set(CMAKE_EXE_LINKER_FLAGS "${ROOT_LD_FLAGS} RootDictionary.cc -I./include")
endif(ROOT_FOUND)

# target_link_libraries(${myexe} ${Geant4_LIBRARIES})

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS ${mytarget} DESTINATION bin)


