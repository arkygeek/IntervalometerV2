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
Timer::~Timer()
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
  mGlobalDesired = theTimer.GlobalDesired();
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
  mGlobalDesired = theTimer.GlobalDesired();
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

long Timer::GlobalDesired() const
{
  return mGlobalDesired;
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

void Timer::setFocusHold(unsigned long theFocusHold)
{
  mFocusHold=theFocusHold;
}

void Timer::setShutterHold(unsigned long theShutterHold)
{
  mShutterHold=theShutterHold;
}

void Timer::setButtonHold(unsigned long theButtonHold)
{
  mButtonHold=theButtonHold;
}

void Timer::setLightingLag(unsigned long theLightingLag)
{
  mLightingLag=theLightingLag;
}

void Timer::setShutterLag(unsigned long theShutterLag)
{
  mShutterLag=theShutterLag;
}

void Timer::setInterShootLag(unsigned long theInterShotLag)
{
  mInterShotLag=theInterShotLag;
}

void Timer::setBaseTimeToMarkStartOfShot(unsigned long theBaseTimeToMarkStartOfShot)
{
  mBaseTimeToMarkStartOfShot=theBaseTimeToMarkStartOfShot;
}

void Timer::setBaseTimeToMarkStartOfSequence(unsigned long theBaseTimeToMarkStartOfSequence)
{
  mBaseTimeToMarkStartOfSequence=theBaseTimeToMarkStartOfSequence;
}

void Timer::setUseMechanicalMovement(bool theUseMechanicalMovement)
{
  mUseMechanicalMovement=theUseMechanicalMovement;
}

void Timer::setUseDesired(bool theUseDesired)
{
  mUseDesired=theUseDesired;
}

void Timer::setGlobalDesired(long theGlobalDesired)
{
  mGlobalDesired=theGlobalDesired;
}

void Timer::setDesired(long theDesired)
{
  mDesired=theDesired;
}

void Timer::setShutterExpectedOpenTimeCalculated(long theShutterExpectedOpenTimeCalculated)
{
  mShutterExpectedOpenTimeCalculated=theShutterExpectedOpenTimeCalculated;
}

void Timer::setLastTimeCameraTriggered(unsigned long theLastTimeCameraTriggered)
{
  mLastTimeCameraTriggered=theLastTimeCameraTriggered;
}

void Timer::setTimeToNextEvent(long theTimeToNextEvent)
{
  mTimeToNextEvent=theTimeToNextEvent;
}

void Timer::setNextEventTime(long theNextEventTime)
{
  mNextEventTime=theNextEventTime;
}

void Timer::setTimeToStartFocusMode(long theTimeToStartFocusMode)
{
  mTimeToStartFocusMode=theTimeToStartFocusMode;
}

void Timer::setTimeToSetLightingMode(long theTimeToSetLightingMode)
{
  mTimeToSetLightingMode=theTimeToSetLightingMode;
}

void Timer::setTimeToSendTrigger(long theTimeToSendTrigger)
{
  mTimeToSendTrigger=theTimeToSendTrigger;
}

void Timer::setTimeToStopTrigger(long theTimeToStopTrigger)
{
  mTimeToStopTrigger=theTimeToStopTrigger;
}

void Timer::setTimeToRestart(long theTimeToRestart)
{
  mTimeToRestart=theTimeToRestart;
}

void Timer::setDoneSendTrigger(bool theDoneSendTrigger)
{
  mDoneSendTrigger=theDoneSendTrigger;
}

void Timer::setDoneSetLightingMode(bool theDoneSetLightingMode)
{
  mDoneSetLightingMode=theDoneSetLightingMode;
}

void Timer::setDoneStartFocusMode(bool theDoneStartFocusMode)
{
  mDoneStartFocusMode=theDoneStartFocusMode;
}

void Timer::setDoneStopTrigger(bool theDoneStopTrigger)
{
  mDoneStopTrigger=theDoneStopTrigger;
}
