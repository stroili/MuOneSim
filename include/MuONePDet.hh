//
// C++ Interface: MuONePDet
//
// Description:
//
//
// Author: Roberto Stroili <stroili@pd.infn.it>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef MuONePDet_HH
#define MuONePDet_HH

#include "G4Types.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"

class MuONePDet {
  private:
    //  device dimensions
    G4double _xsize;
    G4double _ysize;
    G4double _zsize;

    //  window dimensions
    G4double _wXsize;
    G4double _wYsize;
    G4double _wZsize;

    //  silicon dimensions
    G4double _siXsize;
    G4double _siYsize;
    G4double _siZsize;

    G4Box*           _envelop;
    G4Box*           _window;
    G4Box*           _silicon;
    G4LogicalVolume* _sipm_log;
    G4LogicalVolume* _window_log;
    G4LogicalVolume* _silicon_log;
    G4PVPlacement*   _sipm_phys;

  public:
    MuONePDet();
    ~MuONePDet();

    G4LogicalVolume* log() { return _sipm_log; }
    G4LogicalVolume* si_log() { return _silicon_log; }
    G4double xsize() { return _xsize; }
    G4double ysize() { return _ysize; }
    G4double zsize() { return _zsize; }
};

#endif
