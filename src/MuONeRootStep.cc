#include "RootStep.hh"
#include "MuONeRootStep.hh"

#include "RecorderBase.hh"

#include "MuONeUserEventInformation.hh"
#include "MuONeUserTrackInformation.hh"
#include "MuONeTrajectory.hh"
#include "G4TrajectoryContainer.hh"

#include "MuONeSteppingAction.hh"
#include "MuONeEventAction.hh"
#include "MuONeTrackingAction.hh"
#include "MuONeSteppingMessenger.hh"

#include <string>

#include "globals.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4SteppingManager.hh"
#include "G4TrackingManager.hh"
#include "G4SDManager.hh"
#include "G4EventManager.hh"
#include "G4ProcessManager.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4Event.hh"
#include "G4StepPoint.hh"
#include "G4TrackStatus.hh"
#include "G4TrackVector.hh"
#include "G4ThreeVector.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4VisAttributes.hh"
#include "G4VVisManager.hh"
#include "G4VisManager.hh"
#include "G4UserSteppingAction.hh"

#include "TROOT.h"
#include "Rtypes.h"
#include "TObject.h"
#include "TClass.h"
#include "TClonesArray.h"
#include "TFile.h"
#include "TTree.h"
#include "TBrowser.h"
#include "TBranch.h"

#include "globals.hh"

MuONeRootStep::MuONeRootStep ( const G4Step* aStep ) : RootStep()
{

//  G4TrackingManager *tm = G4TrackingManager::GeTTrackingManager();
//  G4Track* tm = G4Track::GetTrack();

  G4Track* tm = aStep->GetTrack();

  MuONeUserTrackInformation* trackInformation
  = ( MuONeUserTrackInformation* ) tm->GetUserInformation();
  MuONeUserEventInformation* eventInformation
  = ( MuONeUserEventInformation* ) G4EventManager::GetEventManager()
    ->GetConstCurrentEvent()->GetUserInformation();

  G4StepPoint* thePrePoint = aStep->GetPreStepPoint();
  G4VPhysicalVolume* thePrePV = thePrePoint->GetPhysicalVolume();

  G4StepPoint* thePostPoint = aStep->GetPostStepPoint();
  G4VPhysicalVolume* thePostPV = thePostPoint->GetPhysicalVolume();

  G4OpBoundaryProcessStatus boundaryStatus = Undefined;
  static G4OpBoundaryProcess* boundary = NULL;

  if ( !boundary ) {
    G4ProcessManager* pm
    = aStep->GetTrack()->GetDefinition()->GetProcessManager();
    G4int nprocesses = pm->GetProcessListLength();
    G4ProcessVector* pv = pm->GetProcessList();
    G4int i;
    for ( i = 0; i < nprocesses; i++ ) {
      if ( ( *pv ) [i]->GetProcessName() == "OpBoundary" ) {
        boundary = ( G4OpBoundaryProcess* ) ( *pv ) [i];
        break;
      }
    }
  }

  if ( tm->GetParentID() == 0 ) {

//     G4TrackVector* fSecondary=fpSteppingManager->GetfSecondary();
//     G4int tN2ndariesTot = fpSteppingManager->GetfN2ndariesAtRestDoIt()
//         + fpSteppingManager->GetfN2ndariesAlongStepDoIt()
//         + fpSteppingManager->GetfN2ndariesPostStepDoIt();

    /*    if ( tN2ndariesTot > 0 ) {
          int ns = 0;
    //      theCloneStep->Clear();
          for(size_t i=0; i<(*fSecondary).size(); i++){
    //        theStep = new( (*theCloneStep)[ns] ) MyStep();
            G4Track* at = (*fSecondary)[i];
    //        fPDG   = at->GetDefinition()->GetPDGEncoding();
    //        fKinEn = at->GetKineticEnergy()/MeV;
    //        fTotEn = at->GetTotalEnergy()/MeV;
    //        fMom   = at->GetMomentum().mag()/MeV;
    //        fVel   = at->GetVelocity();
    //        theStep->sfPDG = fPDG;
    //        theStep->sfKinEn = fKinEn;
    //        theStep->sfTotEn = fTotEn;
    //        theStep->sfMom = fMom ;
    //        theStep->sfVel = fVel;
            SetPDGEncoding( at->GetDefinition()->GetPDGEncoding() );
            SetKineticEnergy( at->GetKineticEnergy()/MeV );
            SetTotalEnergy( at->GetTotalEnergy()/MeV );
            SetMomentum( at->GetMomentum().mag()/MeV );
            SetVelocity( at->GetVelocity() );
            ns++;
          }
        }*/
  }

}

MuONeRootStep::~MuONeRootStep()
{
}
