/* Library of helper funcs */
#include "helper_funcs.h"
#include "mcc_generated_files/pwm.h"
#include "mcc_generated_files/timer_1ms.h"
#define PWM_CLK_DIV 0x02 
#define PWM_PULSE_TIME 33/2
#define WITHIN 10

// Global Variables
volatile uint16_t ms = 0; // Used to count number of ms
volatile uint16_t step = 0; // Used in the timer to count number of steps taken
volatile uint16_t desired_pos = 0; // The desired number of steps to be taken
volatile uint16_t cur_pos = 0; // The current relative position of stepper motor shaft
volatile uint16_t prev_pos = 0;
volatile uint16_t x = 0;



FLAGS flags;

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
void lib_blink(){
	if((flags.led_flag) == 0){
            lib_stall(9000);
            LATEbits.LATE0 = !LATEbits.LATE0; // Blink RED LED1
            flags.all_flags |= 1; // Set lsb
        }
        else {
            lib_stall(9000);
            LATEbits.LATE1 = !LATEbits.LATE1; // Blink RED LED2
            flags.all_flags &= 0xFE; // Clear lsb
        }
}

/* 
 * This function sets the timer flag to true once the 1ms 
 * timer triggers an interrupt. 
 * NOTE: Does not clear the flag
 */
void DEBOUNCE_Tasks(){
	flags.all_flags |= 0x02; // Set the second bit
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
 * In other words set a particular pwm instance's clock to be master clock divided by 
 * value selected in set_PWM_CLK_DIV
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

/*
 * Changes the direction of the stepper motor
 * Done by swapping the dc and phase of pmw1<->pwm2
 * and pwm3<->pwm4
 * Not the most efficient so hopefully compiler optimizes this 
 * NOTE: Toggles the step_dir flag. 
 */
void change_stepper_dir(){
    stop_stepper();
    uint16_t swap1 = 0x0;
    uint16_t swap2 = 0x0;

    swap1 = PG1DC; // Mask for pwm2 DC
    swap2 = PG2DC; // Mask for pwm1 DC (read)

    PG1DC = swap2;
    PG2DC = swap1; // Values swaped (write)

    swap1 = PG3DC;
    swap2 = PG4DC; 

    PG3DC = swap2;
    PG4DC = swap1; 

    swap1 = PG3PHASE;
    swap2 = PG4PHASE;

    PG3PHASE = swap2;
    PG4PHASE = swap1;

    swap1 = PG2PHASE;
    swap2 = PG1PHASE;

    PG2PHASE = swap2;
    PG1PHASE = swap1;

    start_stepper();
    flags.step_dir ^= 1;
 }

/*
 * This function is called in main every loop iteration. It takes the system flag variables
 * as input as well as the number of steps required. The function will turn the motor to the 
 * desired position: shaft rotated by num_steps in the direction dir.
 */

/*
 * Stops stepper motor by stopping PWM
 */
void stop_stepper(){
    PWM_GENERATOR pwm1 = PWM_GENERATOR_1;
    PWM_GENERATOR pwm2 = PWM_GENERATOR_2;
    PWM_GENERATOR pwm3 = PWM_GENERATOR_3;
    PWM_GENERATOR pwm4 = PWM_GENERATOR_4;
    PWM_ModuleDisable(pwm1);
    PWM_ModuleDisable(pwm2);
    PWM_ModuleDisable(pwm3);
    PWM_ModuleDisable(pwm4);
    flags.on_off = 0;
}

/*
 * Starts stepper motor by starting PWM
 */ 
void start_stepper(){
    PWM_GENERATOR pwm1 = PWM_GENERATOR_1;
    PWM_GENERATOR pwm2 = PWM_GENERATOR_2;
    PWM_GENERATOR pwm3 = PWM_GENERATOR_3;
    PWM_GENERATOR pwm4 = PWM_GENERATOR_4;
    PWM_ModuleEnable(pwm1);
    PWM_ModuleEnable(pwm2);
    PWM_ModuleEnable(pwm3);
    PWM_ModuleEnable(pwm4);
    flags.on_off = 1;
}

/*
 * Useful for debuging code. Sets LED1 high
 */
void DEBUG(){
    LATEbits.LATE3 ^= 1; // Toggle debug pin
}


/*
 * Counts the number of steps, passes the responsibilty of clearing the number 
 * of steps
 */
void STEPCOUNT_TASK(void){
    if(flags.on_off == 1){ // Only exe if motor is on
        if(ms == PWM_PULSE_TIME){ 
            ms = 0;
            step++;
            if(flags.step_dir == 0){
                cur_pos++;
            } 
            else if(flags.step_dir == 1){
                cur_pos--;
            }
            STEPCONTROL_TASK();
        }
        else{
            ms++;
        }
    }   
    
    return;
}

/*
 * Drives the motor to the correct angle. If the number of steps == the desired step
 * i.e. the stepper motor is at the correct angle, clear the steps variable and stop motor
 */
void STEPCONTROL_TASK(void){    
    if(step >= desired_pos){
        step = 0;
        stop_stepper();
    }
    return;
}

/* 
 * This ISR drives the motor. 
 */
void START_TASK(void){
    if(desired_pos != x){ // Motor needs to move
        if(x > cur_pos){
            if( flags.step_dir == 1){
                change_stepper_dir();
            }

            desired_pos = x - prev_pos;
            
            if(!within_5( desired_pos, cur_pos) && flags.on_off == 0){ 
                start_stepper();
                prev_pos = cur_pos;
            }
        }
        else if(x < cur_pos){
            if(flags.step_dir == 0){
                change_stepper_dir(); 
            }

            desired_pos = prev_pos - x;
            
            if(!within_5( desired_pos, cur_pos) && flags.on_off == 0){ 
                start_stepper();
                prev_pos = cur_pos;
            }
        }
    }
}

/*
 * Checks if within a constant
 */ 
bool within_5(uint16_t prev, uint16_t current){
    if(current <= prev + WITHIN && current  >= prev - WITHIN){
        return true;
    } else if (prev == 0){
        return true;
    }
    return false;
}

//Updates the PWM output duty cycle/on time.
void PWM_UpdateDC(uint16_t newDC)
{    
    //Update the PWM duty cycle register now.
    PG1DC = newDC;
    
    //Let the hardware know we wrote new value to the register and it should perform an internal bufferred update procedure.
    PG1STAT |= 0x0008;   //Set the UPDREQ bit to initiate the hardware update
}