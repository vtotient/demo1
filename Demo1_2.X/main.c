// Files generated by MCC 
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "mcc_generated_files/system.h"
#include "helper_funcs.c"
#include "debounce.h"
#include "leds.h"
#include "buttons.h"
#include "buttons.c"

typedef enum
{
    BUTTON_COLOR_RED = 0,
    BUTTON_COLOR_GREEN = 1,
    BUTTON_COLOR_BLUE = 2
} BUTTON_COLOR;


//------------------------------------------------------------------------------
//Global variables
//------------------------------------------------------------------------------
static volatile BUTTON_COLOR buttonColor = BUTTON_COLOR_RED;

//void ButtonS1Callback(bool pressed)
//{
//}


int main(void)
{
    // initialize the device
    SYSTEM_Initialize(); 

    // Enable buttons as inputs
    BUTTON_Enable(BUTTON_S1);

//    DEBOUNCE_Initialize();
//
//    button_s1 = DEBOUNCE_AddButton(&BUTTON_S1_IsPressed);
//    DEBOUNCE_SetCallback(button_s1, &ButtonS1Callback);

    int short flag = 0;

    while (1)
    {
        lib_blink(flag);

        if(BUTTON_IsPressed(BUTTON_S1) == 1){
            LATBbits.LATB14 = !LATBbits.LATB14;
        }

    }

}
