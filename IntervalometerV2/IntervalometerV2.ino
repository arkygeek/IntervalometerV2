/*
 * Intervalometer sketch.
 * restart of project following radical cutdown of feature set.
 *
 *  Created on: 28 oct 2014
 * Author: jrp
 *

 *-------------------------------------------------------------------------*
 * G-code bits bodged from GNU GPL:                                        *
 * 2 Axis CNC Demo v2 - supports Adafruit motor shields v1 and v2          *
 * dan@marginallycelver.com 2013-08-30                                     *
 * see http://www.github.com/MarginallyClever/GcodeCNCDemo for more info   *
 *-------------------------------------------------------------------------*/

/***************************************************************************\
 * IntervalometerV2.ino                                                    *
 * ------------------------------------------------------------------------*
 * begin              : May 2015                                           *
 * copyright          : (C) 2015 by Jason Jorgenson.  All rights reserved  *
 * email              : jjorgenson@gmail.com                               *
\***************************************************************************/

/***************************************************************************\
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
\***************************************************************************/

// test gitHub is working properly
// ok, it works - but I needed to enter my username and pw for gitHub
// which is probably ok. However, I am not sure how that will work for
// collaborating with JR :-/  hmmm
// JR may have to use git@github.com:arkygeek/IntervalometerV2.git
// as per the suggestion at:
// http://www.pavley.com/2013/06/12/getting-xcode-and-github-to-work-together-like-besties/

#include <Arduino.h>
#include <arduino.h>

// local includes
#include "LocalLibrary.h" // some prototypes live here for now
#include "timer.h"
#include "defaults.h"
#include "LiquidCrystal.h"

/***************************************************************************\
 *                             *                                           *
 *  ARDUINO PIN CONFIGURATION  *                                           *
 *                             *                                           *
 *******************************                                           *
 *                                                                         *
 *                                                                         *
 *                          AdaFruit Ind.                                  *
 *          PIN  |  STD   | Logger Shield |     Mine     |  Intervalometer *
 * ==============+========+===============+==============+================ *
 * DIGITAL   13  |  SCK   |       x       |              |                 *
 *           12  |  MISO  |       x       |              |                 *
 *           11~ |  MOSI  |       x       |              |                 *
 *           10~ |  SS    |       x       |   Kill LED   |    Kill LED     *
 *               +--------+---------------+--------------+---------------- *
 *            9~ |        |               |   LCD LED    |    LCD LED      *
 *            8  |        |               |              |   Keyboard C    *
 *            7  |        |               | Trigger OUT  |  Trigger OUT    *
 *            6~ |        |               |  Room Light  |   Room Light    *
 *            5~ |        |               |  Focus LED   |    Focus LED    *
 *               +--------+---------------+--------------+---------------- *
 *            4  |        |               |  Focus OUT   |    Focus OUT    *
 *            3~ |  INT   |               |              |   Keyboard D    *
 *            2  |  INT   |               |     STOP     |      STOP       *
 *            1  |  TX    |               |              |                 *
 *            0  |  RX    |               |              |                 *
 * ==============+========+===============+==============+================ *
 * ANALOG     0  |        |               |    Sensor    |     Sensor      *
 *            1  |        |               | Focus Button |  Focus Button   *
 *            2  |        |               |Trigger Button|  Trigger Button *
 *            3  |        |               | Start Button |  Start Button   *
 *            4  |  I2C   |       x       |   LCD I2C    |      LCD I2C    *
 *            5  |  I2C   |       x       |   LCD I2C    |      LCD I2C    *
\***************************************************************************/


// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(37, 36, 35, 34, 33, 32);// 9, 8, 3, 2);


Timer mTimer;
Defaults mDefaults;

#define EFOCUS 8
#define ELIGHTS 12

/** defs for g-code interpreter */
#define VERSION    (2)  // firmware version
#define BAUD       (115200)  // How fast is the Arduino talking?
#define MAX_BUF    (64)  // What is the longest message Arduino can store?

int mNumberOfShotsTaken = 0;
int mNumberOfShotsOnScreen = 0;

/* globals for g-code interpreting */
char mBuffer[MAX_BUF]; // where we store the message until we get a ';'
int mBufferSize; // how much is in the buffer
long mLineNumber=0;
boolean mIsRunning;

//vars for control of multi-shot stuff

int mShootingMode;
#define SINGLESHOT 1
#define MULTISHOT  2

