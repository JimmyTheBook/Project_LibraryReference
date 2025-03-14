/*  Display_Management.cpp
 *   
 *    This file manages the Nextion Display.
 *
 */


// include the global file (which includes the Include file and Protos file)
#include    "Globals_m7.h"


 /*-------------------------------------------*/
 /* -- 	v_SetupDisplay_v
  *-------------------------------------------
  *
  *	This routine initiates the display and sets it up.
  *
  *	Return:
  *		None
  *
  *	Parameters:
  *		None
  */

void
v_SetupDisplay_v()
{
    // DEBUG
    MAIN_DEBUG_IS_MED
    {
        // send debug info
        Serial.println( "Initialize display." );
    }

    // initiate the class
    NxtDsplyptr_MainDisplay = new Nextion_Display( &Serial1, &Serial, NEXTION_HW_DEBUG_HIGH );

    return;
}

/* -------------------------------------------
 * -- 	
 * -------------------------------------------
 *
 *	This routine initiates the Startup Screen, and loads the fields
 *
 *	Return:
 *		None
 *
 *	Parameters:
 *		None
 */