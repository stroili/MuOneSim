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
#include "G4VUserEventInformation.hh"
#include "G4ThreeVector.hh"
#include "globals.hh"

#ifndef MuONeUserEventInformation_h
#define MuONeUserEventInformation_h 1

class MuONeUserEventInformation : public G4VUserEventInformation
{
  public:
    MuONeUserEventInformation();
    ~MuONeUserEventInformation();

    inline void Print() const {};

    void IncShifted() {
      _shiftedCount++;
    }
    void IncPhotonCount_Scint() {
      _photonCount_Scint++;
    }
    void IncPhotonCount_Ceren() {
      _photonCount_Ceren++;
    }
    void IncEDep ( G4double dep ) {
      _totE += dep;
    }
    void IncAbsorption() {
      _absorptionCount++;
    }
    void IncBoundaryAbsorption() {
      _boundaryAbsorptionCount++;
    }
    void IncHitCount ( G4int i = 1 ) {
      _hitCount += i;
    }

    void SetEWeightPos ( const G4ThreeVector& p ) {
      _eWeightPos = p;
    }
    void SetReconPos ( const G4ThreeVector& p ) {
      _reconPos = p;
    }
    void SetConvPos ( const G4ThreeVector& p ) {
      _convPos = p;
      _convPosSet = true;
    }
    void SetPosMax ( const G4ThreeVector& p, G4double edep ) {
      _posMax = p;
      _edepMax = edep;
    }

    void SetInfos(const G4ThreeVector& muExitMom, const G4ThreeVector& eProdMom,
                  const G4ThreeVector& eExitMom, double muE, double eE, int gammas, int others) {
      if (muExitMom.mag2() != 0.) _muonExitMom = muExitMom;
      if (eProdMom.mag2() != 0.) _eProdMom = eProdMom;
      if (eExitMom.mag2() != 0.) _eExitMom = eExitMom;
      if (muE != 0.) _muE = muE;
      if (eE != 0.) _eE = eE;
      _gammas += gammas;
      _others += others;
    }

    void SetMuExitMom(const G4ThreeVector& muExitMom) {
      _muonExitMom = muExitMom;
    }

    void SeteProdMom(const G4ThreeVector& eProdMom) {
      _eProdMom = eProdMom;
    }

    void SeteExitMom(const G4ThreeVector& eExitMom) {
      _eExitMom = eExitMom;
    }

    void SetMuEnergy(double muE) {
      _muE = muE;
    }

    void SeteEnergy(double eE) {
      _eE = eE;
    }

    void seteZProd(double z) {
      _eZprod = z;
    }

    void addGamma() {
      ++_gammas;
    }

    void addOthers() {
      ++_others;
    }

    void addGammaEnergy(double energy) {
      _gammaEnergy += energy;
    }

    void addOthersEnergy(double energy) {
      _othersEnergy += energy;
    }

    void SetSteps(int s) {
      _steps=s;
    }
    
    void GetMuonExitMom(G4ThreeVector& v) {
      v = _muonExitMom;
    }

    void GeteProdMom(G4ThreeVector& v) {
      v = _eProdMom;
    }

    void GeteExitMom(G4ThreeVector& v) {
      v = _eExitMom;
    }

    double GetMuEnergy() {
      return _muE;
    }

    double GeteEnergy() {
      return _eE;
    }

    double GeteZProd() {
      return _eZprod;
    }

    double GetGammaEnergy() {
      return _gammaEnergy;
    }

    double GetOthersEnergy() {
      return _othersEnergy;
    }

    int GetGammas() {
      return _gammas;
    }

    int GetOthers() {
      return _others;
    }

    int GetSteps() {
      return _steps;
    }

    G4int GetPhotonCount_Scint() const {
      return _photonCount_Scint;
    }
    G4int GetPhotonCount_Ceren() const {
      return _photonCount_Ceren;
    }
    G4int GetHitCount() const {
      return _hitCount;
    }
    G4double GetEDep() const {
      return _totE;
    }
    G4int GetAbsorptionCount() const {
      return _absorptionCount;
    }
    G4int GetShiftedCount() const {
      return _shiftedCount;
    }
    G4int GetBoundaryAbsorptionCount() const {
      return _boundaryAbsorptionCount;
    }

    G4ThreeVector GetEWeightPos() {
      return _eWeightPos;
    }
    G4ThreeVector GetReconPos() {
      return _reconPos;
    }
    G4ThreeVector GetConvPos() {
      return _convPos;
    }
    G4ThreeVector GetPosMax() {
      return _posMax;
    }
    G4double GetEDepMax() {
      return _edepMax;
    }
    G4double IsConvPosSet() {
      return _convPosSet;
    }

    //Gets the total photon count produced
    G4int GetPhotonCount() {
      return _photonCount_Scint + _photonCount_Ceren;
    }

    void IncPMTSAboveThreshold() {
      _pmtsAboveThreshold++;
    }
    G4int GetPMTSAboveThreshold() {
      return _pmtsAboveThreshold;
    }

  private:

    G4int _hitCount;
    G4int _photonCount_Scint;
    G4int _photonCount_Ceren;
    G4int _absorptionCount;
    G4int _shiftedCount;
    G4int _boundaryAbsorptionCount;

    G4ThreeVector _muonExitMom;
    G4ThreeVector _eProdMom;
    G4ThreeVector _eExitMom;
    G4double  _muE;
    G4double  _eE;
    G4double  _eZprod;
    G4double  _gammaEnergy;
    G4double  _othersEnergy;
    G4int     _gammas;
    G4int     _others;
    G4int     _steps;

    G4double _totE;

    //These only have meaning if totE > 0
    //If totE = 0 then these wont be set by EndOfEventAction
    G4ThreeVector _eWeightPos;
    G4ThreeVector _reconPos; //Also relies on hitCount>0
    G4ThreeVector _convPos;//true (initial) converstion position
    G4bool        _convPosSet;
    G4ThreeVector _posMax;
    G4double      _edepMax;

    G4int         _pmtsAboveThreshold;

};

#endif
