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

#ifndef MuONePrimaryGeneratorAction_h
#define MuONePrimaryGeneratorAction_h 1

#include "G4GeneralParticleSource.hh"
#include "G4VUserPrimaryGeneratorAction.hh"

class G4ParticleGun;
class G4Event;
class MuONePrimaryGeneratorMessenger;

class MuONePrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    MuONePrimaryGeneratorAction();
    ~MuONePrimaryGeneratorAction();

  public:
    void GeneratePrimaries ( G4Event* anEvent );
    inline void SetMomentum ( G4double val ) {
      _momentum = val;
    }
    inline G4double GetMomentum() const {
      return _momentum;
    }
    inline void SetSigmaMomentum ( G4double val ) {
      _sigmaMomentum = val;
    }
    inline G4double GetSigmaMomentum() const {
      return _sigmaMomentum;
    }
    inline void SetSigmaAngle ( G4double val ) {
      _sigmaAngle = val;
    }
    inline G4double GetSigmaAngle() const {
      return _sigmaAngle;
    }
    void SetPositions ( G4ThreeVector pos );

    void SetParticle ( G4String part );
    inline void SetRandom ( G4bool randm ) {
      _random = randm;
    }

  private:
    G4ParticleGun*                     _particleGun;
    MuONePrimaryGeneratorMessenger*    _gunMessenger;
    G4ParticleDefinition*              _positron;
    G4ParticleDefinition*              _muon;
    G4ParticleDefinition*              _pion;
    G4ParticleDefinition*              _kaon;
    G4ParticleDefinition*              _proton;
    G4double                           _momentum;
    G4double                           _sigmaMomentum;
    G4double                           _sigmaAngle;
    G4ThreeVector                      _pos;
    G4String                           _particle;
    G4bool                             _random;

};

#endif