// global takes (for timelapse)
int mGlobalTakesCount = 2;
unsigned long mGlobalTakesInterval = 20000;
unsigned long mGlobalTakesLastDesired;
int mGlobalTakesCurrent;

// m is multishot mode
// motor control vars
boolean mMultishotMechActive;
unsigned long mMultishotMechLastActive;
int mMultishotMechCount = 3;
unsigned long mMultishotMechInterval = 3000;
unsigned long mMultishotMechLastDesired;
int mMultishotMechCurrent;

long mMultishotMechMinPoint= 30070; //stored in microns
long mMultishotMechMaxPoint= 40000;
long mMultishotMechStepSize= 1000;
long mMultishotMechCurrentPoint;


//local takes (for multishot exposure bracketing etc)
int mMultishotLocalTakesCount = 2;
unsigned long mMultishotLocalTakesInterval = 0;
unsigned long mMultishotLocalTakesLastDesired;
int mMultishotLocalTakesCurrent;

//int shots; //(number of shots to take, after all other looping, may be 1;setting)

int sNextEvent;

    /*/////////////////////////////////
   //                               //|
  //   Primary Arduino Functions   // |
 //                               //  |
///////////////////////////////////   |
|                                 |  /
|       setup() and loop()        | /
|_________________________________*/
 

void setup() {
  
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("IV2");
  
  pinMode(mDefaults.LightDiagnosticsPin(), OUTPUT);
  
  Serial.begin(BAUD); // open coms
  Serial1.begin(BAUD); // open link to g-code interpreter
  
  //for serial g-code stuff
  Help(); // say hello
  Ready();
  
  /** 
    * @TODO load settings from somewhere; until then, just set defaults
    */
  
  mTimer.setFocusHold(400);
  mTimer.setLightingLag(100);
  mTimer.setShutterHold(100);
  mTimer.setButtonHold(10);
  
  mTimer.setShutterLag(10);
  mTimer.setInterShootLag(1000); //max 2.5fps
  mTimer.setLastTimeCameraTriggered(0);
  
  pinMode(mDefaults.TurnOffLED(), OUTPUT); //ambient light, Active low
  digitalWrite(mDefaults.TurnOffLED(),LOW);
  
  pinMode(mDefaults.PanelLcdLED(), OUTPUT); // panel LED, active low
  digitalWrite(mDefaults.PanelLcdLED(),LOW);
  
  pinMode(mDefaults.TriggerOut(), OUTPUT); // active high
  digitalWrite(mDefaults.TriggerOut(),LOW);
  
  //  pinMode(preLED, OUTPUT); // ambient light active low
  //  digitalWrite(preLED,LOW);
  
  digitalWrite(mDefaults.FocusLED(),HIGH);
  pinMode(mDefaults.FocusLED(), OUTPUT); // pwm active low
  digitalWrite(mDefaults.FocusLED(),HIGH);
  
  pinMode(mDefaults.FocusOut(), OUTPUT); // active high
  digitalWrite(mDefaults.FocusOut(),LOW);
  
  digitalWrite(mDefaults.LightDiagnosticsPin(),LOW);
  delay(500);
  digitalWrite(mDefaults.LightDiagnosticsPin(),HIGH);
  
  mIsRunning = false;
  
  //multishot stuff -- testing defaults
  //mGTakesCount = 3;
  //mGTakesInterval = 2000;
  //mLTakesCount = 2;
  //mLTakesInterval = 500;
}

void loop() {
  if (mNumberOfShotsOnScreen != mNumberOfShotsTaken)
  {
    lcd.setCursor(0, 1);
    // print the number of seconds since reset:
    lcd.print(mNumberOfShotsTaken);
    mNumberOfShotsOnScreen = mNumberOfShotsTaken;
  }
  
  if (mIsRunning)
  {
    // Serial.print("loop-r");
    CheckEvents();
    CheckInputs();
    
    //later dev below...
    //check events (again)
    //update display
    
  }
  else
  {
    CheckInputs();
    CheckSerial();
    
    //Serial.print("loop-n");
    //update display (later development)
  }
}


     /*///////////////////////////////
    //                             //|
   //      Additional Functions   // |
  //                             //  |
 /////////////////////////////////   |
 |                               |   |
 | void SendZPos(long theInput)  |   |
 | void CallMech()               |   |
 | void CallFocus()              |   |
 | void CallLights()             |   |
 | void CallTrigger()            |   |
 | void CallUnTrig()             |  /
 | CallResetOutputs()            | /
 |_______________________________*/


