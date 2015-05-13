/* LCD.H
 *
 * Useful definitions and function prototypes for the LCD interface
 */

#ifndef __LCD_H
#define	__LCD_H

#define _XTAL_FREQ 32000000

/* Define the bit assignments for connecting the LCD. We'll make this selectable.
 * Note that bit 3 of port E (on J5) can only be an input in digital mode on
 * the PIC16F1947 processor, so we must make our pin assignments accordingly.
 * Define the bits that are used for data (7-4) and control (3-0). This gives
 * us the following connections:
 *
 * Mercury port pin	LCD connector pin
 *    1 (+5)			 2
 *    2 (GND)			 1
 *    3 (Enable)		 6
 *    4 (RS)			 4
 *    5 (RW)			 5
 *    6 (RDY)			 3 (contrast)
 *    7 (D4)			11
 *    8 (D5)			12
 *    9 (D6)			13
 *   10 (D7)			14
 *
 * Only ONE of the following conditional sections should be enabled.
 */
#define LCD_Enable 0
#define LCD_RS 1
#define LCD_RW 2
#define LCD_RDY 3

#if 0
  #define LCDPORT PORTD
  #define LCDTRIS TRISD
#endif

#if 1
  #define LCDPORT PORTE
  #define LCDTRIS TRISE
#endif

#if 0
  #define LCDPORT PORTF
  #define LCDTRIS TRISF
#endif

void lcd_command( unsigned char cmd );
void lcd_data( unsigned char info );
unsigned char lcd_read( unsigned char cmd );
void lcd_init( void );
void lcd_at ( unsigned int row, unsigned int col);

#endif	/* __LCD_H */

