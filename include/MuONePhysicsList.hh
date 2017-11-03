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

#ifndef MuONePhysicsList_h
#define MuONePhysicsList_h 1

#include "G4VUserPhysicsList.hh"
#include "globals.hh"

class G4Cerenkov;
class G4Scintillation;
class G4OpAbsorption;
class G4OpRayleigh;
class G4OpBoundaryProcess;
class G4OpWLS;

class MuONePhysicsList: public  G4VUserPhysicsList
{
  public:
    MuONePhysicsList();
    virtual ~MuONePhysicsList();

  public:
    // SetCuts()
    virtual void SetCuts();

    // Construct particle and physics
    void ConstructParticle();
    void ConstructProcess();

    //these methods Construct particles
    void ConstructBosons();
    void ConstructLeptons();
    void ConstructMesons();
    void ConstructBaryons();
    void ConstructShortLiveds();

    //these methods Construct physics processes and register them
    void ConstructGeneral();
    void ConstructEM();
    void ConstructOp();

    //for the Messenger
    void SetVerbose(G4int);
    void SetNbOfPhotonsCerenkov(G4int);

  private:
    G4Cerenkov*          _theCerenkovProcess;
    G4Scintillation*     _theScintillationProcess;
    G4OpAbsorption*      _theAbsorptionProcess;
    G4OpRayleigh*        _theRayleighScatteringProcess;
    G4OpBoundaryProcess* _theBoundaryProcess;
    G4OpWLS*             _theWLSProcess;
};


#endif