void SendZPos(long theInput) // microns
{
  double myOutput = theInput/1000.0;
  
  Serial1.print(F("G1 Z"));
  Serial1.print((myOutput),3);
  Serial1.println(F(" F475;"));
}

void CallMech()
{
  //digitalWrite(LIGHTDIAGPIN,LOW);
  //sMechDn = true;first action, so no check needed
  
  
  mMultishotMechCurrentPoint = (mMultishotMechCurrent
                                * mMultishotMechStepSize
                                + mMultishotMechMinPoint);
  
  //send gcode
  SendZPos (mMultishotMechCurrentPoint); // all done in microns
  mMultishotMechActive = true;
}

void CallFocus()
{
  digitalWrite(mDefaults.LightDiagnosticsPin(),LOW);
  
  //mDoneStartFocusMode = true;
  mTimer.setDoneStartFocusMode(true);
  //focus lighting on
  digitalWrite(mDefaults.FocusLED(),LOW); // pwm active low
  //focus trigger on
  digitalWrite(mDefaults.FocusOut(),HIGH); // active high
}

void CallLights()
{
  digitalWrite(mDefaults.LightDiagnosticsPin(),HIGH);
  //mDoneSetLightingMode = true;
  mTimer.setDoneSetLightingMode(true);
  //ambient lighting off
  digitalWrite(mDefaults.TurnOffLED(),HIGH); //ambient light, Active low
  digitalWrite(mDefaults.PanelLcdLED(),HIGH); // panel LED, active low
  
  //photographic lighting on (for later implementation)
  
  //focus lighting off
  digitalWrite(mDefaults.FocusLED(),HIGH); // pwm active low
}

void CallTrigger()
{
  mNumberOfShotsTaken++;
  digitalWrite(mDefaults.LightDiagnosticsPin(),LOW);
  //mDoneSendTrigger = true;
  mTimer.setDoneSendTrigger(true);
  //main trigger on
  digitalWrite(mDefaults.TriggerOut(),HIGH); // active high
  
  Serial.println(millis());
  
  //store last trigger value using current sync value
  //mLastTimeCameraTriggered = mShutterExpectedOpenTimeCalculated + mBaseTimeToMarkStartOfShot;
  unsigned long myTemp = mTimer.ShutterExpectedOpenTimeCalculated();
  unsigned long myTemp2 = mTimer.BaseTimeToMarkStartOfShot();
  mTimer.setLastTimeCameraTriggered(myTemp + myTemp2);
}

void CallUnTrig()
{
  digitalWrite(mDefaults.LightDiagnosticsPin(),HIGH);
  mTimer.setDoneStopTrigger(true);
  
  //main trigger off
  digitalWrite(mDefaults.TriggerOut(),LOW); // active high
  
  //focus trigger off
  digitalWrite(mDefaults.FocusOut(),LOW); // active high
}

/**
  * for later implemention
  */
void CallResetOutputs()
{
  //ambient lighting on
  digitalWrite(mDefaults.TurnOffLED(),LOW); //ambient light, Active low
  digitalWrite(mDefaults.PanelLcdLED(),LOW); // panel LED, active low
}

/**
  * prepares the input buffer to receive a new message
  * and tells the serial connected device it is ready for more.
  */
void Ready() {
  mBufferSize=0; // clear input buffer
  Serial.print(F(">")); // signal ready to receive input
}

/**
  * display helpful information
  */
void Help() {
  Serial.print(F("intervalometer with Gcode  "));
  Serial.println(VERSION);
  Serial.println(F("Commands:"));
  Serial.println(F("T1;-- trigger camera fully once"));
  Serial.println(F("T2;-- trigger full programed sequence"));
  Serial.println(F("T100;- this help message"));
}

unsigned long GetBaseTimeToMarkStartOfShot()
{
  return millis() - mTimer.BaseTimeToMarkStartOfShot();
}

void EndMultiShot()
{
  //if using mech, return motor to start
  //lighting??
  
  mIsRunning = false; // end sequence
  Serial.println("Ending multi");
}
/**
 *  update only if larger, but tolerant of clockovers
 */
void UpdateGlobalDesired(unsigned long theInput) // TODO -- check this actually works
{
  if (theInput - mTimer.GlobalDesired() < 3000000000uL)
  {
    mTimer.setGlobalDesired(theInput);
  }
}

