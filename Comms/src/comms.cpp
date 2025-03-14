/*  Class_Comms_RemoteProtocol.cpp
 *   
 *    This file defines the code and operation of the Class_Comms_RemoteProtocol class.
 *
 */

// include the header file
#include "comms.h"

// include the Arduino
#include <Arduino.h>

#include <iostream>
using namespace std;

/*-------------------------------------------
* -- 	Constructor
*/

class_Comms_RemoteProtocol::class_Comms_RemoteProtocol( COMMS_REMOTEPROTOCOL_PARAMS struct_NewParams )
{
    // store the params
    struct_Params = struct_NewParams;

    // set the command state
    uchar_CommandState = COMMAND_IDLE;

    // clear the command started flag
    struct_LocalCommand.bool_CommandStarted = false;

    Serial.println("Constructing protocol class.");

    return;
}


/*-------------------------------------------
 * -- 	v_InitiatePort_v
 */

void
class_Comms_RemoteProtocol::v_InitiatePort_v()
{
    // create local temp port pointers
    HardwareSerial* hwserial_Temp;

    // check to make sure there's a port
    if ( struct_Params.voidptr_Port != NULL )
    {
        // initaite based on the type of port
        switch( struct_Params.u8_PortType )
        {
            case COMMS_REMOTEPROTOCOL_PORT_TYPE_UART:
                // cast the port
                hwserial_Temp = (HardwareSerial *) struct_Params.voidptr_Port;

                // init the port
                hwserial_Temp->begin( struct_Params.uint_Speed );

                while ( hwserial_Temp )

            default:

            break;
        }
        
        // provide a delay so the hardware can initialize
        delay(15);

        COMMS_DEBUG_IS_LOW
        {
            // send debug message
            struct_Params.hwserial_DebugPort->println( "Port initiaited.");
            struct_Params.hwserial_DebugPort->print( "Port speed: ");
            struct_Params.hwserial_DebugPort->println( struct_Params.uint_Speed, DEC );
        }
    }
    return;
}


/*-------------------------------------------
 * -- 	bool_Handshake_struct
 */

bool
class_Comms_RemoteProtocol::bool_Handshake_struct( COMMS_REMOTEPROTOCOL_HANDSHAKE struct_HandshakeParams )
{
    // local variables
    bool    bool_AckGood = false;
    unsigned long   ulong_CurrentCounter;
    unsigned long   ulong_StartingCounter;

    // determine the handshake type...
    switch( struct_HandshakeParams.uchar_HandshakeType )
    {
        /*-------------------------------------------
         *		ASCII handshake
         */
        case COMMS_HANDSHAKE_TYPE_ASCII:

            COMMS_DEBUG_IS_MED
            {
                // send message
                struct_Params.hwserial_DebugPort->println("Being ASCII handshake...");
            }

            // set the starting timer to trigger an update...
            ulong_StartingCounter = millis() - 1001;

            while ( struct_HandshakeParams.uint_AckTimeout_sec > 0 )
            {
                // get the current counter
                ulong_CurrentCounter = millis();

                // every second provide an update
                ulong_CurrentCounter = millis();
                if ( ( ulong_CurrentCounter - ulong_StartingCounter) > 1000 )
                {
                    // store the message
                    strcpy( (char*) &(u8arr_TransmitBuffer[0]), "Handshake - send any byte to confirm... " );
                    // set the transmit lenght
                    v_SetTransmitLengthASCII_v();

                    // store the countdown value
                    v_LoadASCIINumberToArray_charptr_u8( (char*) &(u8arr_TransmitBuffer[ struct_LocalCommand.u8_TransmitBufferSize ]),
                                                         struct_HandshakeParams.uint_AckTimeout_sec );
                    // set the transmit lenght
                    v_SetTransmitLengthASCII_v();

                    // store the message
                    strcat( (char*) &(u8arr_TransmitBuffer[0]), " sec..." );
                    // set the transmit lenght
                    v_SetTransmitLengthASCII_v();

                    // send the message
                    v_SendData_bool( true );
                    
                    // decrement the counter
                    struct_HandshakeParams.uint_AckTimeout_sec--;

                    // reset the local counter
                    ulong_StartingCounter = ulong_CurrentCounter;
                }

                // look for response
                if ( bool_CheckReceiveData_u8ptr( &u8arr_TransmitBuffer[0] ) )
                {
                    // we've recieved the ACK
                    bool_AckGood = true;

                    // set the countdown timer to stop
                    struct_HandshakeParams.uint_AckTimeout_sec = 0;
                }

            }

            // send a response
            if ( bool_AckGood )
            {
                // store the message
                strcpy( (char*) &(u8arr_TransmitBuffer[0]), "Handchake CONFIRMED.  Port ready." );
            }
            else
            {
                // store the message
                strcpy( (char*) &(u8arr_TransmitBuffer[0]), "Handchake NOT RECEIVED.  Port may not be ready." );
            }


            // set the transmit lenght
            v_SetTransmitLengthASCII_v();

            // send the message
            v_SendData_bool( true );

            COMMS_DEBUG_IS_MED
            {
                // send message
                struct_Params.hwserial_DebugPort->print("Handshake ");

                // if we got an ACK...
                if ( bool_AckGood )
                    struct_Params.hwserial_DebugPort->println("RECEIVED.");
                else
                    struct_Params.hwserial_DebugPort->println("NOT RECEIVED.");
            }

        break;

        /*-------------------------------------------
         *		Undefined handshake type 
         */
        default:

        break;

    }

    // return the status of the handshake sequence
    return bool_AckGood;
}

