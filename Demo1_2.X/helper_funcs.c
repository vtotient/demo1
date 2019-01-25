/* Library of helper funcs */
#include "helper_funcs.h"
#define PWM_CLK_DIV 0x02; 

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
 * This function sets the timer flag to true once the 1ms 
 * timer triggers an interrupt. 
 * NOTE: Does not clear the flag
 */
void DEBOUNCE_Tasks(FLAGS *flag){
	flag->all_flags |= 0x02; // Set the second bit
}

/* 
 * This fucntion sets the LEDs depending on the potentiometer
 * INPUT: Potentiometer percentage
 */
void set_RGB_LED(int pot){
    if(pot >= 0 && pot <= 33){ 
        LED_GREEN = 1;
        LED_RED = 0;
        LED_BLUE = 0;
    }
    if(pot > 33 && pot <= 66){
        LED_GREEN = 0;
        LED_RED = 1;
        LED_BLUE = 0;
    }
    if(pot > 66 && pot <= 100){
        LED_GREEN = 1;
        LED_RED = 0;
        LED_BLUE = 0;
    }
}

/*
 * PURPOSE: To set PWM duty cycle, phase and period
 * RETURN : True if successfully read inputs, false otherwise
 */
bool config_PWM(PWM_GENERATOR pwmx, uint16_t dc, uint16_t phase, uint16_t per){
    switch(pwmx){
        case PWM_GENERATOR_1 :
            PWM_ModuleDisable(pwmx);
            PG1CONHbits.MDCSEL = 0; 
            PG1CONHbits.MPERSEL = 0;
            PG1CONHbits.MPHSEL = 0;
            PWM_DutyCycleSet(pwmx, dc); // 1/4 of the PER
            PWM_PeriodSet(pwmx, per);
            PWM_PhaseSet(pwmx, phase);
            return true;
        case PWM_GENERATOR_2 :
            PWM_ModuleDisable(pwmx);
            PG2CONHbits.MDCSEL = 0; 
            PG2CONHbits.MPERSEL = 0;
            PG2CONHbits.MPHSEL = 0;
            PWM_DutyCycleSet(pwmx, dc); // 1/2 of the PER
            PWM_PeriodSet(pwmx, per);
            PWM_PhaseSet(pwmx, phase); // Equal to the DC of PWM1
            return true;
        case PWM_GENERATOR_3 :
            PWM_ModuleDisable(pwmx);
            PG3CONHbits.MDCSEL = 0; 
            PG3CONHbits.MPERSEL = 0;
            PG3CONHbits.MPHSEL = 0;
            PWM_DutyCycleSet(pwmx, dc); // 3/4 of the period
            PWM_PeriodSet(pwmx, per);
            PWM_PhaseSet(pwmx, phase); // Equal to the DC of PWM2
            return true;
        case PWM_GENERATOR_4 :
            PWM_ModuleDisable(pwmx);
            PG4CONHbits.MDCSEL = 0; 
            PG4CONHbits.MPERSEL = 0;
            PG4CONHbits.MPHSEL = 0; // Disable master dc, phase, and period
            PWM_DutyCycleSet(pwmx, dc);
            PWM_PeriodSet(pwmx, per); 
            PWM_PhaseSet(pwmx, phase); // Equal to the DC of PWM3
            return true;
        default : return false;
    }
}

/*
 * Enable the PWM channel's clock as: master clock divided clock divider circuit
 */
void enable_PWM_CLK_DIV(PWM_GENERATOR pwmx){

    switch(pwmx){
        case PWM_GENERATOR_1 :
            PG1CONLbits.CLKSEL = 0x02;
            break;
        case PWM_GENERATOR_2 :
            PG2CONLbits.CLKSEL = 0x02;
            break;
        case PWM_GENERATOR_3 :
            PG3CONLbits.CLKSEL = 0x02;
            break;
        case PWM_GENERATOR_4 :
            PG4CONLbits.CLKSEL = 0x02;
            break;
        default : break;
    }
}


/*
 * Set the ratio for clock division (PWM)
 */
void set_PWM_CLK_DIV(CLK_RATIO ratio){
    PCLKCONbits.DIVSEL = ratio; // The clock division ratio is 1:2
}