void RecalculateShot()
{
  //select max value from... and put in sync
  long myTime;
  
  // to prevent camera being over worked do this:
  //mShutterExpectedOpenTimeCalculated = mLastTimeCameraTriggered+mInterShotLag
  //                                     - mBaseTimeToMarkStartOfShot;
  
  long myTemp1 = mTimer.LastTimeCameraTriggered();
  long mytemp2 = mTimer.BaseTimeToMarkStartOfShot();
  long temp = myTemp1-mytemp2;
  mTimer.setShutterExpectedOpenTimeCalculated(temp);
  // @TODO: is this referenced correctly, mBaseTimeToMarkStartOfShot???
  //        ... I think so.
  
  //sanity check for the above, default to ~1fps if the it's trying more than a minute delay.
  if (mTimer.ShutterExpectedOpenTimeCalculated() > 60000)
  {
    mTimer.setShutterExpectedOpenTimeCalculated(1000);
    Serial.println("err: Intershot_lag stuffup");
  }
  
  //TODO: check stage_last_moved+stage_lag
  //insert sanity check for the above??
  // Serial.print("syncA= ");Serial.println(sync);
  // Serial.print("last_sync= ");Serial.println(last_sync);
  
  // quickest the processes can be sorted out in:
  myTime = millis()
           + mTimer.FocusHold()
           + mTimer.LightingLag()
           - mTimer.BaseTimeToMarkStartOfShot();
  
  if (myTime > mTimer.ShutterExpectedOpenTimeCalculated())
  {
    mTimer.setShutterExpectedOpenTimeCalculated(myTime);
  }
  
  //Serial.print("t= ");Serial.println(t);
  //Serial.print("syncB= ");Serial.println(sync);
  //Serial.print("mBaseTimeToMarkStartOfShot= ");Serial.println(mBaseTimeToMarkStartOfShot);
  
  // if the desired target time is actually attainable
  // (ie, later than soonest possible time), go with that.
  if (mTimer.UseDesired() && mTimer.Desired() > mTimer.ShutterExpectedOpenTimeCalculated())
  {
    mTimer.setShutterExpectedOpenTimeCalculated(mTimer.Desired());
  }
  
  mTimer.setTimeToSetLightingMode(mTimer.ShutterExpectedOpenTimeCalculated()
                                  - mTimer.LightingLag());
  mTimer.setTimeToStartFocusMode(mTimer.ShutterExpectedOpenTimeCalculated()
                                 - mTimer.LightingLag()
                                 - mTimer.FocusHold());
  mTimer.setTimeToSendTrigger(mTimer.ShutterExpectedOpenTimeCalculated()
                              - mTimer.ShutterLag());
  mTimer.setTimeToStopTrigger(mTimer.TimeToSendTrigger() + mTimer.ButtonHold());
  mTimer.setTimeToRestart(mTimer.ShutterExpectedOpenTimeCalculated()
                          + mTimer.ShutterHold());
  
  //Serial.print("glabal sync= ");Serial.println(mBaseTimeToMarkStartOfShot+sync);
  //Serial.print("sLightsTime= ");Serial.println(sLightsTime);
  //Serial.print("sFocusTime= ");Serial.println(sFocusTime);
  //Serial.print("TriggerTime= ");Serial.println(sTriggerTime);
  //Serial.print("unTrigTime= ");Serial.println(sUnTrigTime);
  //Serial.print("ResetTime= ");Serial.println(sResetTime);
}

void StartShot()
{
  mTimer.setBaseTimeToMarkStartOfShot(millis());
  
  //work out last_sync??
  
  //work out desired??
  if (mTimer.UseDesired())
  {
    mTimer.setDesired(mTimer.GlobalDesired()
                      - mTimer.BaseTimeToMarkStartOfShot());
  }
  
  mTimer.setDoneStartFocusMode(false);
  mTimer.setDoneSetLightingMode(false);
  mTimer.setDoneSendTrigger(false);
  mTimer.setDoneStopTrigger(false);
  
  //start stage move if required
  if (mTimer.UseMechanicalMovement())
  {
    CallMech();
    // @TODO -- work out how ot wait for mechanical stuff -- ignore for now.
  }
  
  RecalculateShot();
  
  mTimer.FocusHold() > 0 ? sNextEvent=EFOCUS : sNextEvent=ELIGHTS;
}

