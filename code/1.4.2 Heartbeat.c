/* Name: 1.4.2 Heartbeat.c
 * Author: Qihan Shan 
 * Description: Heartbeat LED pattern using PWM with subroutines
 */

 #include "MEAM_general.h"  //includes the resources included in the MEAM_general.h file

 // Function prototypes
 void pwm_init(void);
 void smooth_transition(unsigned int start_intensity, unsigned int end_intensity, 
                       unsigned int duration_ms, unsigned int max_intensity);
 void heartbeat_pattern(void);
 void set_max_intensity(unsigned int new_max_pwm_value);
 
 // PWM configuration variables
 unsigned int max_pwm_value = 999;  // ICR1 value for PWM
 
 int main(void)
 {
     _clockdivide(0); //set the clock speed to 16Mhz
     
     // Initialize PWM system
     pwm_init();
     
     // Main heartbeat loop
     for(;;){
         heartbeat_pattern();
     }
     
     return 0;   /* never reached */
 }
 
 // Initialize Timer1 for PWM on PB5
 void pwm_init(void)
 {
     // Configure PB5 as output for PWM (OC1A)
     set(DDRB, 5);  // Set PB5 as output (OC1A pin)
     
     // Configure Timer1 for Fast PWM Mode
     // WGM13:0 = 1110 (Fast PWM, TOP = ICR1)
     ICR1 = max_pwm_value;  // Set TOP value for PWM
     
     // Configure Timer1 Control Register A
     // COM1A1:0 = 10 (Clear OC1A on compare match, set OC1A at TOP)
     // WGM11:0 = 10 (Fast PWM mode, part of WGM13:0 = 1110)
     TCCR1A = (1 << COM1A1) | (1 << WGM11);
     
     // Configure Timer1 Control Register B  
     // WGM13:12 = 11 (Fast PWM mode, part of WGM13:0 = 1110)
     // CS12:10 = 010 (Prescaler = 8)
     TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11);
     
     // Start with LED off
     OCR1A = 0;
 }
 
// Set a new maximum PWM intensity by updating TOP (ICR1) safely
void set_max_intensity(unsigned int new_max_pwm_value)
{
    max_pwm_value = new_max_pwm_value;
    ICR1 = max_pwm_value;
    if (OCR1A > max_pwm_value) {
        OCR1A = max_pwm_value;
    }
}
 
// Smooth transition between two intensity levels
void smooth_transition(unsigned int start_intensity, unsigned int end_intensity, 
                      unsigned int duration_ms, unsigned int max_intensity)
{
    unsigned int steps = 50;  // Number of interpolation steps
    unsigned int step_delay = duration_ms / steps;
    int intensity_delta = (int)end_intensity - (int)start_intensity;
    unsigned int step;  // Declare loop variable outside (C89/C90 compatible)
    
    for(step = 0; step <= steps; step++){
        // Linear interpolation between start and end intensity (percentage 0..100)
        unsigned int current_intensity = (unsigned int)((int)start_intensity + ((long)intensity_delta * (long)step) / (long)steps);
        
        // Apply maximum intensity cap and scale to PWM TOP (ICR1)
        unsigned long capped_percent = ((unsigned long)current_intensity * (unsigned long)max_intensity) / 100UL; // 0..100
        unsigned int pwm_value = (unsigned int)((capped_percent * (unsigned long)max_pwm_value) / 100UL);
        
        // Set PWM duty cycle
        OCR1A = pwm_value;
        
        _delay_ms(step_delay);
    }
}
 
 // Heartbeat pattern: lub-dub with specific timing
 void heartbeat_pattern(void)
 {
     // Heartbeat sequence (lub-dub)
     // t=0      i = 0
     smooth_transition(0, 100, 100, 100);    // t=0 to t=0.1: 0% to 100%
     
     // t=0.1   i = 100
     smooth_transition(100, 0, 400, 100);    // t=0.1 to t=0.5: 100% to 0%
     
     // t=0.5   i = 0
     smooth_transition(0, 50, 100, 100);     // t=0.5 to t=0.6: 0% to 50%
     
     // t=0.6   i = 50
     smooth_transition(50, 0, 400, 100);     // t=0.6 to t=1.0: 50% to 0%
     
     // t=1.0   i = 0
     // Rest period: 2 seconds at 0%
     _delay_ms(2000);                        // t=1.0 to t=3.0: rest
     
     // Repeat heartbeat (lub-dub)
     // t=3.0   i = 0
     smooth_transition(0, 100, 100, 100);    // t=3.0 to t=3.1: 0% to 100%
     
     // t=3.1   i = 100
     smooth_transition(100, 0, 400, 100);    // t=3.1 to t=3.5: 100% to 0%
     
     // t=3.5   i = 0
     smooth_transition(0, 50, 100, 100);     // t=3.5 to t=3.6: 0% to 50%
     
     // t=3.6   i = 50
     smooth_transition(50, 0, 400, 100);     // t=3.6 to t=4.0: 50% to 0%
     
     // t=4.0   i = 0
     // End of cycle - will restart automatically
 }
 
 