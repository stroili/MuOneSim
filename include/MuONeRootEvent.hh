#ifndef MuONeRootEvent_H
#define MuONeRootEvent_H

#include "RootEvent.hh"

class G4Event;

class MuONeRootEvent: public RootEvent
{
    // This class is a "wrapper" for an example ROOT event class.  It's
    // needed because if MyEvent had any reference to G4 classes,
    // we would not be able to use rootcint and other ROOT tools on that
    // class.

  public:
    MuONeRootEvent ( const G4Event* );
    virtual ~MuONeRootEvent();
};

#endif
