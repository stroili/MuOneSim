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
#include "MuONeTrajectory.hh"
#include "MuONeTrackingAction.hh"
#include "MuONeUserTrackInformation.hh"
#include "MuONeDetectorConstruction.hh"
#include "RecorderBase.hh"

#include "G4TrackingManager.hh"
#include "G4Track.hh"
#include "G4ParticleTypes.hh"

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
MuONeTrackingAction::MuONeTrackingAction ( RecorderBase* r )
    : _recorder ( r )
{}

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
void MuONeTrackingAction::PreUserTrackingAction ( const G4Track* aTrack )
{
  //Let this be up to the user via vis.mac
  //  fpTrackingManager->SetStoreTrajectory(true);

  //Use custom trajectory class
  fpTrackingManager->SetTrajectory ( new MuONeTrajectory ( aTrack ) );

  //This user track information is only relevant to the photons
  fpTrackingManager->SetUserTrackInformation ( new MuONeUserTrackInformation );

  /*  const G4VProcess* creator = aTrack->GetCreatorProcess();
  if(creator)
    G4cout<<creator->GetProcessName()<<G4endl;
  */
}

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
void MuONeTrackingAction::PostUserTrackingAction ( const G4Track* aTrack )
{
  MuONeTrajectory* trajectory = ( MuONeTrajectory* ) fpTrackingManager->GimmeTrajectory();
  MuONeUserTrackInformation*
  trackInformation = ( MuONeUserTrackInformation* ) aTrack->GetUserInformation();

  //Lets choose to draw only the photons that hit the sphere and a pmt
  if ( aTrack->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition() ) {

    const G4VProcess* creator = aTrack->GetCreatorProcess();
//     G4cout << "creator: " <<creator->GetProcessName()<< G4endl;
    if ( creator && creator->GetProcessName() == "OpWLS" ) {
      trajectory->WLS();
      trajectory->SetDrawTrajectory ( true );
    }

    /*    if(MuONeDetectorConstruction::GetSphereOn()){
          if((trackInformation->GetTrackStatus()&hitPMT)&&
      (trackInformation->GetTrackStatus()&hitSphere)){
     trajectory->SetDrawTrajectory(true);
          }
        }
        else{*/
    if ( trackInformation->GetTrackStatus() &hitPMT )
      trajectory->SetDrawTrajectory ( true );
//     }
  } else //draw all other trajectories
    trajectory->SetDrawTrajectory ( true );

  if ( trackInformation->GetForceDrawTrajectory() )
    trajectory->SetDrawTrajectory ( true );

  if ( _recorder ) _recorder->RecordTrack ( aTrack );
}
