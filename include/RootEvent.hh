#ifndef RootEvent_H
#define RootEvent_H

// This class can be used with ROOT's dictionary generation, built-in
// I/O, and RTTI (Run-Time Type Identification).  To enable this
// functionality, this class must be derived from class TObject, and
// all its members must also be derived from TObject.
#include "TObject.h"
#include "TObjArray.h"
// Don't do a forward declaration of TOrdCollection here.  If you do,
// the class dictionary won't compiler properly.
#include "TOrdCollection.h"

#define MAXPMTS 10

#include <vector>
#include <map>
using namespace std;

class RootEvent : public TObject
{
  public:

    RootEvent();
    virtual ~RootEvent();

    Int_t GetRunID() const {
      return m_runNumber;
    }
    Int_t GetEventID() const {
      return m_eventNumber;
    }
    Int_t entries() const {
      return m_interaction;
    }
    Int_t GetNPMTs() const {
      return _npmts;
    }
    Int_t GetNphotons ( Int_t i ) const;
    Double_t GetTime ( Int_t i, Int_t j );
    Double_t GetSortedTime ( Int_t i, Int_t j );
    Double_t GetWLength ( Int_t i, Int_t j );
    Double_t MaxTime();
    Double_t MinTime();
    Double_t MaxTime(Int_t i);
    Double_t MinTime(Int_t i);
//     vector<Float_t>& GetTimes(Int_t i) const { Float_t t = (_times[i])[1]; return t}
    Int_t GetNphotons() const;

//  Int_t count const;
//  Int_t NNeut const { return m_neutral; }
//  Double_t charE const { return m_chargedEnergy; }
//  Double_t neutE const { return m_neutralEnergy; }

    // The following data members are protected, not public, because I
    // inherit a Geant4-ROOT "persistency" class from this one.  It is
    // this persistency class that will set the values of the data
    // memebers.

  protected:
    void SetRunID ( Int_t r ) {
      m_runNumber = r;
    }
    void SetEventID ( Int_t e ) {
      m_eventNumber = e;
    }
    void SetNTraj ( Int_t t ) {
      m_interaction = t;
    }
    void SetNPMTs ( Int_t n ) {
      _npmts = n;
    }
    void SetNphotons (Int_t pmt, Int_t phots ) {
      _nphotons[pmt] = phots ;
    }
    void SetTimes ( Int_t phots, Double_t time ) {
      ( _times[phots] ).push_back ( time );
    }
    void SetWLengths ( Int_t phots, Double_t wlen ) {
      ( _wlengths[phots] ).push_back ( wlen );
    }

//  void runID(Int_t r) { m_runNumber = r; }
//  void eventID(Int_t e) { m_eventNumber = e; }

//  void SetCount;
    void SetNNeut(Int_t n) {m_neutral = n;}
    void SetNChar(Int_t n) {m_charged = n;}
    void SetNCharE(Double_t ce) {m_chargedEnergy = ce;}
    void SetNNeutE(Double_t ne) {m_neutralEnergy = ne;}

  private:
    Int_t m_runNumber;
    Int_t m_eventNumber;
    Int_t m_interaction;
    Int_t _npmts;

//     mapfloats _times;
    std::map< Int_t, vector<Double_t> > _times;
    std::map< Int_t, vector<Double_t> > _wlengths;
    std::map< int, int > _nphotons;
    Int_t m_count;
    Int_t m_neutral;
    Int_t m_charged;
    Double_t m_chargedEnergy;
    Double_t m_neutralEnergy;

  public:
    ClassDef ( RootEvent, 1 )
};

#endif
