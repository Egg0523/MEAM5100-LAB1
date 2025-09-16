/* Name: 1.3.2 Clock_Prescaler.c
 * Author: Qihan Shan 
 * Description: Demonstrates system clock prescaler effects using timer registers
 */

#include "MEAM_general.h"  //includes the resources included in the MEAM_general.h file

// Global variable to track timer overflow count
volatile unsigned int timer_overflow_count = 0;

// Timer1 overflow interrupt service routine
ISR(TIMER1_OVF_vect)
{
    timer_overflow_count++;
}

int main(void)
{
    // Configure PB5 as output for LED
    set(DDRB, 5);  // Set PB5 as output
    
    // Configure Timer1 with prescaler 1024 for consistent measurement
    TCCR1B = 0x05;  // Set prescaler to 1024 (CS12=1, CS11=0, CS10=1)
    TCNT1 = 0;      // Start timer from 0
    TIMSK1 = 0x01;  // Enable Timer1 overflow interrupt
    
    sei();  // Enable global interrupts
    
    // Test different system clock prescaler settings
    unsigned int overflow_count_16MHz = 0;
    unsigned int overflow_count_8MHz = 0;
    unsigned int overflow_count_4MHz = 0;
    
    // Test 1: Default 16MHz (prescaler = 0)
    _clockdivide(0); // 16MHz
    timer_overflow_count = 0;
    TCNT1 = 0;
    
    // Wait for 1 second worth of overflows at 16MHz
    // At 16MHz with prescaler 1024: 16MHz/1024 = 15.625kHz
    // Overflow every 65536/15625 = 4.194 seconds
    // So we need to wait for about 1/4.194 = 0.238 overflows for 1 second
    // Let's wait for 1 overflow to get measurable data
    while(timer_overflow_count < 1) {
        // Wait for timer overflow
    }
    overflow_count_16MHz = timer_overflow_count;
    
    // Test 2: 8MHz (prescaler = 1)
    _clockdivide(1); // 8MHz
    timer_overflow_count = 0;
    TCNT1 = 0;
    
    while(timer_overflow_count < 1) {
        // Wait for timer overflow
    }
    overflow_count_8MHz = timer_overflow_count;
    
    // Test 3: 4MHz (prescaler = 2)
    _clockdivide(2); // 4MHz
    timer_overflow_count = 0;
    TCNT1 = 0;
    
    while(timer_overflow_count < 1) {
        // Wait for timer overflow
    }
    overflow_count_4MHz = timer_overflow_count;
    
    // Now demonstrate the effect with LED blinking
    // Set back to 16MHz for normal operation
    _clockdivide(0); // 16MHz
    
    // Blink LED at different clock speeds to show the effect
    for(;;){
        // Blink pattern to show clock speed effect
        toggle(PORTB, 5);
        _delay_ms(500);  // This delay will be affected by clock prescaler
        
        // The LED blinking rate will change when we adjust system clock prescaler
        // because _delay_ms() is based on system clock cycles
    }
    
    return 0;   /* never reached */
}

