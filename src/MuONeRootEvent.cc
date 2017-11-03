#include "MuONeRootEvent.hh"
#include "RootEvent.hh"

#include "Rtypes.h"
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"

#include "RecorderBase.hh"

#include "MuONeUserEventInformation.hh"
#include "MuONeTrajectory.hh"
#include "G4TrajectoryContainer.hh"

#include <string>

#include "globals.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4Event.hh"

#include "MuONeHit.hh"
#include "MuONePMTHit.hh"

MuONeRootEvent::MuONeRootEvent ( const G4Event* anEvent ) : RootEvent()
{
//   G4cout << "\t...recorder" << G4endl;
  G4RunManager* rm = G4RunManager::GetRunManager();
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  G4int scintCollID = SDman->GetCollectionID ( "scintCollection" );
  G4int pmtCollID = SDman->GetCollectionID ( "pmtHitCollection" );

  SetRunID ( rm->GetCurrentRun()->GetRunID() );
  SetEventID ( anEvent->GetEventID() );

  MuONeUserEventInformation* eventInformation
  = ( MuONeUserEventInformation* ) anEvent->GetUserInformation();

  G4TrajectoryContainer* trajectoryContainer = anEvent->GetTrajectoryContainer();

  G4int n_trajectories = 0;
  if ( trajectoryContainer ) n_trajectories = trajectoryContainer->entries();

  MuONeHitsCollection* SHC = 0;
  MuONePMTHitsCollection* PHC = 0;
  G4HCofThisEvent* HCE = anEvent->GetHCofThisEvent();

  //Get the hit collections
  if ( HCE ) {
    if ( scintCollID >= 0 ) SHC = ( MuONeHitsCollection* ) ( HCE->GetHC ( scintCollID ) );
    if ( pmtCollID >= 0 ) PHC = ( MuONePMTHitsCollection* ) ( HCE->GetHC ( pmtCollID ) );
  }

  //Hits in scintillator
  if ( SHC ) {
    int n_hit = SHC->entries();
    G4ThreeVector  eWeightPos ( 0. );
    G4double edep;
    G4double edepMax = 0;

    for ( int i = 0; i < n_hit; i++ ) { //gather info on hits in scintillator
      edep = ( *SHC ) [i]->GetEdep();
      eventInformation->IncEDep ( edep ); //sum up the edep
      eWeightPos += ( *SHC ) [i]->GetPos() * edep;//calculate energy weighted pos
      if ( edep > edepMax ) {
        edepMax = edep;//store max energy deposit
        G4ThreeVector posMax = ( *SHC ) [i]->GetPos();
        eventInformation->SetPosMax ( posMax, edep );
      }
    }
    /*    if (eventInformation->GetEDep()==0.) {
          G4cout<<"No hits in the scintillator this event."<<G4endl;
        } else {
              //Finish calculation of energy weighted position
          eWeightPos/=eventInformation->GetEDep();
          eventInformation->SetEWeightPos(eWeightPos);

          G4cout << "\tEnergy weighted position of hits in LXe : "
                << eWeightPos/mm << G4endl;
        }*/
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
    SetNPMTs ( pmts );
    for ( G4int i = 0; i < pmts; i++ ) {
/*      mintimes.push_back ( ( *PHC ) [i]->GetMinTime() );
      maxtimes.push_back ( ( *PHC ) [i]->GetMaxTime() );
      avetimes.push_back ( ( *PHC ) [i]->GetAvergTime() );
      secondtimes.push_back ( ( *PHC ) [i]->GetSecondTime() );
      thirdtimes.push_back ( ( *PHC ) [i]->GetThirdTime() );*/
      int pmtID = (*PHC )[i]->GetPMTNumber();
      G4int nphotons = ( *PHC ) [i]->GetPhotonCount();
      SetNphotons ( pmtID, nphotons );
      eventInformation->IncHitCount ( nphotons );
      reconPos += ( *PHC ) [i]->GetPMTPos() * nphotons;
//       ( *PHC ) [i]->SortTimes();
      for ( G4int j = 0; j < nphotons; j++ ) {
        SetTimes ( pmtID, ( *PHC ) [i]->GetTime ( j ) / CLHEP::ns );
        SetWLengths ( pmtID, ( *PHC ) [i]->GetWLength ( j ) );
      }
    }
  }

  /*  if (_verbose>0)
    {
        //End of event output. later to be controlled by a verbose level
      G4cout << "\tNumber of photons that hit PMTs in this event : "
          << eventInformation->GetHitCount() << G4endl;
      G4cout << "\tNumber of PMTs above threshold("<<_pmtThreshold<<") : "
          << eventInformation->GetPMTSAboveThreshold() << G4endl;
      G4cout << "\tNumber of photons produced by scintillation in this event : "
          << eventInformation->GetPhotonCount_Scint() << G4endl;
      G4cout << "\tNumber of photons produced by cerenkov in this event : "
          << eventInformation->GetPhotonCount_Ceren() << G4endl;
      G4cout << "\tNumber of photons absorbed (OpAbsorption) in this event : "
          << eventInformation->GetAbsorptionCount() << G4endl;
      G4cout << "\tNumber of photons absorbed at boundaries (OpBoundary) in "
          << "this event : " << eventInformation->GetBoundaryAbsorptionCount()
          << G4endl;
      G4cout << "Unacounted for photons in this event : "
          << (eventInformation->GetPhotonCount_Scint() +
          eventInformation->GetPhotonCount_Ceren() -
          eventInformation->GetAbsorptionCount() -
          eventInformation->GetHitCount() -
          eventInformation->GetBoundaryAbsorptionCount())
          << G4endl;
      for (int i=0; i<pmts; i++) {
        G4cout << "PMT # " << i << " min time (ns) " << mintimes[i]/ns << " max time (ns) " << maxtimes[i]/ns
            << " average time (ns) " << avetimes[i]/ns << G4endl;
        G4cout << "\tsecond photon time (ns) " << secondtimes[i]/ns  << " third time (ns) "
            << thirdtimes[i]/ns << G4endl;

      }
    }*/

//  G4TrajectoryContainer* tc = aEvent->GetTrajectoryContainer();
  //
 G4int NTraj = 0;
 if (trajectoryContainer ) NTraj = trajectoryContainer->entries();
 SetNTraj( NTraj );
  //
 G4int count = 0;
  //
 int NChar=0;
 int NNeut=0;
 double chargE=0.;
 double neutrE=0.;
//  if (NTraj == 1) {
//     MuONeTrajectory* trj = (MuONeTrajectory*)
//        ((*(anEvent->GetTrajectoryContainer()))[0]);
// //     G4cout << "\t...muone? " << trj->GetParticleName() << " " << trj->GetInitialKineticEnergy()/CLHEP::GeV << G4endl;
// //     G4cout << "\t...\t " << trj->GetPoint(0)->GetPosition() << " " << trj->GetTrackID() << " " << trj->GetParentID() << G4endl;
//  } else {
   for (G4int i=0; i<NTraj; ++i) {
      MuONeTrajectory* trj = (MuONeTrajectory*)
         ((*(anEvent->GetTrajectoryContainer()))[i]);
      if (trj->GetCharge()!=0) {
        ++NChar;
        chargE += trj->GetInitialKineticEnergy();
      } else {
        ++NNeut;
        neutrE += trj->GetInitialKineticEnergy();
      }
      G4cout << "\tpoints in trajectory: " << trj->GetPointEntries() << " - " << trj->GetParticleName() << G4endl;
      for (int k=0; k<trj->GetPointEntries(); ++k) {
        G4cout << "\t\t" << k << "\t" << trj->GetPoint(k)->GetPosition() << G4endl;
      }
//       G4cout << trj->GetParticleName() << " " << trj->GetInitialKineticEnergy()/CLHEP::GeV << G4endl;
//       G4cout << "\t...\t " << trj->GetPoint(0)->GetPosition() << " " << trj->GetTrackID() << " " << trj->GetParentID() << G4endl;
   }
   ++count;
//  }
//  SetCount( count );
  //
  //

//  if ( charged>0 ) ++NChar;
//  if ( neutr>0 ) ++NNeut;

 SetNChar( NChar );
 SetNNeut( NNeut );
//  SetNCharE( chargE / CLHEP::GeV );
//  SetNNeutE( neutrE / CLHEP::GeV );

}

MuONeRootEvent::~MuONeRootEvent()
{
}
