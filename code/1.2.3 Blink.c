/* Name: 1.2.3 Blink.c
 * Author: Qihan Shan 
 * Description: Blinks LED on PB5 using _delay_ms() routine
 */

#include "MEAM_general.h"  //includes the resources included in the MEAM_general.h file

int main(void)
{
    _clockdivide(0); //set the clock speed to 16Mhz
    // Configure PB5 as output for LED
    set(DDRB, 5);  // Set PB5 as output
    set(PORTB, 5); // Turn on LED initially
    
    _delay_ms(1000); // wait 1000 ms when at 16 MHz

    for(;;){
        toggle(PORTB, 5);  // Toggle LED on PB5
        _delay_ms(1000);   // wait 1000ms (1 second)
    }
    return 0;   /* never reached */
}

