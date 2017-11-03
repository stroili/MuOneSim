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
#include "MuONePhysicsList.hh"

#include "globals.hh"
#include <iomanip>

#include "G4BosonConstructor.hh"
#include "G4OpticalPhoton.hh"
#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4ShortLivedConstructor.hh"

#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"

#include "G4Material.hh"
#include "G4MaterialTable.hh"

#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"

#include "G4Cerenkov.hh"
#include "G4Scintillation.hh"
#include "G4OpAbsorption.hh"
#include "G4OpRayleigh.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4OpWLS.hh"

#include "G4LossTableManager.hh"
#include "G4EmSaturation.hh"

MuONePhysicsList::MuONePhysicsList() :   G4VUserPhysicsList()
{
  // default cut value  (1.0mm)
  // defaultCutValue = 1.0 * mm;
  _theCerenkovProcess           = 0;
  _theScintillationProcess      = 0;
  _theAbsorptionProcess         = 0;
  _theRayleighScatteringProcess = 0;
  _theBoundaryProcess           = 0;
  _theWLSProcess                = 0;

//   pMessenger = new MuONePhysicsListMessenger(this);
  SetVerboseLevel(0);
}

MuONePhysicsList::~MuONePhysicsList()
{
}

void MuONePhysicsList::ConstructParticle()
{
  // In this method, static member functions should be called
  // for all particles which you want to use.
  // This ensures that objects of these particle types will be
  // created in the program.

  ConstructBosons();
  ConstructLeptons();
  ConstructMesons();
  ConstructBaryons();
//  ConstructShortLiveds(); FIXME:it doesn't work.....
}

void MuONePhysicsList::ConstructBosons()
{
  // Construct all bosons
  G4BosonConstructor pConstructor;
  pConstructor.ConstructParticle();

  // pseudo-particles
  G4Geantino::GeantinoDefinition();
  G4ChargedGeantino::ChargedGeantinoDefinition();

  // gamma
  G4Gamma::GammaDefinition();

  // optical photon
//   G4OpticalPhoton::OpticalPhotonDefinition();
}

void MuONePhysicsList::ConstructLeptons()
{
  // Construct all leptons
  G4LeptonConstructor pConstructor;
  pConstructor.ConstructParticle();
  // leptons
/*  G4Electron::ElectronDefinition();
  G4Positron::PositronDefinition();
  G4NeutrinoE::NeutrinoEDefinition();
  G4AntiNeutrinoE::AntiNeutrinoEDefinition();
  G4MuonPlus::MuonPlusDefinition();
  G4MuonMinus::MuonMinusDefinition();
  G4NeutrinoMu::NeutrinoMuDefinition();
  G4AntiNeutrinoMu::AntiNeutrinoMuDefinition();*/
}

void MuONePhysicsList::ConstructMesons()
{
 //  mesons
  G4MesonConstructor pConstructor;
  pConstructor.ConstructParticle();
}

void MuONePhysicsList::ConstructBaryons()
{
  //  Construct all barions
  G4BaryonConstructor pConstructor;
  pConstructor.ConstructParticle();

  //  barions
/*  G4Proton::ProtonDefinition();
  G4AntiProton::AntiProtonDefinition();
  G4Neutron::NeutronDefinition();
  G4AntiNeutron::AntiNeutronDefinition();*/
}

void MuONePhysicsList::ConstructShortLiveds()
{
  //  Construct  resonaces and quarks
  G4ShortLivedConstructor pConstructor;
  pConstructor.ConstructParticle();
}


void MuONePhysicsList::ConstructProcess()
{
  AddTransportation();
  ConstructGeneral();
  ConstructEM();
  ConstructOp();
}

#include "G4Decay.hh"

void MuONePhysicsList::ConstructGeneral()
{
  // Add Decay Process
  G4Decay* theDecayProcess = new G4Decay();
  auto theParticleIterator = GetParticleIterator();
  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    if (theDecayProcess->IsApplicable(*particle)) {
      pmanager ->AddProcess(theDecayProcess);
      // set ordering for PostStepDoIt and AtRestDoIt
      pmanager ->SetProcessOrdering(theDecayProcess, idxPostStep);
      pmanager ->SetProcessOrdering(theDecayProcess, idxAtRest);
    }
  }
}

#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"

#include "G4eMultipleScattering.hh"
#include "G4MuMultipleScattering.hh"

#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"

#include "G4MuIonisation.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"

#include "G4hIonisation.hh"

