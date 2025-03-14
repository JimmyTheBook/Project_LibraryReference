/*  Service_Routines_m7.cpp
 *   
 *    This file manages the service routines in the m7 program.
 *
 */

// include the global file (which includes the Include file and Protos file)
#include    "Globals_m7.h"

void
v_Main_Service_Routine_v()
{
    // Servive the comm ports
    gptr_classComms_Controller->v_ServiceSerial_v();

    // Service the blinking LED loop
    class_LED_Blinker_BuiltIn.v_Service_BlinkLED_v();

    return;
}