void AdvanceMultiShot()
{
  // check for clockover
  if (++mMultishotLocalTakesCurrent >= mMultishotLocalTakesCount)
  {  // clockover
    mMultishotLocalTakesCurrent = 0;
    if (++mMultishotMechCurrent >= mMultishotMechCount)
    {
      mMultishotMechCurrent = 0;
      if (++mGlobalTakesCurrent >= mGlobalTakesCount)
         {
           mGlobalTakesCurrent = 0;
           EndMultiShot();
         }
      else //mGTakes tickover
      {
        //gDesired = last_sync+mGTakesInterval; //TODO -- fix
        UpdateGlobalDesired(mGlobalTakesLastDesired + mGlobalTakesInterval);
      }
      mGlobalTakesLastDesired = mTimer.GlobalDesired();
    }
    else //mMech tickover
    {
      //setMotorPosition();X done in shot setup
      UpdateGlobalDesired(mMultishotMechLastDesired + mMultishotMechInterval);
    }
    mMultishotMechLastDesired = mTimer.GlobalDesired();
  }
  else //mLTakes tick
  {
    //gDesired = last_sync+mLTakesInterval;
    UpdateGlobalDesired(mMultishotLocalTakesLastDesired+mMultishotLocalTakesInterval);
  }
  
  mMultishotLocalTakesLastDesired = mTimer.GlobalDesired();
  
  if (mIsRunning) {StartShot();}
  
  // tickovers only happen between normal shots of that level,
  // NOT when clockover of the level above happenes.
}

void EndShot()
{
  // call reset outputs to sort out lights etc
  // later this should be divided up to avoid strobing the room light
  
  CallResetOutputs();
  
  if (mShootingMode == MULTISHOT )
  {
    AdvanceMultiShot();
  }
  else
  {
    mIsRunning = false; //single shot mode
  }
}

void CheckEvents()
{
  unsigned long myNewTime;
  myNewTime = GetBaseTimeToMarkStartOfShot();
  
  if (!mTimer.DoneStartFocusMode()
      && (mTimer.TimeToStartFocusMode()
          <= myNewTime))
  {
    CallFocus();
  }
  if (!mTimer.DoneSetLightingMode()
      && (mTimer.TimeToSetLightingMode()
          <= myNewTime))
  {
    CallLights();
  }
  if (!mTimer.DoneSendTrigger()
      && (mTimer.TimeToSendTrigger()
          <= myNewTime))
  {
    CallTrigger();
  }
  if (!mTimer.DoneStopTrigger()
      && (mTimer.TimeToStopTrigger()
          <= myNewTime))
  {
    CallUnTrig();
  }
  
  //reset time??
  if (mTimer.TimeToRestart() <= myNewTime)
  {
    EndShot();
  }
}

void StartSingle()
{
  if (mIsRunning) {return;}
  
  Serial.println("starting single");
  mTimer.setUseMechanicalMovement(false);
  mTimer.setUseDesired(false);
  //gDesired = mBaseTimeToMarkStartOfShot; // desired not relevent for single shots
  mIsRunning = true;
  mShootingMode = SINGLESHOT;
  StartShot();
}

void StartMulti()
{
  if (mIsRunning) {return;}
  
  Serial.println("starting multi");
  mTimer.setUseMechanicalMovement(true);
  mTimer.setUseDesired(true);
  //gDesired = mBaseTimeToMarkStartOfShot; // desired not relevent for single shots
  mIsRunning = true;
  mShootingMode = MULTISHOT;
  
  // global takes (for timelapse)
  mGlobalTakesLastDesired = millis();
  mGlobalTakesCurrent = 0;
  
  //mech for motorised stage moves
  mMultishotMechLastDesired = millis();
  mMultishotMechCurrent = 0;
  
  //local takes (for multishot exposure bracketing etc)
  mMultishotLocalTakesLastDesired = millis();
  mMultishotLocalTakesCurrent = 0;
  
  StartShot();
}

void CheckButtonStates()
{
  int myAnalogValue = analogRead(mDefaults.ButtonsOnPanel());
  boolean myRet = 1; // SEE NOTE BELOW
  // @TODO find out what myRet is for - it seems it does nothing
  
  //Serial.println(val);
  //delay(4);
  
  if (myAnalogValue < 185) { StartSingle(); } // green or NC???
  else if (myAnalogValue < 328) { StartMulti(); } // blue
  else if (myAnalogValue < 515) {               } // black
  else if (myAnalogValue < 708) {               } // white
  else if (myAnalogValue < 900) {               } //yellow
  else { myRet = 0; }
}

