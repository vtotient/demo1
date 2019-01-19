/*******************************************************************************
Copyright 2016 Microchip Technology Inc. (www.microchip.com)

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*******************************************************************************/
#include <xc.h>
#include <stdint.h>

#include "leds_rgb.h"

#include "sccp.h"

#ifndef FCY
    #define FCY 8000000
#endif

#define LED_RGB_RED_SCCP_INSTANCE   ((struct SCCP_INSTANCE*)&CCP3CON1L)
#define LED_RGB_GREEN_SCCP_INSTANCE ((struct SCCP_INSTANCE*)&CCP4CON1L)
#define LED_RGB_BLUE_SCCP_INSTANCE  ((struct SCCP_INSTANCE*)&CCP5CON1L)

void LED_RGB_Enable(LED_RGB led)
{
    switch(led)
    {
        case LED_RGB_LD3:            
            SCCP_InitPWMOutput(LED_RGB_RED_SCCP_INSTANCE, FCY/2, 20000, 0.16);    //Enable SCCP PWM output for driving the RGB Red LED
            SCCP_InitPWMOutput(LED_RGB_GREEN_SCCP_INSTANCE, FCY/2, 20000, 0.07);    //Enable SCCP PWM output for driving the RGB Green LED
            SCCP_InitPWMOutput(LED_RGB_BLUE_SCCP_INSTANCE, FCY/2, 20000, 0.00);    //Enable SCCP PWM output for driving the RGB Blue LED
            break;
        
        default:
            break;
    }
}

//Update the PWM duty cycle of the appropriate RGB LED control channel

void LED_RGB_SetRed(LED_RGB led, uint16_t red)
{
    SCCP_UpdatePWMDutyCycle(LED_RGB_RED_SCCP_INSTANCE, (float)red / 1024);
}
    
void LED_RGB_SetGreen(LED_RGB led, uint16_t green)
{
    SCCP_UpdatePWMDutyCycle(LED_RGB_GREEN_SCCP_INSTANCE, (float)green / 1024);
}

void LED_RGB_SetBlue(LED_RGB led, uint16_t blue)
{
    SCCP_UpdatePWMDutyCycle(LED_RGB_BLUE_SCCP_INSTANCE, (float)blue / 1024);
}

void LED_RGB_Set(LED_RGB led, uint16_t red, uint16_t green, uint16_t blue)
{
    LED_RGB_SetRed(led, red);
    LED_RGB_SetGreen(led, green);
    LED_RGB_SetBlue(led, blue);
}
