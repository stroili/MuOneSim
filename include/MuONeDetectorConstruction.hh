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
#ifndef MuONeDetectorConstruction_H
#define MuONeDetectorConstruction_H 1

#include <fstream>
#include <map>

class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Box;
class G4Tubs;
class MuONePMTSD;
class MuONeSD;
class MuONePDet;

#include "G4Material.hh"
#include "MuONeDetectorMessenger.hh"
#include "G4VisAttributes.hh"
#include "G4RotationMatrix.hh"
#include "G4VUserDetectorConstruction.hh"

class MuONeDetectorConstruction : public G4VUserDetectorConstruction
{
  public:

    MuONeDetectorConstruction();
    ~MuONeDetectorConstruction();

    G4VPhysicalVolume* Construct();

    //Functions to modify the geometry
    void SetDimensions ( G4ThreeVector dims );
    void SetHousingThickness ( G4double d_mtl );
    void SetNX ( G4int nx );
    void SetNY ( G4int ny );
    void SetNZ ( G4int nz );
    void SetPMTRadius ( G4double outerRadius_pmt );
    void SetFiberRadius ( G4double fiberRadius );
    void SetDefaults();

    //Get values
    G4double GetScintX() {
      return _scint_x;
    }
    G4double GetScintY() {
      return _scint_y;
    }
    G4double GetScintZ() {
      return _scint_z;
    }
    G4double GetHousingThickness() {
      return _d_mtl;
    }
    G4int GetNX() {
      return _nx;
    }
    G4int GetNY() {
      return _ny;
    }
    G4int GetNZ() {
      return _nz;
    }
    G4double GetPMTRadius() {
      return _outerRadius_pmt;
    }

    G4double GetFiberRadius() {
      return _fiberRadius;
    }

    //rebuild the geometry based on changes. must be called
    void UpdateGeometry();

    G4bool GetUpdated() {
      return _updated;
    }

    void SetHousingReflectivity ( G4double r ) {
      _refl = r;
      _updated = true;
    }

    G4double GetHousingReflectivity() {
      return _refl;
    }

    void SetWLSSlabOn ( G4bool b ) {
      _WLSslab = b;
      _updated = true;
    }

    G4bool GetWLSSlabOn() {
      return _WLSslab;
    }

    void SetMainVolumeOn ( G4bool b ) {
      _mainVolume = b;
      _updated = true;
    }

    G4bool GetMainVolumeOn() {
      return _mainVolume;
    }

    void SetNFibers ( G4int n ) {
      _nfibers = n;
      _updated = true;
    }

    G4int GetNFibers() {
      return _nfibers;
    }

    void SetSlabZ ( G4double z ) {
      _slab_z = z * CLHEP::cm;
      _updated = true;
    }

    G4double GetSlabZ() {
      return _slab_z;
    }

    void SetPMTeff ( G4String s ) {
      _PMTeffFile = s;
      _updated = true;
    }

    void SetWLSabsl ( G4String s ) {
      _WLSabslFile = s;
      _updated = true;
    }

    void SetWLSemis ( G4String s ) {
      _WLSemisFile = s;
      _updated = true;
    }

    void SetWLSrind ( G4String s ) {
      _WLSrindexFile = s;
      _updated = true;
    }

    void SetTargetWidth( G4double targetWidth ) {
      _target_z = targetWidth;
      _updated = true;
    }

    void SetMainScintYield ( G4double y );
    void SetWLSScintYield ( G4double y );

  private:

    void PlacePMTs ( G4LogicalVolume* pmt_Log,
                     G4RotationMatrix* rot, G4double &a, G4double &b, G4double da,
                     G4double db, G4double amin, G4double bmin, G4int na, G4int nb,
                     G4double &x, G4double &y, G4double &z, G4int &k,
                     MuONePMTSD* sd );

    void DefineMaterials();
    void SurfaceProperties();
    void getEfficiency(std::ifstream& f, std::map<double, double>& eff);
    void getWLSprop(G4String file, std::map<double, double>& prop);
    void setWLSProperties();

    G4VPhysicalVolume* ConstructDetector();

    MuONeDetectorMessenger* _detectorMessenger;

    G4bool                     _updated;

    G4Box*                     _experimentalHall_box;
    G4LogicalVolume*           _experimentalHall_log;
    G4VPhysicalVolume*         _experimentalHall_phys;

    static G4LogicalVolume*    _housing_log;
    G4Box*                     _housing_box;
    G4VPhysicalVolume*         _housing_phys;

    //  target
    G4Box*                     _target_box;
    G4LogicalVolume*           _target_log;
    G4VPhysicalVolume*         _target_phys;

    //  silicon detector
    G4Box*                     _sidet_box;
    G4LogicalVolume*           _sidet_log;
    G4VPhysicalVolume*         _sidet1_phys;
    G4VPhysicalVolume*         _sidet2_phys;
    G4VPhysicalVolume*         _sidet3_phys;

    //Basic Volumes
    //
    G4Tubs*                 _pmt;
    G4Tubs*                 _photocath;

    // Logical volumes
    //
    G4LogicalVolume*        _pmt_log;
    G4LogicalVolume*        _photocath_log;

    // Physical volumes
    //
    G4VPhysicalVolume*      _scint_phys;
    //keeping pointers to these is pointless really since there are many of them
    //but I'm doing it to be consistent
    G4VPhysicalVolume*      _pmt_phys;
    G4VPhysicalVolume*      _photocath_phys;

    //Sensitive Detectors
    static MuONeSD*      _silicon_SD;
    static MuONePMTSD*   _pmt_SD;
    MuONePDet*           _pdet;

    //Materials & Elements
    G4Material*                _Al;
    G4Element*                 _N;
    G4Element*                 _O;
    G4Material*                _Air;
    G4Material*                _Vacuum;
    G4Element*                 _C;
    G4Element*                 _H;
    G4Element*                 _Cs;
    G4Element*                 _I;
    G4Element*                 _Be;
    G4Material*                _Quartz;
    G4Material*                _silicon;
    G4Material*                _CsI;
    G4Material*                _Glass;
    G4Material*                _Pstyrene;
    G4Material*                _PMMA;
    G4Material*                _Pethylene;
    G4Material*                _fPethylene;
    G4Material*                _beryllium;



    //Geometry
    G4double                   _scint_x;
    G4double                   _scint_y;
    G4double                   _scint_z;
    G4double                   _target_x;
    G4double                   _target_y;
    G4double                   _target_z;
    G4double                   _target_pos;

    //  silicon detector
    G4double                   _sidet_x;
    G4double                   _sidet_y;
    G4double                   _sidet_z;
    G4double                   _sidet_pos;
    G4double                   _sidet_dist;

    G4double                   _d_mtl;
    G4int                      _nx;
    G4int                      _ny;
    G4int                      _nz;
    G4double                   _outerRadius_pmt;
    G4int                      _nfibers;
    G4double                   _refl;
    G4bool                     _WLSslab;
    G4bool                     _mainVolume;
    G4double                   _slab_z;
    G4double                   _fiberRadius;

    G4MaterialPropertiesTable* _MPTPStyrene;

    G4String                   _PMTeffFile;
    G4String                   _WLSabslFile;
    G4String                   _WLSemisFile;
    G4String                   _WLSrindexFile;
};

#endif




