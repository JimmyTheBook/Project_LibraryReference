/*  Setup.cpp
 *   
 *    This file defines the setup functions for the GIGA Test Setup module.
 *
 */

// include the global file (which includes the Include file and Protos file)
#include    "Globals_m7.h"

/*-------------------------------------------
 * -- 	setup
 *-------------------------------------------
 *
 *	This routine runs at start up, and sets up needed functions
 *
 *	Return:
 *		None
 *
 *	Parameters:
 *		None
 */
void
setup()
{
    // initiate the variables
    v_SetupVariables_v();

    // initate the comms ports
    v_SetupComms_v();

    // setup display
    v_SetupDisplay_v();

    return;

}

/*-------------------------------------------
 * -- 	v_SetupVariables_v
 *-------------------------------------------
 *
 *	This routine initiates the variables for the program.
 *
 *	Return:
 *		None
 *
 *	Parameters:
 *		None
 */

void
v_SetupVariables_v()
{

    return;
}


/*-------------------------------------------
 * -- 	v_SetupComms_v
 *-------------------------------------------
 *
 *	This routine initiates the comms ports for the program
 *
 *	Return:
 *		None
 *
 *	Parameters:
 *		None
 */

 void
 v_SetupComms_v()
 {
    // Setup the debug port
    Serial.begin( COMMS_REMOTEPROTOCOL_DEFAULT_BAUD );


    // -----------------------------------------------------------// 
    //        Setup the comm port to the controller (laptop)
    //

    COMMS_REMOTEPROTOCOL_PARAMS struct_Temp = { 
        &Serial,                                            // pointer to the port
        &Serial,                                            // pointer to the DEBUG port
        COMMS_REMOTEPROTOCOL_PORT_TYPE_UART,                // port is UART
        COMMS_REMOTEPROTOCOL_DEFAULT_BAUD,                  // the speed to operate the port
        COMMS_DEBUG_OFF,                                    // the debug level
        COMMS_REMOTEPROTOCOL_COMMAND_OVERWRITE,             // flag that defines if commands are overwritten, or we wait
        COMMS_REMOTEPROTOCOL_DEFAULT_TIMEOUT,               // default timeout for commands
        COMMS_REMOTEPROTOCOL_DEFAULT_MAX_PAYLOAD_SIZE       // the maximum payload (which is also the maximum ASCII characted for ASCII commands)
    };

    // create the serial handler for the controlling computer
    gptr_classComms_Controller = new class_Comms_RemoteProtocol( struct_Temp );

    // initiate the comms port
    gptr_classComms_Controller->v_InitiatePort_v();

    // acknowledge the port
    COMMS_REMOTEPROTOCOL_HANDSHAKE struct_Handshake = { COMMS_HANDSHAKE_TYPE_ASCII, true, 10 };
    gptr_classComms_Controller->bool_Handshake_struct( struct_Handshake );

    return;
 }

