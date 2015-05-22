/*
 * Intervalometer sketch.
 * restart of project following radical cutdown of feature set.
 *
 *  Created on: 28 oct 2014
 * Author: jrp
 */


//------------------------------------------------------------------------------
// G-code bits bodged from GNU GPL:
// 2 Axis CNC Demo v2 - supports Adafruit motor shields v1 and v2
// dan@marginallycelver.com 2013-08-30
// please see http://www.github.com/MarginallyClever/GcodeCNCDemo for more information.
//------------------------------------------------------------------------------

#include <Arduino.h>
#include <arduino.h>
#include "LocalLibrary.h"
//#include "AFMotor.h"
//#include <SoftwareSerial.h>
//#include "Adafruit_MotorShield.h"

//const defs

//not used
#define ESTART 1
#define ESTAGE 3
#define EFOCUS 8
#define ELIGHTS 12
#define ETRIG 18
#define EUNTRIG 19
//#define ESYNC 20
#define ERESET 39

//defs for g-code interpreter
#define VERSION    (2)  // firmware version
#define BAUD       (115200)  // How fast is the Arduino talking?
#define MAX_BUF    (64)  // What is the longest message Arduino can store?

//pin definitions etc

const int mPs2DataPin = 17;
const int mPs2IRQpin =  3;

const int mButtonsOnPanel = A1; // analog multi button panel
const int stopButton = A2; // stop button connected to analog pin, this is not a long term solution.
const int mTurnOffLED = 10; //ambient light, Active low
const int mPanelLcdLED = 9; // panel LED, active Low
const int mTriggerOut = 7; // active high
const int mFocusLED = 5; // pwm active low
const int mFocusOut = 4; // active high
const int mLightDiagnosticsPin = 13; // diagnostics LED -- PWM, active low??.
//const int preLED = 6; // ambient light active low --- what?

//glabals for g-code interpreting

char mBuffer[MAX_BUF]; // where we store the message until we get a ';'
int mBufferSize; // how much is in the buffer
long mLineNumber=0;

//var defs

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

  /////////////////
 // timing vars //
/////////////////
class Timer {
private:
  
  
public:
  // setters
  
  
  // getters
  
};
unsigned long mFocusHold; //(0=off;1+ no ms to spend focusing before taking pic, setting)
unsigned long mShutterHold; //(to be replaced later with x-sync detection, time to wait for camera;setting)
unsigned long mButtonHold; //(time to hold trigger line low default 5ms)

unsigned long mLightingLag; //(no of ms before sync to switch lighting;setting, default 100ms)
unsigned long mShutterLag; //(time between trigger and sync;preset)
unsigned long mInterShotLag; //(fastest time between sync times that camera can handle;preset)

unsigned long mBaseTimeToMarkStartOfShot; // base time to mark start of shot
unsigned long mBaseTimeToMarkStartOfSequence; // base time to mark start of sequence
boolean mUseMechanicalMovement; // whether mechanical movement is required
boolean mUseDesired; // whether to use desired of not;
long mGlobalgDesired; //(desired, but references from milli's, temp store for before mBaseTimeToMarkStartOfShot is known)
long mDesired; //(time at which the shutter is desired to go off;calculated at start of shot) (referenced from mBaseTimeToMarkStartOfShot)
long mShutterExpectedOpenTimeCalculated; //(time at which the shutter is expected, to be fully open;calculated) (referenced from mBaseTimeToMarkStartOfShot)
//unsigned long mInterval; //( time delay between shot sync times, may be 0 for "fast";setting)

unsigned long mLastTimeCameraTriggered;
// mLastTimeCameraTriggered: the last time the camera triggered (should likely be when the shutter closed??) (referenced from millis)

// @TODO implement time to next event
long mTimeToNextEvent; // time to next event -- indicates how much time functions can use.

long mNextEventTime; // (time the next event should happen) (from mBaseTimeToMarkStartOfShot)
long mTimeToStartFocusMode; // planned time to start focus mode (from mBaseTimeToMarkStartOfShot)
long mTimeToSetLightingMode; // planned time to set lighing mode (from mBaseTimeToMarkStartOfShot)
long mTimeToSendTrigger; // planned time to send trigger (from mBaseTimeToMarkStartOfShot)
long mTimeToStopTrigger; // planned time to stop trigger (from mBaseTimeToMarkStartOfShot)
long mTimeToRestart; // planned time to finish & start next shot (from mBaseTimeToMarkStartOfShot)

boolean mDoneStartFocusMode; // done start focus mode
boolean mDoneSetLightingMode; // done set lighing mode
boolean mDoneSendTrigger; // done send trigger
boolean mDoneStopTrigger; // done stop trigger

  /////////////////
 //  Functions  //
/////////////////

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
  digitalWrite(mLightDiagnosticsPin,LOW);
  mDoneStartFocusMode = true;
  //focus lighting on
  digitalWrite(mFocusLED,LOW); // pwm active low
  //focus trigger on
  digitalWrite(mFocusOut,HIGH); // active high
}

