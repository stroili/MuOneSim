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

#ifndef MuONePMTHit_h
#define MuONePMTHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"
#include "G4VPhysicalVolume.hh"
#include "G4DataVector.hh"

class G4VTouchable;

class MuONePMTHit : public G4VHit
{
  public:

    MuONePMTHit();
    ~MuONePMTHit();
    MuONePMTHit ( const MuONePMTHit &right );

    const MuONePMTHit& operator= ( const MuONePMTHit &right );
    G4int operator== ( const MuONePMTHit &right ) const;

    inline void *operator new ( size_t );
    inline void operator delete ( void *aHit );

    void Draw();
    void Print();

    inline void SetDrawit ( G4bool b ) {
      _drawit = b;
    }
    inline G4bool GetDrawit() {
      return _drawit;
    }

    inline void IncPhotonCount() {
      _photons++;
    }

    inline void AddTime ( G4double hitTime ) {
      _times->push_back ( hitTime );
    }

    inline void AddWLength ( G4double hitWlen ) {
      _wlengths->push_back ( hitWlen );
    }

    inline G4int GetPhotonCount() {
      return _photons;
    }

    inline void SetPMTNumber ( G4int n ) {
      _pmtNumber = n;
    }
    inline G4int GetPMTNumber() {
      return _pmtNumber;
    }

    inline void SetPMTPhysVol ( G4VPhysicalVolume* physVol ) {
      this->_physVol = physVol;
    }
    inline G4VPhysicalVolume* GetPMTPhysVol() {
      return _physVol;
    }

    inline void SetPMTPos ( G4double x, G4double y, G4double z ) {
      _pos = G4ThreeVector ( x, y, z );
    }

    inline G4ThreeVector GetPMTPos() {
      return _pos;
    }

    G4double GetMinTime();
    G4double GetMaxTime();
    G4double GetAvergTime();
    G4double GetSecondTime();
    G4double GetThirdTime();
    G4double GetSTDTime();
//     void SortTimes();
    G4double GetSortedTime ( unsigned int i );

    G4double GetTime ( unsigned int i ) {
      return ( *_times ) [i];
    }
    G4double GetWLength ( unsigned int i ) {
      return ( *_wlengths ) [i];
    }


  private:
    G4int              _pmtNumber;
    G4int              _photons;
    G4ThreeVector      _pos;
    G4VPhysicalVolume* _physVol;
    G4bool             _drawit;
    G4DataVector*      _times;
    G4DataVector*      _wlengths;
};

typedef G4THitsCollection<MuONePMTHit> MuONePMTHitsCollection;

extern G4Allocator<MuONePMTHit> MuONePMTHitAllocator;

inline void* MuONePMTHit::operator new ( size_t )
{
  void *aHit;
  aHit = ( void * ) MuONePMTHitAllocator.MallocSingle();
  return aHit;
}

inline void MuONePMTHit::operator delete ( void *aHit )
{
  MuONePMTHitAllocator.FreeSingle ( ( MuONePMTHit* ) aHit );
}

#endif


