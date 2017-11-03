// This code implementation is the intellectual property of
// the GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
#include "MuONePrimaryGeneratorMessenger.hh"
#include "MuONePrimaryGeneratorAction.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"
#include "G4ios.hh"

MuONePrimaryGeneratorMessenger::MuONePrimaryGeneratorMessenger (
  MuONePrimaryGeneratorAction * mpga )
    : _target ( mpga )
{
  _momentumCmd = new G4UIcmdWithADoubleAndUnit ( "/MuONe/generator/momentum", this );
  _momentumCmd->SetGuidance ( "Mean momentum of primaries" );
  _momentumCmd->SetParameterName ( "p", true );
  _momentumCmd->SetRange ( "p>=0." );
  _momentumCmd->SetDefaultValue ( 1. );
  _momentumCmd->SetDefaultUnit ( "GeV" );

  _sigmaMomCmd = new
  G4UIcmdWithADoubleAndUnit ( "/MuONe/generator/sigmaMomentum", this );
  _sigmaMomCmd->SetGuidance ( "Sigma momentum of primaries" );
  _sigmaMomCmd->SetParameterName ( "p", true );
  _sigmaMomCmd->SetRange ( "p>=0." );
  _sigmaMomCmd->SetDefaultValue ( 1. );
  _sigmaMomCmd->SetDefaultUnit ( "GeV" );

  G4ThreeVector defPos ( 0., 1., -20. );
  _posCmd =
    new G4UIcmdWith3VectorAndUnit ( "/MuONe/generator/position", this );
  _posCmd->SetGuidance ( "Set the position of the gun." );
//   _posCmd->SetParameterName("_pos_x","_pos_y","_pos_z",false);
  _posCmd->SetDefaultValue ( defPos );
  _posCmd->SetDefaultUnit ( "cm" );

  _particleCmd = new G4UIcmdWithAString ( "/MuONe/generator/particle", this );
  _particleCmd->SetDefaultValue ( "e-" );

  _setRandomCmd = new G4UIcmdWithABool ( "/MuONe/generator/setRandom", this );
  _setRandomCmd->SetDefaultValue ( false );
}

MuONePrimaryGeneratorMessenger::~MuONePrimaryGeneratorMessenger()
{
  delete _momentumCmd;
  delete _sigmaMomCmd;
  delete _sigmaAngCmd;
  delete _posCmd;
  delete _particleCmd;
  delete _setRandomCmd;
}

void MuONePrimaryGeneratorMessenger::SetNewValue ( G4UIcommand * command, G4String newValue )
{
  if ( command == _momentumCmd ) {
    _target->SetMomentum ( _momentumCmd->GetNewDoubleValue ( newValue ) );
  } else if ( command == _sigmaMomCmd ) {
    _target->SetSigmaMomentum ( _sigmaMomCmd->GetNewDoubleValue ( newValue ) );
  } else if ( command == _sigmaAngCmd ) {
    _target->SetSigmaAngle ( _sigmaAngCmd->GetNewDoubleValue ( newValue ) );
  } else if ( command == _posCmd ) {
    _target->SetPositions ( _posCmd->GetNew3VectorValue ( newValue ) );
  } else if ( command == _particleCmd ) {
    _target->SetParticle ( newValue );
  } else if ( command == _setRandomCmd ) {
    _target->SetRandom ( _setRandomCmd->GetNewBoolValue ( newValue ) );
  }
}

G4String MuONePrimaryGeneratorMessenger::GetCurrentValue ( G4UIcommand * command )
{
  G4String cv;
  if ( command == _momentumCmd ) {
    cv = _momentumCmd->ConvertToString ( _target->GetMomentum(), "GeV" );
  }
  if ( command == _sigmaMomCmd ) {
    cv = _sigmaMomCmd->ConvertToString ( _target->GetSigmaMomentum(), "GeV" );
  }
  if ( command == _sigmaAngCmd ) {
    cv = _sigmaAngCmd->ConvertToString ( _target->GetSigmaAngle(), "deg" );
  }

  return cv;
}