void CallLights()
{
  digitalWrite(mLightDiagnosticsPin,HIGH);
  mDoneSetLightingMode = true;

  //ambient lighting off
  digitalWrite(mTurnOffLED,HIGH); //ambient light, Active low
  digitalWrite(mPanelLcdLED,HIGH); // panel LED, active low
  
  //photographic lighting on (for later implementation)
  
  //focus lighting off
  digitalWrite(mFocusLED,HIGH); // pwm active low
}

void CallTrigger()
{
  digitalWrite(mLightDiagnosticsPin,LOW);
  mDoneSendTrigger = true;
  
  //main trigger on
  digitalWrite(mTriggerOut,HIGH); // active high
  
  Serial.println(millis());
  
  //store last trigger value using current sync value
  mLastTimeCameraTriggered = mShutterExpectedOpenTimeCalculated+mBaseTimeToMarkStartOfShot;
}

void CallUnTrig()
{
  digitalWrite(mLightDiagnosticsPin,HIGH);
  mDoneStopTrigger = true;
  
  //main trigger off
  digitalWrite(mTriggerOut,LOW); // active high
  
  //focus trigger off
  digitalWrite(mFocusOut,LOW); // active high
}

void CallResetOutputs() // for later implemention
{
  //ambient lighting on
  digitalWrite(mTurnOffLED,LOW); //ambient light, Active low
  digitalWrite(mPanelLcdLED,LOW); // panel LED, active low
}

