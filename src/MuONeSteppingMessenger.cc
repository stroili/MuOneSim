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
#include "MuONeSteppingMessenger.hh"
#include "MuONeSteppingAction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithABool.hh"

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
MuONeSteppingMessenger::MuONeSteppingMessenger ( MuONeSteppingAction* step )
    : _stepping ( step )
{
  _oneStepPrimariesCmd = new G4UIcmdWithABool ( "/MuONe/oneStepPrimaries", this );
  _oneStepPrimariesCmd->SetGuidance ( "Only allows primaries to go one step in the scintillator volume before being killed." );
}

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
MuONeSteppingMessenger::~MuONeSteppingMessenger()
{
  delete _oneStepPrimariesCmd;
}

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
void
MuONeSteppingMessenger::SetNewValue ( G4UIcommand* command, G4String newValue )
{
  if ( command == _oneStepPrimariesCmd ) {
    _stepping->SetOneStepPrimaries ( _oneStepPrimariesCmd
                                     ->GetNewBoolValue ( newValue ) );
  }
}
