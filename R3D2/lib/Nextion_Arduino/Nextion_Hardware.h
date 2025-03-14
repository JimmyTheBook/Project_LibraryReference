/*  Nextion_Hardware.h
 *   
 *    This file defines the class structure used as the hardware interface to the Nextion display.
 *
 */

 
#ifndef    _NEXTION_HARDWARE_H
#define    _NEXTION_HARDWARE_H
 
#include "Arduino.h"

// include the Nextion files needed
#include "Nextion_TouchObjects.h"

// define the debug message states
#define NEXTION_HW_DEBUG_OFF       0
#define NEXTION_HW_DEBUG_LOW       1
#define NEXTION_HW_DEBUG_MEDIUM    3
#define NEXTION_HW_DEBUG_HIGH      7

// defaults
#define NEXTION_HW_RECV_BUFFER_SIZE     50
#define NEXTION_HW_TX_BUFFER_SIZE       75

// define the inline debug selection statements
#define NEXTION_HW_DEBUG_IS_LOW      if ( u8_DEBUGLevel >= NEXTION_HW_DEBUG_LOW )
#define NEXTION_HW_DEBUG_IS_MED      if ( u8_DEBUGLevel >= NEXTION_HW_DEBUG_MEDIUM )
#define NEXTION_HW_DEBUG_IS_HIGH     if ( u8_DEBUGLevel >= NEXTION_HW_DEBUG_HIGH )


/*-------------------------------------------
 *
 *		Nextion_Display class
 *          This class controls the hardware interface to the Nextion dispplay.
 *
 */

class Nextion_Display
{
    // public members
    public:

        /*-------------------------------------------*/
        /* -- 	Constructor
         *-------------------------------------------
         *
         *	This routine constructs the class
         *
         *	Return:
         *		None
         *
         *	Parameters:
         *		hwserialptr_NewCommPort = the serial port connected to the display
         *      hwserialptr_NewDEBUGPort = the serial port for sending DEBUG information
         *      u8_NewDEBUGLevel = the starting debug level 
         */

        Nextion_Display( HardwareSerial* hwserialptr_NewCommPort, HardwareSerial* hwserialptr_NewDEBUGPort = NULL, uint8_t u8_NewDEBUGLevel = 0 );

        /*-------------------------------------------*/
        /* -- 	Initialize
         *-------------------------------------------
         *
         *	This routine initiates the display
         *
         *	Return:
         *		None
         *
         *	Parameters:
         *		None
         */
        void Initialize();


        /*-------------------------------------------*/
        /* -- 	SendCommand
         *-------------------------------------------
         *
         *	This routine initiates the Startup Screen, and loads the fields
         *
         *	Return:
         *		None
         *
         *	Parameters:
         *		constcharptr_Command = pointer to the command to be sent
         */
        void SendCommand( const char* constcharptr_Command );


        /*-------------------------------------------*/
        /* -- 	ServiceReceive
         *-------------------------------------------
         *
         *	This routine services the RX side of communications.
         *      This is intended to be called constantly.
         *  The routine is a state machine, and starts recording on the first byte.
         *  Once a byte is received, further bytes are stored.
         *  For the Nextion Instruction set, the first byte recieved is the command identifying the type of response.
         *  After the initial byte (command byte), the routine constantly looks for the ending sequence (0xFF 0xFF 0xFF).
         *      When this sequence is encountered, the sequence is removed from the buffer and the flag is set that a transmission was received.
         * 
         *	Return:
         *		None
         *
         *	Parameters:
         *		None
         */

        void ServiceReceive();

    // private members
    private:

        /*-------------------------------------------
         *
         *		Private variables
         *
         */

        // pointer for the comms port
        HardwareSerial*     hwserial_CommunicationPort;

        // pointer for the DEBUG port
        HardwareSerial*     hwserial_DEBUGPort;

        // variable for debug level
        uint8_t    u8_DEBUGLevel;

        // buffer for incoming transmission strings
        uint8_t     u8arr_ReceiveBuffer[ NEXTION_HW_RECV_BUFFER_SIZE ];
        // buffer locator for receive
        uint8_t     u8_RecieveBufferIndex;
};

#endif
 