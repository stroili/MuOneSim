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

#ifndef MuONeDetectorMessenger_h
#define MuONeDetectorMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class MuONeDetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWithAnInteger;
class G4UIcommand;
class G4UIcmdWithABool;
class G4UIcmdWithADouble;
class G4UIcmdWithAString;

class MuONeDetectorMessenger: public G4UImessenger
{
  public:
    MuONeDetectorMessenger ( MuONeDetectorConstruction* );
    ~MuONeDetectorMessenger();

    void SetNewValue ( G4UIcommand*, G4String );

  private:
    MuONeDetectorConstruction*        _MuONeDetector;
    G4UIdirectory*                    _detectorDir;
    G4UIdirectory*                    _volumesDir;
    G4UIcmdWith3VectorAndUnit*        _dimensionsCmd;
    G4UIcmdWithADoubleAndUnit*        _housingThicknessCmd;
    G4UIcmdWithADoubleAndUnit*        _pmtRadiusCmd;
    G4UIcmdWithADoubleAndUnit*        _fiberRadiusCmd;
    G4UIcmdWithAnInteger*             _nxCmd;
    G4UIcmdWithAnInteger*             _nyCmd;
    G4UIcmdWithAnInteger*             _nzCmd;
    G4UIcmdWithABool*                 _sphereCmd;
    G4UIcmdWithADouble*               _reflectivityCmd;
    G4UIcmdWithABool*                 _wlsCmd;
    G4UIcmdWithABool*                 _lxeCmd;
    G4UIcmdWithAnInteger*             _nFibersCmd;
    G4UIcmdWithADouble*               _slabzCmd;
    G4UIcommand*                      _updateCmd;
    G4UIcommand*                      _defaultsCmd;
    G4UIcmdWithADouble*               _MainScintYield;
    G4UIcmdWithADouble*               _WLSScintYield;
    G4UIcmdWithAString*               _PMTeffCmd;
    G4UIcmdWithAString*               _WLSabslCmd;
    G4UIcmdWithAString*               _WLSemisCmd;
    G4UIcmdWithAString*               _WLSrindCmd;
    G4UIcmdWithADoubleAndUnit*        _targetWidthCmd;
};

#endif

