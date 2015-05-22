/***************************************************************************
 timer.h  -  The timer class for IntervalometerV2
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

#ifndef __IntervalometerV2__test__
#define __IntervalometerV2__test__

#include <Arduino.h>
#include <string.h>

class Timer {

public:
  /** Constructor */
  Timer();
  /** Destructor */
  ~Timer();
  /** Copy Constructor */
  Timer(const Timer& theTimer);
  /** Assignment operator */
  Timer& operator = (const Timer& theTimer);
  
  // Accessors
  /** 1+ number of ms to spend focusing before taking pic
    * 0 is off
    * setting */
  unsigned long FocusHold() const;
  
  /** to be replaced later with x-sync detection,
    * time to wait for camera
    * setting */
  unsigned long ShutterHold() const;
  
  /** time to hold trigger 
    * line low 
    * default 5ms */
  unsigned long ButtonHold() const;
  
  /** number of ms before sync to switch lighting
    * default 100ms
    * setting */
  unsigned long LightingLag() const;
  
  /** time between trigger and sync
    * preset */
  unsigned long ShutterLag() const;
  
  /** fastest time between sync times that camera can handle
    * preset */
  unsigned long InterShotLag() const;
  
  /** base time to mark start of shot */
  unsigned long BaseTimeToMarkStartOfShot() const;
  
  /** base time to mark start of sequence */
  unsigned long BaseTimeToMarkStartOfSequence() const;
  
  /** whether mechanical movement is required */
  boolean UseMechanicalMovement() const;

  /** whether to use desired of not */
  boolean UseDesired() const;
  
  /** desired
    * @REF millis,
    * temp store for before mBaseTimeToMarkStartOfShot is known */
  long GlobalDesired() const;

  /** time at which the shutter is desired to go off
    * calculated at start of shot
    * @REF mBaseTimeToMarkStartOfShot */
  long Desired() const;
  
  /** time at which the shutter is expected, to be fully open
    * calculated
    * @REF mBaseTimeToMarkStartOfShot */
  long ShutterExpectedOpenTimeCalculated() const;
  
  /** time delay between shot sync times, 
    * may be 0 for "fast"
    * setting */
  //unsigned long mInterval;
  
  /** the last time the camera triggered 
    * should likely be when the shutter closed??
    * @REF millis */
  unsigned long LastTimeCameraTriggered() const;
  
  
  /** @TODO implement time to next event
    * time to next event
    * indicates how much time functions can use. */
  long TimeToNextEvent() const;
  
  /** time the next event should happen
    * @REF mBaseTimeToMarkStartOfShot */
  long NextEventTime() const;
  
  /** planned time to start focus mode
    * @REF mBaseTimeToMarkStartOfShot */
  long TimeToStartFocusMode() const;
  
  /** planned time to set lighing mode
    * @REF mBaseTimeToMarkStartOfShot */
  long TimeToSetLightingMode() const;
  
  /** planned time to send trigger
    * @REF mBaseTimeToMarkStartOfShot */
  long TimeToSendTrigger() const;
  
  /** planned time to stop trigger
    * @REF mBaseTimeToMarkStartOfShot */
  long TimeToStopTrigger() const;
  
  /** planned time to finish & start next shot
    * @REF mBaseTimeToMarkStartOfShot */
  long TimeToRestart() const;
  
  /** done start focus mode */
  boolean DoneStartFocusMode() const;
  
  /** done set lighing mode */
  boolean DoneSetLightingMode() const;
  
  /** done send trigger */
  boolean DoneSendTrigger() const;
  
  /** done stop trigger */
  boolean DoneStopTrigger() const;
  
  
  // Mutators
  
  /** Set FocusHold
    * @see FocusHold
    */
  void setFocusHold(unsigned long theFocusHold);
  
  void setShutterHold (unsigned long theShutterHold);
  
  void setButtonHold (unsigned long theButtonHold);
  
  void setLightingLag (unsigned long theLightingLag);
  
  void setShutterLag (unsigned long theShutterLag);
  
  void setInterShootLag (unsigned long theInterShotLag);
  
  void setBaseTimeToMarkStartOfShot (unsigned long theBaseTimeToMarkStartOfShot);
  
  void setBaseTimeToMarkStartOfSequence (unsigned long theBaseTimeToMarkStartOfSequence);
  
  void setUseMechanicalMovement (bool theUseMechanicalMovement);
  
  void setUseDesired (bool theUseDesired);
  
  void setGlobalDesired (long theGlobalDesired);
  
  void setDesired (long theDesired);
  
  void setShutterExpectedOpenTimeCalculated (long theShutterExpectedOpenTimeCalculated);
  
  void setLastTimeCameraTriggered (unsigned long theLastTimeCameraTriggered);
  
  void setTimeToNextEvent (long theTimeToNextEvent);
  
  void setNextEventTime (long theNextEventTime);
  
  void setTimeToStartFocusMode (long theTimeToStartFocusMode);
  
  void setTimeToSetLightingMode (long theTimeToSetLightingMode);
  
  void setTimeToSendTrigger (long theTimeToSendTrigger);
  
  void setTimeToStopTrigger (long theTimeToStopTrigger);
  
  void setTimeToRestart (long theTimeToRestart);
  
  void setDoneStartFocusMode (bool theDoneStartFocusMode);
  
  void setDoneSetLightingMode (bool theDoneSetLightingMode);
  
  void setDoneSendTrigger (bool theDoneSendTrigger);
  
  void setDoneStopTrigger (bool theDoneStopTrigger);
  
private:
  
  unsigned long mFocusHold;
  
  unsigned long mShutterHold;
  
  unsigned long mButtonHold;
  
  unsigned long mLightingLag;
  
  unsigned long mShutterLag;
  
  unsigned long mInterShotLag;
  
  unsigned long mBaseTimeToMarkStartOfShot;
  
  unsigned long mBaseTimeToMarkStartOfSequence;
  
  boolean mUseMechanicalMovement;
  
  boolean mUseDesired;
  
  long mGlobalDesired;
  
  long mDesired;
  
  long mShutterExpectedOpenTimeCalculated;
  
  unsigned long mLastTimeCameraTriggered;
  
  long mTimeToNextEvent;
  
  long mNextEventTime;
  
  long mTimeToStartFocusMode;
  
  long mTimeToSetLightingMode;
  
  long mTimeToSendTrigger;
  
  long mTimeToStopTrigger;
  
  long mTimeToRestart;
  
  boolean mDoneStartFocusMode;
  
  boolean mDoneSetLightingMode;
  
  boolean mDoneSendTrigger;
  
  boolean mDoneStopTrigger;
  
};




#endif /* defined(__IntervalometerV2__test__) */
