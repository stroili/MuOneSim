#ifndef MuONeRoot_H
#define MuONeRoot_H

#include "RecorderBase.hh"

class TFile;
class TTree;
class G4Run;
class G4Event;

class MuONeRootEvent;
class MuONeRootStep;
class MuONeRootMessenger;

class MuONeRoot : public RecorderBase
{

  public:

    MuONeRoot();
    virtual ~MuONeRoot();

    virtual void RecordBeginOfRun ( const G4Run * );
    virtual void RecordEndOfRun ( const G4Run * );
    virtual void RecordBeginOfEvent ( const G4Event* );
    virtual void RecordEndOfEvent ( const G4Event* );
    virtual void RecordTrack ( const G4Track* );
    virtual void RecordStep ( const G4Step* );

    void SetFileName ( const G4String filename );

//  TTree* GetTreePointer() {return theTree;}
//    void Fill();

  private:

    TFile*                 _theFile;
    TTree*                 _theTree;
    MuONeRootEvent*      _theEvent;
    MuONeRootStep*       _theStep;
    G4String               _fname;

    double          _muonThetaExit;
    double          _muonPhiExit;
    double          _eThetaProd;
    double          _ePhiProd;
    double          _eThetaExit;
    double          _ePhiExit;
    double          _muE;
    double          _eE;
    double          _eZprod;
    int             _steps;
    double          _gammaEnergy;
    double          _othersEnergy;
    int             _gammas;
    int             _others;

    MuONeRootMessenger* _recorderMessenger;

    void OpenFile();
    void RenameFile();    // method to rename output file if already present in
                          // xrootd catalog
//    MuONeRootStep *theCloneStep;
//    TClonesArray* theCloneStep;

};

#endif