/*-------------------------------------------
 * -- 	v_DebugMessages_bool
 *-------------------------------------------
 *
 *	This routine turns on / off the debug messages
 *      This can be used to turn on debug messages to a comm port monitored by a human.
 *
 *	Return:
 *		None
 *
 *	Parameters:
 *		ucharNewLevel   = value of the new debug level
                        = DEFAULT = 0 (turns off)
 */

void
class_Comms_RemoteProtocol::v_DebugMessages_uchar( unsigned char ucharNewLevel = 0 )
{
    // set the debug flag
    struct_Params.u8_DebugMessages = ucharNewLevel;

    return;
}

/*-------------------------------------------
 * -- 	v_ServiceSerial_v
 *-------------------------------------------
 *
 *	This routine monitors the comm and process bytes when they are received.
 *      This routine in intended to be called in a constant loop.
 *
 *	Return:
 *		None
 *
 *	Parameters:
 *		None
 */

void
class_Comms_RemoteProtocol::v_ServiceSerial_v()
{
    // local variables
    bool    bool_IsCommandComplete = false;
    static  unsigned long   ulong_TimeoutTimer;
    unsigned char uchar_NewByte;

    // get the current millis
    unsigned long ulong_CurrentMillis = millis();

    // check the comm port for bytes... (and get the byte if there is one)
    if ( bool_CheckReceiveData_u8ptr( &uchar_NewByte ) )
    {
        // reset the timer
        ulong_TimeoutTimer = ulong_CurrentMillis + struct_Params.uint_CommandTimeout;

        // send debug message
        COMMS_DEBUG_IS_HIGH
        {
            // send message
            struct_Params.hwserial_DebugPort->print( "Received byte: 0x" );
            struct_Params.hwserial_DebugPort->println( uchar_NewByte, HEX );
        }

        // send it to the state machine
        bool_IsCommandComplete = bool_CommandByte_uchar_bool( uchar_NewByte );
    }
    else    // no bytes
    {
        // check if we're still in a command...
        if ( struct_LocalCommand.bool_CommandStarted )
        {
            // if the timeout has timed out...
            if ( ulong_CurrentMillis >= ulong_TimeoutTimer )
            {
                // timeout has occurred...  set the command to complete
                bool_IsCommandComplete = true;

                // clear the command started flag
                struct_LocalCommand.bool_CommandStarted = false;

                // set the command to invalid
                struct_LocalCommand.bool_IsValid = false;

                // reset the state machine
                bool_CommandByte_uchar_bool( 0x00, true );

                // send debug message
                COMMS_DEBUG_IS_MED
                {
                    // send message
                    struct_Params.hwserial_DebugPort->println( "Command Timed out." );
                }

            }
        }

    }

    // if the command is complete...
    if ( bool_IsCommandComplete )
    {
        // send debug message
        COMMS_DEBUG_IS_MED
        {
            // send message
            struct_Params.hwserial_DebugPort->println( "Command receieved:" );
            struct_Params.hwserial_DebugPort->print( "     Command: 0x");
            struct_Params.hwserial_DebugPort->println( int(struct_LocalCommand.uchar_Command), HEX );
            struct_Params.hwserial_DebugPort->print( "     Payload size: ");
            struct_Params.hwserial_DebugPort->println( struct_LocalCommand.uint_PayloadSize );

            struct_Params.hwserial_DebugPort->print( "     ");
            for ( unsigned int i = 0; i < struct_LocalCommand.uint_PayloadSize ; i++ )
                struct_Params.hwserial_DebugPort->write( u8arr_ReceiveBuffer[i] );

            struct_Params.hwserial_DebugPort->println( "" );

            struct_Params.hwserial_DebugPort->print( "    Checksum: " );
            // if it's invalid, add the "IN"...
            if ( !struct_LocalCommand.bool_IsValid )
                struct_Params.hwserial_DebugPort->print( "IN" );
            // add the "VALID"
            struct_Params.hwserial_DebugPort->println( "VALID" );
        }

        // send the ACK
        v_SendAck_v();

        // send debug message
        COMMS_DEBUG_IS_LOW
        {
            // send message
            struct_Params.hwserial_DebugPort->println( "ACK sent." );
        }

        // process the command
        v_ProcessCommand_v();

        // reset the command start flag
        struct_LocalCommand.bool_CommandStarted = false;

        // send debug message
        COMMS_DEBUG_IS_LOW
        {
            // send message
            struct_Params.hwserial_DebugPort->println( "Command processed." );
            struct_Params.hwserial_DebugPort->println( "" );
        }
    }

    return;
}

