/*  COMMON_LED_Functions_Blinker.h
 *
 *  This is part of the COMMON_ARDUINOS_FUNCTIONS library.
 *    
 *    This file defines the class that creates an LED blinker. 
 *      This class uses the millis() function for timing.
 *
 *
 */


// include the Arduino library
#include <Arduino.h>

// only initate once
#ifndef _COMMON_LED_Functions_Blinker_h
#define _COMMON_LED_Functions_Blinker_h


// defines
#define LED_OFF     false
#define LED_ON      true

//  define the struct for blinker parameters
struct struct_LED_Blinker_params
{
    unsigned char   uchar_LED_Pin;
    unsigned int    uint_OnTime;
    unsigned int    uint_OffTime;
};

#endif

//-------------------------------------------
// -- 	Define the Blinker class

class class_LED_Blinker
{
    public:
        // constructor
        class_LED_Blinker();
        class_LED_Blinker( struct_LED_Blinker_params struct_NewParams );
        // destructor
        ~class_LED_Blinker();

        // function to setup the blinker
        void v_SetupParams_params( struct_LED_Blinker_params struct_NewParams );

        // function to run the blinker
        void v_Service_BlinkLED_v();

    private:
        // basic structure for parameters
        struct_LED_Blinker_params   struct_Params;

        // internal counter
        unsigned long   ulong_CurrentTimer;

        // internal state of LED
        bool    bool_isOn;
};

