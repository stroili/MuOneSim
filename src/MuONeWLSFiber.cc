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
#include "MuONeWLSFiber.hh"
#include "globals.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalBorderSurface.hh"

G4LogicalVolume* MuONeWLSFiber::_clad2_log = NULL;

MuONeWLSFiber::MuONeWLSFiber ( G4RotationMatrix *pRot,
                                     const G4ThreeVector &tlate,
                                     G4LogicalVolume *pMotherLogical,
                                     G4bool pMany,
                                     G4int pCopyNo,
                                     MuONeDetectorConstruction* c )
    : G4PVPlacement ( pRot, tlate, new G4LogicalVolume ( new G4Box ( "temp", 1, 1, 1 ),
                      G4Material::GetMaterial ( "Vacuum" ),
                      "temp", 0, 0, 0 ),
                      "Cladding2", pMotherLogical, pMany, pCopyNo ),
    _constructor ( c )
{
  CopyValues();

  if ( !_clad2_log || _updated ) {
    // The Fiber
    //
    G4Tubs* Fiber_tube =
      new G4Tubs ( "Fiber", _fiber_rmin, _fiber_rmax, _fiber_z, _fiber_sphi, _fiber_ephi );

    G4LogicalVolume* Fiber_log =
      new G4LogicalVolume ( Fiber_tube, G4Material::GetMaterial ( "PMMA" ),
                            "Fiber", 0, 0, 0 );

    // Cladding (first layer)
    //
    G4Tubs* clad1_tube =
      new G4Tubs ( "Cladding1", _clad1_rmin, _clad1_rmax, _clad1_z, _clad1_sphi,
                   _clad1_ephi );

    G4LogicalVolume* clad1_log =
      new G4LogicalVolume ( clad1_tube, G4Material::GetMaterial ( "Pethylene" ),
                            "Cladding1", 0, 0, 0 );

    // Cladding (second layer)
    //
    G4Tubs* clad2_tube =
      new G4Tubs ( "Cladding2", _clad2_rmin, _clad2_rmax, _clad2_z, _clad2_sphi,
                   _clad2_ephi );

    _clad2_log =
      new G4LogicalVolume ( clad2_tube, G4Material::GetMaterial ( "fPethylene" ),
                            "Cladding2", 0, 0, 0 );
    G4VisAttributes* clad2_va = new G4VisAttributes ( G4Colour ( 0.3, 0.5, 0.8 ) );
    clad2_va->SetForceSolid ( true );
    _clad2_log->SetVisAttributes ( clad2_va );

    new G4PVPlacement ( 0, G4ThreeVector ( 0., 0., 0. ), Fiber_log,
                        "Fiber", clad1_log, false, 0 );
    new G4PVPlacement ( 0, G4ThreeVector ( 0., 0., 0. ), clad1_log,
                        "Cladding1", _clad2_log, false, 0 );
  }

  SetLogicalVolume ( _clad2_log );
}

void MuONeWLSFiber::CopyValues()
{
  _updated = _constructor->GetUpdated();

  _fiber_rmin = 0.00 * CLHEP::cm;
  _fiber_rmax = _constructor->GetFiberRadius();
  _fiber_z    = _constructor->GetScintX() / 2;
  _fiber_sphi = 0.00 * CLHEP::deg;
  _fiber_ephi = 360. * CLHEP::deg;

  _clad1_rmin = 0.;// fiber_rmax;
  _clad1_rmax = _fiber_rmax + 0.015 * _fiber_rmax;

  _clad1_z    = _fiber_z;
  _clad1_sphi = _fiber_sphi;
  _clad1_ephi = _fiber_ephi;

  _clad2_rmin = 0.;//clad1_rmax;
  _clad2_rmax = _clad1_rmax + 0.015 * _fiber_rmax;

  _clad2_z    = _fiber_z;
  _clad2_sphi = _fiber_sphi;
  _clad2_ephi = _fiber_ephi;
}
