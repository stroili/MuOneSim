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
#include "MuONeDetectorConstruction.hh"
#include "MuONePMTSD.hh"
#include "MuONeSD.hh"
#include "MuONeDetectorMessenger.hh"
#include "MuONeWLSSlab.hh"
#include "MuONePDet.hh"

#include "G4SDManager.hh"
#include "G4RunManager.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4OpticalSurface.hh"
#include "G4MaterialTable.hh"
#include "G4VisAttributes.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "globals.hh"
#include "G4SolidStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4GeometryManager.hh"
#include "G4UImanager.hh"
#include "G4NistManager.hh"

MuONeSD* MuONeDetectorConstruction::_silicon_SD;
MuONePMTSD* MuONeDetectorConstruction::_pmt_SD;
G4LogicalVolume* MuONeDetectorConstruction::_housing_log = NULL;

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
MuONeDetectorConstruction::MuONeDetectorConstruction()
  : _MPTPStyrene ( NULL ), _PMTeffFile(), _WLSabslFile("WLSabsorption.dat"),
                   _WLSemisFile("WLSemission.dat"), _WLSrindexFile("WLSrindex.dat")
{
  SetDefaults();
  _detectorMessenger = new MuONeDetectorMessenger ( this );
}

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
MuONeDetectorConstruction::~MuONeDetectorConstruction()
{
}

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
void MuONeDetectorConstruction::DefineMaterials()
{
  G4NistManager* man = G4NistManager::Instance();

  G4double a;  // atomic mass
  G4double z;  // atomic number
  G4double density;

  G4int polyPMMA = 1;
  G4int nC_PMMA = 3 + 2 * polyPMMA;
  G4int nH_PMMA = 6 + 2 * polyPMMA;

  G4int polyeth = 1;
  G4int nC_eth = 2 * polyeth;
  G4int nH_eth = 4 * polyeth;

  //***Elements
  _H  = new G4Element ( "H", "H", z = 1., a = 1.01 * CLHEP::g / CLHEP::mole );
  _C  = new G4Element ( "C", "C", z = 6., a = 12.01 * CLHEP::g / CLHEP::mole );
  _N  = new G4Element ( "N", "N", z = 7., a = 14.01 * CLHEP::g / CLHEP::mole );
  _O  = new G4Element ( "O", "O", z = 8., a = 16.00 * CLHEP::g / CLHEP::mole );
  _Cs = new G4Element ( "Cs", "Cs", z = 55., a = 132.905 * CLHEP::g / CLHEP::mole );
  _I  = new G4Element ( "I", "I", z = 53., a = 126.904 * CLHEP::g / CLHEP::mole );
  _Be  = new G4Element ( "Be", "Be", z = 4., a = 9.0121831 * CLHEP::g / CLHEP::mole );

  G4Element* _Si = new G4Element("Silicon", "Si", z = 14., a = 28.09*CLHEP::g/CLHEP::mole);

  _beryllium = new G4Material ( "Beryllium", z = 4., a = 9.0121831 * CLHEP::g / CLHEP::mole, density = 1.85 * CLHEP::g / CLHEP::cm3 );

  G4Material* csi = man->FindOrBuildMaterial("G4_CESIUM_IODIDE");
  G4cout << csi << G4endl;
  //***Materials
  // CsI
  _CsI = new G4Material ( "CsI", density = 4.53 * CLHEP::g / CLHEP::cm3, 2 );
  _CsI->AddElement(_Cs, 5 );
  _CsI->AddElement(_I,  5 );
  G4cout << _CsI << G4endl;

  //Aluminum
  _Al = new G4Material ( "Al", z = 13., a = 26.98 * CLHEP::g / CLHEP::mole, density = 2.7 * CLHEP::g / CLHEP::cm3 );
  //Vacuum
  _Vacuum = new G4Material ( "Vacuum", z = 1., a = 1.01 * CLHEP::g / CLHEP::mole,
                             density = CLHEP::universe_mean_density, kStateGas, 0.1 * CLHEP::kelvin,
                             1.e-19 * CLHEP::pascal );
  //Air
  _Air = new G4Material ( "Air", density = 1.29 * CLHEP::mg / CLHEP::cm3, 2 );
  _Air->AddElement ( _N, 70*CLHEP::perCent );
  _Air->AddElement ( _O, 30*CLHEP::perCent );
  //Glass
  _Glass = new G4Material ( "Glass", density = 1.032 * CLHEP::g / CLHEP::cm3, 2 );
  _Glass->AddElement ( _C, 91.533*CLHEP::perCent );
  _Glass->AddElement ( _H, 8.467*CLHEP::perCent );
// Quartz
// -------
//  density = 2.200*g/CLHEP::cm3; // fused quartz
  density = 2.64*CLHEP::g/CLHEP::cm3;  // crystalline quartz (c.f. PDG)
  _Quartz = new G4Material("Quartz",density, 2);
  _Quartz->AddElement(_Si, 1) ;
  _Quartz->AddElement(_O , 2) ;
  //Silicon
  _silicon = new G4Material ( "Silicon", z = 14., a = 29.09 * CLHEP::g / CLHEP::mole, density = 2.33 * CLHEP::g / CLHEP::cm3 );

  //Polystyrene
  _Pstyrene = new G4Material ( "Polystyrene", density = 1.032 * CLHEP::g / CLHEP::cm3, 2 );
  _Pstyrene->AddElement ( _C, 8 );
  _Pstyrene->AddElement ( _H, 8 );
  //Fiber(PMMA)
  _PMMA = new G4Material ( "PMMA", density = 1190 * CLHEP::kg / CLHEP::m3, 3 );
  _PMMA->AddElement ( _H, nH_PMMA );
  _PMMA->AddElement ( _C, nC_PMMA );
  _PMMA->AddElement ( _O, 2 );
  //Cladding(polyethylene)
  _Pethylene = new G4Material ( "Pethylene", density = 1200 * CLHEP::kg / CLHEP::m3, 2 );
  _Pethylene->AddElement ( _H, nH_eth );
  _Pethylene->AddElement ( _C, nC_eth );
  //Double cladding(flourinated polyethylene)
  _fPethylene = new G4Material ( "fPethylene", density = 1400 * CLHEP::kg / CLHEP::m3, 2 );
  _fPethylene->AddElement ( _H, nH_eth );
  _fPethylene->AddElement ( _C, nC_eth );


      // Refractive index

  const G4int N_RINDEX_QUARTZ = 2 ;
  G4double lambda_min = 200*CLHEP::nm;
  G4double lambda_max = 700*CLHEP::nm;
  G4double X_RINDEX_QUARTZ[N_RINDEX_QUARTZ] = {1.*CLHEP::eV, 7.14*CLHEP::eV} ;
  G4double RINDEX_QUARTZ[N_RINDEX_QUARTZ] = {1.54, 1.54};

  G4MaterialPropertiesTable *MPT_PMT = new G4MaterialPropertiesTable();
  MPT_PMT->AddProperty("RINDEX", X_RINDEX_QUARTZ, RINDEX_QUARTZ, N_RINDEX_QUARTZ);

  _Quartz->SetMaterialPropertiesTable(MPT_PMT);

  //***Material properties tables

  const G4int MuONe_NUMENTRIES = 3;
  G4double MuONe_Energy[MuONe_NUMENTRIES]    = { 1.0*CLHEP::eV , 7.07*CLHEP::eV, 7.14*CLHEP::eV };

  G4double Glass_RIND[MuONe_NUMENTRIES] = {1.49, 1.49, 1.49};
  G4double Glass_AbsLength[MuONe_NUMENTRIES] = {420.*CLHEP::cm, 420.*CLHEP::cm, 420.*CLHEP::cm};
  G4MaterialPropertiesTable *Glass_mt = new G4MaterialPropertiesTable();
  Glass_mt->AddProperty ( "ABSLENGTH", MuONe_Energy, Glass_AbsLength,
                          MuONe_NUMENTRIES );
  Glass_mt->AddProperty ( "RINDEX", MuONe_Energy, Glass_RIND, MuONe_NUMENTRIES );
  _Glass->SetMaterialPropertiesTable ( Glass_mt );

  G4double Vacuum_Energy[MuONe_NUMENTRIES] = {1.0*CLHEP::eV, 7.0*CLHEP::eV, 7.14*CLHEP::eV};
  G4double Vacuum_RIND[MuONe_NUMENTRIES] = {1., 1., 1.};
  G4MaterialPropertiesTable *Vacuum_mt = new G4MaterialPropertiesTable();
  Vacuum_mt->AddProperty ( "RINDEX", Vacuum_Energy, Vacuum_RIND, MuONe_NUMENTRIES );
  _Vacuum->SetMaterialPropertiesTable ( Vacuum_mt );
  _Air->SetMaterialPropertiesTable ( Vacuum_mt );//Give air the same rindex

  const G4int WLS_NUMENTRIES = 4;
  G4double WLS_Energy[] = {1.00*CLHEP::eV, 2.87*CLHEP::eV, 2.90*CLHEP::eV, 7.14*CLHEP::eV};

  G4double RIndexPstyrene[WLS_NUMENTRIES] = { 1.5, 1.5, 1.5, 1.5};
  G4double Absorption1[WLS_NUMENTRIES] = {2.*CLHEP::cm, 2.*CLHEP::cm, 2.*CLHEP::cm, 2.*CLHEP::cm};
  G4double ScintilFast[WLS_NUMENTRIES] = {0.00, 0.00, 1.00, 1.00};
//   G4double ScintilSlow[WLS_NUMENTRIES] = {0.1, 0.1, 0.20, 0.20};
  _MPTPStyrene = new G4MaterialPropertiesTable();
  _MPTPStyrene->AddProperty ( "RINDEX", WLS_Energy, RIndexPstyrene, WLS_NUMENTRIES );
  _MPTPStyrene->AddProperty ( "ABSLENGTH", WLS_Energy, Absorption1, WLS_NUMENTRIES );
  _MPTPStyrene->AddProperty ( "FASTCOMPONENT", WLS_Energy, ScintilFast,
                              WLS_NUMENTRIES );
//   _MPTPStyrene->AddProperty ( "SLOWCOMPONENT", WLS_Energy, ScintilSlow,
//                               WLS_NUMENTRIES );
  _MPTPStyrene->AddConstProperty ( "SCINTILLATIONYIELD", 10. / CLHEP::keV );
  _MPTPStyrene->AddConstProperty ( "RESOLUTIONSCALE", 1.0 );
  _MPTPStyrene->AddConstProperty ( "FASTTIMECONSTANT", 10.*CLHEP::ns );
//   _MPTPStyrene->AddConstProperty ( "SLOWTIMECONSTANT", 40.*CLHEP::ns );
  _Pstyrene->SetMaterialPropertiesTable ( _MPTPStyrene );
//   _MPTPStyrene->DumpTable();

  // CsI optical characteristics
  const G4int CsI_NUMENTRIES = 12;
  G4double CsI_Energy[CsI_NUMENTRIES] = {1.0*CLHEP::eV, 1.03*CLHEP::eV, 1.13*CLHEP::eV, 1.24*CLHEP::eV, 1.38*CLHEP::eV,
    1.55*CLHEP::eV, 1.77*CLHEP::eV, 2.07*CLHEP::eV, 2.48*CLHEP::eV, 3.1*CLHEP::eV, 4.13*CLHEP::eV, 7.14*CLHEP::eV };
  G4double CsI_RIND[CsI_NUMENTRIES] = {1.7526, 1.75268, 1.75463, 1.75721, 1.76074,
      1.76575, 1.77323, 1.78520, 1.80635, 1.85027, 1.97872, 3.1};
  const G4int CsI_SCIENTRIES = 21;
  G4double CsI_SciEn[CsI_SCIENTRIES] = {1.0*CLHEP::eV, 1.77*CLHEP::eV, 1.82*CLHEP::eV, 1.88*CLHEP::eV, 1.94*CLHEP::eV,
        2.*CLHEP::eV, 2.07*CLHEP::eV, 2.14*CLHEP::eV, 2.21*CLHEP::eV, 2.3*CLHEP::eV, 2.38*CLHEP::eV, 2.48*CLHEP::eV, 2.58*CLHEP::eV, 2.7*CLHEP::eV,
        2.82*CLHEP::eV, 2.95*CLHEP::eV, 3.1*CLHEP::eV, 3.26*CLHEP::eV, 3.44*CLHEP::eV, 3.65*CLHEP::eV, 7.14*CLHEP::eV };
  G4double CsI_SCINT[CsI_SCIENTRIES] = {0., 0.013, 0.019, 0.029, .040, 0.055, 0.073, 0.092,
        0.104, 0.114, 0.111, 0.104, 0.085, 0.064, 0.042, 0.031, 0.016, 0.005, 0.002, 0., 0.};
  const G4int CsI_ABSLENTRIES = 4;
  G4double CsI_ABSLEn[CsI_ABSLENTRIES] = {1.00*CLHEP::eV, 2.87*CLHEP::eV, 2.90*CLHEP::eV, 7.14*CLHEP::eV};
  G4double CsI_Absorption[CsI_ABSLENTRIES] = {2.*CLHEP::cm, 2.*CLHEP::cm, 2.*CLHEP::cm, 2.*CLHEP::cm};
  G4double CsI_Fast[CsI_ABSLENTRIES] = {0.00, 0.00, 1.00, 1.00};

  G4MaterialPropertiesTable *csi_mt = new G4MaterialPropertiesTable();
  csi_mt->AddProperty ( "RINDEX", CsI_Energy, CsI_RIND, CsI_NUMENTRIES );
  csi_mt->AddProperty ( "SCINTILLATION", CsI_SciEn, CsI_SCINT, CsI_SCIENTRIES );
  csi_mt->AddConstProperty ( "RESOLUTIONSCALE", 1.0 );
  csi_mt->AddConstProperty ( "FASTTIMECONSTANT", 10.*CLHEP::ns );
  csi_mt->AddConstProperty ( "SCINTILLATIONYIELD", 54. / CLHEP::keV );
  csi_mt->AddProperty ( "ABSLENGTH", CsI_ABSLEn, CsI_Absorption, CsI_ABSLENTRIES );
  csi_mt->AddProperty ( "FASTCOMPONENT", CsI_SciEn, CsI_SCINT, CsI_SCIENTRIES );
  _CsI->SetMaterialPropertiesTable ( csi_mt );
  csi_mt->DumpTable();

  G4cout << _CsI << G4endl;
//   setWLSProperties();

}

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
G4VPhysicalVolume* MuONeDetectorConstruction::Construct()
{
  DefineMaterials();
  return ConstructDetector();
}

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
G4VPhysicalVolume* MuONeDetectorConstruction::ConstructDetector()
{
  //The experimental hall walls are all 1m away from housing walls
  G4double expHall_x =  1.*CLHEP::m;
  G4double expHall_y =  1.*CLHEP::m;
  G4double expHall_z =  10.*CLHEP::m;

  //Create experimental hall
  _experimentalHall_box
  = new G4Box ( "expHall_box", expHall_x, expHall_y, expHall_z );
  _experimentalHall_log = new G4LogicalVolume ( _experimentalHall_box,
      _Vacuum, "expHall_log", 0, 0, 0 );
  _experimentalHall_phys = new G4PVPlacement ( 0, G4ThreeVector(),
      _experimentalHall_log, "expHall", 0,
      false, 0 );

  _experimentalHall_log->SetVisAttributes ( G4VisAttributes::Invisible );

  G4double housing_x = _scint_x + _d_mtl;
  G4double housing_y = _scint_y + _d_mtl;
  G4double housing_z = _scint_z + _d_mtl;

  //*************************** target
  _target_box = new G4Box ( "target_box", _target_x / 2., _target_y / 2.,
                            _target_z / 2. );

  _target_log = new G4LogicalVolume ( _target_box,
                                       G4Material::GetMaterial ( "Beryllium" ),
                                       "target_log", 0, 0, 0 );
  G4ThreeVector tpos(0., 0., 0.);
  _target_phys = new G4PVPlacement ( 0, tpos, _target_log,
                                      "target", _experimentalHall_log,
                                      false, 0 );

  //*************************** silicon detector
  _sidet_box = new G4Box ( "sidet_box", _sidet_x / 2., _sidet_y / 2.,
                           _sidet_z / 2. );

  _sidet_log = new G4LogicalVolume ( _sidet_box,
                                      G4Material::GetMaterial ( "Silicon" ),
                                      "sidet_log", 0, 0, 0 );
  G4ThreeVector dpos(0., 0., _sidet_pos);
  _sidet1_phys = new G4PVPlacement ( 0, dpos, _sidet_log,
                                     "sidet1", _experimentalHall_log,
                                     false, 0 );
  dpos.setZ(_sidet_pos+_sidet_dist);
  _sidet2_phys = new G4PVPlacement ( 0, dpos, _sidet_log,
                                     "sidet2", _experimentalHall_log,
                                     false, 0 );
  dpos.setZ(_sidet_pos+_sidet_dist*2.);
  _sidet3_phys = new G4PVPlacement ( 0, dpos, _sidet_log,
                                     "sidet3", _experimentalHall_log,
                                     false, 0 );

  //*************************** housing and scintillator
  _housing_box = new G4Box ( "housing_box", housing_x / 2., housing_y / 2.,
                             housing_z / 2. );

  _housing_log = new G4LogicalVolume ( _housing_box,
                                       G4Material::GetMaterial ( "Air" ),
                                       "housing_log", 0, 0, 0 );
//   _housing_phys = new G4PVPlacement ( 0, G4ThreeVector(100.,0.,0.), _housing_log,
//                                       "housing", _experimentalHall_log,
//                                       false, 0 );
  _housing_log->SetVisAttributes ( G4VisAttributes::Invisible );


  setWLSProperties();
  //Place the WLS slab
//   if(_WLSslab){
  G4VPhysicalVolume* slab = new MuONeWLSSlab ( 0, G4ThreeVector ( 0., 0., 0. ),
      _housing_log, false, 0, this );

  //Surface properties for the WLS slab
//   G4OpticalSurface* ScintWrap = new G4OpticalSurface ( "ScintWrap" );
//
//   new G4LogicalBorderSurface ( "ScintWrap", slab, _housing_phys, ScintWrap );
//
//   ScintWrap->SetType ( dielectric_metal );
//   ScintWrap->SetFinish ( polished );
//   ScintWrap->SetModel ( glisur );

//   const G4int NUM = 2;
//
//   G4double pp[NUM] = {1.0*CLHEP::eV, 7.14*CLHEP::eV};
//   G4double reflectivity[NUM] = {.98, .98};
//   G4double efficiency[NUM] = {0.0, 0.0};
//
//   G4MaterialPropertiesTable* ScintWrapProperty
//   = new G4MaterialPropertiesTable();
//
//   ScintWrapProperty->AddProperty ( "REFLECTIVITY", pp, reflectivity, NUM );
//   ScintWrapProperty->AddProperty ( "EFFICIENCY", pp, efficiency, NUM );
//   ScintWrap->SetMaterialPropertiesTable ( ScintWrapProperty );

  _pdet = new MuONePDet();


//   SurfaceProperties();

  G4RotationMatrix* rotm = new G4RotationMatrix();
  rotm->rotateY ( 270*CLHEP::deg );
  /*  _pmt_phys = new G4PVPlacement(rotm,G4ThreeVector(0.*CLHEP::cm,0.*CLHEP::cm,0.*CLHEP::cm),
    _pmt_log,"pmt",_housing_log,false,0);*/

//   G4VisAttributes* pmt_va = new G4VisAttributes ( G4Colour ( 0.3, 0.0, 0.0 ) );
//   pmt_va->SetForceSolid ( true );
//   _pmt_log->SetVisAttributes ( pmt_va );

  //***********Arrange pmts around the outside of housing**********
  //---pmt sensitive detector
  G4SDManager* SDman = G4SDManager::GetSDMpointer();

  if ( !_silicon_SD ) {
    _silicon_SD = new MuONeSD ( "/MuONeDet/siliconSD" );
    SDman->AddNewDetector ( _silicon_SD );
    //Created here so it exists as silicon detectors are being placed
  }
//
//   _pmt_SD->InitPMTs ( ( _nx*_ny + _nx*_nz + _ny*_nz ) *2 ); //let pmtSD know # of pmts
  //-------

  G4RotationMatrix* rot = new G4RotationMatrix();
  G4double z = (_scint_z + _pdet->zsize())/2.;
  new G4PVPlacement ( rot, G4ThreeVector ( 0., 0., z ), _pdet->log(), "pdet",
                      _housing_log, false, 0 );
//   _pmt_SD->SetPMTPos( 0, 0., 0., z );

  G4double dx = 0;
  if ( _nx != 0 ) dx = _scint_x / _nx;
  G4double dy = 0;
  if ( _ny != 0 ) dy = _scint_y / _ny;
  G4double dz = 0;
  if ( _nz != 0 ) dz = _scint_z / _nz;

/*  G4double x, y, z;
  G4double xmin = -_scint_x / 2. - dx / 2.;
  G4double ymin = -_scint_y / 2. - dy / 2.;
  G4double zmin = -_scint_z / 2. - dz / 2.;
  G4int k = 0;*/
  /*
    z = -_scint_z/2. - height_pmt;      //front
    PlacePMTs(_pmt_log,0,x,y,dx,dy,xmin,ymin,_nx,_ny,x,y,z,k,_pmt_SD);
    G4RotationMatrix* rm_z = new G4RotationMatrix();
    rm_z->rotateY(180*deg);
    z = _scint_z/2. + height_pmt;       //back
    PlacePMTs(_pmt_log,rm_z,x,y,dx,dy,xmin,ymin,_nx,_ny,x,y,z,k,_pmt_SD);
    */

//   G4RotationMatrix* rm_y1 = new G4RotationMatrix();
//   rm_y1->rotateY ( -90*deg );
//   x = -_scint_x / 2. - height_pmt;    //left
//   PlacePMTs ( _pmt_log, rm_y1, y, z, dy, dz, ymin, zmin, _ny, _nz, x, y, z, k, _pmt_SD );
//   G4RotationMatrix* rm_y2 = new G4RotationMatrix();
//   rm_y2->rotateY ( 90*deg );
//   x = _scint_x / 2. + height_pmt;    //right
//   PlacePMTs ( _pmt_log, rm_y2, y, z, dy, dz, ymin, zmin, _ny, _nz, x, y, z, k, _pmt_SD );

  /*
  G4RotationMatrix* rm_x1 = new G4RotationMatrix();
  rm_x1->rotateX(90*deg);
  y = -_scint_y/2. - height_pmt;     //bottom
  PlacePMTs(_pmt_log,rm_x1,x,z,dx,dz,xmin,zmin,_nx,_nz,x,y,z,k,_pmt_SD);
  G4RotationMatrix* rm_x2 = new G4RotationMatrix();
  rm_x2->rotateX(-90*deg);
  y = _scint_y/2. + height_pmt;      //top
  PlacePMTs(_pmt_log,rm_x2,x,z,dx,dz,xmin,zmin,_nx,_nz,x,y,z,k,_pmt_SD);
  */
  //**********Setup Sensitive Detectors***************

  //sensitive detector is not actually on the photocathode.
  //processHits gets done manually by the stepping action.
  //It is used to detect when photons hit and get absorbed&detected at the
  //boundary to the photocathode (which doesnt get done by attaching it to a
  //logical volume.
  //It does however need to be attached to something or else it doesnt get
  //reset at the begining of events
//   _photocath_log->SetSensitiveDetector ( _pmt_SD );

//   _pdet->si_log()->SetSensitiveDetector ( _pmt_SD );

  return _experimentalHall_phys;
}

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
void MuONeDetectorConstruction::SetDimensions ( G4ThreeVector dims )
{
  this->_scint_x = dims[0];
  this->_scint_y = dims[1];
  this->_scint_z = dims[2];
  _updated = true;
}

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
void MuONeDetectorConstruction::SetHousingThickness ( G4double d_mtl )
{
  this->_d_mtl = d_mtl;
  _updated = true;
}

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
void MuONeDetectorConstruction::SetNX ( G4int nx )
{
  this->_nx = nx;
  _updated = true;
}

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
void MuONeDetectorConstruction::SetNY ( G4int ny )
{
  this->_ny = ny;
  _updated = true;
}

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
void MuONeDetectorConstruction::SetNZ ( G4int nz )
{
  this->_nz = nz;
  _updated = true;
}

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
void MuONeDetectorConstruction::SetFiberRadius ( G4double fiberRadius )
{
  this->_fiberRadius = fiberRadius;
  _updated = true;
}

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
void MuONeDetectorConstruction::SetPMTRadius ( G4double outerRadius_pmt )
{
  this->_outerRadius_pmt = outerRadius_pmt;
  _updated = true;
}

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
void MuONeDetectorConstruction::SetDefaults()
{
  //Resets to default values
  _d_mtl = 0.0635 * CLHEP::cm;

  _scint_x = 4. * CLHEP::cm;
  _scint_y = 4. * CLHEP::cm;
  _scint_z = 1. * CLHEP::cm;

  _target_x = 10. * CLHEP::cm;
  _target_y = 10. * CLHEP::cm;
  _target_z = 1. * CLHEP::cm;
  _target_pos = 0. * CLHEP::cm;

  _sidet_x = 10. * CLHEP::cm;
  _sidet_y = 10. * CLHEP::cm;
  _sidet_z = 0.6 * CLHEP::mm;
  _sidet_pos = _target_z*0.5 + 1. * CLHEP::cm;
  _sidet_dist = 23. * CLHEP::cm;

  _nx = 0;
  _ny = 1;
  _nz = 1;

  _outerRadius_pmt = .1 * CLHEP::cm;

  _fiberRadius = 0.05 * CLHEP::cm;

  _refl = 1.0;

  _nfibers = 15;
  _WLSslab = false;
  _mainVolume = true;
  _slab_z = 2.5 * CLHEP::mm;

  G4UImanager::GetUIpointer()
  ->ApplyCommand ( "/MuONe/detector/scintYieldFactor 1." );

  if ( _MPTPStyrene ) _MPTPStyrene->AddConstProperty ( "SCINTILLATIONYIELD", 10. / CLHEP::keV );

  _updated = true;
}