void MuONePhysicsList::ConstructEM()
{
  auto theParticleIterator = GetParticleIterator();
  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();

    if (particleName == "gamma") {
    // gamma
      // Construct processes for gamma
//      pmanager->AddDiscreteProcess(new G4GammaConversion());
//      pmanager->AddDiscreteProcess(new G4ComptonScattering());
//      pmanager->AddDiscreteProcess(new G4PhotoElectricEffect());

    } else if (particleName == "e-") {
    //electron
      // Construct processes for electron
      pmanager->AddProcess(new G4eMultipleScattering(), -1, 1, 1);
      pmanager->AddProcess(new G4eIonisation(),         -1, 2, 2);
      pmanager->AddProcess(new G4eBremsstrahlung(),     -1, 3, 3);

    } else if (particleName == "e+") {
    //positron
      // Construct processes for positron
      pmanager->AddProcess(new G4eMultipleScattering(), -1, 1, 1);
      pmanager->AddProcess(new G4eIonisation(),         -1, 2, 2);
      pmanager->AddProcess(new G4eBremsstrahlung(),     -1, 3, 3);
      pmanager->AddProcess(new G4eplusAnnihilation(),    0,-1, 4);

    } else if( particleName == "mu+" ||
               particleName == "mu-"    ) {
    //muon
     // Construct processes for muon
                 pmanager->AddProcess(new G4MuMultipleScattering(), -1, 1, 1);
                 pmanager->AddProcess(new G4MuIonisation(),         -1, 2, 2);
                 pmanager->AddProcess(new G4MuBremsstrahlung(),     -1, 3, 3);
                 pmanager->AddProcess(new G4MuPairProduction(),     -1, 4, 4);

    } else {
      if ((particle->GetPDGCharge() != 0.0) &&
                      (particle->GetParticleName() != "chargedgeantino")) {
     // all others charged particles except geantino
//         pmanager->AddProcess(new G4hMultipleScattering(), -1,1,1);
        pmanager->AddProcess(new G4hIonisation(),         -1,2,2);
      }
    }
  }
}

void MuONePhysicsList::ConstructOp()
{
  _theCerenkovProcess           = new G4Cerenkov("Cerenkov");
  _theScintillationProcess = new G4Scintillation("Scintillation");
  _theAbsorptionProcess     = new G4OpAbsorption();
  _theRayleighScatteringProcess = new G4OpRayleigh();
  _theBoundaryProcess  = new G4OpBoundaryProcess();
  _theWLSProcess = new G4OpWLS();
  _theWLSProcess->UseTimeProfile ( "delta" );
//  theCerenkovProcess->DumpPhysicsTable();
//  theScintillationProcess->DumpPhysicsTable();
//  theAbsorptionProcess->DumpPhysicsTable();
//  theRayleighScatteringProcess->DumpPhysicsTable();

  SetVerbose(0);

  _theCerenkovProcess->SetMaxNumPhotonsPerStep(20);
  _theCerenkovProcess->SetMaxBetaChangePerStep(10.0);
  _theCerenkovProcess->SetTrackSecondariesFirst(true);

  _theScintillationProcess->SetScintillationYieldFactor(1.);
  _theScintillationProcess->SetTrackSecondariesFirst(true);

  // Use Birks Correction in the Scintillation process

  G4EmSaturation* emSaturation = G4LossTableManager::Instance()->EmSaturation();
  _theScintillationProcess->AddSaturation(emSaturation);

  G4OpticalSurfaceModel themodel = unified;
// // //   _theBoundaryProcess->SetModel(themodel);

  auto theParticleIterator = GetParticleIterator();
  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();
    if (_theCerenkovProcess->IsApplicable(*particle)) {
      pmanager->AddProcess(_theCerenkovProcess);
      pmanager->SetProcessOrdering(_theCerenkovProcess,idxPostStep);
    }
    if (_theScintillationProcess->IsApplicable(*particle)) {
      pmanager->AddProcess(_theScintillationProcess);
      pmanager->SetProcessOrderingToLast(_theScintillationProcess, idxAtRest);
      pmanager->SetProcessOrderingToLast(_theScintillationProcess, idxPostStep);
    }
    if (particleName == "opticalphoton") {
      G4cout << " AddDiscreteProcess to OpticalPhoton " << G4endl;
      pmanager->AddDiscreteProcess(_theAbsorptionProcess);
      pmanager->AddDiscreteProcess(_theRayleighScatteringProcess);
      pmanager->AddDiscreteProcess(_theBoundaryProcess);
      pmanager->AddDiscreteProcess(_theWLSProcess);
    }
  }
}

void MuONePhysicsList::SetVerbose(G4int verbose)
{
  _theCerenkovProcess->SetVerboseLevel(verbose);
  _theScintillationProcess->SetVerboseLevel(verbose);
  _theAbsorptionProcess->SetVerboseLevel(verbose);
  _theRayleighScatteringProcess->SetVerboseLevel(verbose);
  _theBoundaryProcess->SetVerboseLevel(verbose);
  _theWLSProcess->SetVerboseLevel(verbose);
}

void MuONePhysicsList::SetNbOfPhotonsCerenkov(G4int MaxNumber)
{
  _theCerenkovProcess->SetMaxNumPhotonsPerStep(MaxNumber);
}

void MuONePhysicsList::SetCuts()
{
  //  " G4VUserPhysicsList::SetCutsWithDefault" method sets
  //   the default cut value for all particle types
  //
  SetCutsWithDefault();

  if (verboseLevel>0) DumpCutValuesTable();
}
