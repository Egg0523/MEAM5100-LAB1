/* Name: 1.3.3 Hardware_PWM.c
 * Author: Qihan Shan 
 * Description: Hardware PWM on PB5 using Timer1 WGM modes for variable duty cycle
 */

#include "MEAM_general.h"  //includes the resources included in the MEAM_general.h file

int main(void)
{
    // Variable duty cycle (0-100, representing percentage)
    unsigned char duty_cycle = 50;  // Default to 50% duty cycle
    
    _clockdivide(0); //set the clock speed to 16Mhz
    
    // Configure PB5 as output for PWM (OC1A)
    set(DDRB, 5);  // Set PB5 as output (OC1A pin)
    
    // Configure Timer1 for Fast PWM Mode
    // WGM13:0 = 1110 (Fast PWM, TOP = ICR1)
    // This gives us full control over PWM frequency and duty cycle
    
    // Set PWM frequency to ~1kHz for visible LED dimming
    // Frequency = 16MHz / (Prescaler * (ICR1 + 1))
    // For 1kHz with prescaler 8: ICR1 = (16MHz / (8 * 1000Hz)) - 1 = 1999
    ICR1 = 1999;  // Set TOP value for ~1kHz PWM frequency
    
    // Configure Timer1 Control Register A
    // COM1A1:0 = 10 (Clear OC1A on compare match, set OC1A at TOP)
    // WGM11:0 = 10 (Fast PWM mode, part of WGM13:0 = 1110)
    TCCR1A = (1 << COM1A1) | (1 << WGM11);
    
    // Configure Timer1 Control Register B  
    // WGM13:12 = 11 (Fast PWM mode, part of WGM13:0 = 1110)
    // CS12:10 = 010 (Prescaler = 8)
    TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11);
    
    // Set initial duty cycle (50%)
    OCR1A = (ICR1 * duty_cycle) / 100;  // 50% duty cycle
    
    // Demonstrate different duty cycles
    for(;;){
        // 0% duty cycle (LED completely OFF)
        OCR1A = 0;
        _delay_ms(2000);
        
        // 25% duty cycle
        OCR1A = (ICR1 * 25) / 100;
        _delay_ms(2000);
        
        // 50% duty cycle
        OCR1A = (ICR1 * 50) / 100;
        _delay_ms(2000);
        
        // 75% duty cycle
        OCR1A = (ICR1 * 75) / 100;
        _delay_ms(2000);
        
        // 100% duty cycle (LED completely ON)
        OCR1A = ICR1;
        _delay_ms(2000);
    }
    
    return 0;   /* never reached */
}