/*-------------------------------------------
 * -- 	bool_CommandByte_uchar_bool
 *-------------------------------------------
 *
 *	This routine receives bytes and attempts to build a command.
 *      It determines whether the byte starts a new command based on 3 things:
 *          Local variable that indicates a command has started
 *          Time since the last byte (if it's bigger than expected, it's starting new)
 *      When it determines the last byte has been received it calculate the checksum and returns if things were successful
 *
 *	Return:
 *		bool    = TRUE is the command is complete and accurate
 *
 *	Parameters:
 *		uchar_NewByte = the byte to add to the command
 */

bool
class_Comms_RemoteProtocol::bool_CommandByte_uchar_bool( unsigned char uchar_NewByte, bool bool_Reset )
{
    // local variables
    bool    bool_isCommandComplete = false;
    bool    bool_SkipCommand = false;
    static  unsigned int    suint_PayloadPointerIndex;
    static  unsigned int    suint_IgnoreCommand_Size;

    // check if we need to reset the state machine
    if ( bool_Reset )
    {
        // set the state machine to reset itself
        uchar_CommandState = COMMAND_RESET_STATE_MACHINE;
    }

    // send debug message
    COMMS_DEBUG_IS_HIGH
    {
        // send message
        struct_Params.hwserial_DebugPort->print( "Command Byte State: " );
        struct_Params.hwserial_DebugPort->println( uchar_CommandState, HEX );
    }


    //    
    // -----------------------------------------------------------// 
    // 		Process the byte through the command state machine
    //    

    // based on the state of the machine...
    switch( uchar_CommandState )
    {

        // -----------------------------------------------------------// 
        // 		STATE - COMMAND_IDLE
        //          starting point of the state machine
        //
        
        case COMMAND_IDLE:  // this is the first byte
            // FIRST BYTE = Command

            // determine the overwrite mode
            //  if we're NOT overwriting commands...
            if ( struct_Params.bool_OverwriteCommands == false )
            {
                // check if there's a command waiting...
                if ( struct_LocalCommand.bool_IsValid == false )
                {
                    // command waiting, ignore current command
                    bool_SkipCommand = true;

                    // set the state to ignore the bytes
                    uchar_CommandState = COMMAND_IGNORE_COMMAND;

                    // send debug message
                    COMMS_DEBUG_IS_MED
                    {
                        // send message
                        struct_Params.hwserial_DebugPort->println( "First byte recieved, but command in progress.  Ignoring bytes." );
                    }

                }
            }

            // if we're good to get the command...
            if ( bool_SkipCommand == false )
            {
                // set the flag that a command started
                struct_LocalCommand.bool_CommandStarted = true;

                // send debug message
                COMMS_DEBUG_IS_HIGH
                {
                    // send message
                    struct_Params.hwserial_DebugPort->println( "Command started." );
                }
                
                //  store the command
                struct_LocalCommand.uchar_Command = uchar_NewByte;

                // determine the next state based on the command...
                switch ( struct_LocalCommand.uchar_Command )
                {
                    case COMMS_COMMAND_ASCII:
                        // set the payload size (in this case, the MAX)
                        struct_LocalCommand.uint_PayloadSize = struct_Params.uint_MaxPayloadSize;

                        // send debug message
                        COMMS_DEBUG_IS_MED
                        {
                            // send message
                            struct_Params.hwserial_DebugPort->print( "ASCII Command Received.  Max size: " );
                            struct_Params.hwserial_DebugPort->print( struct_LocalCommand.uint_PayloadSize, DEC );
                            struct_Params.hwserial_DebugPort->println( " characters." );
                        }

                        // setup the payload
                        v_ReadyPayload_v();

                        // set the payload pointer location
                        suint_PayloadPointerIndex = 0;


                        // ASCII command... there won't be a payload size byte, go directly to storing bytes
                        uchar_CommandState = COMMAND_GET_PAYLOAD;


                    break;

                    default:        // all other commands
                        // update the state
                        uchar_CommandState = COMMAND_PAYLOAD_SIZE;
                    break;

                }

                // send debug message
                COMMS_DEBUG_IS_MED
                {
                    // send message
                    struct_Params.hwserial_DebugPort->print( "Command received: 0x" );
                    struct_Params.hwserial_DebugPort->println( struct_LocalCommand.uchar_Command, HEX );
                }

            }
        break;

        // -----------------------------------------------------------// 
        // 		STATE - COMMAND_PAYLOAD_SIZE
        //          For non-ASCII commands, this byte is the number of bytes in the payload
        //          This is skipped for ASCII commands.
        //
        
        case COMMAND_PAYLOAD_SIZE:  // second byte
            // SECOND BYTE = Payload size

            //  check the size
            if ( (unsigned int) uchar_NewByte > COMMS_REMOTEPROTOCOL_DEFAULT_MAX_PAYLOAD_SIZE )
            {
                // error trap - payload size too big
                // reset to max
                uchar_NewByte = COMMS_REMOTEPROTOCOL_DEFAULT_MAX_PAYLOAD_SIZE;

                // send debug message
                COMMS_DEBUG_IS_MED
                {
                    // send message
                    struct_Params.hwserial_DebugPort->println( "ERROR payload too large, setting to default." );
                }
            }

            // store the payload size
            struct_LocalCommand.uint_PayloadSize = uchar_NewByte;

            // setup the payload
            v_ReadyPayload_v();
 
            // set the payload pointer location
            suint_PayloadPointerIndex = 0;

            // go to the next state
            uchar_CommandState = COMMAND_GET_PAYLOAD;
            
        break;

        // -----------------------------------------------------------// 
        // 		STATE - COMMAND_GET_PAYLOAD
        //          This state receives and stores the data bytes.
        //          For ASCII commands, it looks for the CR / LF to end the command
        //          For non-ASCII commands, it's looking to recevie the number of bytes from the payload size
        //
        
        case COMMAND_GET_PAYLOAD:
            // Each byte is the next in the payload

            // store the byte
            u8arr_ReceiveBuffer[ suint_PayloadPointerIndex ] = uchar_NewByte;

            // send debug message
            COMMS_DEBUG_IS_HIGH
            {
                // send message
                struct_Params.hwserial_DebugPort->print( "Payload byte stored: 0x" );
                struct_Params.hwserial_DebugPort->println(  u8arr_ReceiveBuffer[ suint_PayloadPointerIndex ], HEX );
            }

            // go to the next byte
            suint_PayloadPointerIndex++;

            // check if we've filled the payload...
            if ( suint_PayloadPointerIndex >= struct_LocalCommand.uint_PayloadSize )
            {
                // next state determined by command type...
                if ( struct_LocalCommand.uchar_Command == COMMS_COMMAND_ASCII )
                {
                    // payload is now full... go to the next state
                    uchar_CommandState = COMMAND_IDLE;

                    // return that the command is done
                    bool_isCommandComplete = true;
                    
                    // command is not valid (yet, the command will be checked next), we've hit the end of the payload
                    struct_LocalCommand.bool_IsValid = false; 

                    // send debug message
                    COMMS_DEBUG_IS_HIGH
                    {
                        // send message
                        struct_Params.hwserial_DebugPort->println( "Payload complete." );
                    }
                }
                else    // non-ASCII command
                {
                    // go to the next state
                    uchar_CommandState = COMMAND_CHECKSUM;

                    // send debug message
                    COMMS_DEBUG_IS_HIGH
                    {
                        // send message
                        struct_Params.hwserial_DebugPort->println( "Payload complete." );
                    }
                }
            }


            // if we're an ASCII command, recheck if we're done...
            if ( struct_LocalCommand.uchar_Command == COMMS_COMMAND_ASCII )
            {
                // ASCII command... look for the LINE FEED (which would be the last)
                if ( u8arr_ReceiveBuffer[ suint_PayloadPointerIndex - 1 ] == COMMS_LF )
                {
                    // if the byte before it is the CR...
                    if ( u8arr_ReceiveBuffer[ suint_PayloadPointerIndex - 2 ] == COMMS_CR )
                    {
                        // return that the command is done and valid (all properly ended ASCII commands are valid)
                        bool_isCommandComplete = true;
                        
                        // set the struct valid flag
                        struct_LocalCommand.bool_IsValid = true;

                        // set the payload size to the number of characters received
                        struct_LocalCommand.uint_PayloadSize = suint_PayloadPointerIndex - 2;

                        // reset back to the idle state
                        uchar_CommandState = COMMAND_IDLE;
                        
                        // send debug message
                        COMMS_DEBUG_IS_MED
                        {
                            // send message
                            struct_Params.hwserial_DebugPort->println( "ASCII Command completed." );
                        }
                        COMMS_DEBUG_IS_HIGH
                        {
                            // send message
                            struct_Params.hwserial_DebugPort->print( "Number of characters: " );
                            struct_Params.hwserial_DebugPort->println( struct_LocalCommand.uint_PayloadSize );
                        }
                    }
                }
            }
        break;

        // -----------------------------------------------------------// 
        // 		STATE - COMMAND_CHECKSUM
        //          This state receives the checksum and verifies it.

        case COMMAND_CHECKSUM:
            // CHECKSUM

            // store the checksum byte
            struct_LocalCommand.uchar_Checksum = uchar_NewByte;

            // check the checksum
            v_ChecksumVerification_v();

            // return that the command is done and valid
            bool_isCommandComplete = true;

            // reset back to the idle state
            uchar_CommandState = COMMAND_IDLE;
            
            // send debug message
            COMMS_DEBUG_IS_MED
            {
                // send message
                struct_Params.hwserial_DebugPort->print( "Checksum: 0x" );
                struct_Params.hwserial_DebugPort->println( struct_LocalCommand.uchar_Checksum, HEX );
            }

            // send debug message
            COMMS_DEBUG_IS_LOW
            {                
                // give the checksum result
                struct_Params.hwserial_DebugPort->print( "Check was verified as " );

                // if it's invalid, add the "IN"...
                if ( !struct_LocalCommand.bool_IsValid )
                    struct_Params.hwserial_DebugPort->print( "IN" );

                // add the "VALID"
                struct_Params.hwserial_DebugPort->println( "VALID" );
            }

        break;

        // -----------------------------------------------------------// 
        // 		STATE - COMMAND_IGNORE_COMMAND
        //          This state is used when we're ignoring a command

        case COMMAND_IGNORE_COMMAND:
            // we need to ignore this command

            // this byte is still the size, so store it (and add the checksum byte)
            suint_IgnoreCommand_Size = (unsigned int) uchar_NewByte + 1;

            // go to the next state
            uchar_CommandState = COMMAND_IGNORE_BYTES;
        break;

        // -----------------------------------------------------------// 
        // 		STATE - COMMAND_IGNORE_BYTES
        //          This state does not store bytes being recieved.

        case COMMAND_IGNORE_BYTES:
            // ignore these bytes, but keep count
            //  decrement the count
            suint_IgnoreCommand_Size--;

            // if we're done
            if ( suint_IgnoreCommand_Size == 0 )
            {
                // reset back to the idle state
                uchar_CommandState = COMMAND_IDLE;                
            }
        break;

        // -----------------------------------------------------------// 
        // 		STATE - COMMAND_RESET_STATE_MACHINE
        //          This state resets the state machine (like after a timeout)

        case COMMAND_RESET_STATE_MACHINE:
            // need to reset the state machine, start with the struct
            struct_LocalCommand.bool_CommandStarted = false;

            // set the state to idle for the next pass
            uchar_CommandState = COMMAND_IDLE;
        break;

    }
    
    //
    // -----------------------------------------------------------// 

    return bool_isCommandComplete;
}


