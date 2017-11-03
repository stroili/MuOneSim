#ifndef RootStep_H
#define RootStep_H

// This class can be used with ROOT's dictionary generation, built-in
// I/O, and RTTI (Run-Time Type Identification).  To enable this
// functionality, this class must be derived from class TObject, and
// all its members must also be derived from TObject.
#include "TObject.h"
// Don't do a forward declaration of TOrdCollection here.  If you do,
// the class dictionary won't compiler properly.
#include "TOrdCollection.h"

class RootStep : public TObject
{
  public:

    RootStep();
    virtual ~RootStep();

    Int_t GetPDGEncoding() const {
      return m_stepPDG;
    }
    Double_t GetKineticEnergy() const {
      return m_stepKinEn;
    }
    Double_t GetTotalEnergy() const {
      return m_stepTotEn;
    }
    Double_t GetMomentum() const {
      return m_stepMom;
    }
    Double_t GetVelocity() const {
      return m_stepVel;
    }

    // The following data members are protected, not public, because I
    // inherit a Geant4-ROOT "persistency" class from this one.  It is
    // this persistency class that will set the values of the data
    // memebers.

  protected:
    void SetPDGEncoding ( Int_t p ) {
      m_stepPDG = p;
    }
    void SetKineticEnergy ( Double_t k ) {
      m_stepKinEn = k;
    }
    void SetTotalEnergy ( Double_t t ) {
      m_stepTotEn = t;
    }
    void SetMomentum ( Double_t m ) {
      m_stepMom = m;
    }
    void SetVelocity ( Double_t v ) {
      m_stepVel = v;
    }

  private:
    Int_t m_stepPDG;
    Double_t m_stepKinEn;
    Double_t m_stepTotEn;
    Double_t m_stepMom;
    Double_t m_stepVel;

  public:
    ClassDef ( RootStep, 1 )
};

#endif
