/*  Nextion_Object.h
 *   
 *    This file defines the structure of a Nextion Object
 *
 */

 
#ifndef    _NEXTION_OBJECT_H
#define    _NEXTION_OBJECT_H

#include "Arduino.h"


/*-------------------------------------------
 *
 *		Define the NextObject class
 *          This class is the base of all Nextion objects
 *          The class has 3 main values:
 *                  PageID
 *                  ComponentID
 *                  ComponentName
 *
 */ 

class NextionObject
{
    // Public members
    public:

        /*-------------------------------------------*/
        /* -- 	Constructor
         *-------------------------------------------
         *
         *	This routine initiates class.
         *
         *	Return:
         *		None
         *
         *	Parameters:
         *		byte_PID = the page ID
         *      byte_CID = the component ID
         *      constcharptr_CNAME = pointer to the name
         */
        NextionObject( byte byte_PID, byte byte_CID, const char * constcharptr_CNAME );


        /*-------------------------------------------*/
        /* --   PrintParams
         *-------------------------------------------
         *
         *	This routine sends the objects infomration to a serial port (for debugging purposes).
         *
         *	Return:
         *		None
         *
         *	Parameters:
         *		hwsrptr_TargetPort = pointer to the serial port to output the params
         */

        void PrintParams( HardwareSerial* hwsrptr_TargetPort );


    // Protected members
    protected:

        /*-------------------------------------------*/
        /* -- 	byte_GetPageID
         *-------------------------------------------
         *
         *	This routine returns the page ID
         *
         *	Return:
         *		byte = PageID
         *
         *	Parameters:
         *		None
         */

        byte byte_GetPageID();

        
        /*-------------------------------------------*/
        /* -- 	byte_GetComponentID
         *-------------------------------------------
         *
         *	This routine returns the page ID
         *
         *	Return:
         *		byte = ComponentID
         *
         *	Parameters:
         *		None
         */
        
        byte byte_GetComponentID();


        /*-------------------------------------------*/
        /* -- 	constcharptr_GetComponentName
         *-------------------------------------------
         *
         *	This routine returns the page ID
         *
         *	Return:
         *		const char * = pointer to the ComponentName
         *
         *	Parameters:
         *		None
         */

        const char * constcharptr_GetComponentName();

    // Private members
    private:
        byte            byte_PageID;
        byte            byte_ComponentID;
        const char*     constcharptr_ComponentName;
};

#endif