/*-------------------------------------------
 * -- 	v_SetTimeout_uint
 *-------------------------------------------
 *
 *	This routine changes the command timeout
 *
 *	Return:
 *		None
 *
 *	Parameters:
 *		uint_NewTimeout = the new timeout value
 */

void
class_Comms_RemoteProtocol::v_SetTimeout_uint( unsigned int uint_NewTimeout )
{
    // store the new timeout
    struct_Params.uint_CommandTimeout = uint_NewTimeout;

    return;
}

/*-------------------------------------------
 * -- 	v_SendAck_v
 *-------------------------------------------
 *
 *	This routine sends the ACK command.
 *
 *	Return:
 *		None
 *
 *	Parameters:
 *		None
 */

void
class_Comms_RemoteProtocol::v_SendAck_v()
{
    COMMS_DEBUG_IS_HIGH
    {
        // send debug
        struct_Params.hwserial_DebugPort->println( "Starting ACK process...");
    }

    // the type of ACK depends on the type of command...
    switch ( struct_LocalCommand.uchar_Command )
    {
        case COMMS_COMMAND_ASCII:
            // determine what to send for checksum validity
            if ( struct_LocalCommand.bool_IsValid )
            {
                // load the string
                strcpy( (char *) &(u8arr_TransmitBuffer[0]), "Acknowledge - VALID command received." );
            }
            else    // invalid checksum
            {
                // load the string
                strcpy( (char *) &(u8arr_TransmitBuffer[0]), "Acknowledge - NOT VALID command received." );
            }

            // set the buffer size
            v_SetTransmitLengthASCII_v();

            COMMS_DEBUG_IS_HIGH
            {
                // send debug
                struct_Params.hwserial_DebugPort->println( "Sending ACK for ASCII command.");
                struct_Params.hwserial_DebugPort->print( "Size of string: ");
                struct_Params.hwserial_DebugPort->println( struct_LocalCommand.u8_TransmitBufferSize, DEC );
            }
        break;


        default:        // non-ASCII command

            COMMS_DEBUG_IS_HIGH
            {
                // send debug
                struct_Params.hwserial_DebugPort->println( "ACKing a non-ASCII command...");
            }

            // start the ACK
            u8arr_TransmitBuffer[0] = COMMS_COMMAND_ACK;
            // repeat the payload size
//            u8arr_TransmitBuffer[1] = struct_LocalCommand.uint_PayloadSize;

            // determine what to send for checksum validity
            if ( struct_LocalCommand.bool_IsValid )
            u8arr_TransmitBuffer[2] = COMMS_CHECKSUM_VALID;
            else
            u8arr_TransmitBuffer[2] = COMMS_CHECKSUM_INVALID;

            // finish with the ACK
            u8arr_TransmitBuffer[3] = COMMS_COMMAND_ACK;

        break;
    }

    // send the message
    v_SendData_bool( true );

    return;
}


