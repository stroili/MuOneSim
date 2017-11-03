// This code implementation is the intellectual property of
// the GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
#ifndef MuONePrimaryGeneratorMessenger_h
#define MuONePrimaryGeneratorMessenger_h 1

class MuONePrimaryGeneratorAction;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWithABool;
class G4UIcmdWithAString;

#include "G4UImessenger.hh"
#include "globals.hh"

class MuONePrimaryGeneratorMessenger: public G4UImessenger
{
  public:
    MuONePrimaryGeneratorMessenger ( MuONePrimaryGeneratorAction* mpga );
    ~MuONePrimaryGeneratorMessenger();

  public:
    void SetNewValue ( G4UIcommand * command, G4String newValues );
    G4String GetCurrentValue ( G4UIcommand * command );

  private:
    MuONePrimaryGeneratorAction * _target;

  private: //commands
    G4UIcmdWithADoubleAndUnit*  _momentumCmd;
    G4UIcmdWithADoubleAndUnit*  _sigmaMomCmd;
    G4UIcmdWithADoubleAndUnit*  _sigmaAngCmd;
    G4UIcmdWith3VectorAndUnit*  _posCmd;
    G4UIcmdWithAString*         _particleCmd;
    G4UIcmdWithABool*           _setRandomCmd;
};

#endif

