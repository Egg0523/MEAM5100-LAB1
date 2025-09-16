/* Name: 1.2.4 Variable-Duty-Cycle.c
 * Author: Qihan Shan 
 * Description: Blinks LED on PB5 with variable duty cycle using _delay_ms() routine
 */

 #include "MEAM_general.h"  //includes the resources included in the MEAM_general.h file

 int main(void)
 {
     // Variable duty cycle (0-100, representing percentage)
     int duty_cycle = 25;  // Default duty cycle, change this value to change the duty cycle
     
     // Timing variables (in milliseconds)
     int period_ms = 1000;  // Total period: 1000ms (1 second)
     int on_time_ms;        // Time LED is ON
     int off_time_ms;       // Time LED is OFF
     
     _clockdivide(0); //set the clock speed to 16Mhz
     
     // Configure PB5 as output for LED
     set(DDRB, 5);  // Set PB5 as output
     
     // Calculate timing based on duty cycle
     on_time_ms = (period_ms * duty_cycle) / 100; // calculate the on time based on the duty cycle
     off_time_ms = period_ms - on_time_ms;        // calculate the off time based on the duty cycle
     
     // Handle special cases
     if (duty_cycle == 0) {
         // 0% duty cycle: LED always OFF
         clear(PORTB, 5);
         for(;;) {
             // LED stays off, just delay to prevent infinite loop
             _delay_ms(1000);
         }
     }
     else if (duty_cycle == 100) {
         // 100% duty cycle: LED always ON
         set(PORTB, 5);
         for(;;) {
             // LED stays on, just delay to prevent infinite loop
             _delay_ms(1000);
         }
     }
     else {
         // Variable duty cycle: LED blinks with specified duty cycle
         for(;;){
             // Turn LED ON
             set(PORTB, 5);
             _delay_ms(on_time_ms);
             
             // Turn LED OFF
             clear(PORTB, 5);
             _delay_ms(off_time_ms);
         }
     }
     
     return 0;   /* never reached */
 }
 
 