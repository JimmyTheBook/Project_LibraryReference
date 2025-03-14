/*  Class_Comms_RemoteProtocol.h
 *   
 *    This file defines the structure for the Class_Comms_RemoteProtocol class.
 *      The RemoteProtocol is used to connect slave boards to a master and pass infomration between the two.
 *      The protocol define the structure of the command protocol, as well as processing.
 *
 *
 *      DEBUG - The debug values allows debug messages to be sent on the comm port.
 *              This is typically only used when comm port is attached to a terminal.
 * 
 *              DEBUG = 0 is OFF
 *              DEBUG > 0 is LOW... only major messages (completed actions)
 *              DEBUG >= 3 is MEDIUM... additional stages and steps added (stage changes, key actions)
 *              DEBUG >= 7 is HIGH... will be everything (every step)
 */

#include <Arduino.h>
#include <string.h>

// only initiate once
#ifndef _CLASS_COMMS_REMOTEPROTOCOL_H
#define _CLASS_COMMS_REMOTEPROTOCOL_H

// define types of ports
#define COMMS_REMOTEPROTOCOL_PORT_TYPE_UART         0x01
#define COMMS_REMOTEPROTOCOL_PORT_TYPE_I2C          0x02
#define COMMS_REMOTEPROTOCOL_PORT_TYPE_SPI          0x03

// define default values
#define COMMS_REMOTEPROTOCOL_DEFAULT_BAUD                   9600
#define COMMS_REMOTEPROTOCOL_DEFAULT_TIMEOUT                50  // msecs
#define COMMS_REMOTEPROTOCOL_DEFAULT_MAX_PAYLOAD_SIZE       100  // bytes
#define COMMS_REMOTEPROTOCOL_DEFAULT_MAX_TRANSMIT_SIZE      100  // bytes
#define COMMS_REMOTEPROTOCOL_COMMAND_OVERWRITE              true
#define COMMS_CR                                    0x0D
#define COMMS_LF                                    0x0A
#define COMMS_NUMBER_OFFSET                         0x30
#define COMMS_END_STRING                            0x00

// define handshake types
#define COMMS_HANDSHAKE_TYPE_ASCII      0x10
#define COMMS_HANDSHAKE_TYPE_BINARY     0x20

// define the ack types
#define COMMS_ACK_TYPE_HUMAN            0x25
#define COMMS_ACK_TYPE_SINGLE_BYTE      0x50

// define the command states
#define COMMAND_IDLE                    0x00
#define COMMAND_PAYLOAD_SIZE            0x01
#define COMMAND_GET_PAYLOAD             0x02
#define COMMAND_CHECKSUM                0x03
#define COMMAND_IGNORE_COMMAND          0x04
#define COMMAND_IGNORE_BYTES            0x05
#define COMMAND_RESET_STATE_MACHINE     0x06

// define the debug message states
#define COMMS_DEBUG_OFF       0
#define COMMS_DEBUG_LOW       1
#define COMMS_DEBUG_MEDIUM    3
#define COMMS_DEBUG_HIGH      7

// define the inline debug selection statements
#define COMMS_DEBUG_IS_LOW      if ( struct_Params.u8_DebugMessages >= COMMS_DEBUG_LOW )
#define COMMS_DEBUG_IS_MED      if ( struct_Params.u8_DebugMessages >= COMMS_DEBUG_MEDIUM )
#define COMMS_DEBUG_IS_HIGH     if ( struct_Params.u8_DebugMessages >= COMMS_DEBUG_HIGH )

// define commands
#define COMMS_COMMAND_ASCII         0x2D
#define COMMS_COMMAND_ACK           0x41
#define COMMS_CHECKSUM_VALID        0x59
#define COMMS_CHECKSUM_INVALID      0x4E
#define COMMS_COMMAND_PING          0x50
#define COMMS_COMMAND_DEBUG_LEVEL   0x44

// create the global ASCII command array
#define     COMMS_REMOTEPROTOCOL_NUMBER_ASCII_COMMANDS      2
#define     COMMS_REMOTEPROTOCOL_MAX_COMMAND_CHARS          15

// create the indexes for the commands
#define     COMMS_REMOTEPROTOCOL_ASCII_CMD_INVALID          0xFF
#define     COMMS_REMOTEPROTOCOL_ASCII_CMD_PING             0
#define     COMMS_REMOTEPROTOCOL_ASCII_CMD_DEBUG_LEVEL      1

// create the array of recognized ASCII commands
const char gCOMMS_REMOTEPROTOCOL_ASCII_COMMANDS[ COMMS_REMOTEPROTOCOL_NUMBER_ASCII_COMMANDS ][ COMMS_REMOTEPROTOCOL_MAX_COMMAND_CHARS ]
    {
        "ping",
        "debug_level"
    };


/* -------------------------------------------
 * -- 	COMMS_REMOTEPROTOCOL_PARAMS
 * -------------------------------------------
 *
 *	This structure defines 
 *
 *	Return:
 *		None
 *
 *	Parameters:
 *		None
 */
struct COMMS_REMOTEPROTOCOL_PARAMS
{
    void*               voidptr_Port;               // pointer to the port
    HardwareSerial*     hwserial_DebugPort;         // pointer to the DEBUG port
    uint8_t             u8_PortType;                // the type of port using the protocol
    unsigned int        uint_Speed;                 // the speed to operate the port
    uint8_t             u8_DebugMessages;           // the debug level
    bool                bool_OverwriteCommands;     // flag that defines if commands are overwritten, or we wait
    unsigned int        uint_CommandTimeout;        // default timeout for commands
    unsigned int        uint_MaxPayloadSize;        // the maximum payload (which is also the maximum ASCII characted for ASCII commands)
};

