//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
#include "MuONeDetectorMessenger.hh"
#include "MuONeDetectorConstruction.hh"

#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcommand.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAString.hh"
#include "G4Scintillation.hh"

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
MuONeDetectorMessenger::MuONeDetectorMessenger ( MuONeDetectorConstruction* MuONeDetect )
    : _MuONeDetector ( MuONeDetect )
{
  //Setup a command directory for detector controls with guidance
  _detectorDir = new G4UIdirectory ( "/MuONe/detector/" );
  _detectorDir->SetGuidance ( "Detector geometry control" );

  _volumesDir = new G4UIdirectory ( "/MuONe/detector/volumes/" );
  _volumesDir->SetGuidance ( "Enable/disable volumes" );

  //Various commands for modifying detector geometry
  _dimensionsCmd =
    new G4UIcmdWith3VectorAndUnit ( "/MuONe/detector/dimensions", this );
  _dimensionsCmd->SetGuidance ( "Set the dimensions of the detector volume." );
  _dimensionsCmd->SetParameterName ( "_scint_x", "_scint_y", "_scint_z", false );
  _dimensionsCmd->SetDefaultUnit ( "cm" );

  _housingThicknessCmd = new G4UIcmdWithADoubleAndUnit
  ( "/MuONe/detector/housingThickness", this );
  _housingThicknessCmd->SetGuidance ( "Set the thickness of the housing." );
  _housingThicknessCmd->SetParameterName ( "d_mtl", false );
  _housingThicknessCmd->SetDefaultUnit ( "cm" );

  _pmtRadiusCmd = new G4UIcmdWithADoubleAndUnit
  ( "/MuONe/detector/pmtRadius", this );
  _pmtRadiusCmd->SetGuidance ( "Set the radius of the PMTs." );
  _pmtRadiusCmd->SetParameterName ( "radius", false );
  _pmtRadiusCmd->SetDefaultUnit ( "cm" );

  _fiberRadiusCmd = new G4UIcmdWithADoubleAndUnit
      ( "/MuONe/detector/fiberRadius", this );
  _fiberRadiusCmd->SetGuidance ( "Set the radius of the WLS fiber." );
  _fiberRadiusCmd->SetParameterName ( "_fiberRadius", false );
  _fiberRadiusCmd->SetDefaultUnit ( "cm" );

  _nxCmd = new G4UIcmdWithAnInteger ( "/MuONe/detector/nx", this );
  _nxCmd->SetGuidance ( "Set the number of PMTs along the x-dimension." );
  _nxCmd->SetParameterName ( "nx", false );

  _nyCmd = new G4UIcmdWithAnInteger ( "/MuONe/detector/ny", this );
  _nyCmd->SetGuidance ( "Set the number of PMTs along the y-dimension." );
  _nyCmd->SetParameterName ( "ny", false );

  _nzCmd = new G4UIcmdWithAnInteger ( "/MuONe/detector/nz", this );
  _nzCmd->SetGuidance ( "Set the number of PMTs along the z-dimension." );
  _nzCmd->SetParameterName ( "nz", false );

  _sphereCmd = new G4UIcmdWithABool ( "/MuONe/detector/volumes/sphere", this );
  _sphereCmd->SetGuidance ( "Enable/Disable the sphere." );

  _reflectivityCmd = new G4UIcmdWithADouble ( "/MuONe/detector/reflectivity",
      this );
  _reflectivityCmd->SetGuidance ( "Set the reflectivity of the housing." );

  _wlsCmd = new G4UIcmdWithABool ( "/MuONe/detector/volumes/wls", this );
  _wlsCmd->SetGuidance ( "Enable/Disable the WLS slab" );

  _lxeCmd = new G4UIcmdWithABool ( "/MuONe/detector/volumes/lxe", this );
  _wlsCmd->SetGuidance ( "Enable/Disable the main detector volume." );

  _nFibersCmd = new G4UIcmdWithAnInteger ( "/MuONe/detector/nfibers", this );
  _nFibersCmd->SetGuidance ( "Set the number of WLS fibers in the WLS slab." );

  _slabzCmd = new G4UIcmdWithADouble ( "/MuONe/detector/slabz", this );
  _slabzCmd->SetGuidance ( "Set the z dimension of the slab in cm." );

  _updateCmd = new G4UIcommand ( "/MuONe/detector/update", this );
  _updateCmd->SetGuidance ( "Update the detector geometry with changed values." );
  _updateCmd->SetGuidance
  ( "Must be run before beamOn if detector has been changed." );

  _defaultsCmd = new G4UIcommand ( "/MuONe/detector/defaults", this );
  _defaultsCmd->SetGuidance ( "Set all detector geometry values to defaults." );
  _defaultsCmd->SetGuidance ( "(Update still required)" );

  _MainScintYield = new G4UIcmdWithADouble ( "/MuONe/detector/MainScintYield", this );
  _MainScintYield->SetGuidance ( "Set scinitillation yield of main volume." );
  _MainScintYield->SetGuidance ( "Specified in photons/MeV" );

  _WLSScintYield = new G4UIcmdWithADouble ( "/MuONe/detector/WLSScintYield", this );
  _WLSScintYield->SetGuidance ( "Set scintillation yield of WLS Slab" );
  _WLSScintYield->SetGuidance ( "Specified in photons/MeV" );

  _PMTeffCmd = new G4UIcmdWithAString ( "/MuONe/detector/PMTefficiency", this );
  _PMTeffCmd->SetGuidance ( "Set PMT efficiency" );

  _WLSabslCmd = new G4UIcmdWithAString ( "/MuONe/detector/WLSabsorption", this );
  _WLSabslCmd->SetGuidance ( "Set WLS absorption lenth" );
  _WLSabslCmd->SetDefaultValue("WLSabsorption.dat");
  _WLSabslCmd->SetParameterName("test", true, true);

  _WLSemisCmd = new G4UIcmdWithAString ( "/MuONe/detector/WLSemission", this );
  _WLSemisCmd->SetGuidance ( "Set WLS emission properties" );
  _WLSemisCmd->SetDefaultValue("WLSemission.dat");

  _WLSrindCmd = new G4UIcmdWithAString ( "/MuONe/detector/WLSrindex", this );
  _WLSrindCmd->SetGuidance ( "Set WLS refraction index" );
  _WLSrindCmd->SetDefaultValue("WLSrindex.dat");

  _targetWidthCmd = new G4UIcmdWithADoubleAndUnit
  ( "/MuONe/detector/targetThickness", this );
  _targetWidthCmd->SetGuidance ( "Set the thickness of the Be target." );
  _targetWidthCmd->SetParameterName ( "targetWidth", false );
  _targetWidthCmd->SetDefaultUnit ( "cm" );

}

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
MuONeDetectorMessenger::~MuONeDetectorMessenger()
{
  delete _dimensionsCmd;
  delete _housingThicknessCmd;
  delete _pmtRadiusCmd;
  delete _fiberRadiusCmd;
  delete _nxCmd;
  delete _nyCmd;
  delete _nzCmd;
  delete _updateCmd;
  delete _detectorDir;
  delete _volumesDir;
  delete _defaultsCmd;
  delete _sphereCmd;
  delete _wlsCmd;
  delete _lxeCmd;
  delete _nFibersCmd;
  delete _reflectivityCmd;
  delete _MainScintYield;
  delete _WLSScintYield;
  delete _slabzCmd;
  delete _PMTeffCmd;
}

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
void MuONeDetectorMessenger::SetNewValue ( G4UIcommand* command,
    G4String newValue )
{
  if ( command == _dimensionsCmd ) {
    _MuONeDetector->SetDimensions ( _dimensionsCmd->GetNew3VectorValue ( newValue ) );
  } else if ( command == _housingThicknessCmd ) {
    _MuONeDetector->SetHousingThickness ( _housingThicknessCmd->GetNewDoubleValue ( newValue ) );
  } else if ( command == _pmtRadiusCmd ) {
    _MuONeDetector->SetPMTRadius ( _pmtRadiusCmd->GetNewDoubleValue ( newValue ) );
  } else if ( command == _fiberRadiusCmd ) {
    _MuONeDetector->SetFiberRadius ( _fiberRadiusCmd->GetNewDoubleValue ( newValue ) );
  } else if ( command == _nxCmd ) {
    _MuONeDetector->SetNX ( _nxCmd->GetNewIntValue ( newValue ) );
  } else if ( command == _nyCmd ) {
    _MuONeDetector->SetNY ( _nyCmd->GetNewIntValue ( newValue ) );
  } else if ( command == _nzCmd ) {
    _MuONeDetector->SetNZ ( _nzCmd->GetNewIntValue ( newValue ) );
  } else if ( command == _updateCmd ) {
    _MuONeDetector->UpdateGeometry();
  } else if ( command == _defaultsCmd ) {
    _MuONeDetector->SetDefaults();
  } else if ( command == _sphereCmd ) {
//     _MuONeDetector->SetSphereOn(_sphereCmd->GetNewBoolValue(newValue));
  } else if ( command == _reflectivityCmd ) {
    _MuONeDetector
    ->SetHousingReflectivity ( _reflectivityCmd->GetNewDoubleValue ( newValue ) );
  } else if ( command == _wlsCmd ) {
    _MuONeDetector->SetWLSSlabOn ( _wlsCmd->GetNewBoolValue ( newValue ) );
  } else if ( command == _lxeCmd ) {
    _MuONeDetector->SetMainVolumeOn ( _lxeCmd->GetNewBoolValue ( newValue ) );
  } else if ( command == _nFibersCmd ) {
    _MuONeDetector->SetNFibers ( _nFibersCmd->GetNewIntValue ( newValue ) );
  } else if ( command == _slabzCmd ) {
    _MuONeDetector->SetSlabZ ( _slabzCmd->GetNewDoubleValue ( newValue ) );
  } else if ( command == _MainScintYield ) {
    _MuONeDetector->SetMainScintYield ( _MainScintYield->GetNewDoubleValue ( newValue ) );
  } else if ( command == _WLSScintYield ) {
    _MuONeDetector->SetWLSScintYield ( _WLSScintYield->GetNewDoubleValue ( newValue ) );
  } else if ( command == _PMTeffCmd ) {
    _MuONeDetector->SetPMTeff ( newValue );
  } else if ( command == _WLSabslCmd ) {
    _MuONeDetector->SetWLSabsl ( newValue );
  } else if ( command == _WLSemisCmd ) {
    _MuONeDetector->SetWLSemis ( newValue );
  } else if ( command == _WLSrindCmd ) {
    _MuONeDetector->SetWLSrind ( newValue );
  } else if ( command == _targetWidthCmd ) {
    _MuONeDetector->SetTargetWidth( _targetWidthCmd->GetNewDoubleValue ( newValue ) );
  }
}