/**
 * prepares the input buffer to receive a new message and tells the serial connected device it is ready for more.
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

void setup() {
  pinMode(mLightDiagnosticsPin, OUTPUT);
  
  Serial.begin(BAUD); // open coms
  Serial1.begin(BAUD); // open link to g-code interpreter
  
  //for serial g-code stuff
  Help(); // say hello
  Ready();
  
  //@TODO load settings from somewhere; until then, just set defaults
  
  mFocusHold = 400;
  mLightingLag = 100;
  mShutterHold = 100;
  mButtonHold = 10;
  
  mShutterLag = 10;
  mInterShotLag = 1000; //max 2.5fps
  
  mLastTimeCameraTriggered = 0;
  
  pinMode(mTurnOffLED, OUTPUT); //ambient light, Active low
  digitalWrite(mTurnOffLED,LOW);
  
  pinMode(mPanelLcdLED, OUTPUT); // panel LED, active low
  digitalWrite(mPanelLcdLED,LOW);
  
  pinMode(mTriggerOut, OUTPUT); // active high
  digitalWrite(mTriggerOut,LOW);
  
  //  pinMode(preLED, OUTPUT); // ambient light active low
  //  digitalWrite(preLED,LOW);
  
  digitalWrite(mFocusLED,HIGH);
  pinMode(mFocusLED, OUTPUT); // pwm active low
  digitalWrite(mFocusLED,HIGH);
  
  pinMode(mFocusOut, OUTPUT); // active high
  digitalWrite(mFocusOut,LOW);
  
  digitalWrite(mLightDiagnosticsPin,LOW);
  delay(500);
  digitalWrite(mLightDiagnosticsPin,HIGH);
  
  mIsRunning = false;
  
  //multishot stuff -- testing defaults
  //mGTakesCount = 3;
  //mGTakesInterval = 2000;
  //mLTakesCount = 2;
  //mLTakesInterval = 500;
}

unsigned long GetBaseTimeToMarkStartOfShot()
{
  return millis() - mBaseTimeToMarkStartOfShot;
}

void EndMultiShot()
{
  //if using mech, return motor to start
  //lighting??
  
  mIsRunning = false; // end sequence
  Serial.println("Ending multi");
}

void UpdateGDesired(unsigned long theInput) // TODO -- check this actually works
{
  //update only if larger, but tolerant of clockovers
  if (theInput - mGlobalgDesired < 3000000000uL)
  {
    mGlobalgDesired = theInput;
  }
}

void RecalculateShot()
{
  //select max value from... and put in sync
  long myTime;
  
  // to prevent camera being over worked do this:
  mShutterExpectedOpenTimeCalculated = mLastTimeCameraTriggered+mInterShotLag
                                       - mBaseTimeToMarkStartOfShot;
  // @TODO: is this referenced correctly, mBaseTimeToMarkStartOfShot???
  //        ... I think so.
  
  //sanity check for the above, default to ~1fps if the it's trying more than a minute delay.
  if (mShutterExpectedOpenTimeCalculated>60000)
  {
    mShutterExpectedOpenTimeCalculated = 1000;
    Serial.println("err: Intershot_lag stuffup");
  }
  
  //TODO: check stage_last_moved+stage_lag
  //insert sanity check for the above??
  // Serial.print("syncA= ");Serial.println(sync);
  // Serial.print("last_sync= ");Serial.println(last_sync);
  
  // quickest the processes can be sorted out in:
  myTime = millis() + mFocusHold+mLightingLag - mBaseTimeToMarkStartOfShot;
  
  if (myTime > mShutterExpectedOpenTimeCalculated)
  {
    mShutterExpectedOpenTimeCalculated = myTime;
  }
  
  //Serial.print("t= ");Serial.println(t);
  //Serial.print("syncB= ");Serial.println(sync);
  //Serial.print("mBaseTimeToMarkStartOfShot= ");Serial.println(mBaseTimeToMarkStartOfShot);
  
  // if the desired target time is actually attainable
  // (ie, later than soonest possible time), go with that.
  if (mUseDesired && mDesired > mShutterExpectedOpenTimeCalculated)
  {
    mShutterExpectedOpenTimeCalculated = mDesired;
  }
  
  mTimeToSetLightingMode = mShutterExpectedOpenTimeCalculated - mLightingLag;
  mTimeToStartFocusMode = mShutterExpectedOpenTimeCalculated - mLightingLag - mFocusHold;
  mTimeToSendTrigger = mShutterExpectedOpenTimeCalculated - mShutterLag;
  mTimeToStopTrigger = mTimeToSendTrigger + mButtonHold;
  mTimeToRestart = mShutterExpectedOpenTimeCalculated + mShutterHold;
  
  //Serial.print("glabal sync= ");Serial.println(mBaseTimeToMarkStartOfShot+sync);
  //Serial.print("sLightsTime= ");Serial.println(sLightsTime);
  //Serial.print("sFocusTime= ");Serial.println(sFocusTime);
  //Serial.print("TriggerTime= ");Serial.println(sTriggerTime);
  //Serial.print("unTrigTime= ");Serial.println(sUnTrigTime);
  //Serial.print("ResetTime= ");Serial.println(sResetTime);
}

void StartShot()
{
  mBaseTimeToMarkStartOfShot = millis();
  
  //work out last_sync??
  
  //work out desired??
  if (mUseDesired) {mDesired = mGlobalgDesired-mBaseTimeToMarkStartOfShot;}
  
  mDoneStartFocusMode = false;
  mDoneSetLightingMode = false;
  mDoneSendTrigger = false;
  mDoneStopTrigger = false;
  
  //start stage move if required
  if (mUseMechanicalMovement)
  {
    CallMech();
    // @TODO -- work out how ot wait for mechanical stuff -- ignore for now.
  }
  
  RecalculateShot();
  
  mFocusHold > 0 ? sNextEvent=EFOCUS : sNextEvent=ELIGHTS;
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
        UpdateGDesired(mGlobalTakesLastDesired + mGlobalTakesInterval);
      }
      mGlobalTakesLastDesired = mGlobalgDesired;
    }
    else //mMech tickover
    {
      //setMotorPosition();X done in shot setup
      UpdateGDesired(mMultishotMechLastDesired + mMultishotMechInterval);
    }
    mMultishotMechLastDesired = mGlobalgDesired;
  }
  else //mLTakes tick
  {
    //gDesired = last_sync+mLTakesInterval;
    UpdateGDesired(mMultishotLocalTakesLastDesired+mMultishotLocalTakesInterval);
  }
  
  mMultishotLocalTakesLastDesired = mGlobalgDesired;
  
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
  
  if (!mDoneStartFocusMode && (mTimeToStartFocusMode <= myNewTime)) { CallFocus(); }
  if (!mDoneSetLightingMode && (mTimeToSetLightingMode <= myNewTime)) { CallLights(); }
  if (!mDoneSendTrigger && (mTimeToSendTrigger <= myNewTime)) { CallTrigger(); }
  if (!mDoneStopTrigger && (mTimeToStopTrigger <= myNewTime)) { CallUnTrig(); }
  
  //reset time??
  if (mTimeToRestart <= myNewTime) { EndShot(); }
}

void StartSingle()
{
  if (mIsRunning) {return;}
  
  Serial.println("starting single");
  mUseMechanicalMovement = false;
  mUseDesired = false;
  //gDesired = mBaseTimeToMarkStartOfShot; // desired not relevent for single shots
  mIsRunning = true;
  mShootingMode = SINGLESHOT;
  StartShot();
}

void StartMulti()
{
  if (mIsRunning) {return;}
  
  Serial.println("starting multi");
  mUseMechanicalMovement = true;
  mUseDesired = true;
  
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
  int myAnalogValue = analogRead(mButtonsOnPanel);
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
 **/
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

void loop() {
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
    
    ////Serial.print("loop-n");
    //update display (later development)
  }
}

/**
 * write a string followed by a float to the serial line.  Convenient for debugging.
 * @input code the string.
 * @input val the float.
 */
/*void debug_serial_output(char *code,float val)
{
  Serial.print(code);
  Serial.println(val);
}*/