/*-------------------------------------------
 * -- 	COMMS_REMOTEPROTOCOL_COMMAND
 *-------------------------------------------
 *
 *	This structure is used for commands coming to the  protocol
 *
 */
struct COMMS_REMOTEPROTOCOL_COMMAND
{
    bool                bool_CommandStarted;        // a command is being received
    unsigned char       uchar_Command;              // the initial byte = command
    unsigned int        uint_PayloadSize;           // the size of the payload
    uint8_t             u8_TransmitBufferSize;      // size of the transmit buffer
    unsigned char       uchar_Checksum;             // checksum byte
    bool                bool_IsValid;               // flag if the transmission is valid
    bool                bool_CommandReady;          // flag that a comment is ready to be processed by main program
};

/*-------------------------------------------
 * -- 	COMMS_REMOTEPROTOCOL_HANDSHAKE
 *-------------------------------------------
 *
 *	This struct is for the handshake protocol
 *
 *	Return:
 *		None
 *
 *	Parameters:
 *		None
 */
struct COMMS_REMOTEPROTOCOL_HANDSHAKE
{
    unsigned char       uchar_HandshakeType;
    bool                bool_WaitforAck;
    unsigned int        uint_AckTimeout_sec;
};

#endif

class class_Comms_RemoteProtocol
{
    public:

        /*-------------------------------------------
         * -- 	Constructor
         *-------------------------------------------
         *
         *	This routine is the constructor
         *
         *	Return:
         *		None
         *
         *	Parameters:
         *		struct_NewParams = struct for the protocol parameters
         */

        class_Comms_RemoteProtocol( COMMS_REMOTEPROTOCOL_PARAMS struct_NewParams );

        /*-------------------------------------------
         * -- 	v_InitiatePort_v
         *-------------------------------------------
         *
         *	This routine initiates the port
         *
         *	Return:
         *		None
         *
         *	Parameters:
         *		None
         */
        
        void v_InitiatePort_v();

        /*-------------------------------------------
         * -- 	bool_Handshake_struct
         *-------------------------------------------
         *
         *	This routine handshakes on the port with the receiver.
         *
         *	Return:
         *		bool    = TRUE if the ack was received (or if an ACK was not requested)
         *              = FALSE if an ACK was not received
         *
         *	Parameters:
         *		struct_HandshakeParams = struct of the handshake parameters
         */

        bool bool_Handshake_struct( COMMS_REMOTEPROTOCOL_HANDSHAKE );



        void v_DebugMessages_uchar( unsigned char );
        void v_ServiceSerial_v();
        bool bool_CommandByte_uchar_bool( unsigned char, bool = false );
        void v_SetTimeout_uint( unsigned int );
        void v_SendAck_v();

    /*-------------------------------------------
     *		Private members
     */
    private:

        /* -------------------------------------------
         * -- 	v_SendData_bool 
         * -------------------------------------------
         *
         *	This routine puts the payload data onto port
         *
         *	Return:
         *		None
         *
         *	Parameters:
         *		bool_SendNewLine = TRUE if the newline character(s) should be sent 
         */

        void v_SendData_bool( bool bool_SendNewLine = false );

        /* -------------------------------------------
         * -- 	bool_CheckReceiveData_u8ptr
         * -------------------------------------------
         *
         *	This routine checks if there is data ready to be received.
         *
         *	Return:
         *		bool = TRUE if data is available
         *
         *	Parameters:
         *		u8ptr_TargetByte = pointer to where to store the byte if one is available
         */

        bool bool_CheckReceiveData_u8ptr( uint8_t * u8ptr_TargetByte );

        // private subroutines
        void v_ChecksumVerification_v();
        void v_ProcessCommand_v();
        void v_ConvertAsciiCommand_v();
        void v_ReadyPayload_v();

        /* -------------------------------------------
         * -- 	v_LoadASCIINumberToArray_charptr_u8
         * -------------------------------------------
         *
         *	This routine initiates the Startup Screen, and loads the fields
         *      This routine does NOT ensure there is room in the char array.
         *
         *	Return:
         *		None
         *
         *	Parameters:
         *		charptr_Target = the array of characters to put the value into
         *      u8_Value = the value
         */

        void v_LoadASCIINumberToArray_charptr_u8( char* charptr_Target, uint8_t u8_Value );

        /* -------------------------------------------
         * -- 	v_SetTransmitLengthASCII_v
         * -------------------------------------------
         *
         *	This routine sets the transmit buffer size based on the ASCII string in the buffer.
         *
         *	Return:
         *		None
         *
         *	Parameters:
         *		None
         */

        void v_SetTransmitLengthASCII_v();

        /*-------------------------------------------
        *		Private variables
        */

        // the struct for params
        COMMS_REMOTEPROTOCOL_PARAMS    struct_Params;

        // variable to track the status of a command
        unsigned char   uchar_CommandState;

        // local command storage
        COMMS_REMOTEPROTOCOL_COMMAND    struct_LocalCommand;

        // pointer for the transmit buffer
        uint8_t        u8arr_TransmitBuffer[ COMMS_REMOTEPROTOCOL_DEFAULT_MAX_TRANSMIT_SIZE ];

        // pointer for the payload buffer
        uint8_t        u8arr_ReceiveBuffer [ COMMS_REMOTEPROTOCOL_DEFAULT_MAX_PAYLOAD_SIZE ];



};

