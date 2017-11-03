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
#include <vector>

#include "MuONeEventAction.hh"
#include "MuONeHit.hh"
#include "MuONePMTHit.hh"
#include "MuONeUserEventInformation.hh"
#include "MuONeTrajectory.hh"
#include "RecorderBase.hh"

#include "G4EventManager.hh"
#include "G4SDManager.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4VVisManager.hh"
#include "G4ios.hh"
#include "G4UImanager.hh"

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
MuONeEventAction::MuONeEventAction ( RecorderBase* r )
    : _recorder ( r ), _saveThreshold ( 0 ), _scintCollID ( -1 ), _pmtCollID ( -1 ), _verbose ( 0 ),
    _pmtThreshold ( 1 ), _forcedrawphotons ( false ), _forcenophotons ( false ),
    _counter ( 0 )
{
  _eventMessenger = new MuONeEventMessenger ( this );
}

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
MuONeEventAction::~MuONeEventAction()
{
  delete _eventMessenger;
}

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
void MuONeEventAction::BeginOfEventAction ( const G4Event* anEvent )
{

  //New event, add the user information object
  G4EventManager::
  GetEventManager()->SetUserInformation ( new MuONeUserEventInformation );

  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  if ( _scintCollID < 0 )
    _scintCollID = SDman->GetCollectionID ( "scintCollection" );
  if ( _pmtCollID < 0 )
    _pmtCollID = SDman->GetCollectionID ( "pmtHitCollection" );

  _counter++;

  if ( _recorder ) _recorder->RecordBeginOfEvent ( anEvent );
}

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
void MuONeEventAction::EndOfEventAction ( const G4Event* anEvent )
{

  MuONeUserEventInformation* eventInformation
  = ( MuONeUserEventInformation* ) anEvent->GetUserInformation();

  G4TrajectoryContainer* trajectoryContainer = anEvent->GetTrajectoryContainer();

  G4int n_trajectories = 0;
  if ( trajectoryContainer ) n_trajectories = trajectoryContainer->entries();

  // extract the trajectories and draw them
  if ( G4VVisManager::GetConcreteInstance() ) {
    for ( G4int i = 0; i < n_trajectories; i++ ) {
      MuONeTrajectory* trj = ( MuONeTrajectory* )
                                ( ( * ( anEvent->GetTrajectoryContainer() ) ) [i] );
      if ( trj->GetParticleName() == "opticalphoton" ) {
        trj->SetForceDrawTrajectory ( _forcedrawphotons );
        trj->SetForceNoDrawTrajectory ( _forcenophotons );
      }
      trj->DrawTrajectory ( 50 );
    }
  }

  MuONeHitsCollection* SHC = 0;
  MuONePMTHitsCollection* PHC = 0;
  G4HCofThisEvent* HCE = anEvent->GetHCofThisEvent();

  //Get the hit collections
  if ( HCE ) {
    if ( _scintCollID >= 0 ) SHC =
        ( MuONeHitsCollection* ) ( HCE->GetHC ( _scintCollID ) );
    if ( _pmtCollID >= 0 ) PHC = ( MuONePMTHitsCollection* ) ( HCE->GetHC ( _pmtCollID ) );
  }

  //Hits in scintillator
  if ( SHC ) {
    int n_hit = SHC->entries();
    G4ThreeVector  eWeightPos ( 0. );
    G4double edep;
    G4double edepMax = 0;

    for ( int i = 0;i < n_hit;i++ ) { //gather info on hits in scintillator
      edep = ( *SHC ) [i]->GetEdep();
      eventInformation->IncEDep ( edep ); //sum up the edep
      eWeightPos += ( *SHC ) [i]->GetPos() * edep;//calculate energy weighted pos
      if ( edep > edepMax ) {
        edepMax = edep;//store max energy deposit
        G4ThreeVector posMax = ( *SHC ) [i]->GetPos();
        eventInformation->SetPosMax ( posMax, edep );
      }
    }
    if ( eventInformation->GetEDep() == 0. ) {
      if ( _verbose > 0 ) G4cout << "No hits in the scintillator this event." << G4endl;
    } else {
      //Finish calculation of energy weighted position
      eWeightPos /= eventInformation->GetEDep();
      eventInformation->SetEWeightPos ( eWeightPos );
      if ( _verbose > 0 ) {
        G4cout << "\tEnergy weighted position of hits in LXe : "
        << eWeightPos / CLHEP::mm << G4endl;
      }
    }
    if ( _verbose > 0 ) {
      G4cout << "\tTotal energy deposition in scintillator : "
      << eventInformation->GetEDep() / CLHEP::keV << " (keV)" << G4endl;
    }
  }

/*  std::vector<G4double> mintimes;
  std::vector<G4double> maxtimes;
  std::vector<G4double> avetimes;
  std::vector<G4double> secondtimes;
  std::vector<G4double> thirdtimes;*/
  G4int pmts = 0;
  if ( PHC ) {
    G4ThreeVector reconPos;
    pmts = PHC->entries();
    //Gather info from all PMTs
    for ( G4int i = 0;i < pmts;i++ ) {
/*      mintimes.push_back ( ( *PHC ) [i]->GetMinTime() );
      maxtimes.push_back ( ( *PHC ) [i]->GetMaxTime() );
      avetimes.push_back ( ( *PHC ) [i]->GetAvergTime() );
      secondtimes.push_back ( ( *PHC ) [i]->GetSecondTime() );
      thirdtimes.push_back ( ( *PHC ) [i]->GetThirdTime() );*/
      eventInformation->IncHitCount ( ( *PHC ) [i]->GetPhotonCount() );
      reconPos += ( *PHC ) [i]->GetPMTPos() * ( *PHC ) [i]->GetPhotonCount();
      if ( ( *PHC ) [i]->GetPhotonCount() >= _pmtThreshold ) {
        eventInformation->IncPMTSAboveThreshold();
      } else { //wasnt above the threshold, turn it back off
        ( *PHC ) [i]->SetDrawit ( false );
      }
    }

    if ( eventInformation->GetHitCount() > 0 ) { //dont bother unless there were hits
      reconPos /= eventInformation->GetHitCount();
      if ( _verbose > 0 ) {
        G4cout << "\tReconstructed position of hits in crystal : "
        << reconPos / CLHEP::mm << G4endl;
      }
      eventInformation->SetReconPos ( reconPos );
    }
    PHC->DrawAllHits();
  }

  if ( _verbose > 0 ) {
    //End of event output. later to be controlled by a verbose level
    G4cout << "\tNumber of photons that hit PMTs in this event : "
    << eventInformation->GetHitCount() << G4endl;
    G4cout << "\tNumber of PMTs above threshold(" << _pmtThreshold << ") : "
    << eventInformation->GetPMTSAboveThreshold() << G4endl;
    G4cout << "\tNumber of photons produced by scintillation in this event : "
    << eventInformation->GetPhotonCount_Scint() << G4endl;
    G4cout << "\tNumber of photons produced by cerenkov in this event : "
    << eventInformation->GetPhotonCount_Ceren() << G4endl;
    G4cout << "\tNumber of photons absorbed (OpAbsorption) in this event : "
    << eventInformation->GetAbsorptionCount() << G4endl;
    G4cout << "\tNumber of photons shifted (OpWLS) in this event : "
        << eventInformation->GetShiftedCount() << G4endl;
    G4cout << "\tNumber of photons absorbed at boundaries (OpBoundary) in "
    << "this event : " << eventInformation->GetBoundaryAbsorptionCount()
    << G4endl;
    G4cout << "Unaccounted for photons in this event : "
    << ( eventInformation->GetPhotonCount_Scint() +
         eventInformation->GetPhotonCount_Ceren() -
         eventInformation->GetAbsorptionCount() -
         eventInformation->GetHitCount() -
         eventInformation->GetBoundaryAbsorptionCount() )
    << G4endl;
/*    for ( int i = 0; i < pmts; i++ ) {
      G4cout << "PMT # " << i << " min time (ns) " << mintimes[i] / ns << " max time (ns) " << maxtimes[i] / ns
      << " average time (ns) " << avetimes[i] / ns << G4endl;
      G4cout << "\tsecond photon time (ns) " << secondtimes[i] / ns  << " third time (ns) "
      << thirdtimes[i] / ns << G4endl;

    }*/
  }
  //If we have set the flag to save 'special' events, save here
  if ( _saveThreshold && eventInformation->GetPhotonCount() <= _saveThreshold )
    G4RunManager::GetRunManager()->rndmSaveThisEvent();

  if ( _counter % 10000 == 0 ) {
    G4cout << "generated " << _counter << " events" << G4endl;
  }

  if ( _recorder ) _recorder->RecordEndOfEvent ( anEvent );
}

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
void MuONeEventAction::SetSaveThreshold ( G4int save )
{
  /*Sets the save threshold for the random number seed. If the number of photons
  generated in an event is lower than this, then save the seed for this event
  in a file called run###evt###.rndm
  */
  _saveThreshold = save;
  G4RunManager::GetRunManager()->SetRandomNumberStore ( true );
  G4RunManager::GetRunManager()->SetRandomNumberStoreDir ( "random/" );
  //  G4UImanager::GetUIpointer()->ApplyCommand("/random/setSavingFlag 1");
}




