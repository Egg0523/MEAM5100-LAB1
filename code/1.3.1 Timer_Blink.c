/* Name: 1.3.1 Timer_Blink.c
 * Author: Qihan Shan 
 * Description: Blinks LED on PB5 at 20Hz using Timer1 interrupt
 */

 #include "MEAM_general.h"  //includes the resources included in the MEAM_general.h file

 // Global variable to track timer overflow count
 volatile unsigned int timer_overflow_count = 0;
 
 // Timer1 overflow interrupt service routine
 ISR(TIMER1_OVF_vect)
 {
     timer_overflow_count++;
     
     // 20Hz = 50ms period, so toggle every 25ms
     // At 16MHz with prescaler 1024: 16MHz/1024 = 15.625kHz
     // Timer overflow every 65536/15625 = 4.194 seconds
     // For 20Hz toggle (25ms), we need: 25000us * 15.625kHz = 390.625 counts
     // Round to 391 counts for 25.024ms (close to 25ms)
     
     // Reset timer for next cycle
     TCNT1 = 65536 - 391;  // 391 counts = 25.024ms at 15.625kHz
 }
 
 int main(void)
 {
     _clockdivide(0); //set the clock speed to 16Mhz
     
     // Configure PB5 as output for LED
     set(DDRB, 5);  // Set PB5 as output
     
     // Configure Timer1 for 20Hz blinking
     // 20Hz means toggle every 1/(2*20) = 25ms
     // At 16MHz with prescaler 1024: 16MHz/1024 = 15.625kHz
     // For 25ms: 25000us * 15.625kHz = 390.625 timer counts
     // Use 391 counts for 25.024ms (very close to 25ms)
     
     TCCR1B = 0x05;  // Set prescaler to 1024 (CS12=1, CS11=0, CS10=1)
     TCNT1 = 65536 - 391;  // Set initial timer value for 391 counts
     TIMSK1 = 0x01;  // Enable Timer1 overflow interrupt
     
     sei();  // Enable global interrupts
     
     for(;;){
         // Main loop - LED toggling is handled by interrupt
         // The interrupt will toggle the LED every 25.024ms (19.98Hz ≈ 20Hz)
         if (timer_overflow_count >= 1) {
             toggle(PORTB, 5);  // Toggle LED
             timer_overflow_count = 0;  // Reset counter
         }
     }
     
     return 0;   /* never reached */
 }
  
  