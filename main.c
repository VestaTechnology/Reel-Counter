/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Reel Counter
 * James Burnworth
 * Final Version; 4/29/14
 *
 * This project was undertaken as a way to showcase the capabilities
 * of our new board family, Mercury, and make inventory a little 
 * easier.  Instead of renting or purchasing a reel counter we were 
 * able to build one with the skeleton of an old counter, a Mercury
 * 16 and parts we had on hand.  The Mercury16 handled the project 
 * with aplomb.  Even running the free version of MPLAB only 11.4% 
 * of program space was used and I can't turn the reel counter fast
 * enough to cause timing issues.  Check the General Forum on 
 * vestatech.com for pictures, a circuit schematic and a detailed
 * build chronology.
 */

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "interrupt.h"
#include "LCD.h"
#include "interface.h"

main(void)
{
	/**********INITIALIZATIONS AND CONFIGURATIONS****************/
	/* Initialize communications */
	interface_init();
	lcd_init();

	/*********** RUNNING ****************************************/
	welcome();	

	while (1) {
		if (updateFlag())	updateLCD();
	}

}