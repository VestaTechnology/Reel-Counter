/* interface.h
 *
 * Useful definitions and function prototypes for user interface.
 * Initialize the LCD on Mercury connector J5.  Debounce the 
 * three control buttons, give them functionality, and print 
 * updated values to the LCD.
 */

#ifndef __INTERFACE_H
#define	__INTERFACE_H

#include <GenericTypeDefs.h>

#define _XTAL_FREQ 32000000

/* Buttons are on pull-up resistors connected to RC0-RC2 */
#define bttnPORT PORTC
#define reset_bttn 0
#define up_bttn 1
#define down_bttn 2

void interface_init();
void putch(unsigned char);
enum _BOOL rawKeyPress(UINT8 bttn);
enum _BOOL debounceReset();
enum _BOOL debounceUp();
enum _BOOL debounceDown();
void updateLCD();
void moreHolesPer();
void lessHolesPer();
void welcome();

#endif