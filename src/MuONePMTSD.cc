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
#include "MuONePMTSD.hh"
#include "MuONePMTHit.hh"
#include "MuONeDetectorConstruction.hh"
#include "MuONeUserTrackInformation.hh"

#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4ParticleDefinition.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4ios.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleDefinition.hh"

const double convert = 1.23984e-6;

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
MuONePMTSD::MuONePMTSD ( G4String name )
    : G4VSensitiveDetector ( name ), _pmtHitCollection ( 0 ), _pmtPositionsX ( 0 ),
    _pmtPositionsY ( 0 ), _pmtPositionsZ ( 0 )
{
  collectionName.insert ( "pmtHitCollection" );
}

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
MuONePMTSD::~MuONePMTSD()
{}

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
void MuONePMTSD::Initialize ( G4HCofThisEvent* HCE )
{
  _pmtHitCollection = new MuONePMTHitsCollection
  ( SensitiveDetectorName, collectionName[0] );
  //Store collection with event and keep ID
  static G4int HCID = -1;
  if ( HCID < 0 ) {
    HCID = GetCollectionID ( 0 );
  }
  HCE->AddHitsCollection ( HCID, _pmtHitCollection );
}

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
G4bool MuONePMTSD::ProcessHits ( G4Step* aStep, G4TouchableHistory* ROhist )
{
  return ProcessHits_constStep ( aStep, ROhist );
}

//Generates a hit and uses the postStepPoint's mother volume replica number
//PostStepPoint because the hit is generated manually when the photon is
//absorbed by the photocathode
//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
G4bool MuONePMTSD::ProcessHits_constStep ( const G4Step* aStep,
    G4TouchableHistory* )
{
  //need to know if this is an optical photon
  if ( aStep->GetTrack()->GetDefinition()
       != G4OpticalPhoton::OpticalPhotonDefinition() ) return false;

  //User replica number 1 since photocathode is a daughter volume
  //to the pmt which was replicated
  G4int pmtNumber =
    aStep->GetPostStepPoint()->GetTouchable()->GetReplicaNumber ( 1 );
  G4VPhysicalVolume* physVol =
    aStep->GetPostStepPoint()->GetTouchable()->GetVolume ( 1 );

  G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
  G4double hitTime = preStepPoint->GetGlobalTime();

  //Find the correct hit collection
  G4int n = _pmtHitCollection->entries();
  MuONePMTHit* hit = NULL;
  for ( G4int i = 0;i < n;i++ ) {
    if ( ( *_pmtHitCollection ) [i]->GetPMTNumber() == pmtNumber ) {
      hit = ( *_pmtHitCollection ) [i];
      break;
    }
  }

  if ( hit == NULL ) { //this pmt wasnt previously hit in this event
    hit = new MuONePMTHit(); //so create new hit
    hit->SetPMTNumber ( pmtNumber );
    hit->SetPMTPhysVol ( physVol );
    _pmtHitCollection->insert ( hit );
    hit->SetPMTPos ( ( *_pmtPositionsX ) [pmtNumber], ( *_pmtPositionsY ) [pmtNumber],
                     ( *_pmtPositionsZ ) [pmtNumber] );
  }
  G4double lambda = CLHEP::c_light * CLHEP::h_Planck /
            ( aStep->GetTrack()->GetDynamicParticle()->GetTotalEnergy() ) / CLHEP::nm;
  hit->IncPhotonCount(); //increment hit for the selected pmt
  hit->AddTime ( hitTime );
  hit->AddWLength ( lambda );

  //   if(!MuONeDetectorConstruction::GetSphereOn()){
  hit->SetDrawit ( true );
  //If the sphere is disabled then this hit is automaticaly drawn
//   }
//   else{//sphere enabled
//     MuONeUserTrackInformation* trackInfo=
//         (MuONeUserTrackInformation*)aStep->GetTrack()->GetUserInformation();
//     if(trackInfo->GetTrackStatus()&hitSphere)
//       //only draw this hit if the photon has hit the sphere first
//       hit->SetDrawit(true);
//   }


  return true;
}

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
void MuONePMTSD::EndOfEvent ( G4HCofThisEvent* )
{
}

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
void MuONePMTSD::clear()
{
}

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
void MuONePMTSD::DrawAll()
{
}

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
void MuONePMTSD::PrintAll()
{
}