//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
void MuONeDetectorConstruction::UpdateGeometry()
{

  // clean-up previous geometry
  G4GeometryManager::GetInstance()->OpenGeometry();

  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();
  G4LogicalSkinSurface::CleanSurfaceTable();
  G4LogicalBorderSurface::CleanSurfaceTable();

  //define new one
  G4RunManager::GetRunManager()->DefineWorldVolume ( ConstructDetector() );
  G4RunManager::GetRunManager()->GeometryHasBeenModified();

  _updated = false;
}

void MuONeDetectorConstruction::SetMainScintYield ( G4double y )
{
//   _LXe_mt->AddConstProperty ( "SCINTILLATIONYIELD", y / MeV );
}

void MuONeDetectorConstruction::SetWLSScintYield ( G4double y )
{
  _MPTPStyrene->AddConstProperty ( "SCINTILLATIONYIELD", y / CLHEP::MeV );
}

void MuONeDetectorConstruction::PlacePMTs ( G4LogicalVolume* pmt_log,
    G4RotationMatrix *rot,
    G4double &a, G4double &b, G4double da,
    G4double db, G4double amin,
    G4double bmin, G4int na, G4int nb,
    G4double &x, G4double &y, G4double &z,
    G4int &k, MuONePMTSD* sd )
{
  /*PlacePMTs : a different way to parameterize placement that does not depend on
    calculating the position from the copy number

    pmt_log = logical volume for pmts to be placed
    rot = rotation matrix to apply
    a,b = coordinates to vary(ie. if varying in the xy plane then pass x,y)
    da,db = value to increment a,b by
    amin,bmin = start values for a,b
    na,nb = number of repitions in a and b
    x,y,z = just pass x,y, and z by reference (the same ones passed for a,b)
    k = copy number to start with
    sd = sensitive detector for pmts
  */
  G4cout << amin << " " << bmin << " " << a << " " << b << " "  << da <<" " << db << " " << nb << " "  << na << G4endl;
  a = amin;
  for ( G4int j = 1; j <= na; ++j ) {
    a += da;
    b = bmin;
    for ( G4int i = 1; i <= nb; ++i ) {
      b += db;
      G4cout << amin << " " << bmin << " " << a << " " << b << " "  << da <<" " << db << " " << nb << " "  << na << G4endl;
      new G4PVPlacement ( rot, G4ThreeVector ( x, y, z ), pmt_log, "pmt",
                          _housing_log, false, k );
      sd->SetPMTPos ( k, x, y, z );
      ++k;
    }
  }
}

