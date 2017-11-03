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

#ifndef MuONeHit_h
#define MuONeHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"
#include "G4VPhysicalVolume.hh"

class MuONeHit : public G4VHit
{
  public:

    MuONeHit();
    MuONeHit ( G4VPhysicalVolume* pVol );
    ~MuONeHit();
    MuONeHit ( const MuONeHit &right );
    const MuONeHit& operator= ( const MuONeHit &right );
    G4int operator== ( const MuONeHit &right ) const;

    inline void *operator new ( size_t );
    inline void operator delete ( void *aHit );

    void Draw();
    void Print();

    inline void SetEdep ( G4double de ) {
      _edep = de;
    }
    inline void AddEdep ( G4double de ) {
      _edep += de;
    }
    inline G4double GetEdep() {
      return _edep;
    }

    inline void SetPos ( G4ThreeVector xyz ) {
      _pos = xyz;
    }
    inline G4ThreeVector GetPos() {
      return _pos;
    }

    inline const G4VPhysicalVolume * GetPhysV() {
      return _physVol;
    }

  private:
    G4double                 _edep;
    G4ThreeVector            _pos;
    const G4VPhysicalVolume* _physVol;

};

typedef G4THitsCollection<MuONeHit> MuONeHitsCollection;

extern G4Allocator<MuONeHit> MuONeHitAllocator;

inline void* MuONeHit::operator new ( size_t )
{
  void *aHit;
  aHit = ( void * ) MuONeHitAllocator.MallocSingle();
  return aHit;
}

inline void MuONeHit::operator delete ( void *aHit )
{
  MuONeHitAllocator.FreeSingle ( ( MuONeHit* ) aHit );
}

#endif


