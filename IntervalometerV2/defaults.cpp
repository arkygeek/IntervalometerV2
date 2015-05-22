/***************************************************************************
 defaults.cpp  -  The defaults for IntervalometerV2
 ---------------------------------------------------------------------------
 begin                : May 2015
 copyright            : (C) 2015 by Jason Jorgenson.  All rights reserved.
 email                : jjorgenson@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include "defaults.h"

// Constructor
Defaults::Defaults()
{
  /** pin definitions etc */
  mPs2DataPin = 17;
  mPs2IRQpin =  3;
  
  mButtonsOnPanel = A1; // analog multi button panel
  mStopButton = A2; // stop button connected to analog pin (not a long term solution)
  mTurnOffLED = 10; //ambient light, Active low
  mPanelLcdLED = 9; // panel LED, active Low
  mTriggerOut = 7; // active high
  mFocusLED = 5; // pwm active low
  mFocusOut = 4; // active high
  mLightDiagnosticsPin = 13; // diagnostics LED -- PWM, active low??.
}
Defaults::~Defaults()
{
  
}

// Copy Constructor
Defaults::Defaults(const Defaults& theDefaults)
{
  mPs2DataPin = theDefaults.Ps2DataPin();
  mPs2IRQpin =  theDefaults.Ps2IRQpin();
  mButtonsOnPanel = theDefaults.ButtonsOnPanel();
  mStopButton = theDefaults.StopButton();
  mTurnOffLED = theDefaults.TurnOffLED();
  mPanelLcdLED = theDefaults.PanelLcdLED();
  mTriggerOut = theDefaults.TriggerOut();
  mFocusLED = theDefaults.FocusLED();
  mFocusOut = theDefaults.FocusOut();
  mLightDiagnosticsPin = theDefaults.LightDiagnosticsPin();
}

Defaults& Defaults::operator=(const Defaults& theDefaults)
{
  if (this == &theDefaults) return *this; // gracefully handle self assignment
  
  mPs2DataPin = theDefaults.Ps2DataPin();
  mPs2IRQpin =  theDefaults.Ps2IRQpin();
  mButtonsOnPanel = theDefaults.ButtonsOnPanel();
  mStopButton = theDefaults.StopButton();
  mTurnOffLED = theDefaults.TurnOffLED();
  mPanelLcdLED = theDefaults.PanelLcdLED();
  mTriggerOut = theDefaults.TriggerOut();
  mFocusLED = theDefaults.FocusLED();
  mFocusOut = theDefaults.FocusOut();
  mLightDiagnosticsPin = theDefaults.LightDiagnosticsPin();
}

// getters

int Defaults::Ps2DataPin() const
{
  return mPs2DataPin;
}

int Defaults::Ps2IRQpin() const
{
  return mPs2IRQpin;
}

int Defaults::ButtonsOnPanel() const
{
  return mButtonsOnPanel;
}

int Defaults::StopButton() const
{
  return mStopButton;
}

int Defaults::TurnOffLED() const
{
  return mTurnOffLED;
}

int Defaults::PanelLcdLED() const
{
  return mPanelLcdLED;
}

int Defaults::TriggerOut() const
{
  return mTriggerOut;
}

int Defaults::FocusLED() const
{
  return mFocusLED;
}

int Defaults::FocusOut() const
{
  return mFocusOut;
}

int Defaults::LightDiagnosticsPin() const
{
  return mLightDiagnosticsPin;
}

// setters

void Defaults::setPs2DataPin (int thePs2DataPin)
{
  mPs2DataPin=thePs2DataPin;
}

void Defaults::setPs2IRQpin (int thePs2IRQpin)
{
  mPs2IRQpin=thePs2IRQpin;
}

void Defaults::setButtonsOnPanel (int theButtonsOnPanel)
{
  mButtonsOnPanel=theButtonsOnPanel;
}

void Defaults:: setStopButton (int theStopButton)
{
  mStopButton=theStopButton;
}

void Defaults::setTurnOffLED (int theTurnOffLED)
{
  mTurnOffLED = theTurnOffLED;
}

void Defaults::setPanelLcdLED (int thePanelLcdLED)
{
  mPanelLcdLED=thePanelLcdLED;
}

void Defaults::setTriggerOut (int theTriggerOut)
{
  mTriggerOut=theTriggerOut;
}

void Defaults::setFocusLED(int theFocusLED)
{
  mFocusLED=theFocusLED;
}

void Defaults::setFocusOut(int theFocusOut)
{
  mFocusOut=theFocusOut;
}

void Defaults::setLightDiagnosticsPin(int theLightDiagnosticsPin)
{
  mLightDiagnosticsPin=theLightDiagnosticsPin;
}
