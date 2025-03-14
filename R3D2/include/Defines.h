/*  Defines.h
 *   
 *    This file defines the commenting guidelines for file developed by Jim Pomeroy
 *
 */

#ifndef _DEFINES_H
#define _DEFINES_H

// define the debug message states
#define MAIN_DEBUG_OFF       0
#define MAIN_DEBUG_LOW       1
#define MAIN_DEBUG_MEDIUM    3
#define MAIN_DEBUG_HIGH      7

// define the inline debug selection statements
#define MAIN_DEBUG_IS_LOW      if ( MAIN_DEBUG_LEVEL >= MAIN_DEBUG_LOW )
#define MAIN_DEBUG_IS_MED      if ( MAIN_DEBUG_LEVEL >= MAIN_DEBUG_MEDIUM )
#define MAIN_DEBUG_IS_HIGH     if ( MAIN_DEBUG_LEVEL >= MAIN_DEBUG_HIGH )

// define main DEBUG level
#define MAIN_DEBUG_LEVEL          MAIN_DEBUG_HIGH


#endif
