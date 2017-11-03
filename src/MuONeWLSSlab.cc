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
#include "MuONeWLSSlab.hh"
#include "MuONeWLSFiber.hh"

#include "globals.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4SDManager.hh"
#include "MuONeSD.hh"

G4LogicalVolume* MuONeWLSSlab::_ScintSlab_log = NULL;
MuONeSD* MuONeWLSSlab::_scint_SD;

MuONeWLSSlab::MuONeWLSSlab ( G4RotationMatrix *pRot,
                                   const G4ThreeVector &tlate,
                                   G4LogicalVolume *pMotherLogical,
                                   G4bool pMany,
                                   G4int pCopyNo,
                                   MuONeDetectorConstruction* c )
    : G4PVPlacement ( pRot, tlate, new G4LogicalVolume ( new G4Box ( "temp", 1, 1, 1 ),
                      G4Material::GetMaterial ( "Vacuum" ), "temp", 0, 0, 0 ),
                      "Slab", pMotherLogical, pMany, pCopyNo ), _constructor ( c )
{
  CopyValues();

  if ( !_ScintSlab_log || _updated ) {

    G4Box* ScintSlab_box = new G4Box ( "Slab", _scint_x/2., _scint_y/2., _scint_z/2. );

    _ScintSlab_log
    = new G4LogicalVolume ( ScintSlab_box,
                            G4Material::GetMaterial ( "CsI" ),
                            "Slab", 0, 0, 0 );
    G4VisAttributes* slab_va = new G4VisAttributes ( G4Colour ( 0.0, 0.0, 0.8 ) );
    slab_va->SetForceWireframe ( true );
    _ScintSlab_log->SetVisAttributes ( slab_va );

    G4double spacing = _scint_y / _nfibers;

    G4RotationMatrix* rm = new G4RotationMatrix();
    rm->rotateY ( 90*CLHEP::deg );

    //Place fibers
//     for ( G4int i = 0;i < _nfibers;i++ ) {
//       G4double Y = - ( spacing ) * ( _nfibers - 1 ) * 0.5 + i * spacing;
//       new MuONeWLSFiber ( rm, G4ThreeVector ( 0., Y, 0. ), _ScintSlab_log, false, 0,
//                              _constructor );
//     }

  }

  G4SDManager* SDman = G4SDManager::GetSDMpointer();

  if ( !_scint_SD ) { //determine if it has already been created
    _scint_SD = new MuONeSD ( "/MuONeDet/scintSD" );
    SDman->AddNewDetector ( _scint_SD );
  }
  _ScintSlab_log->SetSensitiveDetector ( _scint_SD );

  SetLogicalVolume ( _ScintSlab_log );
}

void MuONeWLSSlab::CopyValues()
{
  _updated = _constructor->GetUpdated();

  _scint_x = _constructor->GetScintX();
  _scint_y = _constructor->GetScintY();
  _scint_z = _constructor->GetScintZ();
//   _nfibers = _constructor->GetNFibers();
  _slab_z  = _constructor->GetSlabZ();
}