/* -------------------------------------------
* -- 	v_SendData_bool 
*/

void
class_Comms_RemoteProtocol::v_SendData_bool( bool bool_SendNewLine )
{
    HardwareSerial * hwserial_Temp;

    COMMS_DEBUG_IS_MED
    {
        // send debug message
        struct_Params.hwserial_DebugPort->println( "Starting data transfer...");
    }

    // the write method is based on the port...
    switch( struct_Params.u8_PortType )
    {
        case COMMS_REMOTEPROTOCOL_PORT_TYPE_UART:

            COMMS_DEBUG_IS_MED
            {
                // send debug message
                struct_Params.hwserial_DebugPort->println( "Sending transmit buffer to UART...");
            }

            COMMS_DEBUG_IS_HIGH
            {
                // send debug message
                struct_Params.hwserial_DebugPort->print( "Size of string = ");
                struct_Params.hwserial_DebugPort->println( struct_LocalCommand.u8_TransmitBufferSize );                
            }

            // create a temp pointer
            hwserial_Temp = (HardwareSerial *) ( struct_Params.voidptr_Port );

            // the transmit buffer
            for ( int i=0; i < struct_LocalCommand.u8_TransmitBufferSize ; i++ )
                hwserial_Temp->write( u8arr_TransmitBuffer[ i ] );
            
            // if the newline is set...
            if ( bool_SendNewLine )
                hwserial_Temp->println("");

        break;

        default:
            // port not specified
            COMMS_DEBUG_IS_LOW
            {
                // send debug message
                struct_Params.hwserial_DebugPort->print("ERROR - attemped to write to port type: 0x");
                struct_Params.hwserial_DebugPort->println( struct_Params.u8_PortType, HEX );
            }

        break;
    }

    // reset the transmit buffer pointer to the beginning
    struct_LocalCommand.u8_TransmitBufferSize = 0;

    COMMS_DEBUG_IS_MED
    {
        // send debug message
        struct_Params.hwserial_DebugPort->println( "Data sent.");
    }

    return;
}


