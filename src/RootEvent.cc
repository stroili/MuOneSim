#include "RootEvent.hh"

#include "TOrdCollection.h"
#include "TString.h"

#include <iostream>
#include <algorithm>

ClassImp ( RootEvent )

RootEvent::RootEvent()
{
  // Class constructor.  Creates an empty event.
  m_runNumber = -1;
  m_eventNumber = -1;
  m_interaction = -1;
//  m_count = -1;
//  m_neutral = -1;
//  m_chargedEnergy = -1.0;
//  m_neutralEnergy = -1.0;

}

RootEvent::~RootEvent()
{
}

Double_t RootEvent::MaxTime()
{
  Double_t maxt = 0.;
  for ( Int_t i = 0; i < _npmts; i++ ) {
    if ( _times[i][GetNphotons ( i )-1] > maxt ) maxt = _times[i][GetNphotons ( i )-1];
  }
  return maxt;
}

Double_t RootEvent::MinTime()
{
  Double_t mint = 100000.;
  for ( Int_t i = 0; i < _npmts; i++ ) {
    if ( _times[i][0] < mint ) mint = _times[i][0];
  }
  return mint;
}

Double_t RootEvent::MinTime ( Int_t i )
{
  if ( i < _times.size() ) {
    std::vector<Double_t> v = _times[i];
    return *min_element ( v.begin(), v.end() );
  } else {
    return 1000.;
  }
}

Double_t RootEvent::MaxTime ( Int_t i )
{
  if ( i < _times.size() ) {
    std::vector<Double_t> v = _times[i];
    return *max_element ( v.begin(), v.end() );
  } else {
    return 1000.;
  }
}

// Int_t RootEvent::GetNphotons() const
// {
//   int num=0;
//   for (int i=0; i<_npmts; i++) {
//     num +=_nphotons[i];
//   }
//   return num;
// }

Double_t RootEvent::GetWLength ( Int_t i, Int_t j )
{
  if ( i < _wlengths.size() && j < ( _wlengths[i] ).size() ) {
    return _wlengths[i][j];
  } else {
    cout << i << " " << _wlengths.size() << "\t" << j << " " << ( _wlengths[i] ).size() << endl;
    return 0.;
  }
}

Double_t RootEvent::GetTime ( Int_t i, Int_t j )
{
  if ( i < _times.size() && j < ( _times[i] ).size() ) {
    return _times[i][j];
  } else {
    return 0.;
  }
}

Double_t RootEvent::GetSortedTime ( Int_t i, Int_t j )
{
  if ( i < _times.size() ) {
    std::vector<Double_t> v = _times[i];
    std::sort ( v.begin(), v.end() );
    if ( j < v.size() ) {
      return v[j];
    } else {
      return 0.;
    }
  } else {
    return 0.;
  }
}

Int_t RootEvent::GetNphotons ( Int_t i ) const
{
  std::map< Int_t , std::vector<Double_t> >::const_iterator iter = _times.find ( i );
  if ( iter != _times.end() ) {
    return ( iter->second ).size();
  } else {
    return 0;
  }
}

Int_t RootEvent::GetNphotons() const
{
  int num = 0;
  for ( Int_t i = 0; i < _npmts; i++ ) {
    num += GetNphotons ( i );
  }
  return num;
}
