/* LCD interface routines.
 *
 * These routines are written to control an alphanumeric LCD that uses the
 * Hitachi 44780 LCD controller chip or equivalent. It presumes that the LCD
 * will be attached to one of the extra I/O ports of the Mercury board (J4,
 * J5, or J6)  The LCD is attached to J5 for this project.
 */

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "LCD.h"


/* Function prototypes for internal-use-only routines.
 */
void lcd_delay( void );
unsigned char lcd_xfer( unsigned char port, unsigned char direction, unsigned char info );


/* Send a one-byte command value to the LCD. There is a minimum delay after
 * each command. The HOME and CLEAR commands have a longer required delay than
 * the other commands (about 1.64msec worst-case). If we can read the busy bit,
 * it is more efficient to check the status of the LCD at the beginning of the
 * command, because we can then be running other code during the busy time. If
 * we cannot read the busy bit, then we must hard-code a delay after the command
 * has been written.
 */
void lcd_command( unsigned char cmd )
{
	lcd_delay();
	/* Set up for command transfer */
	lcd_xfer(0, 0, cmd);

}


/* Send one byte of display data to the LCD.
 */
void lcd_data( unsigned char info )
{
	lcd_delay();
	/* Set up for data transfer */
	lcd_xfer(1, 0, info);
}


/* Read one byte of data from the LCD. If port is 0 (command), then we'll
 * read the busy status and current cursor address. If port is 1 (data), then
 * we'll read the character at the current cursor position.
 */
unsigned char lcd_read( unsigned char port )
{
	unsigned char result;

	result = lcd_xfer(port, 1, 0xFF);
	return result;
}


/* Set up the LCD for operation with our desired characteristics.
 */
void lcd_init( void )
{
	/* Allow LCD hardware some time to initialize. */
	__delay_ms(24);
	/* Send the initialization commands */
	lcd_command(0x28);  /* Four-bit data interface, two lines, 5x7 font*/
	lcd_command(0x0C);  /* Display on, cursor off, no blinking cursor */
	lcd_command(0x06);  /* Increment character position, no display shift */
	lcd_command(0x01);  /* Clear display and move cursor home */
}


/* Transfer one byte to or from the LCD.
 * Direction = 0 -> write; 1 -> read
 */
unsigned char lcd_xfer( unsigned char port, unsigned char direction, unsigned char info )
{
	unsigned char result, temp;

	if (direction & 1)
	{
		LCDTRIS = 0xF0;					/* Read operation - data bits are inputs */
	}
	else
	{
		LCDTRIS = 0x00;					/* Write operation - data bits are outputs */
	}
	/* Transfer the high nybble */
	temp = (info & 0xF0);		     	/* Put the data into the port's data bits */
	temp |= (port & 1) << LCD_RS;   	/* Set command or data */
	temp |= (direction & 1) << LCD_RW;  /* Set read or write */
	LCDPORT = temp;						/* Set everything up prior to enable */
	LCDPORT = temp | (1 << LCD_Enable);
	result = (LCDPORT & 0xF0);	   	/* Grab the data in case it's a read */
	LCDPORT = temp;

	/* Transfer the low nybble */
	temp = (info << 4) & 0xF0;  	    /* Put the data into the port's data bits */
	temp |= (port & 1) << LCD_RS;   	/* Set command or data */
	temp |= (direction & 1) << LCD_RW;  /* Set read or write */
	LCDPORT = temp;
	LCDPORT = temp | (1 << LCD_Enable);
	result |= (LCDPORT & 0xF0) >> 4;
	LCDPORT = temp;

	return result;
}


/* Because of the way we've structured the code, with this delay routine
 * being called prior to the transfer of a character or command, we have
 * no good way of knowing how long a timeout we need to set without using
 * global data to note what, specifically, the previous transfer was. Thus,
 * we use a standard timeout that will work for the worst-case commands.
 */
void lcd_delay( void )
{
	int x;

	for (x = 0; x < 25; x++)
	{
		if ((lcd_read(0) & 0x80) == 0x00)
			return;
	}
}
/* Accepts the value of a row and column to send the LCD cursor to.
 * Uses lcd_command to set the DDRAM address.
 */

void lcd_at ( unsigned int row, unsigned int col) 
{
	row = min(row,1);
	col = min(col,15);
	unsigned char addressDD = (0x80 + ((row & 1) * 64) + ((row & 2) * 10) + col);
	lcd_command(addressDD);
}