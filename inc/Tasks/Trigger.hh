// Copyright 2016.  Los Alamos National Security, LLC.
// This file is part of UCNB_Analyzer.
// This program is distributed under the terms of the GNU General Public License, version 2.0.  See LICENSE.md included in top directory of this distribution.

// File: Trigger.hh
// Purpose: Simple trigger object

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
