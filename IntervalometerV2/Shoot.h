//
//  Shoot.h
//  IntervalometerV2
//
//  Created by Jason Jorgenson on 17/06/2015.
//  Copyright (c) 2015 Jason Jorgenson. All rights reserved.
//

#ifndef __IntervalometerV2__Shoot__
#define __IntervalometerV2__Shoot__

#include <Arduino.h>
#include <string.h>

class Shoot {
  
public:
  /** Constructor */
  Shoot();
  /** Destructor */
  ~Shoot();
  /** Copy Constructor */
  Shoot(const Shoot& theShoot);
  /** Assignment operator */
  Shoot& operator = (const Shoot& theShoot);
  
  // Accessors
  /**
   *
   */
  unsigned long FocusHold() const;
  
  int ShootingMode() const;

  
  // global takes (for timelapse)
  int GlobalTakesCount() const;
  unsigned long GlobalTakesInterval() const;
  unsigned long GlobalTakesLastDesired() const;
  int GlobalTakesCurrent() const;
  
  // Motor control vars
  bool MechActive() const;
  unsigned long MechLastActive() const;
  int MechCount() const;
  unsigned long MechInterval() const;
  unsigned long MechLastDesired() const;
  int MechCurrent() const;
  
  long MechMinPoint() const; //stored in icrons
  long MechMaxPoint() const;
  long MechStepSize() const;
  long MechCurrentPoint() const;
  
  //local takes (for multishot exposure bracketing etc)
  int LocalTakesCount() const;
  unsigned long LocalTakesInterval() const;
  unsigned long LocalTakesLastDesired() const;
  int LocalTakesCurrent() const;
  // Mutators
  void IncrementLocalTakesCurrent ();
  void IncrementMechCurrent ();
  void IncrementGlobalTakesCurrent ();
  
  
  void AdvanceShoot ();
  void UpdateGlobalDesired ();
  
  void setShootingMode (int theShootingMode);
  void setGlobalTakesCount (unsigned long theGlobalTakesCount);
  void setGlobalTakesInterval (unsigned long theGlobalTakesInterval);
  void setGlobalTakesLastDesired (unsigned long theGlobalTakesLastDesired);
  void setGlobalTakesCurrent (int theGlobalTakesCurrent);
  
  // motor control vars
  void setMechActive (bool theShootMechActive);
  void setMechLastActive (unsigned long theShootMechLastActive);
  void setMechCount (int theShootMechCount);
  void setMechInterval (unsigned long theShootMechInterval);
  void setMechLastDesired (unsigned long theShootMechLastDesired);
  void setMechCurrent (int theShootMechCurrent);
  void setMechMinPoint (long theShootMechMinPoint); //stored in microns
  void setMechMaxPoint (long theShootMechMaxPoint);
  void setMechStepSize (long theShootMechStepSize);
  void setMechCurrentPoint (long theShootMechCurrentPoint);

  //local takes (for Shoot exposure bracketing etc)
  void setLocalTakesCount (int theShootLocalTakesCount);
  void setLocalTakesInterval (unsigned long theShootLocalTakesInterval);
  void setLocalTakesLastDesired (unsigned long theShootLocalTakesLastDesired);
  void setLocalTakesCurrent (int theShootLocalTakesCurrent);
  
private:
  int mShootingMode;
  
  // global takes (for timelapse)
  int mGlobalTakesCount;
  unsigned long mGlobalTakesInterval;
  unsigned long mGlobalTakesLastDesired;
  int mGlobalTakesCurrent;
  
  // motor control vars
  bool mMechActive;
  unsigned long mMechLastActive;
  int mMechCount;
  unsigned long mMechInterval;
  unsigned long mMechLastDesired;
  int mMechCurrent;
  
  long mMechMinPoint; //stored in microns
  long mMechMaxPoint;
  long mMechStepSize;
  long mMechCurrentPoint;
  
  //local takes (for Shoot exposure bracketing etc)
  int mLocalTakesCount;
  unsigned long mLocalTakesInterval;
  unsigned long mLocalTakesLastDesired;
  int mLocalTakesCurrent;
};

  
#endif /* defined(__IntervalometerV2__Shoot__) */
