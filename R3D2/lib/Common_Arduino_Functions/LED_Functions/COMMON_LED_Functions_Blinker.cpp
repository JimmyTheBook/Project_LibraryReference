/*  COMMON_LED_Functions_Blinker.cpp
 *
 *  This is part of the COMMON_ARDUINOS_FUNCTIONS library.
 *    
 *    This file defines the class that creates an LED blinker. 
 *      This class uses the millis() function for timing.
 *
 *
 */

// include the header
#include "COMMON_LED_Functions_Blinker.h"

// include the Arduino library
#include <Arduino.h>


/*-------------------------------------------
 * -- 	class_LED_Blinker
 *-------------------------------------------
 *
 *	This constructor sets up the blinker class with no parameters.
 *
 *	Return:
 *		None
 *
 *	Parameters:
 *		struct_NewParams     = parameters for the blinker
 */

class_LED_Blinker::class_LED_Blinker()
{
    // do nothing
    return;
}


/*-------------------------------------------
 * -- 	class_LED_Blinker
 *-------------------------------------------
 *
 *	This constructor sets up the blinker class.
 *
 *	Return:
 *		None
 *
 *	Parameters:
 *		struct_NewParams     = parameters for the blinker
 */

class_LED_Blinker::class_LED_Blinker( struct_LED_Blinker_params struct_NewParams )
{
    // call the routine that sets up the blinker
    v_SetupParams_params( struct_NewParams );

    return;
}

/*-------------------------------------------
 * -- 	class_LED_Blinker (destructor)
 *-------------------------------------------
 *
 *	This destructor closes the blinker class.
 *
 *	Return:
 *		None
 *
 *	Parameters:
 *		None
 */


class_LED_Blinker::~class_LED_Blinker()
{
    return;
}


/*-------------------------------------------
 * -- 	v_SetupParams_params
 *-------------------------------------------
 *
 *	This rountine sets up the blinker by storing the parameters.
 *
 *	Return:
 *		None
 *
 *	Parameters:
 *		struct_NewParams     = parameters for the blinker
 */

void
class_LED_Blinker::v_SetupParams_params( struct_LED_Blinker_params struct_NewParams )
{
    // store the parameters
    struct_Params = struct_NewParams;

    // set the internal flag
    bool_isOn = LED_OFF;

    // set the output
    digitalWrite( struct_Params.uchar_LED_Pin, bool_isOn );

    // set the pin output
    pinMode( struct_Params.uchar_LED_Pin, OUTPUT );

    // set the current milli timer
    ulong_CurrentTimer = millis() + struct_Params.uint_OffTime;

    return;
}


/*-------------------------------------------
 * -- 	v_Service_BlinkLED_v
 *-------------------------------------------
 *
 *	This rountine blinks the LED.
 *      This routine is intended to be POLLED.
 *      To work properly, the routine needs to be called much faster than the blink time.
 *
 *	Return:
 *		None
 *
 *	Parameters:
 *		None
 */


void
class_LED_Blinker::v_Service_BlinkLED_v()
{
    // internal variables
    unsigned long   ulongTempMillis;

    // get the millis
    ulongTempMillis = millis();

    // if we are ready to change state
    if ( ulongTempMillis > ulong_CurrentTimer )
    {
        // if the LED is off...
        if ( bool_isOn == LED_OFF )
        {
            // set the state to ON
            bool_isOn = LED_ON;

            // reset the timer
            ulong_CurrentTimer = ulongTempMillis + struct_Params.uint_OnTime;
        }
        else
        {
            // set the state to OFF
            bool_isOn = LED_OFF;

            // reset the timer
            ulong_CurrentTimer = ulongTempMillis + struct_Params.uint_OffTime;
        }

        // set the output
        digitalWrite( struct_Params.uchar_LED_Pin, bool_isOn );
    }
    
    return;
}

