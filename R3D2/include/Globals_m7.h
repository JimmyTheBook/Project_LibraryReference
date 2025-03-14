/*  Globals_m7.h
 *   
 *    This file defines the global variables for the GIGA Test Setup m7 program.
 *
 */

// include the Defines
#include    "Defines.h"

// include the Includes
#include    "Includes_m7.h"

// include the Protos
#include    "Protos_m7.h"

// separate based on one file
#ifdef MAIN_FILE

//  Initiate the variables for the blinking LED

struct_LED_Blinker_params   struct_LED_Blinker_BuiltIn = {  LED_BUILTIN,
                                                            20,
                                                            20
};

class_LED_Blinker   class_LED_Blinker_BuiltIn( struct_LED_Blinker_BuiltIn );

/*-------------------------------------------
 *
 *		Variables for the Comms
 *
 */

class_Comms_RemoteProtocol*     gptr_classComms_Controller;


/*-------------------------------------------
 *
 *		Display variables
 *
 */

Nextion_Display*    NxtDsplyptr_MainDisplay;

#else

extern  struct_LED_Blinker_params   struct_LED_Blinker_BuiltIn;
extern  class_LED_Blinker   class_LED_Blinker_BuiltIn;

extern  class_Comms_RemoteProtocol*     gptr_classComms_Controller;

extern  Nextion_Display*    NxtDsplyptr_MainDisplay;

#endif
