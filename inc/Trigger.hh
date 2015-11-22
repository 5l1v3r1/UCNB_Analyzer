// File: Trigger.hh
// Name: Leah Broussard
// Date: 2015/6/25
// Purpose: Simple trigger object
//
// Revision History:
// 2015/6/25:  LJB  Created

#ifndef TRIGGER_HH__
#define TRIGGER_HH__

struct trigger_t{
  Double_t E;
  Double_t T;
  Double_t Shaping;
  Double_t Integration;
  Double_t Chi2;
  Double_t TrapE;
  Double_t AveTrapE;
  Double_t MidTrapE;
  Double_t TrapT;
  Double_t Flat0;
  Double_t Flat1;
  Int_t ch;
  Int_t up;
  Int_t down;
};

#endif // TRIGGER_HH__
