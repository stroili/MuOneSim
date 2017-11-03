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

#ifndef MuONeEventAction_h
#define MuONeEventAction_h 1

#include "MuONeEventMessenger.hh"
#include "G4UserEventAction.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"

class G4Event;
class RecorderBase;

class MuONeEventAction : public G4UserEventAction
{
  public:
    MuONeEventAction ( RecorderBase* );
    ~MuONeEventAction();

  public:
    void BeginOfEventAction ( const G4Event* );
    void EndOfEventAction ( const G4Event* );

    void SetSaveThreshold ( G4int save );

    void SetEventVerbose ( G4int v ) {
      _verbose = v;
    }

    void SetPMTThreshold ( G4int t ) {
      _pmtThreshold = t;
    }

    void SetForceDrawPhotons ( G4bool b ) {
      _forcedrawphotons = b;
    }
    void SetForceDrawNoPhotons ( G4bool b ) {
      _forcenophotons = b;
    }

  private:
    RecorderBase*           _recorder;
    MuONeEventMessenger*    _eventMessenger;

    G4int                   _saveThreshold;

    G4int                   _scintCollID;
    G4int                   _pmtCollID;

    G4int                   _verbose;

    G4int                   _pmtThreshold;

    G4bool                  _forcedrawphotons;
    G4bool                  _forcenophotons;

    G4int                   _counter;
};

#endif


