
// function prototypes

void SendZPos(long inp);
void CallMech();
void CallFocus();
void CallLights();
void CallTrigger();
void CallUnTrig();
void CallResetOutputs();
void Ready();
void Help();
unsigned long GetBaseTimeToMarkStartOfShot();
void EndMultiShot();
void UpdateGDesired(unsigned long theInput);
void RecalculateShot();
void StartShot();
void AdvanceMultiShot();
void EndShot();
void CheckEvents();
void StartSingle();
void StartMulti();
void CheckButtonStates();
void CheckEmergencyStopState();
void CheckInputs();
float ParseNumber(char theCode,float theValue);
void ProcessGCommand();
void CheckSerial();
