/* "interrupt.h"
 *
 * This file contains definitions and prototypes for the 
 * Interrupt Service Routine and related functions.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
 
#ifndef INTERRUPT_COUNTING_H_INCLUDED
#define INTERRUPT_COUNTING_H_INCLUDED

#include <GenericTypeDefs.h>

// Label RBO as LDR1 and RB1 as LDR2
#define LDR1 PORTBbits.RB0
#define LDR2 PORTBbits.RB1

void interrupt count_timer(void);
int getHoles();
int getErrors();
void resetCount();
enum _BOOL updateFlag();
void printDebug();


#endif