/***************************************************************************
 timer.cpp  -  The timer class for IntervalometerV2
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

#include "timer.h"

// Constructor
Timer::Timer()
{
  
}

// Copy Constructor
Timer::Timer(const Timer& theTimer)
{
  mFocusHold = theTimer.FocusHold();
  mShutterHold = theTimer.ShutterHold();
  mButtonHold = theTimer.ButtonHold();
  
  mLightingLag = theTimer.LightingLag();
  mShutterLag = theTimer.ShutterLag();
  mInterShotLag = theTimer.InterShotLag();
  
  mBaseTimeToMarkStartOfShot = theTimer.BaseTimeToMarkStartOfShot();
  mBaseTimeToMarkStartOfSequence = theTimer.BaseTimeToMarkStartOfSequence();
  mUseMechanicalMovement = theTimer.UseMechanicalMovement();
  mUseDesired = theTimer.UseDesired();
  mGlobalgDesired = theTimer.GlobalgDesired();
  mDesired = theTimer.Desired();
  mShutterExpectedOpenTimeCalculated = theTimer.ShutterExpectedOpenTimeCalculated();
  
  mLastTimeCameraTriggered = theTimer.LastTimeCameraTriggered();
  mTimeToNextEvent = theTimer.TimeToNextEvent();
  mNextEventTime = theTimer.NextEventTime();
  mTimeToStartFocusMode = theTimer.TimeToStartFocusMode();
  mTimeToSetLightingMode = theTimer.TimeToSetLightingMode();
  mTimeToSendTrigger = theTimer.TimeToSendTrigger();
  mTimeToStopTrigger = theTimer.TimeToStopTrigger();
  mTimeToRestart = theTimer.TimeToRestart();
  
  mDoneStartFocusMode = theTimer.DoneStartFocusMode();
  mDoneSetLightingMode = theTimer.DoneSetLightingMode();
  mDoneSendTrigger = theTimer.DoneSendTrigger();
  mDoneStopTrigger = theTimer.DoneStopTrigger();
}

Timer& Timer::operator=(const Timer& theTimer)
{
  if (this == &theTimer) return *this; // gracefully handle self assignment
  
  mFocusHold = theTimer.FocusHold();
  mShutterHold = theTimer.ShutterHold();
  mButtonHold = theTimer.ButtonHold();
  
  mLightingLag = theTimer.LightingLag();
  mShutterLag = theTimer.ShutterLag();
  mInterShotLag = theTimer.InterShotLag();
  
  mBaseTimeToMarkStartOfShot = theTimer.BaseTimeToMarkStartOfShot();
  mBaseTimeToMarkStartOfSequence = theTimer.BaseTimeToMarkStartOfSequence();
  mUseMechanicalMovement = theTimer.UseMechanicalMovement();
  mUseDesired = theTimer.UseDesired();
  mGlobalgDesired = theTimer.GlobalgDesired();
  mDesired = theTimer.Desired();
  mShutterExpectedOpenTimeCalculated = theTimer.ShutterExpectedOpenTimeCalculated();
  
  mLastTimeCameraTriggered = theTimer.LastTimeCameraTriggered();
  mTimeToNextEvent = theTimer.TimeToNextEvent();
  mNextEventTime = theTimer.NextEventTime();
  mTimeToStartFocusMode = theTimer.TimeToStartFocusMode();
  mTimeToSetLightingMode = theTimer.TimeToSetLightingMode();
  mTimeToSendTrigger = theTimer.TimeToSendTrigger();
  mTimeToStopTrigger = theTimer.TimeToStopTrigger();
  mTimeToRestart = theTimer.TimeToRestart();
  
  mDoneStartFocusMode = theTimer.DoneStartFocusMode();
  mDoneSetLightingMode = theTimer.DoneSetLightingMode();
  mDoneSendTrigger = theTimer.DoneSendTrigger();
  mDoneStopTrigger = theTimer.DoneStopTrigger();
}

unsigned long Timer::FocusHold() const
{
  return mFocusHold;
}

unsigned long Timer::ShutterHold() const
{
  return mShutterHold;
}

unsigned long Timer::ButtonHold() const
{
  return mButtonHold;
}

unsigned long Timer::LightingLag() const
{
  return mLightingLag;
}

unsigned long Timer::ShutterLag() const
{
  return mShutterLag;
}

unsigned long Timer::InterShotLag() const
{
  return mInterShotLag;
}

unsigned long Timer::BaseTimeToMarkStartOfShot() const
{
  return mBaseTimeToMarkStartOfShot;
}

unsigned long Timer::BaseTimeToMarkStartOfSequence() const
{
  return mBaseTimeToMarkStartOfSequence;
}

boolean Timer::UseMechanicalMovement() const
{
  return mUseMechanicalMovement;
}


boolean Timer::UseDesired() const
{
  return mUseDesired;
}


long Timer::GlobalgDesired() const
{
  return mGlobalgDesired;
}


long Timer::Desired() const
{
  return mDesired;
}


long Timer::ShutterExpectedOpenTimeCalculated() const
{
  return mShutterExpectedOpenTimeCalculated;
}


//unsigned long Timer::Interval;

unsigned long Timer::LastTimeCameraTriggered() const
{
  return mLastTimeCameraTriggered;
}



long Timer::TimeToNextEvent() const
{
  return mTimeToNextEvent;
}


long Timer::NextEventTime() const
{
  return mNextEventTime;
}


long Timer::TimeToStartFocusMode() const
{
  return mTimeToStartFocusMode;
}


long Timer::TimeToSetLightingMode() const
{
  return mTimeToSetLightingMode;
}


long Timer::TimeToSendTrigger() const
{
  return mTimeToSendTrigger;
}


long Timer::TimeToStopTrigger() const
{
  return mTimeToStopTrigger;
}


long Timer::TimeToRestart() const
{
  return mTimeToRestart;
}


boolean Timer::DoneStartFocusMode() const
{
  return mDoneStartFocusMode;
}


boolean Timer::DoneSetLightingMode() const
{
  return mDoneSetLightingMode;
}


boolean Timer::DoneSendTrigger() const
{
  return mDoneSendTrigger;
}


boolean Timer::DoneStopTrigger() const
{
  return mDoneStopTrigger;
}



