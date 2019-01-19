/* ISR to detect S1 being pushed */

void __atribute__((interrupt)) _IC1Interrupt(void){

	TRISBbits.TRISB14 = !TRISBbits.TRISB14;
	IFS0bits.IFS01 = 0; // Clear the flag bit
	
}