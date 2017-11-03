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
#include <algorithm>
#include <numeric>

#include "MuONePMTHit.hh"
#include "G4ios.hh"
#include "G4VVisManager.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"

G4Allocator<MuONePMTHit> MuONePMTHitAllocator;

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
MuONePMTHit::MuONePMTHit()
    : _pmtNumber ( -1 ), _photons ( 0 ), _physVol ( 0 ), _drawit ( false )
{
  _times = new G4DataVector();
  _wlengths = new G4DataVector();
}

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
MuONePMTHit::~MuONePMTHit()
{
  delete _times;
  delete _wlengths;
}

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
MuONePMTHit::MuONePMTHit ( const MuONePMTHit &right )
    : G4VHit()
{
  _pmtNumber = right._pmtNumber;
  _photons   = right._photons;
  _physVol   = right._physVol;
  _drawit    = right._drawit;
  _times     = right._times;
  _wlengths  = right._wlengths;
}

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
const MuONePMTHit& MuONePMTHit::operator= ( const MuONePMTHit & right )
{
  _pmtNumber = right._pmtNumber;
  _photons   = right._photons;
  _physVol   = right._physVol;
  _drawit    = right._drawit;
  _times     = right._times;
  _wlengths  = right._wlengths;
  return *this;
}

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
G4int MuONePMTHit::operator== ( const MuONePMTHit &right ) const
{
  return ( _pmtNumber == right._pmtNumber );
}

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
void MuONePMTHit::Draw()
{
  if ( _drawit && _physVol ) { //ReDraw only the PMTs that have hit counts > 0
    //Also need a physical volume to be able to draw anything
    G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
    if ( pVVisManager ) { //Make sure that the VisManager exists
      G4VisAttributes attribs ( G4Colour ( 1., 1., 0. ) );
      attribs.SetForceSolid ( true );
      G4RotationMatrix rot;
      if ( _physVol->GetRotation() ) //If a rotation is defined use it
        rot = * ( _physVol->GetRotation() );
      G4Transform3D trans ( rot, _physVol->GetTranslation() );//Create transform
      pVVisManager->Draw ( *_physVol, attribs, trans );//Draw it
    }
  }
}

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
void MuONePMTHit::Print()
{
}

G4double MuONePMTHit::GetMinTime()
{
  return * ( std::min_element ( _times->begin(), _times->end() ) );
}

G4double MuONePMTHit::GetMaxTime()
{
  return * ( std::max_element ( _times->begin(), _times->end() ) );
}

G4double MuONePMTHit::GetAvergTime()
{
  return ( std::accumulate ( _times->begin(), _times->end(), G4double ( 0. ) ) / _times->size() );
}

G4double MuONePMTHit::GetSecondTime()
{
  G4DataVector tmpvec = *_times;
  std::sort ( tmpvec.begin(), tmpvec.end() );
  if ( tmpvec.size() > 1 ) {
    return tmpvec[1];
  } else {
    return -100.;
  }
}

// void MuONePMTHit::SortTimes()
// {
//   std::sort ( _times->begin(), _times->end() );
// }


G4double MuONePMTHit::GetThirdTime()
{
  /*  std::sort ( _times->begin(), _times->end() );
    return ( *_times ) [2];*/
  G4DataVector tmpvec = *_times;
  std::sort ( tmpvec.begin(), tmpvec.end() );
  if ( tmpvec.size() > 2 ) {
    return tmpvec[2];
  } else {
    return -100.;
  }
}

G4double MuONePMTHit::GetSortedTime ( unsigned int i )
{
  G4DataVector tmpvec = *_times;
  std::sort ( tmpvec.begin(), tmpvec.end() );
  if ( i < _times->size() ) {
    return tmpvec[i];
  } else {
    return -1.;
  }
}

G4double MuONePMTHit::GetSTDTime()
{
  return 0.;
}