/* -------------------------------------------
* -- 	bool_CheckReceiveData_u8ptr
*/

bool
class_Comms_RemoteProtocol::bool_CheckReceiveData_u8ptr( uint8_t * u8ptr_TargetByte )

{
    // local variable
    bool    bool_IsData = false;
    HardwareSerial *    hwserial_Temp;

    // check for data based on the port type
    switch ( struct_Params.u8_PortType )
    {
        case COMMS_REMOTEPROTOCOL_PORT_TYPE_UART:
            // assign the port
            hwserial_Temp = (HardwareSerial *) struct_Params.voidptr_Port;

            // check for bytes
            bool_IsData = hwserial_Temp->available();

            // if a byte is available...
            if ( bool_IsData )
            {
                // store the byte
                *u8ptr_TargetByte = hwserial_Temp->read();
            }
        break;

        default:

        break;
    }

    return bool_IsData; 
}


/*-------------------------------------------
 * -- 	v_ChecksumVerification_v
 *-------------------------------------------
 *
 *	This routine checks the checksum and sets the flag
 *
 *	Return:
 *		None
 *
 *	Parameters:
 *		None
 */

void
class_Comms_RemoteProtocol::v_ChecksumVerification_v()
{
    // clear the flag to start
    struct_LocalCommand.bool_IsValid = false;

    // determine the checksum based on the command
    switch( struct_LocalCommand.uchar_Command )
    {
        case    COMMS_COMMAND_PING:
        case    COMMS_COMMAND_DEBUG_LEVEL:
            // checksum should match the command
            if ( struct_LocalCommand.uchar_Checksum == struct_LocalCommand.uchar_Command )
                // they match, set the checksum flag
                struct_LocalCommand.bool_IsValid = true;
            break;

        default:
            // command not recognized, leave checksum as true
            struct_LocalCommand.bool_IsValid = false;
        break;
    }

    return;
}

/*-------------------------------------------
 * -- 	v_ProcessCommand_v
 *-------------------------------------------
 *
 *	This routine processes the command received.
 *
 *	Return:
 *		None
 *
 *	Parameters:
 *		None
 */

void
class_Comms_RemoteProtocol::v_ProcessCommand_v()
{
    // if the command is an ASCII command...
    if ( struct_LocalCommand.uchar_Command == COMMS_COMMAND_ASCII )
    {
        // first convert the ASCII command to a non-ASCII command
        v_ConvertAsciiCommand_v();
    }

    // verify the command and checksum is valid...
    if ( struct_LocalCommand.bool_IsValid )
    {
        // decide what to do based on the command...
        switch( struct_LocalCommand.uchar_Command )
        {
            case    COMMS_COMMAND_PING:  // pinged, just ack
                COMMS_DEBUG_IS_LOW
                {
                    // send message
                    struct_Params.hwserial_DebugPort->println( "PING acknowledged." );
                }
            break;

            case    COMMS_COMMAND_DEBUG_LEVEL:  // change the debug level
                // check if we're just asking for current value...
                if ( u8arr_ReceiveBuffer[0] == 255 )
                {
                    // send debug message
                    COMMS_DEBUG_IS_HIGH
                    {
                        // send message
                        struct_Params.hwserial_DebugPort->println( "Request for current debug level." );
                    }

                    // set response
                    strcpy( (char*) u8arr_TransmitBuffer, "Debug level = " );

                    // get the length of the string
                    uint8_t u8_Temp = strlen( (char*) u8arr_TransmitBuffer );

                    // add the value
                    v_LoadASCIINumberToArray_charptr_u8( (char*) &( u8arr_TransmitBuffer[ u8_Temp ] ), struct_Params.u8_DebugMessages );

                    // set the buffer size
                    struct_LocalCommand.u8_TransmitBufferSize = strlen( (char*) u8arr_TransmitBuffer );

                    v_SendData_bool( true );

                }
                else    // setting new level 
                {
                    // change the debug messaging level
                    struct_Params.u8_DebugMessages = u8arr_ReceiveBuffer[0];

                    // send debug message
                    COMMS_DEBUG_IS_LOW
                    {
                        // send message
                        struct_Params.hwserial_DebugPort->print( "Debug messaging level changed to: " );
                        struct_Params.hwserial_DebugPort->println( struct_Params.u8_DebugMessages, DEC );
                    }
                }
            break;

            default:
                // send debug message
                COMMS_DEBUG_IS_MED
                {
                    // send message
                    struct_Params.hwserial_DebugPort->print( "Unknown command recieved: " );
                    struct_Params.hwserial_DebugPort->println( struct_LocalCommand.uchar_Command, HEX );
                }
            break;
        }

        // reset the valid flag 
        struct_LocalCommand.bool_IsValid = false;
    }

    return;
}


