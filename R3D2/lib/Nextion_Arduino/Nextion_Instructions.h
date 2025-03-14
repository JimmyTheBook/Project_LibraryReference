/*  Nextion_Instructions.h
 *   
 *    This file defines the codes for Nextion instructions and returns
 *
 */

#ifndef NEXTION_INSTRUCTIONS_H
#define NEXTION_INSTRUCTIONS_H

#define NEXTION_RETURN_CMD_FINISHED                 (0x01)
#define NEXTION_RETURN_EVENT_LAUNCHED               (0x88)
#define NEXTION_RETURN_EVENT_UPGRADED               (0x89)
#define NEXTION_RETURN_EVENT_TOUCH_HEAD             (0x65)     
#define NEXTION_RETURN_EVENT_POSITION_HEAD          (0x67)
#define NEXTION_RETURN_EVENT_SLEEP_POSITION_HEAD    (0x68)
#define NEXTION_RETURN_CURRENT_PAGE_ID_HEAD         (0x66)
#define NEXTION_RETURN_STRING_HEAD                  (0x70)
#define NEXTION_RETURN_NUMBER_HEAD                  (0x71)
#define NEXTION_RETURN_INVALID_CMD                  (0x00)
#define NEXTION_RETURN_INVALID_COMPONENT_ID         (0x02)
#define NEXTION_RETURN_INVALID_PAGE_ID              (0x03)
#define NEXTION_RETURN_INVALID_PICTURE_ID           (0x04)
#define NEXTION_RETURN_INVALID_FONT_ID              (0x05)
#define NEXTION_RETURN_INVALID_BAUD                 (0x11)
#define NEXTION_RETURN_INVALID_VARIABLE             (0x1A)
#define NEXTION_RETURN_INVALID_OPERATION            (0x1B)

#define NEXTION_CMD_END                             (0xFF)


#endif
