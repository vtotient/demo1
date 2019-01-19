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


/* Blink LED1 and LED2 */
void lib_blink(int flag){
	if(flag == 0){
            lib_stall(9000);
            LATEbits.LATE0 = !LATEbits.LATE0; // Blink RED LED1
            flag++;
        }
        else {
            lib_stall(9000);
            LATEbits.LATE1 = !LATEbits.LATE1; // Blink RED LED1
            flag = 0;
        }
}