/* Library of helper funcs */
#include "helper_funcs.h"

/* This function stalls the processor for a little while.
 * This is a blocking function, best used only for debugging
 * and early testing.
 */
void lib_stall(int time){
	volatile unsigned long t = 0;

	while(1){
		t++;
		if(t == time) break;
	}
}


/* Blink LED1 and LED2 
 * First bit of flag is the led_flag
 * NOTE: flag is 8bts
 */
void lib_blink(FLAGS *flag){
	if((flag->led_flag) == 0){
            lib_stall(9000);
            LATEbits.LATE0 = !LATEbits.LATE0; // Blink RED LED1
            flag->all_flags |= 1; // Set lsb
        }
        else {
            lib_stall(9000);
            LATEbits.LATE1 = !LATEbits.LATE1; // Blink RED LED2
            flag->all_flags &= 0xFE; // Clear lsb
        }
}

/* 
 * This function sets the time elapsed flag to true once the 1ms 
 * timer triggers an interrupt. 
 * NOTE: Does not clear the flag
 */
void DEBOUNCE_Tasks(FLAGS *flag){
	flag->all_flags |= 0xFE;
}