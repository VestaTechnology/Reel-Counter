#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <GenericTypeDefs.h>
#include "interrupt.h"
#include "LCD.h"
#include "interface.h"

/* count1 and count2 hold total (+ or -) ch 1 and ch 2 triggers 
 * pc holds clock-wise quadrature count
 * nc holds counterclock-wise quadrature count
 * err1 holds count of dual trigger events
 * isr holds count of ISR runs 
 */
int count1 = 0, count2 = 0, pc = 0, nc = 0, err1 = 0;
enum _BOOL update = TRUE;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * ISR: Checks which channel (1 is PORtB bit 0, 2 is PORTB bit 1)
 * generated an interrupt.
 *            - - - - - - - - - - - - -
 * If both channels gennerated interrupts, err1 is incremented 
 *            - - - - - - - - - - - - -
 * pc is the clock-wise count and is incremented when:
 *	1 goes high while 2 is low
 *	1 goes low while 2 is high
 *	2 goes high while 1 is high
 *	2 goes low while 1 is low 
 *            - - - - - - - - - - - - -
 * nc is the counterclock-wise count and is incremented when:
 *	1 goes high while 2 is high
 * 	1 goes low while 2 is low
 * 	2 goes high while 1 is low
 * 	2 goes low while 1 is high 
 *
 *
 *            Trouble-shooting items:
 * -count1 and count2 count interrupts per channel 
 */
 
void interrupt count_timer(void)
{
	//store IOC flag and reset register
	char IOC_flag = IOCBF; 
	IOCBF = 0;
	//store the two channels inputs
	int ch_one = LDR1, ch_two = LDR2;
	if ((IOC_flag & 3) == 3) {
		err1++;
		update = TRUE;
	}
	else if (IOC_flag & 1) {
		count1++;
		/*QUADRATURE COUNTING**************************
		**********************************************/
		if (ch_one) {
			if (ch_two) {
				nc++;
				update = TRUE;
			} else {
				pc++;
				update = TRUE;
			}
		} else {
			if (ch_two) {
				pc++;
				update = TRUE;
			} else {
				nc++;
				update = TRUE;
			}
		}
	}
	else if (IOC_flag & 2){
		count2++;
		/*QUADRATURE COUNTING**************************
		**********************************************/
		if (ch_two) {
			if (ch_one) {
				pc++;
				update = TRUE;
			} else {
				nc++;
				update = TRUE;
			}
		}
		else {
			if (ch_one) {
				nc++;
				update = TRUE;
			} else {
				pc++;
				update = TRUE;
			}
		}
	}
	/* A time based interrupt at 500 us, to debounce switches */
	if (PIR1 & 0x02) {
		if (debounceReset()) {
			resetCount();
			update = TRUE;
		}
		if (debounceUp()) {
			moreHolesPer();
			update = TRUE;
		}
		if (debounceDown()) {
			lessHolesPer();
			update = TRUE;
		} 
		PIR1 &= ~0x02; //clear flag
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * pc or nc (depending on direction of rotation) are incremented
 * 4 times for every hole on the tape. To find the position in 
 * holes on the tape, subtract nc/4 from pc/4.  The absolute 
 * value is taken to show relative holes from starting position 
 */
int getHoles() 
{
	int absolutePos;
	absolutePos = abs((pc/4)-(nc/4));

	return absolutePos;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Returns number of errors detected.  Incremented when ISR is 
 * ran with an interrupt flag on both channels.  With a good 
 * quadrature signal only one channel changes at a time.
 */
int getErrors()
{
	return err1;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Resets all variables used in quadrature decoding to zero.
 */
void resetCount()
{
	count1 = count2 = nc = pc = err1 = 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Checks if the update the that is set when the value of nc, 
 * pc, err1, or holesPerpart changes and returns true if it 
 * is set.
 */
enum _BOOL updateFlag()
{
	if (update) {
		update = FALSE;
		return TRUE;
	}
	return update;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Prints info useful for checking how well the quadrature 
 * decoding is working.  Count1 and count2 are incremented 
 * when an interrupt is generated on Ch1 and Ch2 respectively.
 * The second line shows the total counts in each direction.
 */
void printDebug() 
{
	lcd_at(2,0);
	printf("CH1 %4i CH2 %4i",count1/2,count2/2);
	lcd_at(3,0);
	printf("Pos.(+)%3i(-)%3i",pc/4,nc/4);
}