/*-------------------------------------------
 * -- 	v_ConvertAsciiCommand_v
 *-------------------------------------------
 *
 *	This routine takes an ASCII command and buffer and changes the command struct to a non-ASCII command and payload.
 *
 *	Return:
 *		None
 *
 *	Parameters:
 *		None
 */

 void
 class_Comms_RemoteProtocol::v_ConvertAsciiCommand_v()
 {
    // local variables
    unsigned char uchar_StringPositionPointer;

    // send debug message
    COMMS_DEBUG_IS_MED
    {
        // send message
        struct_Params.hwserial_DebugPort->println( "Converting ASCII command..." );
        struct_Params.hwserial_DebugPort->print( "String size: ");
        struct_Params.hwserial_DebugPort->println( struct_LocalCommand.uint_PayloadSize, DEC );

        struct_Params.hwserial_DebugPort->print( "String :");
        for ( unsigned int i = 0; i < struct_LocalCommand.uint_PayloadSize ; i++ )
            struct_Params.hwserial_DebugPort->write( u8arr_ReceiveBuffer[ i ] );

        struct_Params.hwserial_DebugPort->println( "" );

    }

    // end the string properly
    u8arr_ReceiveBuffer[ struct_LocalCommand.uint_PayloadSize ] = 0x00;

    // send debug message
    COMMS_DEBUG_IS_HIGH
    {
        // send message
        struct_Params.hwserial_DebugPort->print( "ASCII Command: " );
        struct_Params.hwserial_DebugPort->println( (char*) &(u8arr_ReceiveBuffer[0]) );
    }

    // reset the found command index
    unsigned char uchar_CommandIndex = COMMS_REMOTEPROTOCOL_ASCII_CMD_INVALID;

    // try to find the ASCII command in the list...
    for ( unsigned int x=0; x < COMMS_REMOTEPROTOCOL_NUMBER_ASCII_COMMANDS; x++ )
    {
        // send debug message
        COMMS_DEBUG_IS_HIGH
        {
            // send message
            struct_Params.hwserial_DebugPort->print( "Compare: " );
            struct_Params.hwserial_DebugPort->println( gCOMMS_REMOTEPROTOCOL_ASCII_COMMANDS[ x ] );
        }

        // reset the compare boolean
        bool    bool_DoesMatch = true;
        // reset the compare pointer
        uchar_StringPositionPointer = 0;

        while ( gCOMMS_REMOTEPROTOCOL_ASCII_COMMANDS[ x ][ uchar_StringPositionPointer ] != 0x00 )
        {
            // if the character don't match...
            if ( gCOMMS_REMOTEPROTOCOL_ASCII_COMMANDS[ x ][ uchar_StringPositionPointer ] != u8arr_ReceiveBuffer[ uchar_StringPositionPointer ] )
                // clear the flag
                bool_DoesMatch = false;

            // go to the next character
            uchar_StringPositionPointer++;
        }

        // if this is a match...
        if ( bool_DoesMatch )
        {
            // store the command index
            uchar_CommandIndex = x;

            // reset the index so we don't search anymore
            x = COMMS_REMOTEPROTOCOL_NUMBER_ASCII_COMMANDS + 1;
        }

        // send debug message
        COMMS_DEBUG_IS_HIGH
        {
            // send message
            struct_Params.hwserial_DebugPort->print( "Command: " );

            if ( bool_DoesMatch )
                struct_Params.hwserial_DebugPort->println( "match." );
            else
                struct_Params.hwserial_DebugPort->println( "does not match." );

            // send message
            struct_Params.hwserial_DebugPort->print( "Command index: " );
            struct_Params.hwserial_DebugPort->println( uchar_CommandIndex, DEC );
        }
    }

    // based on the command found...
    switch( uchar_CommandIndex )
    {
        /*-------------------------------------------
         *		COMMS_REMOTEPROTOCOL_ASCII_CMD_INVALID
         *          Not a valid command, send notice.
         */
        case COMMS_REMOTEPROTOCOL_ASCII_CMD_INVALID:
            // did not find a command, so invalid

        break;

        /*-------------------------------------------
         *		COMMS_REMOTEPROTOCOL_ASCII_CMD_PING
         *          Ping... set the command
         */
        case COMMS_REMOTEPROTOCOL_ASCII_CMD_PING:
            // ping command... set the command
            struct_LocalCommand.uchar_Command = COMMS_COMMAND_PING;
            
            // set the command to valid
            struct_LocalCommand.bool_IsValid = true;

        break;

        /*-------------------------------------------
         *		COMMS_REMOTEPROTOCOL_ASCII_CMD_DEBUG_LEVEL
         *          Debug level request
         *              If the level is blank or 255... return the current level 
         *              If the level is anything else... set the new level
         */
        case COMMS_REMOTEPROTOCOL_ASCII_CMD_DEBUG_LEVEL:
            // change the debug level... need to get it from the payload

            // send debug message
            COMMS_DEBUG_IS_HIGH
            {
                // send message
                struct_Params.hwserial_DebugPort->println( "Processing ASCII Command to change debug level..." );
            }

            // clear the new level value
            unsigned char uchar_NewLevel = 0x00;

            // search until we hit the end of the payload...
            while ( uchar_StringPositionPointer <= struct_LocalCommand.uint_PayloadSize )
            {
                // get the character
                unsigned char uchar_NewChar = u8arr_ReceiveBuffer[ uchar_StringPositionPointer ];

                // if the character is a space...
                if ( uchar_NewChar == ' ' )
                {
                    // do nothing
                }
                else if ( ( uchar_NewChar >= '0' ) && ( uchar_NewChar <= '9' ) )
                {
                    // it's a number... first move the last digit forward
                    uchar_NewLevel = uchar_NewLevel * 10;

                    // store the new value
                    uchar_NewLevel = uchar_NewLevel + ( int( uchar_NewChar) - COMMS_NUMBER_OFFSET );
                }

                // go to the next character
                uchar_StringPositionPointer++;
            }

            // set the first value in the payload as the new value
            u8arr_ReceiveBuffer[0] = uchar_NewLevel;

            // send debug message
            COMMS_DEBUG_IS_HIGH
            {
                // send message
                struct_Params.hwserial_DebugPort->print( "New value: " );
                struct_Params.hwserial_DebugPort->println( (int)uchar_NewLevel, DEC );
            }

            // debug command... set the command
            struct_LocalCommand.uchar_Command = COMMS_COMMAND_DEBUG_LEVEL;

            // set the command to valid
            struct_LocalCommand.bool_IsValid = true;
            
        break;

    }

    return;
 }


 /*-------------------------------------------
  * -- 	v_ReadyPayload_v
  *-------------------------------------------
  *
  *	This routine gets the payload pointer ready to accept the payload.
  *
  *	Return:
  *		None
  *
  *	Parameters:
  *		None
  */