void MuONeDetectorConstruction::SurfaceProperties()
{
  const G4int num = 2;
  G4double Ephoton[num] = {1.0*CLHEP::eV, 7.14*CLHEP::eV};

  //**Scintillator housing properties
  G4double Reflectivity[num] = {_refl, _refl};
  G4double Efficiency[num] = {0.0, 0.0};
  G4MaterialPropertiesTable* scintHsngPT = new G4MaterialPropertiesTable();
  scintHsngPT->AddProperty ( "REFLECTIVITY", Ephoton, Reflectivity, num );
  scintHsngPT->AddProperty ( "EFFICIENCY", Ephoton, Efficiency, num );
  G4OpticalSurface* OpScintHousingSurface =
    new G4OpticalSurface ( "HousingSurface", unified, polished, dielectric_metal );
  OpScintHousingSurface->SetMaterialPropertiesTable ( scintHsngPT );

  //**Photocathode surface properties
  G4double photocath_REFL[num] = {0., 0.};
  G4MaterialPropertiesTable* photocath_mt = new G4MaterialPropertiesTable();
  std::ifstream effFile ( _PMTeffFile.c_str() );
  if ( !effFile.good() ) {
    G4double photocath_EFF[num] = {1., 1.}; //Enables 'detection' of photons
    photocath_mt->AddProperty ( "EFFICIENCY", Ephoton, photocath_EFF, num );
  } else {
    G4cout << "==> Using photodetector efficiency file " << _PMTeffFile << G4endl;
    std::map<double, double> efficiency;
    getEfficiency ( effFile, efficiency );
    effFile.close();
    std::map<double, double>::iterator iter = efficiency.begin();
    G4int esize = efficiency.size();
    G4double* const pheff = new G4double[esize];
    G4double* const phen = new G4double[esize];
    G4int count = 0;
    while ( iter != efficiency.end() ) {
//       G4cout << iter->first << " " << iter->second << G4endl;
      phen[count]  = iter->first;
      pheff[count] = iter->second / 100.;
      ++iter;
      ++count;
    }
    photocath_mt->AddProperty ( "EFFICIENCY", phen, pheff, esize );
    delete [] phen;
    delete [] pheff;
  }
  photocath_mt->AddProperty ( "REFLECTIVITY", Ephoton, photocath_REFL, num );
  G4OpticalSurface* photocath_opsurf =
    new G4OpticalSurface ( "photocath_opsurf", glisur, polished,
                           dielectric_metal );
  photocath_opsurf->SetMaterialPropertiesTable ( photocath_mt );


  //**Create logical skin surfaces
  new G4LogicalSkinSurface ( "photocath_surf", _housing_log,
                             OpScintHousingSurface );
  new G4LogicalSkinSurface ( "photocath_surf", _pdet->si_log(), photocath_opsurf );
}

