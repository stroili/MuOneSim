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
#ifndef MuONeTrajectory_h
#define MuONeTrajectory_h 1

#include "G4Trajectory.hh"
#include "G4Allocator.hh"
#include "G4ios.hh"
#include "globals.hh"
#include "G4ParticleDefinition.hh"
#include "G4TrajectoryPoint.hh"
#include "G4Track.hh"
#include "G4Step.hh"

class G4Polyline;                   // Forward declaration.

class MuONeTrajectory : public G4Trajectory
{
  public:
    MuONeTrajectory();
    MuONeTrajectory ( const G4Track* aTrack );
    MuONeTrajectory ( MuONeTrajectory & );
    virtual ~MuONeTrajectory();

    virtual void DrawTrajectory ( G4int i_mode = 0 ) const;

    inline void* operator new ( size_t );
    inline void  operator delete ( void* );

    void SetDrawTrajectory ( G4bool b ) {
      _drawit = b;
    }
    void WLS() {
      _wls = true;
    }
    void SetForceDrawTrajectory ( G4bool b ) {
      _forceDraw = b;
    }
    void SetForceNoDrawTrajectory ( G4bool b ) {
      _forceNoDraw = b;
    }

  private:
    G4bool                _wls;
    G4bool                _drawit;
    G4bool                _forceNoDraw;
    G4bool                _forceDraw;
    G4ParticleDefinition* _particleDefinition;
};

extern G4Allocator<MuONeTrajectory> MuONeTrajectoryAllocator;

inline void* MuONeTrajectory::operator new ( size_t )
{
  void* aTrajectory;
  aTrajectory = ( void* ) MuONeTrajectoryAllocator.MallocSingle();
  return aTrajectory;
}

inline void MuONeTrajectory::operator delete ( void* aTrajectory )
{
  MuONeTrajectoryAllocator.FreeSingle ( ( MuONeTrajectory* ) aTrajectory );
}

#endif
