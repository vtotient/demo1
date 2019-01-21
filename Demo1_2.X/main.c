// Files generated by MCC 
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "mcc_generated_files/system.h"
#include "helper_funcs.c"
#include "leds.h"
#include "buttons.h"
#include "buttons.c"
#include "timer_1ms.c"
#include "timer_1ms.h"

#define BUTTON_DEBOUNCE_TIME 15000

//------------------------------------------------------------------------------
//Global variables
//------------------------------------------------------------------------------
static volatile BUTTON_COLOR buttonColor = BUTTON_COLOR_RED;


int main(void)
{
    // initialize the device
    SYSTEM_Initialize(); 

    // Enable buttons as inputs
    BUTTON_Enable(BUTTON_S1);

    // Set the time
    TIMER_SetConfiguration(TIMER_CONFIGURATION_1MS);

    FLAGS flags;
    flags.all_flags = 0; // Set all flags to zero
    //volatile int flag = 0;
    bool btn_pressed, reliable;
    int i;

    while (1)
    {
        lib_blink(&flags);

        if(BUTTON_IsPressed(BUTTON_S1) == 1){
            lib_stall(BUTTON_DEBOUNCE_TIME); // Wait
           // for(i =0; i<30; i++) TIMER_RequestTick(&DEBOUNCE_Tasks, 1);
            btn_pressed = BUTTON_IsPressed(BUTTON_S1) == 1; // Button still pressed?
            reliable = btn_pressed == true; 
            if(!reliable) btn_pressed = false;
            if(btn_pressed) LATBbits.LATB14 = !LATBbits.LATB14;
        }

    }

}
