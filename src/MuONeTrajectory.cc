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
#include "MuONeTrajectory.hh"
#include "G4TrajectoryPoint.hh"
#include "G4Trajectory.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleTypes.hh"
#include "G4ThreeVector.hh"
#include "G4Polyline.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4VVisManager.hh"
#include "G4Polymarker.hh"

G4Allocator<MuONeTrajectory> MuONeTrajectoryAllocator;

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
MuONeTrajectory::MuONeTrajectory()
    : G4Trajectory(), _wls ( false ), _drawit ( false ), _forceNoDraw ( false ),
    _forceDraw ( false )
{
  _particleDefinition = 0;
}

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
MuONeTrajectory::MuONeTrajectory ( const G4Track* aTrack )
    : G4Trajectory ( aTrack ), _wls ( false ), _drawit ( false )
{
  _particleDefinition = aTrack->GetDefinition();
}

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
MuONeTrajectory::MuONeTrajectory ( MuONeTrajectory &right )
    : G4Trajectory ( right ), _wls ( right._wls ), _drawit ( right._drawit )
{
  _particleDefinition = right._particleDefinition;
}

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
MuONeTrajectory::~MuONeTrajectory()
{

}

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
void MuONeTrajectory::DrawTrajectory ( G4int i_mode ) const
{
  //Taken from G4VTrajectory and modified to select colours based on particle
  //type and to selectively eliminate drawing of certain trajectories.

  if ( !_forceDraw && ( !_drawit || _forceNoDraw ) )
    return;

  // If i_mode>=0, draws a trajectory as a polyline and, if i_mode!=0,
  // adds markers - yellow circles for step points and magenta squares
  // for auxiliary points, if any - whose screen size in pixels is
  // given by std::abs(i_mode)/1000.  E.g: i_mode = 5000 gives easily
  // visible markers.

  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if ( !pVVisManager ) return;

  const G4double markerSize = std::abs ( i_mode ) / 1000;
  G4bool lineRequired ( i_mode >= 0 );
  G4bool markersRequired ( markerSize > 0. );

  G4Polyline trajectoryLine;
  G4Polymarker stepPoints;
  G4Polymarker auxiliaryPoints;

//   G4cout << "---------------------------------------------" << G4endl;
//   G4cout << _particleDefinition->GetParticleName() << G4endl;
//   G4cout << "---------------------------------------------" << G4endl;
  for ( G4int i = 0; i < GetPointEntries() ; i++ ) {
    G4VTrajectoryPoint* aTrajectoryPoint = GetPoint ( i );
    const std::vector<G4ThreeVector>* auxiliaries
    = aTrajectoryPoint->GetAuxiliaryPoints();
    if ( auxiliaries ) {
      for ( size_t iAux = 0; iAux < auxiliaries->size(); ++iAux ) {
        const G4ThreeVector pos ( ( *auxiliaries ) [iAux] );
        if ( lineRequired ) {
          trajectoryLine.push_back ( pos );
        }
        if ( markersRequired ) {
          auxiliaryPoints.push_back ( pos );
        }
      }
    }
    const G4ThreeVector pos ( aTrajectoryPoint->GetPosition() );
    if ( lineRequired ) {
      trajectoryLine.push_back ( pos );
    }
    if ( markersRequired ) {
      stepPoints.push_back ( pos );
    }
  }

  if ( lineRequired ) {
    G4Colour colour;
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();

    G4String particleName;

    if ( _particleDefinition == G4OpticalPhoton::OpticalPhotonDefinition() ) {
      if ( _wls ) //WLS photons are red
        colour = G4Colour ( 1., 0., 1. );
      else { //Scintillation and Cerenkov photons are green
        colour = G4Colour ( 0., 1., 0. );
      }
    } else if (_particleDefinition == particleTable->FindParticle ( particleName="mu+" ) or _particleDefinition == particleTable->FindParticle ( particleName="mu-" ) ) {//All other particles are blue
      colour = G4Colour ( 0., 0., 1. );
//       G4cout << "\tmuone" << G4endl;
    } else if (_particleDefinition == particleTable->FindParticle ( particleName="e+" ) or _particleDefinition == particleTable->FindParticle ( particleName="e-" ) ) {//All other particles are blue
      colour = G4Colour ( 1., 0., 1. );
//       G4cout << _particleDefinition->GetParticleName() << G4endl;
//       G4cout << "\telettrone" << G4endl;
    } else {
      colour = G4Colour ( 1., 1., 0. );
//       G4cout << _particleDefinition->GetParticleName() << G4endl;
    }
    G4VisAttributes trajectoryLineAttribs ( colour );
    trajectoryLine.SetVisAttributes ( &trajectoryLineAttribs );
    pVVisManager->Draw ( trajectoryLine );
  }
  if ( markersRequired ) {
    auxiliaryPoints.SetMarkerType ( G4Polymarker::squares );
    auxiliaryPoints.SetScreenSize ( markerSize );
    auxiliaryPoints.SetFillStyle ( G4VMarker::filled );
    G4VisAttributes auxiliaryPointsAttribs ( G4Colour ( 0., 1., 1. ) );  // Magenta
    auxiliaryPoints.SetVisAttributes ( &auxiliaryPointsAttribs );
    pVVisManager->Draw ( auxiliaryPoints );

    stepPoints.SetMarkerType ( G4Polymarker::circles );
    stepPoints.SetScreenSize ( markerSize );
    stepPoints.SetFillStyle ( G4VMarker::filled );
    G4VisAttributes stepPointsAttribs ( G4Colour ( 1., 1., 0. ) );  // Yellow.
    stepPoints.SetVisAttributes ( &stepPointsAttribs );
    pVVisManager->Draw ( stepPoints );
  }
}
