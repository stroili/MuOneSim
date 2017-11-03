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
#include "MuONeSD.hh"
#include "MuONeHit.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4ParticleDefinition.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4ios.hh"
#include "G4VProcess.hh"

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
MuONeSD::MuONeSD ( G4String name )
    : G4VSensitiveDetector ( name )
{
  collectionName.insert ( "siliconCollection" );
}

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
MuONeSD::~MuONeSD()
{}

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
void MuONeSD::Initialize ( G4HCofThisEvent* HCE )
{
  _siliconCollection = new MuONeHitsCollection( SensitiveDetectorName, collectionName[0] );
  //A way to keep all the hits of this event in one place if needed
  static G4int HCID = -1;
  if ( HCID < 0 ) {
    HCID = GetCollectionID ( 0 );
  }
  HCE->AddHitsCollection( HCID, _siliconCollection );
}

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
G4bool MuONeSD::ProcessHits( G4Step* aStep, G4TouchableHistory* )
{
  G4double edep = aStep->GetTotalEnergyDeposit();
  if ( edep == 0. ) return false; //No edep so dont count as hit

  G4StepPoint* thePrePoint = aStep->GetPreStepPoint();
  G4TouchableHistory* theTouchable =
    ( G4TouchableHistory* ) ( aStep->GetPreStepPoint()->GetTouchable() );
  G4VPhysicalVolume* thePrePV = theTouchable->GetVolume();

  G4StepPoint* thePostPoint = aStep->GetPostStepPoint();

  //Get the average position of the hit
  G4ThreeVector pos = thePrePoint->GetPosition() + thePostPoint->GetPosition();
  pos /= 2.;

  MuONeHit* siliconHit = new MuONeHit ( thePrePV );

  siliconHit->SetEdep ( edep );
  siliconHit->SetPos ( pos );

  _siliconCollection->insert ( siliconHit );

  return true;
}

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
void MuONeSD::EndOfEvent ( G4HCofThisEvent* )
{
}

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
void MuONeSD::clear()
{
}

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
void MuONeSD::DrawAll()
{
}

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
void MuONeSD::PrintAll()
{
}

