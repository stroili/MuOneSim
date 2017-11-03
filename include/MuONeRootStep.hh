#ifndef MuONeRootStep_H
#define MuONeRootStep_H

#include "RootStep.hh"

class G4Step;
class G4UserSteppingAction;
class MuONeSteppingAction;

#include "globals.hh"
#include "G4UserSteppingAction.hh"

class RecorderBase;
class MuONeEventAction;
class MuONeTrackingAction;
class MuONeSteppingMessenger;

class MuONeRootStep: public RootStep
{
// This class is a "wrapper" for an example ROOT event class.  It's
// needed because if MyStep had any reference to G4 classes,
// we would not be able to use rootcint and other ROOT tools on that
// class.

  public:
    MuONeRootStep ( const G4Step* );
//  MuONeRootStep(const MuONeSteppingAction*);
    virtual ~MuONeRootStep();
};

#endif
