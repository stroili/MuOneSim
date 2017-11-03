#include "RootStep.hh"

#include "TOrdCollection.h"
#include "TString.h"

#include <iostream>

ClassImp ( RootStep )

RootStep::RootStep()
{
  m_stepPDG = 0;
  m_stepKinEn = 0;
  m_stepTotEn = 0;
  m_stepMom = 0;
  m_stepVel = 0;

}

RootStep::~RootStep()
{
}
