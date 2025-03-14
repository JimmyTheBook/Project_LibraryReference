/*  Nextion_TouchObjects.h
 *   
 *    This file defines the class for Nextion objects that can be "touched".
 *
 */

#ifndef _NEXTION_TOUCHOBJECTS_H
#define _NEXTION_TOUCHOBJECTS_H


// include the right libraries
#include "Arduino.h"
#include "Nextion_Object.h"

//  Nextion event when the touchscreen object is PUSHED
#define NEXTION_TOUCHEVENT_PUSH     (0x01)

//  Nextion event when the touchscreen object is RELEASD
#define NEXTION_TOUCHEVENT_RELEASE  (0x00)  


/*-------------------------------------------
 *
 *		NextioTouchObject
 *          Define the class for touchable objects.
 *          Inherits from the NextionObject class (the base class for all objects)
 *
 */

class NextionTouchObject: public NextionObject
{
    public:

    private:
    
};

#endif
