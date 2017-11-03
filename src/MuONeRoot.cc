#include "MuONeRoot.hh"

#include <sstream>
#include <string>
#include <vector>
using namespace std;

#include <boost/tokenizer.hpp>

#include "Rtypes.h"
#include "TROOT.h"
#include "TFile.h"
// #include "TXNetFile.h"
#include "TTree.h"

#include "RecorderBase.hh"

#include "MuONeRootEvent.hh"
#include "MuONeRootStep.hh"
#include "MuONeRootMessenger.hh"
#include "MuONeUserEventInformation.hh"

#include "globals.hh"
#include "G4Run.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4ThreeVector.hh"

MuONeRoot::MuONeRoot() : _fname ( "test.root" ), _muonThetaExit(0.), _muonPhiExit(0.),
      _eThetaProd(0.), _ePhiProd(0.), _eThetaExit(0.), _ePhiExit(0.),
      _muE(0.), _eE(0.), _eZprod(0.), _gammaEnergy(0.), _othersEnergy(0.), _gammas(0), _others(0)

{

  _recorderMessenger = new MuONeRootMessenger ( this );
  static TROOT rootBase ( "simple", "Test of ROOT persistent I/O" );

}

MuONeRoot::~MuONeRoot()
{

///  theFile->Write();
  _theFile->Close();
  delete _theFile;
///  delete theTree;

}

void MuONeRoot::RecordBeginOfRun ( const G4Run* aRun )
{
  OpenFile();

  _theTree = new TTree ( "theTree", "Analysis Tree" );

  _theEvent = 0;
//   RootEvent * _theEvent = new RootEvent();
//   _theTree->Branch ( "RootEvent", "RootEvent", &_theEvent, 32000, 1 );
  _theTree->Branch ( "muonThetaExit", &_muonThetaExit, "muonThetaExit/D" );
  _theTree->Branch ( "muonPhiExit", &_muonPhiExit, "muonPhiExit/D" );
  _theTree->Branch ( "eThetaProd", &_eThetaProd, "eThetaProd/D" );
  _theTree->Branch ( "ePhiProd", &_ePhiProd, "ePhiProd/D" );
  _theTree->Branch ( "eThetaExit", &_eThetaExit, "eThetaExit/D" );
  _theTree->Branch ( "ePhiExit", &_ePhiExit, "ePhiExit/D" );
  _theTree->Branch ( "muE", &_muE, "muE/D" );
  _theTree->Branch ( "eE", &_eE, "eE/D" );
  _theTree->Branch ( "eZprod", &_eZprod, "eZprod/D" );
  _theTree->Branch ( "steps", &_steps, "steps/I" );
  _theTree->Branch ( "gammaEnergy", &_gammaEnergy, "gammaEnergy/D" );
  _theTree->Branch ( "othersEnergy", &_othersEnergy, "othersEnergy/D" );
  _theTree->Branch ( "gammas", &_gammas, "gammas/I" );
  _theTree->Branch ( "others", &_others, "others/I" );


  //  theCloneStep = 0;
//  theCloneStep = new TClonesArray("MyStep");
//  theTree->Branch("MyStep", &theCloneStep, 32000, 1);

  _theStep = 0;
//   _theTree->Branch("RootStep", "RootStep", &_theStep, 32000, 1);

//  theTree->Branch("Count", &m_Counter, "Count/I");
  //  theCloneEvent = new TClonesArray("theEvent");
//  theTree->Branch("MuONeRootEvent", &theEvent, 32000, 2);

}

void MuONeRoot::RecordEndOfRun ( const G4Run* aRun )
{
  _theFile->Write();
  delete _theTree;
}

void MuONeRoot::RecordBeginOfEvent ( const G4Event* aEvent )
{
  _muonThetaExit=0.;
  _muonPhiExit=0.;
  _eThetaProd=0.;
  _ePhiProd=0.;
  _eThetaExit=0.;
  _ePhiExit=0.;
  _muE=0.;
  _eE=0.;
  _eZprod=0.;
  _gammaEnergy=0.;
  _othersEnergy=0.;
  _gammas=0;
  _others=0;
  _steps=0;
}

