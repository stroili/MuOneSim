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
#include "MuONeEventMessenger.hh"
#include "MuONeEventAction.hh"

#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAnInteger.hh"

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
MuONeEventMessenger::MuONeEventMessenger ( MuONeEventAction* event )
    : _MuONeEvent ( event )
{
  _saveThresholdCmd = new G4UIcmdWithAnInteger ( "/MuONe/saveThreshold", this );
  _saveThresholdCmd->SetGuidance ( "Set the photon count threshold for saving the random number seed for an event." );
  _saveThresholdCmd->SetParameterName ( "photons", true );
  _saveThresholdCmd->SetDefaultValue ( 4500 );
  _saveThresholdCmd->AvailableForStates ( G4State_PreInit, G4State_Idle );

  _verboseCmd = new G4UIcmdWithAnInteger ( "/MuONe/eventVerbose", this );
  _verboseCmd->SetGuidance ( "Set the verbosity of event data." );
  _verboseCmd->SetParameterName ( "verbose", true );
  _verboseCmd->SetDefaultValue ( 1 );

  _pmtThresholdCmd = new G4UIcmdWithAnInteger ( "/MuONe/pmtThreshold", this );
  _pmtThresholdCmd->SetGuidance ( "Set the pmtThreshold (in # of photons)" );

  _forceDrawPhotonsCmd = new G4UIcmdWithABool ( "/MuONe/forceDrawPhotons", this );
  _forceDrawPhotonsCmd->SetGuidance ( "Force drawing of photons." );
  _forceDrawPhotonsCmd
  ->SetGuidance ( "(Higher priority than /MuONe/forceDrawNoPhotons)" );

  _forceDrawNoPhotonsCmd = new G4UIcmdWithABool ( "/MuONe/forceDrawNoPhotons", this );
  _forceDrawNoPhotonsCmd->SetGuidance ( "Force no drawing of photons." );
  _forceDrawNoPhotonsCmd
  ->SetGuidance ( "(Lower priority than /MuONe/forceDrawPhotons)" );
}

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
MuONeEventMessenger::~MuONeEventMessenger()
{
  delete _saveThresholdCmd;
  delete _verboseCmd;
  delete _pmtThresholdCmd;
  delete _forceDrawPhotonsCmd;
  delete _forceDrawNoPhotonsCmd;
}

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
void MuONeEventMessenger::SetNewValue ( G4UIcommand* command, G4String newValue )
{
  if ( command == _saveThresholdCmd ) {
    _MuONeEvent->SetSaveThreshold ( _saveThresholdCmd->GetNewIntValue ( newValue ) );
  } else if ( command == _verboseCmd ) {
    _MuONeEvent->SetEventVerbose ( _verboseCmd->GetNewIntValue ( newValue ) );
  } else if ( command == _pmtThresholdCmd ) {
    _MuONeEvent->SetPMTThreshold ( _pmtThresholdCmd->GetNewIntValue ( newValue ) );
  } else if ( command == _forceDrawPhotonsCmd ) {
    _MuONeEvent->SetForceDrawPhotons ( _forceDrawPhotonsCmd
                                          ->GetNewBoolValue ( newValue ) );
  } else if ( command == _forceDrawNoPhotonsCmd ) {
    _MuONeEvent->SetForceDrawNoPhotons ( _forceDrawNoPhotonsCmd
                                            ->GetNewBoolValue ( newValue ) );
    G4cout << "TEST" << G4endl;
  }
}