void MuONeDetectorConstruction::getEfficiency ( std::ifstream& f, std::map<double, double>& effm )
{
  double en, eff, wlen;
  while ( f >> wlen >> eff ) {
    en = CLHEP::c_light * CLHEP::h_Planck / ( wlen * CLHEP::nm );
    effm[en] = eff;
  }
}


void MuONeDetectorConstruction::getWLSprop ( G4String file,
    std::map < G4double, G4double > & prop )
{
  std::ifstream f ( file.c_str() );
  if ( f.good() ) {
    double en, val, wlen;
    while ( f >> wlen >> val ) {
      en = CLHEP::c_light * CLHEP::h_Planck / ( wlen * CLHEP::nm );
//       G4cout << en << "  " << en / eV << " ++ " << val << " " << 2*CLHEP::cm <<G4endl;
      prop[en] = val;
    }
  } else {
    G4cout << file << " is not a valid properties file; I cannot read it! \n\tABORT"
    << G4endl;
    ::abort();
  }
}

void MuONeDetectorConstruction::setWLSProperties()
{
  std::map<G4double, G4double> absl;
  std::map<G4double, G4double> emis;
  std::map<G4double, G4double> rindex;

  getWLSprop ( _WLSabslFile, absl );
  getWLSprop ( _WLSemisFile, emis );
  getWLSprop ( _WLSrindexFile, rindex );

  G4int size = rindex.size();
  G4double* const ridx = new G4double[size];
  G4double* const phen = new G4double[size];
  G4int count = 0;
  std::map<G4double, G4double>::iterator iter = rindex.begin();
  while ( iter != rindex.end() ) {
//     G4cout << iter->first << " " << iter->second << G4endl;
    phen[count]  = iter->first;
    ridx[count] = iter->second;
    iter++;
    count++;
  }
  G4MaterialPropertiesTable* MPTFiber = new G4MaterialPropertiesTable();
  MPTFiber->AddProperty ( "RINDEX", phen, ridx, size );

  size = absl.size();
  G4double* const abs = new G4double[size];
  G4double* const absphen = new G4double[size];
  count = 0;
  iter = absl.begin();
  while ( iter != absl.end() ) {
//     G4cout << iter->first << " " << iter->second << G4endl;
    absphen[count]  = iter->first;
    abs[count] = (iter->second)*CLHEP::cm;
    ++iter;
    ++count;
  }
  MPTFiber->AddProperty ( "WLSABSLENGTH", absphen, abs, size );

  size = emis.size();
  G4double* const emi = new G4double[size];
  G4double* const emiphen = new G4double[size];
  count = 0;
  iter = emis.begin();
  while ( iter != emis.end() ) {
//     G4cout << iter->first << " " << iter->second << G4endl;
    emiphen[count]  = iter->first;
    emi[count] = iter->second;
    iter++;
    count++;
  }
  MPTFiber->AddProperty ( "WLSCOMPONENT", emiphen, emi, size );

  MPTFiber->AddConstProperty ( "WLSTIMECONSTANT", 0.5*CLHEP::ns );
  _PMMA->SetMaterialPropertiesTable ( MPTFiber );
  MPTFiber->DumpTable();
  const G4int WLS_NUMENTRIES = 4;
  G4double WLS_Energy[] = {1.00*CLHEP::eV, 2.87*CLHEP::eV, 2.90*CLHEP::eV, 7.14*CLHEP::eV};

  G4double RefractiveIndexClad1[WLS_NUMENTRIES] = { 1.49, 1.49, 1.49, 1.49};
  G4MaterialPropertiesTable* MPTClad1 = new G4MaterialPropertiesTable();
  MPTClad1->AddProperty ( "RINDEX", WLS_Energy, RefractiveIndexClad1,
                          WLS_NUMENTRIES );
  MPTClad1->AddProperty ( "ABSLENGTH", absphen, abs, absl.size() );
  _Pethylene->SetMaterialPropertiesTable ( MPTClad1 );

  G4double RefractiveIndexClad2[WLS_NUMENTRIES] = { 1.42, 1.42, 1.42, 1.42};
  G4MaterialPropertiesTable* MPTClad2 = new G4MaterialPropertiesTable();
  MPTClad2->AddProperty ( "RINDEX", WLS_Energy, RefractiveIndexClad2,
                          WLS_NUMENTRIES );
  MPTClad2->AddProperty ( "ABSLENGTH", absphen, abs, absl.size() );
  _fPethylene->SetMaterialPropertiesTable ( MPTClad2 );


  delete [] phen;
  delete [] ridx;
  delete [] absphen;
  delete [] abs;
  delete [] emiphen;
  delete [] emi;

}