void
class_Comms_RemoteProtocol::v_ReadyPayload_v()
{
    // send debug message
    COMMS_DEBUG_IS_HIGH
    {
        // send message
        struct_Params.hwserial_DebugPort->print( "Expected payload size: ");
        struct_Params.hwserial_DebugPort->println( struct_LocalCommand.uint_PayloadSize );
    }

    // if the payload size is 0 then skip the payload...
    if ( struct_LocalCommand.uint_PayloadSize == 0 )
    {
        // go to the next state
        uchar_CommandState = COMMAND_CHECKSUM;
        
        // send debug message
        COMMS_DEBUG_IS_HIGH
        {
            // send message
            struct_Params.hwserial_DebugPort->println( "No payload, move to checksum." );
        }
    }
    
    return;
}


/* -------------------------------------------
* -- 	v_LoadASCIINumberToArray_charptr_u8
*/

void
class_Comms_RemoteProtocol::v_LoadASCIINumberToArray_charptr_u8( char* charptr_Target, uint8_t u8_Value )
{

    COMMS_DEBUG_IS_HIGH
    {
        // send message
        struct_Params.hwserial_DebugPort->print( "Adding number to string = " );
        struct_Params.hwserial_DebugPort->println( u8_Value, DEC );
    }

    // determine how many digits we'll need
    uint8_t u8_Digits = 0;
    uint8_t u8_TempValue = u8_Value;

    // while there's still more digits to find...
    while( u8_TempValue > 0 )
    {
        // increase the digits
        u8_Digits++;
        // remove a digit from the value
        u8_TempValue /= 10;
    }

    // if the value was 0 (digits will still be 0)...
    if ( u8_Digits == 0 )
        // set the numebr of digits to 1
        u8_Digits = 1;

    // store the digits
    for ( uint8_t x=0; x < u8_Digits; x++ )
    {
        // store the current digit
        *( charptr_Target + u8_Digits - x - 1 ) = u8_Value % 10 + 48;

        COMMS_DEBUG_IS_HIGH
        {
            // send message
            struct_Params.hwserial_DebugPort->print( "Adding digit to string = 0x" );
            struct_Params.hwserial_DebugPort->println( *( charptr_Target + u8_Digits - x - 1 ), HEX );
            struct_Params.hwserial_DebugPort->print( "Location = " );
            struct_Params.hwserial_DebugPort->println( u8_Digits - x, DEC );
        }
    
        // remove the last digit
        u8_Value /= 10;
    }

    // add the end string
    *( charptr_Target + u8_Digits ) = 0;

    return;

}


/* -------------------------------------------
* -- 	v_SetTransmitLengthASCII_v
*/

void
class_Comms_RemoteProtocol::v_SetTransmitLengthASCII_v()
{
    // set the string length of the transmit buffer
    struct_LocalCommand.u8_TransmitBufferSize = strlen( (char *) u8arr_TransmitBuffer );

    return;
}

