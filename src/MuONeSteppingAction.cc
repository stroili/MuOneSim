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
#include "MuONeSteppingAction.hh"
#include "MuONeEventAction.hh"
#include "MuONeTrackingAction.hh"
#include "MuONeTrajectory.hh"
#include "MuONePMTSD.hh"
#include "MuONeUserTrackInformation.hh"
#include "MuONeUserEventInformation.hh"
#include "MuONeSteppingMessenger.hh"
#include "RecorderBase.hh"

#include "G4SteppingManager.hh"
#include "G4SDManager.hh"
#include "G4EventManager.hh"
#include "G4ProcessManager.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4Event.hh"
#include "G4StepPoint.hh"
#include "G4TrackStatus.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4OpBoundaryProcess.hh"

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
MuONeSteppingAction::MuONeSteppingAction ( RecorderBase* r )
    : _recorder ( r ), _oneStepPrimaries ( false )
{
  _steppingMessenger = new MuONeSteppingMessenger ( this );
}

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
MuONeSteppingAction::~MuONeSteppingAction()
{
  delete _steppingMessenger;
}

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
void MuONeSteppingAction::UserSteppingAction ( const G4Step * theStep )
{
  G4Track* theTrack = theStep->GetTrack();
  MuONeUserTrackInformation* trackInformation
  = ( MuONeUserTrackInformation* ) theTrack->GetUserInformation();
  MuONeUserEventInformation* eventInformation
  = ( MuONeUserEventInformation* ) G4EventManager::GetEventManager()
    ->GetConstCurrentEvent()->GetUserInformation();

  G4StepPoint* thePrePoint = theStep->GetPreStepPoint();
  G4VPhysicalVolume* thePrePV = thePrePoint->GetPhysicalVolume();

  G4StepPoint* thePostPoint = theStep->GetPostStepPoint();
  G4VPhysicalVolume* thePostPV = thePostPoint->GetPhysicalVolume();

  G4ThreeVector muExitMom;
  G4ThreeVector eProdMom;
  G4ThreeVector eExitMom;

//   G4cout << theTrack->GetParticleDefinition()->GetParticleName() << "\t " << thePrePV->GetName() ;
//   if (thePostPV) G4cout << " +-+ " << thePostPV->GetName();
//   G4cout << G4endl;
  G4OpBoundaryProcessStatus boundaryStatus = Undefined;
  static G4OpBoundaryProcess* boundary = NULL;

  //find the boundary process only once
  if ( !boundary ) {
    G4ProcessManager* pm
    = theStep->GetTrack()->GetDefinition()->GetProcessManager();
    G4int nprocesses = pm->GetProcessListLength();
    G4ProcessVector* pv = pm->GetProcessList();
    G4int i;
    for ( i = 0;i < nprocesses;i++ ) {
      if ( ( *pv ) [i]->GetProcessName() == "OpBoundary" ) {
        boundary = ( G4OpBoundaryProcess* ) ( *pv ) [i];
        break;
      }
    }
  }

  if ( theTrack->GetParentID() == 0 ) {
    //This is a primary track
    if (thePrePV->GetName() == "target") {
      muExitMom = theTrack->GetMomentumDirection();
      eventInformation->SetMuExitMom(muExitMom);
      eventInformation->SetMuEnergy(theTrack->GetKineticEnergy());
//       G4cout << muExitMom << G4endl;
    }
//     G4cout << thePrePoint->GetPosition() << " " << thePostPoint->GetPosition() << " " << theTrack->GetPosition() << " + " << theTrack->GetMomentumDirection() << G4endl;
    G4TrackVector* fSecondary = fpSteppingManager->GetfSecondary();
    G4int tN2ndariesTot = fpSteppingManager->GetfN2ndariesAtRestDoIt()
                          + fpSteppingManager->GetfN2ndariesAlongStepDoIt()
                          + fpSteppingManager->GetfN2ndariesPostStepDoIt();

    //If we havent already found the conversion position and there were
    //secondaries generated, then search for it
    if ( !eventInformation->IsConvPosSet() && tN2ndariesTot > 0 ) {
      for ( size_t lp1 = ( *fSecondary ).size() - tN2ndariesTot;
            lp1 < ( *fSecondary ).size(); lp1++ ) {
        const G4VProcess* creator = ( *fSecondary ) [lp1]->GetCreatorProcess();
        if ( creator ) {
          G4String creatorName = creator->GetProcessName();
          if ( creatorName == "phot" || creatorName == "compt" || creatorName == "conv" ) {
            //since this is happening before the secondary is being tracked
            //the Vertex position has not been set yet(set in initial step)
            eventInformation->SetConvPos ( ( *fSecondary ) [lp1]->GetPosition() );
          }
        }
      }
    }

    if ( _oneStepPrimaries && thePrePV->GetName() == "scintillator" )
      theTrack->SetTrackStatus ( fStopAndKill );
  } else if (theTrack->GetParticleDefinition()->GetParticleName()=="e-") {
    static int steps=1;
    if (thePrePV->GetName() == "target" and theStep->IsFirstStepInVolume()) {
      steps=1;
      eProdMom = theTrack->GetVertexMomentumDirection();
      eventInformation->SeteProdMom(eProdMom);
      eventInformation->seteZProd(thePrePoint->GetPosition().z());
//       G4cout << thePrePoint->GetPosition().z() << " " << thePostPoint->GetPosition().z() << G4endl;
      if (abs(theTrack->GetMomentumDirection().theta()-theTrack->GetVertexMomentumDirection().theta())>0.01 and theTrack->GetKineticEnergy()/CLHEP::GeV>1.) {
        G4cout << "\t\t" << thePrePoint->GetPosition().z() << " " << thePostPoint->GetPosition().z() << " "
                << theTrack->GetMomentumDirection().theta()-theTrack->GetVertexMomentumDirection().theta() << " + "
                << theTrack->GetKineticEnergy()/CLHEP::GeV << "\t------------------------" << G4endl;
        G4cout << thePostPV->GetName() <<G4endl;
      }
//       if (thePostPV->GetName() == "expHall" and theTrack->GetKineticEnergy()/CLHEP::GeV>1.) G4cout << "\t++\t" << theTrack->GetMomentumDirection().theta()-theTrack->GetVertexMomentumDirection().theta()
//         << " == " <<  theTrack->GetKineticEnergy()/CLHEP::GeV << "\t++\t" << G4endl;
    } else if (thePrePV->GetName() == "target" and thePostPV->GetName() == "expHall") {
      eExitMom = theTrack->GetMomentumDirection() ;
      ++steps;
      eventInformation->SeteExitMom(eExitMom);
      eventInformation->SeteEnergy(theTrack->GetKineticEnergy());
      eventInformation->SetSteps(steps);
//       G4cout << "\t\t" << eExitMom << "\t========================" << G4endl;
    } else if (thePrePV->GetName() == "target" ) {
      ++steps;
    }
//     G4cout << thePrePoint->GetPosition() << " " << thePostPoint->GetPosition() << " " << theTrack->GetParticleDefinition()->GetParticleName() << " " << theTrack->GetPosition() << " " << theTrack->GetMomentumDirection() << G4endl;
  } else if (theTrack->GetParticleDefinition()->GetParticleName()=="gamma" and theStep->IsFirstStepInVolume()) {
    eventInformation->addGammaEnergy(theTrack->GetKineticEnergy());
    eventInformation->addGamma();
  } else if (theStep->IsFirstStepInVolume()) {
    eventInformation->addOthersEnergy(theTrack->GetKineticEnergy());
    eventInformation->addOthers();
  }

  if ( !thePostPV ) { //out of world
    return;
  }

  G4ParticleDefinition* particleType = theTrack->GetDefinition();
  if ( particleType == G4OpticalPhoton::OpticalPhotonDefinition() ) {
    //Optical photon only

    if ( thePrePV->GetName() == "target" )
      //force drawing of photons in WLS slab
      trackInformation->SetForceDrawTrajectory ( true );
    else if ( thePostPV->GetName() == "expHall" )
      //Kill photons entering expHall from something other than Slab
      theTrack->SetTrackStatus ( fStopAndKill );

    //Was the photon absorbed by the absorption process
    if ( thePostPoint->GetProcessDefinedStep()->GetProcessName()
         == "OpAbsorption" ) {
      eventInformation->IncAbsorption();
      trackInformation->AddTrackStatusFlag ( absorbed );
    }

    if ( thePostPoint->GetProcessDefinedStep()->GetProcessName()
         == "OpWLS" ) {
      eventInformation->IncShifted();
      trackInformation->AddTrackStatusFlag ( shifted );
    }

    boundaryStatus = boundary->GetStatus();
    //Check to see if the partcile was actually at a boundary
    //Otherwise the boundary status may not be valid
    //Prior to Geant4.6.0-p1 this would not have been enough to check
    if ( thePostPoint->GetStepStatus() == fGeomBoundary ) {
      switch ( boundaryStatus ) {
        case Absorption:
          trackInformation->AddTrackStatusFlag ( boundaryAbsorbed );
          eventInformation->IncBoundaryAbsorption();
          break;
        case Detection: //Note, this assumes that the volume causing detection
          //is the photocathode because it is the only one with
          //non-zero efficiency
          {
            //Triger sensitive detector manually since photon is
            //absorbed but status was Detection
            G4SDManager* SDman = G4SDManager::GetSDMpointer();
            G4String sdName = "/MuONeDet/pmtSD";
            MuONePMTSD* pmtSD = ( MuONePMTSD* ) SDman
                                   ->FindSensitiveDetector ( sdName );
            if ( pmtSD )
              pmtSD->ProcessHits_constStep ( theStep, NULL );
            trackInformation->AddTrackStatusFlag ( hitPMT );
            break;
          }
        case FresnelReflection:
        case TotalInternalReflection:
        case SpikeReflection:
          trackInformation->IncReflections();
          break;
        default:
          break;
      }
    }
  }

  if ( _recorder ) _recorder->RecordStep ( theStep );
}