void MuONeRoot::RecordEndOfEvent ( const G4Event* aEvent )
{

  MuONeUserEventInformation* eventInformation
  = ( MuONeUserEventInformation* ) G4EventManager::GetEventManager()
  ->GetConstCurrentEvent()->GetUserInformation();

  G4ThreeVector v;
  eventInformation->GetMuonExitMom(v);
  _muonThetaExit=v.theta();
  _muonPhiExit=v.phi();
  eventInformation->GeteProdMom(v);
  _eThetaProd=v.theta();
  _ePhiProd=v.phi();
  eventInformation->GeteExitMom(v);
  _eThetaExit=v.theta();
  _ePhiExit=v.phi();
  _muE=eventInformation->GetMuEnergy()/CLHEP::GeV;
  _eE=eventInformation->GeteEnergy()/CLHEP::GeV;
  _eZprod=eventInformation->GeteZProd();
  _gammaEnergy=eventInformation->GetGammaEnergy()/CLHEP::GeV;
  _othersEnergy=eventInformation->GetOthersEnergy()/CLHEP::GeV;
  _gammas=eventInformation->GetGammas();
  _others=eventInformation->GetOthers();
  _steps=eventInformation->GetSteps();

  //void MuONeRoot::Fill();
  //
//   _theEvent = new MuONeRootEvent ( aEvent );
//   _theTree->Write();
  _theTree->Fill();
  delete _theEvent;
}

void MuONeRoot::RecordTrack ( const G4Track* aTrack )
{
}

void MuONeRoot::RecordStep ( const G4Step* aStep )
{
  _theStep = new MuONeRootStep ( aStep );
//  theCloneStep = new MuONeRootStep(aStep);
//  theStep->Write();
//  theStep->Fill();
  delete _theStep;
}

void MuONeRoot::SetFileName ( const G4String filename )
{
  if ( _theFile ) {
    _theFile->Close();
    delete _theFile;
    _theFile = 0;
  }
  _fname = filename;
  OpenFile();
}

void MuONeRoot::OpenFile()
{
//  if ( _theFile ) {
//    _theFile->Close();
//    delete _theFile;
//  }
  bool xrootd = false;
  if ( _fname.find ( "root", 0 ) == 0 ) {
    xrootd = true;
  }
  if ( !_theFile ) {
    //_theFile = new TXNetFile ( _fname, "recreate" );
    if ( xrootd ) {
//       _theFile = new TXNetFile ( _fname );
//       if ( _theFile->IsOpen() ) {
//         _theFile->Close();
//         delete _theFile;
//         _theFile = 0;
//         RenameFile();
//       } else {
//         _theFile->Close();
//         delete _theFile;
//         _theFile = new TXNetFile ( _fname, "recreate" );
//       }
    } else {
      _theFile = new TFile ( _fname, "recreate" );
    }
  } else if ( !_theFile->IsOpen() ) {
    _theFile->ReOpen ( "UPDATE" );
  }
}

void MuONeRoot::RenameFile()
{
  string fname ( _fname );
  int where = fname.rfind ( "-", fname.size() );
  if ( where > 0 ) {
    typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
    boost::char_separator<char> sep ( ".-" );
    tokenizer tokens ( fname, sep );
    vector<string> svec;
    for ( tokenizer::iterator tok_iter = tokens.begin();
          tok_iter != tokens.end(); ++tok_iter ) {
      svec.push_back ( string ( *tok_iter ) );
    }
    istringstream is ( svec[svec.size()-2] );
    int n = 0;
    is >> n;
    if ( is.fail() ) {
      where = fname.find ( ".", 0 );
      ostringstream n;
      n << 1;
      fname.insert ( where, "-" );
      fname.insert ( where + 1, n.str().c_str() );
      _fname = fname;
      OpenFile();
    } else {
      ostringstream os;
      os << n + 1;
      fname.replace ( where + 1, is.str().size(), os.str().c_str(), os.str().size() );
      _fname = fname;
      OpenFile();
    }
  } else {
    where = fname.find ( ".", 0 );
    ostringstream n;
    n << 1;
    fname.insert ( where, "-" );
    fname.insert ( where + 1, n.str().c_str() );
    _fname = fname;
    OpenFile();
  }
}

