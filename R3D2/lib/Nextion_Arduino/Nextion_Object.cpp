/*  NextionObject.cpp
 *   
 *    This file implements the NextionObject class.
 *
 */

#ifndef    _NEXTION_OBJECT_CPP
#define    _NEXTION_OBJECT_CPP

// include the header file
#include    "Nextion_Object.h"


/*-------------------------------------------*/
/* -- 	Constructor
*/

NextionObject::NextionObject( byte byte_PID, byte byte_CID, const char * constcharptr_CNAME )
{
    // store the values
    byte_PageID = byte_PID;
    byte_ComponentID = byte_CID;
    constcharptr_ComponentName = constcharptr_CNAME;
}



/*-------------------------------------------*/
/* --   PrintParams
*/

void
NextionObject::PrintParams( HardwareSerial* hwsrptr_TargetPort )
{
    // send the info to the port
    hwsrptr_TargetPort->print( "[" );
    hwsrptr_TargetPort->print( (uint32_t)this );
    hwsrptr_TargetPort->print( ":" );
    hwsrptr_TargetPort->print( byte_PageID );
    hwsrptr_TargetPort->print( "," );
    hwsrptr_TargetPort->print( byte_ComponentID );
    hwsrptr_TargetPort->print( "," );
    
    // if the name isn't blank...
    if ( constcharptr_ComponentName )
    {
        // send the name
        hwsrptr_TargetPort->print( constcharptr_ComponentName );
    }
    else
    {
        hwsrptr_TargetPort->print( "(null)" );
    }

    hwsrptr_TargetPort->println( "]" );

    return;
}



/*-------------------------------------------*/
/* -- 	byte_GetPageID
*/

byte
NextionObject::byte_GetPageID()
{

    return 0;
}


/*-------------------------------------------*/
/* -- 	byte_GetComponentID
*/

byte
NextionObject::byte_GetComponentID()
{

    return 0;
}


/*-------------------------------------------*/
/* -- 	constcharptr_GetComponentName
*/

const char *
NextionObject::constcharptr_GetComponentName()
{

    return 0;
}
 


#endif