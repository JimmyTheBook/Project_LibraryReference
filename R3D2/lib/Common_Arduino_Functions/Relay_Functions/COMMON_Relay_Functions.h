/*  COMMON_Relay_Functions.h
 *
 *  This is part of the COMMON_ARDUINOS_FUNCTIONS library.
 *    
 *    This file defines the class that manages a relay output. 
 *
 *
 */

// only initate once
#ifndef _COMMON_Relay_Functions_h
#define _COMMON_Relay_Functions_h

// include the Arduino library
#include <Arduino.h>

//  define the struct for relay parameters
struct struct_Relay_Control_params
{
    unsigned char   uchar_LED_Pin;
    bool            bool_isPinHighforRelayClosed;
    unsigned int    uint_TransitionTime_msec;
};


//-------------------------------------------
// -- 	Define the Relay class

class class_Relay_Control
{
    public:
        // constructor
        class_Relay_Control();
        class_Relay_Control( struct_Relay_Control_params struct_NewParams );
        // destructor
        ~class_Relay_Control();

        // function to setup the Relay
        void v_SetupParams_params( struct_Relay_Control_params struct_NewParams, bool bool_StartCLOSED = false );

        // function to run the blinker
        void v_Service_BlinkLED_v();

        // routines to set the relay state
        void v_RelayState_bool( bool isClosed );
        void v_RelayOpen_v();
        void v_RelayClose_v();
        void v_RelayToggle_v();

    private:
        // basic structure for parameters
        struct_Relay_Control_params   struct_Params;

        // internal state of Relay
        bool    bool_CurrentState;

        // set the relay states (easier for code understanding)
        bool    RELAY_OPEN, RELAY_CLOSE;
};


#endif