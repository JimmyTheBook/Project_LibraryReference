/*  COMMON_Relay_Functions.cpp
 *
 *  This is part of the COMMON_ARDUINOS_FUNCTIONS library.
 *    
 *    This file defines the class that manages a relay output. 
 *
 *
 */

// include the header
#include "COMMON_Relay_Functions.h"


/*-------------------------------------------
 * -- 	class_Relay_Control (constructor)
 *-------------------------------------------
 *
 *	This constructor sets up the Relay class with no parameters.
 *
 *	Return:
 *		None
 *
 *	Parameters:
 *		None
 */

class_Relay_Control::class_Relay_Control()
{
    // do nothing
    return;
}


/*-------------------------------------------
 * -- 	class_Relay_Control (constructor)
 *-------------------------------------------
 *
 *	This constructor sets up the Relay class with parameters.
 *
 *	Return:
 *		None
 *
 *	Parameters:
 *		struct_NewParams     = parameters for the Relay
 */

class_Relay_Control::class_Relay_Control( struct_Relay_Control_params struct_NewParams )
{
    // call the routine that sets up the relay
    v_SetupParams_params( struct_NewParams );

    return;
}


/*-------------------------------------------
 * -- 	class_Relay_Control (destructor)
 *-------------------------------------------
 *
 *	This destructor closes the relay class.
 *
 *	Return:
 *		None
 *
 *	Parameters:
 *		None
 */


class_Relay_Control::~class_Relay_Control()
{
    return;
}


/*-------------------------------------------
 * -- 	v_SetupParams_params
 *-------------------------------------------
 *
 *	This rountine sets up the relay by storing the parameters.
 *
 *	Return:
 *		None
 *
 *	Parameters:
 *		struct_NewParams     = parameters for the relay
 */

void
class_Relay_Control::v_SetupParams_params( struct_Relay_Control_params struct_NewParams, bool bool_StartCLOSED )
{
    // store the parameters
    struct_Params = struct_NewParams;

    // set the relay controls
    RELAY_CLOSE = struct_Params.bool_isPinHighforRelayClosed;
    RELAY_OPEN = !RELAY_CLOSE;

    // set the pin output
    pinMode( struct_Params.uchar_LED_Pin, OUTPUT );

    // set the starting situation
    if ( bool_StartCLOSED )
        v_RelayClose_v();
    else
        v_RelayOpen_v();

    return;
}


/*-------------------------------------------
 * --   v_RelayState_bool
 *-------------------------------------------
 *
 *	This rountine sets the relay.
 *
 *	Return:
 *		None
 *
 *	Parameters:
 *		isClosed    = new state of relay
 */

void
class_Relay_Control::v_RelayState_bool( bool newState )
{
    // set the new state
    bool_CurrentState = newState;
    // set the output
    digitalWrite( struct_Params.uchar_LED_Pin, bool_CurrentState );

    return;
}


/*-------------------------------------------
 * --   v_RelayOpen_v
 *-------------------------------------------
 *
 *	This rountine opens the relay.
 *
 *	Return:
 *		None
 *
 *	Parameters:
 *		None
 */

void
class_Relay_Control::v_RelayOpen_v()
{
    // call the routine
    v_RelayState_bool( RELAY_OPEN );

    return;
}


/*-------------------------------------------
 * --   v_RelayClose_v
 *-------------------------------------------
 *
 *	This rountine closes the relay.
 *
 *	Return:
 *		None
 *
 *	Parameters:
 *		None
 */

void
class_Relay_Control::v_RelayClose_v()
{
    // call the routine
    v_RelayState_bool( RELAY_CLOSE );

    return;
}


/*-------------------------------------------
 * --   v_RelayToggle_v
 *-------------------------------------------
 *
 *	This rountine toggles the relay.
 *
 *	Return:
 *		None
 *
 *	Parameters:
 *		None
 */

void
class_Relay_Control::v_RelayToggle_v()
{
    // call the routine
    v_RelayState_bool( !bool_CurrentState );
    return;
}
