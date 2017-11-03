//
// C++ Implementation: MuONePDet
//
// Description:
//
//
// Author: Roberto Stroili <stroili@pd.infn.it>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "MuONePDet.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4ThreeVector.hh"

MuONePDet::MuONePDet() : _xsize(10.*CLHEP::mm), _ysize(10.*CLHEP::mm), _zsize(1.*CLHEP::mm), _wXsize(9.*CLHEP::mm),
                   _wYsize(9.*CLHEP::mm), _wZsize(0.5*CLHEP::mm), _siXsize(9.*CLHEP::mm), _siYsize(9.*CLHEP::mm),
                   _siZsize(0.3*CLHEP::mm)
{
  _envelop = new G4Box ( "sipm_env", _xsize/2., _ysize/2., _zsize/2. );
  _window  = new G4Box ( "sipm_window", _wXsize/2., _wYsize/2., _wZsize/2.  );
  _silicon = new G4Box ( "sipm_silicon", _siXsize/2., _siYsize/2., _siZsize/2.  );

  _sipm_log = new G4LogicalVolume ( _envelop, G4Material::GetMaterial ( "Al" ),
                                   "sipm_log" );
  _window_log = new G4LogicalVolume ( _window, G4Material::GetMaterial ( "Quartz" ),
                                      "window_log" );
  _silicon_log = new G4LogicalVolume ( _silicon, G4Material::GetMaterial ( "Silicon" ),
                                      "silicon_log" );
  G4VisAttributes* si_va = new G4VisAttributes( G4Colour ( 0.3, .0, 0.0 ) );
  si_va->SetForceSolid ( true );
  _silicon_log->SetVisAttributes(si_va);
//   _silicon_log->SetVisAttributes ( G4VisAttributes::Invisible );

  _sipm_phys = new G4PVPlacement ( 0, G4ThreeVector ( 0, 0, -(_zsize - _wZsize)/2. ),
                                        _window_log, "sipm",
                                        _sipm_log, false, 0 );
  G4RotationMatrix* rot = new G4RotationMatrix();
  new G4PVPlacement ( rot, G4ThreeVector ( 0, 0, (-_zsize/2. + _wZsize + _siZsize/2.)),
                      _silicon_log, "sipm_silicon", _sipm_log, false, 0 );

  G4VisAttributes* sipm_va = new G4VisAttributes( G4Colour ( 0.2, .0, 0.4 ) );
  sipm_va->SetForceSolid ( false );
  _sipm_log->SetVisAttributes ( sipm_va );

}

MuONePDet::~MuONePDet()
{
//   delete _window_log;
//   delete _sipm_log;
//   delete _window;
//   delete _envelop;
}
