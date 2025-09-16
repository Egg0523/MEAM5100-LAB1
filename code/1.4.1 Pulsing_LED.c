/* Name: 1.4.1 Asymmetric_Pulsing_LED.c
 * Author: Qihan Shan 
 * Description: Asymmetric pulsing LED on PB5 using PWM with variable timing
 * Pulse Pattern: 0.3s rise (0% to 100%), 0.6s fall (100% to 0%), repeat
 */

 #include "MEAM_general.h"  //includes the resources included in the MEAM_general.h file

 int main(void)
 {
     // Variable timing for pulse phases (in milliseconds)
     // ASYMMETRIC PULSE: Fast rise, slow fall
     unsigned int rise_time_ms = 300;   // Time to increase from 0% to 100% (0.3 seconds)
     unsigned int fall_time_ms = 600;   // Time to decrease from 100% to 0% (0.6 seconds)
     
     // PWM configuration variables
     unsigned int pwm_steps = 100;      // Number of steps for smooth transition (higher = smoother)
     unsigned int step_delay_rise;      // Delay per step during rise phase
     unsigned int step_delay_fall;      // Delay per step during fall phase
     
     // PWM duty cycle variable (0-100%)
     unsigned int duty_cycle = 0;
      
     _clockdivide(0); // Set the clock speed to 16MHz
     
     // =================================================================
     // HARDWARE PWM CONFIGURATION
     // =================================================================
     
     // Configure PB5 as output for PWM (OC1A pin)
     set(DDRB, 5);  // Set PB5 as output (OC1A pin)
     
     // Configure Timer1 for Fast PWM Mode (Mode 14)
     // WGM13:0 = 1110 (Fast PWM, TOP = ICR1)
     ICR1 = 999;  // Set TOP value for ~2kHz PWM frequency
                  // PWM Freq = 16MHz / (8 * 1000) = 2kHz
     
     // Configure Timer1 Control Register A (TCCR1A)
     // COM1A1:0 = 10 (Clear OC1A on compare match, set OC1A at TOP)
     // WGM11:0 = 10 (Fast PWM mode, part of WGM13:0 = 1110)
     TCCR1A = (1 << COM1A1) | (1 << WGM11);
     
     // Configure Timer1 Control Register B (TCCR1B)
     // WGM13:12 = 11 (Fast PWM mode, part of WGM13:0 = 1110)
     // CS12:10 = 010 (Prescaler = 8)
     TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11);
     
     // =================================================================
     // TIMING CALCULATIONS
     // =================================================================
     
     // Calculate step delays based on timing requirements
     // Each step represents 1% intensity change
     step_delay_rise = rise_time_ms / pwm_steps;  // 300ms / 100 = 3ms per step
     step_delay_fall = fall_time_ms / pwm_steps;  // 600ms / 100 = 6ms per step
     
     // Declare loop variables outside the loops (C89/C90 compatible)
     unsigned int step;
     
     // =================================================================
     // MAIN PULSING LOOP - ASYMMETRIC PATTERN
     // =================================================================
     
     for(;;){
         // PHASE 1: FAST RISE (0.3 seconds: 0% to 100% intensity)
         // This creates a quick, sharp increase in brightness
         for(step = 0; step <= pwm_steps; step++){
             duty_cycle = (step * 100) / pwm_steps;  // Calculate duty cycle percentage (0-100%)
             OCR1A = (ICR1 * duty_cycle) / 100;      // Set PWM duty cycle
             _delay_ms(step_delay_rise);             // Wait 3ms for this step
         }
         
         // PHASE 2: SLOW FALL (0.6 seconds: 100% to 0% intensity)
         // This creates a gradual, smooth decrease in brightness
         for(step = pwm_steps; step > 0; step--){
             duty_cycle = ((step - 1) * 100) / pwm_steps;  // Calculate duty cycle percentage (100-0%)
             OCR1A = (ICR1 * duty_cycle) / 100;            // Set PWM duty cycle
             _delay_ms(step_delay_fall);                   // Wait 6ms for this step
         }
         
         // End of cycle - immediately start next pulse (no pause)
         // Total cycle time: 0.3s + 0.6s = 0.9 seconds
     }
      
      return 0;   /* never reached */
  }
  
  