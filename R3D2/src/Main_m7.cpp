/*  Main_m7.cpp
 *   
 *    This file defines the main software program for the GIGA Test Setup module.
 *
 */

//  Define this file as the MAIN_FILE
#define   MAIN_FILE

// include the global file (which includes the Include file and Protos file)
#include    "Globals_m7.h"

// Main program
void loop()
{
  static unsigned long sulong_Counter = millis();
  static unsigned int suint_Counter = 0;

  // put your main code here, to run repeatedly:

  // Service the loops
  v_Main_Service_Routine_v();

  // service the Display

}


