//
//  Shoot.cpp
//  IntervalometerV2
//
//  Created by Jason Jorgenson on 17/06/2015.
//  Copyright (c) 2015 Jason Jorgenson. All rights reserved.
//

#include "Shoot.h"

#define SINGLESHOT 1
#define MULTISHOT  2

// Constructor
Shoot::Shoot()
{
  mShootingMode = 1;
}

// Destructor
Shoot::~Shoot()
{
  
}

// Copy Constructor
Shoot::Shoot(const Shoot& theShoot)
{
  mGlobalTakesCount = theShoot.GlobalTakesCount();
  mGlobalTakesCurrent = theShoot.GlobalTakesCurrent();
  mGlobalTakesInterval = theShoot.GlobalTakesInterval();
  mGlobalTakesLastDesired = theShoot.GlobalTakesLastDesired();
  mMechActive = theShoot.MechActive();
  mMechLastActive = theShoot.MechLastActive();
  mMechCount = theShoot.MechCount();
  mMechInterval = theShoot.MechInterval();
  mMechLastActive = theShoot.MechLastActive();
  mMechCount = theShoot.MechCount();
  mMechInterval = theShoot.MechInterval();
  mMechLastDesired = theShoot.MechLastDesired();
  mMechCurrent = theShoot.MechCurrent();
  mMechMinPoint = theShoot.MechMinPoint();
  mMechMaxPoint = theShoot.MechMaxPoint();
  mMechStepSize = theShoot.MechStepSize();
  mMechCurrentPoint = theShoot.MechCurrentPoint();
  mLocalTakesCount = theShoot.LocalTakesCount();
  mLocalTakesInterval = theShoot.LocalTakesInterval();
  mLocalTakesLastDesired = theShoot.LocalTakesLastDesired();
  mLocalTakesCurrent = theShoot.LocalTakesCurrent();
}


Shoot &Shoot::operator =(const Shoot &theShoot)
{
  if (this == &theShoot) return *this; // gracefully handle self assignment

  
  mGlobalTakesCount = theShoot.GlobalTakesCount();
  mGlobalTakesCurrent = theShoot.GlobalTakesCurrent();
  mGlobalTakesInterval = theShoot.GlobalTakesInterval();
  mGlobalTakesLastDesired = theShoot.GlobalTakesLastDesired();
  mMechActive = theShoot.MechActive();
  mMechLastActive = theShoot.MechLastActive();
  mMechCount = theShoot.MechCount();
  mMechInterval = theShoot.MechInterval();
  mMechLastActive = theShoot.MechLastActive();
  mMechCount = theShoot.MechCount();
  mMechInterval = theShoot.MechInterval();
  mMechLastDesired = theShoot.MechLastDesired();
  mMechCurrent = theShoot.MechCurrent();
  mMechMinPoint = theShoot.MechMinPoint();
  mMechMaxPoint = theShoot.MechMaxPoint();
  mMechStepSize = theShoot.MechStepSize();
  mMechCurrentPoint = theShoot.MechCurrentPoint();
  mLocalTakesCount = theShoot.LocalTakesCount();
  mLocalTakesInterval = theShoot.LocalTakesInterval();
  mLocalTakesLastDesired = theShoot.LocalTakesLastDesired();
  mLocalTakesCurrent = theShoot.LocalTakesCurrent();
}

// getters

int Shoot::ShootingMode() const
{
  return mShootingMode;
}

int Shoot::GlobalTakesCount() const
{
  return mGlobalTakesCount;
}

unsigned long Shoot::GlobalTakesInterval() const
{
  return mGlobalTakesInterval;
}

unsigned long Shoot::GlobalTakesLastDesired() const
{
  return mGlobalTakesLastDesired;
}

int Shoot::GlobalTakesCurrent() const
{
  return mGlobalTakesCurrent;
}

bool Shoot::MechActive() const
{
  return mMechActive;
}

unsigned long Shoot::MechLastActive() const
{
  return mMechLastActive;
}

