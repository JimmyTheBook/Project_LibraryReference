/*  Nextion_Hardware.cpp
 *   
 *    This file implements the NextionHardware class.
 *
 */


#ifndef    _NEXTION_HARDWARE_CPP
#define    _NEXTION_HARDWARE_CPP
 
// include the header file
#include "Nextion_Hardware.h"

// include the instruction file
#include "Nextion_Instructions.h"


/*-------------------------------------------*/
/* -- 	Constructor
*/

Nextion_Display::Nextion_Display( HardwareSerial* hwserialptr_NewCommPort, HardwareSerial* hwserialptr_NewDEBUGPort, uint8_t u8_NewDEBUGLevel )
{
    // store the comms port
    hwserial_CommunicationPort = hwserialptr_NewCommPort;
    
    // store the debug port
    hwserial_DEBUGPort = hwserialptr_NewDEBUGPort;

    // store the debug level
    u8_DEBUGLevel = u8_NewDEBUGLevel;

    // reset the receive buffer location
    u8_RecieveBufferIndex = 0;

    // send debug info
    NEXTION_HW_DEBUG_IS_MED
    {
        // send debug info
        hwserial_DEBUGPort->println( "Nextion display created." );
        hwserial_DEBUGPort->print( "Debug level: ");
        hwserial_DEBUGPort->println( u8_DEBUGLevel, DEC );
    }
    
    return;
}


/*-------------------------------------------*/
/* -- 	Initialize
*/

void
Nextion_Display::Initialize()
{

    return;
}


/*-------------------------------------------*/
/* -- 	SendCommand
*/

void
Nextion_Display::SendCommand( const char* constcharptr_Command )
{
    // first, flush the serial outgoing buffer
    hwserial_CommunicationPort->flush();

    // send the command
    hwserial_CommunicationPort->print( constcharptr_Command );

    NEXTION_HW_DEBUG_IS_MED
    {
        // send debug info
        hwserial_DEBUGPort->println( "Command sent to display." );
    }

    NEXTION_HW_DEBUG_IS_HIGH
    {
        // send debug info
        hwserial_DEBUGPort->println( constcharptr_Command );
    }

    // send the command ending sequence
    hwserial_CommunicationPort->write( NEXTION_CMD_END );
    hwserial_CommunicationPort->write( NEXTION_CMD_END );
    hwserial_CommunicationPort->write( NEXTION_CMD_END );

    return;
}


/*-------------------------------------------*/
/* -- 	ServiceReceive
*/

void Nextion_Display::ServiceReceive()
{
    uint8_t u8_Temp;

    if ( hwserial_CommunicationPort->available() )
    {
        u8_Temp = hwserial_CommunicationPort->read();

        NEXTION_HW_DEBUG_IS_HIGH
        {
            hwserial_DEBUGPort->print(" Display recv: 0x");
            hwserial_DEBUGPort->println( u8_Temp, HEX );
        }
    }

    return;
}


#endif
 