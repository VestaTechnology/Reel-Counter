/* User interface routines.
 *
 * These routines are written to provide interface between 
 * Mercury, three buttons and the quadrature signal generated
 * by the reel counter.  The quadrature signal is connected 
 * to RB0 and RB1.
 *
 * A special thanks is owed to Jack Ganssle for his debounce
 * routines that this application uses shamelessly.
 */
 
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <GenericTypeDefs.h>
#include "interface.h"
#include "interrupt.h"
#include "LCD.h"

/* A variable to hold the number of holesPer part. */
int holesPer = 1;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Initializes I/O not concerned with LCD: 
 * weak pull-ups
 * Timer2
 * Interrupts
 */
void interface_init()
{
	/* Initialize I/O */
	TRISB = 3;	//RB0, RB1 are inputs
	TRISC = 7;	//RC0, RC2, RC3 are inputs
	/* Initialize weak pull-ups */
	OPTION_REG &= ~0x80; //clear bit 7 of OPTION_REG to enable WPU
	WPUB = 3;	//Enable WPU for RBO and RB1
	/* Initialize Timer2 */
	T2CON = 0x0E;	//Enable Timer2, postscale 1:2, prescale 1:16
	PR2 = 0xF9;		//match interrupt after 500 us
	/* Initialize Interrupts */
	INTCON |= 0xC8; 	//Enable gobal, on change, and perph interrupts
	IOCBP = IOCBN = 3;	//Detect rising and falling edges on RB0, RB1
	PIE1 |= 0x02;		//Enable Timer2 interrupt on match
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Output one character to LCD
 */
void putch(unsigned char byte) 
{
	/* output one byte */
	lcd_data(byte);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Returns TRUE if but bttn on bttnPORT is pressed.
 */
enum _BOOL rawKeyPress(UINT8 bttn)
{
	if ((bttnPORT>>bttn) & 0x01)	return FALSE;
	else						return TRUE;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Debounces reset button.  The logical NOT on RawKeyPress 
 * feeds State with 1's when bttn is open and 0's when closed. 
 * After 12 consecutive closed reads on the reset button, 
 * state will be 0xF000 and Debounce Reset returns TRUE.  
 * Called by Timer interrupt every 500 us.
 */
enum _BOOL debounceReset()
{
	static UINT16 State = 0; // Current debounce status 
	State=(State<<1) | !rawKeyPress(reset_bttn) | 0xe000; 
	if(State==0xF000)	return TRUE; 
	return FALSE; 
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Debounces up button.  Returns TRUE after 12 consecutive 
 * closed reads.  See debounceReset comment. 
 * Called by Timer interrupt every 500 us.
 */
enum _BOOL debounceUp()
{
	static UINT16 State = 0; // Current debounce status 
	State=(State<<1) | !rawKeyPress(up_bttn) | 0xe000; 
	if(State==0xF000)	return TRUE; 
	return FALSE; 
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Debounces down button.  Returns TRUE after 12 consecutive 
 * closed reads.  See debounceReset comment.
 * Called by Timer interrupt every 500 us.
 */
enum _BOOL debounceDown()
{
	static UINT16 State = 0; // Current debounce status 
	State=(State<<1) | !rawKeyPress(down_bttn) | 0xe000; 
	if(State==0xF000)	return TRUE; 
	return FALSE; 
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Called when the updateFlag is set.  Updates the LCD 
 * with holes, holes per part and errors
 */
void updateLCD()
{
	lcd_at(0,0);
	printf("Parts:%5i E:%2i",getHoles()/holesPer,getErrors());
	lcd_at(1,0);
	printf("Holes/Part:%2i",holesPer);
//	printDebug();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Increases the number of holesPer part.
 */
void moreHolesPer()
{
	holesPer++;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Decreases the number of holesPer part.
 */
void lessHolesPer()
{
	if (holesPer > 1) {
		holesPer--;
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * A welcome message.
 */
void welcome() 
{
	__delay_ms(24);

	lcd_at(0,2);
	printf("Welcome to");
	lcd_at(1,0);
	printf("DIY Reel Counter!");
	for (int i=0;i<150;i++) {
		__delay_ms(24);
	}
	lcd_command(0x01);

	__delay_ms(24);
}