#ifndef _MAIN_H
#define _MAIN_H

#include <Arduino.h>
#include "Order.h"
#include <SD.h>
#include "LibKeyboard.h"

void sendToGrbl(String);
String resizeForScreen(String, unsigned int);
void serialDrawAdd(String);
void initBasicSetupGRBL();
void upPositionSelectSD();
void downPositionSelectSD();
void pushPositionSelectSD();
void setup();
void loop();
void requestPosition();
String cuteGRBLstring(String , char ,int , bool );
void parse_and_store();
GRBLSTATUS findStatus(String );
String getStatus(GRBLSTATUS );
void clearDirectoryData();
bool addDirectoryData(String );
bool directoryData(File );
void printDirectory(File , int );
boolean returnfirstfile(File dir, File *FName);
bool initSDbasic();
void initSD();
void managementKeyboard(e_Button );
void managementEncoder();
void readSerialGRBL();

#endif