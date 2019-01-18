/* Library of helper funcs */
#include "helper_funcs.h"

// This function stalls the processor for a little while
void lib_stall(int time){
	volatile unsigned long t = 0;

	while(1){
		t++;
		if(t == time) break;
	}

	return void;
}