/***************************************************************************
 defaults.h  -  The Defaults for IntervalometerV2
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


#ifndef __IntervalometerV2__defaults__
#define __IntervalometerV2__defaults__

#include <stdio.h>
#include <Arduino.h>

class Defaults {
  
public:
  /** Constructor */
  Defaults();
  /** Destructor */
  ~Defaults();
  /** Copy Constructor */
  Defaults(const Defaults& theDefaults);
  /** Assignment operator */
  Defaults& operator = (const Defaults& theDefaults);
  
  // Accessors
  
  /** pin definitions etc */
  int Ps2DataPin() const;
  int Ps2IRQpin() const;
  
  int ButtonsOnPanel() const;
  int StopButton() const;
  int TurnOffLED() const;
  int PanelLcdLED() const;
  int TriggerOut() const;
  int FocusLED() const;
  int FocusOut() const;
  int LightDiagnosticsPin() const;

  // Mutators
  
  void setPs2DataPin(int thePs2DataPin);
  void setPs2IRQpin(int thePs2IRQpin);
  void setButtonsOnPanel(int theButtonsOnPanel);
  void setStopButton(int theStopButton);
  void setTurnOffLED(int theTurnOffLED);
  void setPanelLcdLED(int thePanelLcdLED);
  void setTriggerOut(int theTriggerOut);
  void setFocusLED(int theFocusLED);
  void setFocusOut(int theFocusLED);
  void setLightDiagnosticsPin(int theLightDiagnosticsPin);
  
private:
  int mPs2DataPin;
  int mPs2IRQpin;
  
  int mButtonsOnPanel;
  int mStopButton;
  int mTurnOffLED;
  int mPanelLcdLED;
  int mTriggerOut;
  int mFocusLED;
  int mFocusOut;
  int mLightDiagnosticsPin;
};

#endif /* defined(__IntervalometerV2__defaults__) */