int Shoot::MechCount() const
{
  return mMechCount;
}

unsigned long Shoot::MechInterval() const
{
  return mMechInterval;
}

unsigned long Shoot::MechLastDesired() const
{
  return mMechLastDesired;
}

int Shoot::MechCurrent() const
{
  return mMechCurrent;
}

long Shoot::MechMinPoint() const
{
  return mMechMinPoint;
}

long Shoot::MechMaxPoint() const
{
  return mMechMaxPoint;
}

long Shoot::MechStepSize() const
{
  return mMechStepSize;
}

long Shoot::MechCurrentPoint() const
{
  return (mMechCurrent * mMechStepSize) + mMechMinPoint;
}

int Shoot::LocalTakesCount() const
{
  return mLocalTakesCount;
}

unsigned long Shoot::LocalTakesInterval() const
{
  return mLocalTakesInterval;
}

unsigned long Shoot::LocalTakesLastDesired() const
{
  return mLocalTakesLastDesired;
}

int Shoot::LocalTakesCurrent() const
{
  return mLocalTakesCurrent;
}

void Shoot::IncrementLocalTakesCurrent()
{
  mLocalTakesCurrent++;
}

void Shoot::IncrementMechCurrent()
{
  mMechCurrent++;
}

void Shoot::IncrementGlobalTakesCurrent()
{
  mGlobalTakesCurrent++;
}

void Shoot::setShootingMode(int theShootingMode)
{
  mShootingMode = theShootingMode;
}

void Shoot::setGlobalTakesCount(unsigned long theGlobalTakesCount)
{
  mGlobalTakesCount = theGlobalTakesCount;
}

void Shoot::setGlobalTakesInterval(unsigned long theGlobalTakesInterval)
{
  mGlobalTakesInterval = theGlobalTakesInterval;
}

void Shoot::setGlobalTakesLastDesired(unsigned long theGlobalTakesLastDesired)
{
  mGlobalTakesLastDesired = theGlobalTakesLastDesired;
}

void Shoot::setGlobalTakesCurrent(int theGlobalTakesCurrent)
{
  mGlobalTakesCurrent = theGlobalTakesCurrent;
}

void Shoot::setMechActive(bool theMechActive)
{
  mMechActive = theMechActive;
}

void Shoot::setMechLastActive(unsigned long theMechLastActive)
{
  mMechLastActive = theMechLastActive;
}

void Shoot::setMechCount(int theMechCount)
{
  mMechCount = theMechCount;
}

void Shoot::setMechInterval(unsigned long theMechInterval)
{
  mMechInterval = theMechInterval;
}

void Shoot::setMechLastDesired(unsigned long theMechLastDesired)
{
  mMechLastDesired = theMechLastDesired;
}

void Shoot::setMechCurrent(int theMechCurrent)
{
  mMechCurrent = theMechCurrent;
}

void Shoot::setMechMinPoint(long theMechMinPoint)
{
  mMechMinPoint = theMechMinPoint;
}

void Shoot::setMechMaxPoint(long theMechMaxPoint)
{
  mMechMaxPoint = theMechMaxPoint;
}

void Shoot::setMechStepSize(long theMechStepSize)
{
  mMechStepSize = theMechStepSize;
}

void Shoot::setMechCurrentPoint(long theMechCurrentPoint)
{
  mMechCurrentPoint = theMechCurrentPoint;
}

void Shoot::setLocalTakesCount(int theLocalTakesCount)
{
  mLocalTakesCount = theLocalTakesCount;
}

void Shoot::setLocalTakesInterval(unsigned long theLocalTakesInterval)
{
  mLocalTakesInterval = theLocalTakesInterval;
}

void Shoot::setLocalTakesLastDesired(unsigned long theLocalTakesLastDesired)
{
  mLocalTakesLastDesired = theLocalTakesLastDesired;
}

void Shoot::setLocalTakesCurrent(int theLocalTakesCurrent)
{
  mLocalTakesCurrent = theLocalTakesCurrent;
}
