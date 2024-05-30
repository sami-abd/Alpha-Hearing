/*#include "Lab2_Library.h"
// 
// REQUIRES: Nothing
// PROMISES: Set the system clock speed to 32MHz.
//           Configure Timer2 to trigger with the given period in microseconds.
//           Start Timer2. It will automatically restart when it finishes.
void Lab2_ConfigureTimer2(char TimerPeriod_us)
{
    // Set the system clock speed to 32MHz.

    OSCCON = 0xF4;
 

    
    //Wait for the oscillator to switch to its new speed.
    while(OSCSTATbits.HFIOFR == 0);
    
    // T2PR: TMR2 PERIOD REGISTER (Page 287)
    // The timer runs at 1MHz, so it increments every 1us. use these registers to hange the period to 1 second change all three registers
    T2PR = TimerPeriod_us;
    
    // T2CLKCON: TIMER2 CLOCK SELECTION REGISTER (Page 306))
    // Bits 8:4 = 0b00000   These bits are not implemented
    // Bits 3:0 = 0b0010    Clock source is the system clock
    T2CLKCON = 0x02;

    // T2CON: TIMERx CONTROL REGISTER (Page 307)
    // Bit  7   = 0b1       Timer is on.
    // Bits 6:4 = 0b101     Prescaler is 1:32 (Timer increments every 32 cycles)
    // Bits 3:0 = 0b0000    Postscaler is 1:1 
    T2CON = 0xD0;
            
    // TMR2IF is an interrupt flag, and is set whenever Timer2 expires.
    // We are not using interrupts, but it can still be checked manually.
    // It is cleared here as we start the timer.
    PIR1bits.TMR2IF = 0;
    
    
    return;
}

void ConfigureTimer4(char TimerPeriod_us)
{
    // Set the system clock speed to 32MHz.

    OSCCON = 0xF4;
 

    
    //Wait for the oscillator to switch to its new speed.
    while(OSCSTATbits.HFIOFR == 0);
    
    // T2PR: TMR4 PERIOD REGISTER (Page 287)
    // The timer runs at ~4 ms per tick 242.187 ticks/second after using 31 kHz and pre scale 1:128
    T4PR = TimerPeriod_us;
    
    // T2CLKCON: TIMER4 CLOCK SELECTION REGISTER (Page 306))
    // Bits 8:4 = 0b00000   These bits are not implemented
    // Bits 3:0 = 0b0100    Clock source is LFINTOSC 31 khz
    T4CLKCON = 0x04;

    // T2CON: TIMER4 CONTROL REGISTER (Page 307)
    // Bit  7   = 0b1       Timer is on.
    // Bits 6:4 = 0b101     Prescaler is 1:128 (Timer increments every 32 cycles)
    // Bits 3:0 = 0b0000    Postscaler is 1:1 
    T4CON = 0xF0;
            
    // TMR4IF is an interrupt flag, and is set whenever Timer2 expires.
    // We are not using interrupts, but it can still be checked manually.
    // It is cleared here as we start the timer.
    PIR4bits.TMR4IF = 0;
    
  
    
    
    return;
}
 * */