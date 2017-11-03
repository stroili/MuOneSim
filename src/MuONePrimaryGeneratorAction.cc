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
#include "MuONePrimaryGeneratorAction.hh"

#include "MuONePrimaryGeneratorMessenger.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "globals.hh"
#include "Randomize.hh"

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
MuONePrimaryGeneratorAction::MuONePrimaryGeneratorAction()
    : _pos ( 0., 0., -200. ), _particle("mu-")
{
  _gunMessenger = new MuONePrimaryGeneratorMessenger ( this );

  G4int n_particle = 1;
  _particleGun = new G4ParticleGun ( n_particle );

  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();

  G4String particleName;

  _positron = particleTable->FindParticle ( particleName = "e+" );
  _muon = particleTable->FindParticle ( particleName = "mu+" );
  _pion = particleTable->FindParticle ( particleName = "pi+" );
  _kaon = particleTable->FindParticle ( "kaon+" );
  _proton = particleTable->FindParticle ( particleName = "proton" );

  _particleGun->SetParticleDefinition ( particleTable->
                                        FindParticle ( particleName = "gamma" ) );
  //Default energy,position,momentum
  _particleGun->SetParticleEnergy ( 511. * CLHEP::keV );
  _particleGun->SetParticlePosition ( _pos );
  _particleGun->SetParticleMomentumDirection ( G4ThreeVector ( 0., 0., 1. ) );
}

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
MuONePrimaryGeneratorAction::~MuONePrimaryGeneratorAction()
{
  if ( _particleGun ) delete _particleGun;
//   if (_gunMessenger) delete _gunMessenger;
}

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
void MuONePrimaryGeneratorAction::GeneratePrimaries ( G4Event* anEvent )
{

  G4ParticleDefinition* particle ( _muon );
  if ( _random ) {
    G4int i = ( int ) ( 5.*G4UniformRand() );
    switch ( i ) {
      case 0:
        particle = _positron;
        break;
      case 1:
        particle = _muon;
        break;
      case 2:
        particle = _pion;
        break;
      case 3:
        particle = _kaon;
        break;
      default:
        particle = _proton;
        break;
    }
    _particleGun->SetParticleDefinition ( particle );
    G4cout << "particle " << particle->GetParticleName() << G4endl;
  } else {
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    particle = particleTable->FindParticle ( _particle );
//     G4cout << "======================================================================" << G4endl;
//     G4cout << "particle " << _particle << G4endl;
//     G4cout << "======================================================================" << G4endl;
    if ( particle )
      _particleGun->SetParticleDefinition ( particle );
    else
      _particleGun->SetParticleDefinition ( _pion );
  }

  _particleGun->GeneratePrimaryVertex ( anEvent );
}

void MuONePrimaryGeneratorAction::SetParticle ( G4String part )
{
  _particle = part;
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  _particleGun->SetParticleDefinition ( particleTable->FindParticle ( _particle ) );
}

void MuONePrimaryGeneratorAction::SetPositions ( G4ThreeVector pos )
{
  _pos = pos;
  _particleGun->SetParticlePosition ( _pos );
}