void CheckEmergencyStopState()
{
  // nothing here yet
}

void CheckInputs()
{
  CheckButtonStates();
  CheckEmergencyStopState();
  // check if there is enough time then mess about with keyboard checking
}

/**
  * Look for character /code/ in the buffer and read the float that immediately follows it.
  * @return the value found.  If nothing is found, /val/ is returned.
  * @input theCode the character to look for.
  * @input theValue the return value if /code/ is not found.
  */
float ParseNumber(char theCode,float theValue)
{
  char *mypLocation = mBuffer;
  while(mypLocation && *mypLocation && mypLocation < mBuffer + mBufferSize)
  {
    if (*mypLocation == theCode)
    {
      return atof(mypLocation + 1);
    }
    mypLocation=strchr(mypLocation,' ') + 1;
  }
  return theValue;
}

/**
  * Read the input buffer and find any recognized commands.  One G or M command per line.
  */
void ProcessGCommand() {
  // blank lines
  if (mBuffer[0]==';') return;
  
  long myCommand;
  
  // is there a line number?
  myCommand = ParseNumber('N',-1);
  if (myCommand != -1 && mBuffer[0] == 'N')
  {
    // line number must appear first on the line
    if (myCommand != mLineNumber )
    {
      // wrong line number error
      Serial.print(F("BADLINENUM "));
      Serial.println(mLineNumber);
      return;
    }
    // is there a checksum?
    if (strchr(mBuffer,'*') != 0)
    {
      // yes.  is it valid?
      char myChecksum=0;
      int myCurrent=0;
      while (mBuffer[myCurrent] != '*')
      {
        myChecksum ^= mBuffer[myCurrent++];
      }
      myCurrent++; // skip *
      int myAgainst = strtod(mBuffer + myCurrent, NULL);
      if ( myChecksum != myAgainst )
      {
        Serial.print(F("BADCHECKSUM "));
        Serial.println(mLineNumber);
        return;
      }
    }
    else
    {
      Serial.print(F("NOCHECKSUM "));
      Serial.println(mLineNumber);
      return;
    }
    mLineNumber++;
  }
  // changed from "G" to "D" for compatibility, to keep out of way of actual G code
  myCommand = ParseNumber('D',-1);
  switch(myCommand)
  {
    case  0: // move linear
    case  1: // move linear
    //feedrate(parsenumber('F',fr));
    //line( parsenumber('X',(mode_abs?px:0)) + (mode_abs?0:px),
    // parsenumber('Y',(mode_abs?py:0)) + (mode_abs?0:py) );
    break;
    case  4:  //pause(parsenumber('S',0) + parsenumber('P',0)*1000.0f);break; // dwell
    default:  break;
  }
  
  myCommand = ParseNumber('M',-1);
  switch(myCommand) {
    case 100:  Help(); break;
    case 110:  //line_number = parsenumber('N',line_number);break;
    case 114:  //where();break;
    default:  break;
  }
  
  myCommand = ParseNumber('T',-1);
  switch(myCommand) {
    case 1: StartSingle();break; // simple camera trigger
    case 2: StartMulti();break; // simple camera trigger
    case 90: SendZPos(10000L);break;
    case 91: SendZPos(15000L);break;
    case 100:  Help();break;
    case 110:  //line_number = parsenumber('N',line_number);break;
    case 114:  //where();break;
    default:  break;
  }
}

void CheckSerial() {
  // listen for serial commands
  while(Serial.available() > 0)
  {
    // if something is available
    char c=Serial.read(); // get it
    Serial1.print(c); // send it on
    if (c=='\r') continue; // skip it
    Serial.print(c); // repeat it back so I know you got the message
    if (mBufferSize<MAX_BUF) mBuffer[mBufferSize++] = c; // store it
    if (c=='\n')
    {
      // entire message received
      mBuffer[mBufferSize]=0; // end the buffer so string functions work right
      ProcessGCommand(); // do something with the command
      Ready();
    }
  }
  // listen for serial feedback from serial1
  while (Serial1.available() > 0)
  {
    // if something is available
    char myCharacter = Serial1.read(); // get it
    if (myCharacter == '\r') continue; // skip it
    Serial.print(myCharacter); // repeat it back so I know you got the message
  }
}
