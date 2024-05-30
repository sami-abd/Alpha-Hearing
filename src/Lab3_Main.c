#include <pic.h>
#include "Lab3_Config.h"
#include "Lab3_SPI.h"
#include "Lab3_SD.h"
#include <xc.h>
#include <stdio.h>
#include "Lab2_Library.h"


//0x00,0x01,0x5F,0x90) HORSES IN THE BACK SONG SECTOR LOCATION


void Lab2_WriteDAQ(char WriteValue);
void initP(void);
void initDAC(void);
void initB(void);
void resetaudio(void);
void setsector(unsigned char a3, unsigned char a2, unsigned char a1, unsigned char a0);
void Pulse(void);
void Result(void);
void LED(char x);
void Lab2_ConfigureTimer2(char TimerPeriod_us);
void ConfigureTimer4(char TimerPeriod_us);
void wait(unsigned char s);




 unsigned short counter = 0; //setting a counter to keep track of where the pointer is in the memory of the SD card.
 //unsigned char x3 = 0, x2 = 0x03, x1 = 0x34, x0 = 0x50, b = 0, c=0,d=0; //32KHZ 2KHZ SIN
  //unsigned char x3 = 0, x2 = 0x03, x1 = 0x82, x0 = 0x70, b = 0, c=0,d=0; //88.2KHZ 8KHZ SIN 230,000 sector
  //unsigned char x3 = 0, x2 = 0x03, x1 = 0xD0, x0 = 0x90, b = 0, c=0,d=0; //44.1KHZ 8KHZ SIN 250,000 sector
  //unsigned char x3 = 0, x2 = 0x03, x1 = 0x82, x0 = 0x70, b = 0, c=0,d=0; //16KHZ 8KHZ SIN   
  unsigned char x3 = 0, x2 = 0x00, x1 = 0x00, x0 = 0x00, b = 0, c=0,d=0; //44.1 KHz 250Hz sin
  //unsigned char x3 = 0, x2 = 0x01, x1 = 0x5f, x0 = 0x90, b = 0, c=0,d=0; //OLD TOWN ROAD
  


 int e = 0;// intializing to sector 0 and c counter
 
 int sec = 0; //counter for seconds passed

 
 long unsigned int seconds [8]; //records how many full seconds each recording took
 
 long unsigned int ticks [8];  // record how many ticks/242 after every second, 2 <<
 
 double result [8];
 
 
 
#define INPUT PORTBbits.RB2
 
#define GLED PORTBbits.RB0
 
#define YLED PORTAbits.RA3
 
#define RLED PORTBbits.RB3
 
#define SECONDFLAG PIR4bits.TMR4IF
 
// increments c everytime the button is pressed and released

#define CheckButton() {            \
  if (INPUT == 1 && b==0)          \
           b=1;                    \
  if (INPUT == 0 && b==1)          \
       {                           \
           c++;                    \
           b=0;                     \
           wait(50);                \    
       }                           \
   }
 
 // increments variable sec after every second     
#define TimerCheck() { \
    if(SECONDFLAG) {   \
    sec++;           \
    SECONDFLAG = 0;    \
    }                  \
    }
       
           
void main(void) 
{

    //Set the system clock speed to 32MHz and wait for the ready flag.
    OSCCON = 0xF4;
    while(OSCSTATbits.HFIOFR == 0);
    
    //Initialize all B port peripherals
    initB();
    
    //Initialize all required peripherals
    initP();
    
    // Initialize the Digital to Analog Converter
    initDAC();
    
    // Initialize Timer2 at ~44100Hz
    Lab2_ConfigureTimer2(23);
    
    //Lights off
    LED(0);
     
    //Wait for Input to begin program
    while(INPUT==0)
        wait(50);
    
    while(INPUT==1)
        wait(50);
    
    //Start timer4 set to 1 second and begin timer
    ConfigureTimer4(242); 
    
    //lights on
    LED(1);
    
    
    while(1)
    {
    TimerCheck();
    CheckButton();
     
        if (counter==0)
          Start_Read(x3,x2,x1,x0); //beginning the read process every time the counter finishes a sector
     
        Lab2_WriteDAQ(SPI_Read()); //using pre made functions to write each byte through SPI to the DAC
          
        counter++;
          
        if(counter==512) //when the counter reaches the end of a sector this is required to skip the weird bit between.
          {
              SPI_Read(); //reading three times because after each sector there is 12 instructions that occur to load the next block of 512 bytes
              SPI_Read();
              SPI_Read();
              
              counter = 0; //reset the counter
              
              x0++; //increment the sector address
              
              //Increment the sector address 
              if(x0==0)
              {
                  x1++;
                  if(x1==0)
                  {
                      x2++;
                      if(x2==0)
                      {
                          x3++;
                      }
                  }
              }
        
        
              //***************************
              // When a frequency file is playing, if it reaches the end of the file or an input is detected
              // the following instructions occur:
              //
              // 1. Turn lights off
              // 2. Record time 
              // 3. Reset the timer 
              // 4. Wait for input from user 
              // 5. Set the address of the next frequency file
              // 6. Reset all constants
              // 7. Turn lights on
              // ***************************
              
              if(x3==0x00 && x2==0x00 && x1==0x03 && x0==0xA9 || c==1)//250Hz end
              {
                  LED(0);
                  seconds[0] = sec; // populating array with seconds elapsed
                  ticks[0] = (int)T2PR; // populating with ticks after whole number
                  SECONDFLAG = 0; //reset second timer and set flag to zero
                  while(INPUT==0)
                              wait(50);

                  while(INPUT==1)
                              wait(50);
                  setsector(0x00,0x00,0x27,0x10); //500Hz beginning 
                  counter = 0;
                  c=2;
                  sec = 0;
                  SECONDFLAG = 0; //reset second timer and set flag to zero
                  LED(1);
              }
              else if((x3==0x00 && x2==0x00 && x1==0x2A && x0==0xB9) || c==3)//500hz end
              {
                  LED(0);
                  seconds[1] = sec; // populating array with seconds elapsed
                  ticks[1] = (int)T2PR; // populating with ticks after whole number
                  SECONDFLAG = 0; //reset second timer and set flag to zero
                  while(INPUT==0)
                              wait(50);

                  while(INPUT==1)
                              wait(50);
                  setsector(0x00,0x00,0x4E,0x20); //750Hz beginning
                  counter = 0;
                  c=4;
                  sec = 0;
                  SECONDFLAG = 0; //reset second timer and set flag to zero
                  LED(1);
              }
              else if((x3==0x00 && x2==0x00 && x1==0x51 && x0==0xC9) || c==5)//750Hz end *
              {
                  LED(0);
                  seconds[2] = sec; // populating array with seconds elapsed
                  ticks[2] = (int)T2PR; // populating with ticks after whole number
                  SECONDFLAG = 0; //reset second timer and set flag to zero
                  while(INPUT==0)
                              wait(50);

                  while(INPUT==1)
                              wait(50);
                  setsector(0x00,0x00,0x75,0x30); //1000 Hz start
                  counter = 0;
                  sec = 0;
                  c=6;
                  SECONDFLAG = 0; //reset second timer and set flag to zero
                  LED(1);
              }
              else if((x3==0x00 && x2==0x00 && x1==0x78 && x0==0xD9) || c==7)//1000Hz end
              {
                  LED(0);
                  seconds[3] = sec; // populating array with seconds elapsed
                  ticks[3] = (int)T2PR; // populating with ticks after whole number
                  while(INPUT==0)
                          wait(50);

                  while(INPUT==1)
                          wait(50);
                  setsector(0x00,0x00,0x9C,0x40);//2000 Hz start
                  counter = 0;
                  sec = 0;
                  c=8;
                  SECONDFLAG = 0; //reset second timer and set flag to zero
                  LED(1);
              }
              else if((x3==0x00 && x2==0x00 && x1==0x9F && x0==0xE9) || c==9)//2000 Hz end
              {
                  LED(0);
                  seconds[4] = sec; // populating array with seconds elapsed
                  ticks[4] = (int)T2PR; // populating with ticks after whole number
                  while(INPUT==0);
                               wait(50);

                  while(INPUT==1)
                               wait(50);
                  setsector(0x00,0x00,0xC3,0x50);//4000Hz beginning
                  counter = 0;
                  c=10;
                  sec = 0;
                  SECONDFLAG = 0; //reset second timer and set flag to zero
                  LED(1);
              }
              else if((x3==0x00 && x2==0x00 && x1==0xC6 && x0==0xF9) || c==11)//4000 Hz end
              {
                  LED(0);
                  seconds[5] = sec; // populating array with seconds elapsed
                  ticks[5] = (int)T2PR; // populating with ticks after whole number
                  while(INPUT==0)
                              wait(50);

                  while(INPUT==1)
                              wait(50);
                  setsector(0x00,0x00,0xEA,0x60);//6000Hz beginning
                  counter = 0;
                  c=12;
                  sec = 0;
                  SECONDFLAG = 0; //reset second timer and set flag to zero
                  LED(1);
              }
              else if((x3==0x00 && x2==0x00 && x1==0xEE && x0==0x09) || c==13)//6000 Hz end
              {
                  LED(0);
                  seconds[6] = sec; // populating seconds array with seconds elapsed
                  ticks[6] = (int)T2PR; // populating ticks array after whole number
                  while(INPUT==0)
                              wait(50);

                  while(INPUT==1)
                              wait(50);
                  setsector(0x00,0x01,0x11,0x70);//8000Hz beginning
                  counter = 0;
                  c=14;
                  sec = 0;
                  SECONDFLAG = 0; //reset second timer and set flag to zero
                  LED(1);
              }
              else if((x3==0x00 && x2==0x01 && x1==0x14 && x0==0xB0) || c==15)//8000Hz end
              {
                  LED(0);
                  seconds[7] = sec; // populating array with seconds elapsed
                  ticks[7] = (int)T2PR; // populating with ticks after whole number
                  SECONDFLAG = 0; //reset second timer and set flag to zero
                  while(INPUT==0)
                              wait(50);

                  while(INPUT==1)
                              wait(50);
                  
                  //Using recorded timer values to calculate hearing
                  Result();
                  
              }
    }
    
          
          //Wait for Timer2 to expire
           while(!PIR1bits.TMR2IF);
        
        // Clear the interrupt flag. The timer restarts automatically.
        PIR1bits.TMR2IF = 0;
        
        }
}
    
  

  //function to write bytes from SPI into DAC
  void Lab2_WriteDAQ(char WriteValue) 
    {
       DAC1REFH = 0;
       DAC1REFL = WriteValue; //the 8 bit/0-256/1byte data from spi 
       DACLD = 1; //right justified 
       return;
    }

  void initP() //Initialize all required peripherals.
    {
       SPI_Init(); //initializing the serial peripheral interface "SD communication"
       SD_Init();  // initializing SD data transfer
    }
   
  void initDAC() // Initialize the Digital to Analog Converter pg 250/176
    {
       TRISA = 0xF3; //1111_0011 3: RA2 output 
       DAC1CON0 = 0xA0; // 1010_0000 7: DAC enabled, 6: right justified 5:DAC voltage enabled
    }
   
  void initB()
    {
       ANSELB = 0x00; //1== pin>=VIH
       TRISB = 0xD6; //all of B is inputs except B0,B3,B5-> output 0b 1101_0110
       
    }
   
   void resetaudio()
   {
       x0=0;
       x1=0;
       x2=0;
       x3=0;
       c=0;
   }
   
   void setsector(unsigned char a3, unsigned char a2, unsigned char a1, unsigned char a0)
   {
       x0 = a0;
       x1 = a1;
       x2 = a2;
       x3 = a3;
   }
   
   void Pulse()
   {
    Lab2_WriteDAQ(0xFF);
   }
   
   void LED(char x)
   {
      if(x==1)
        {                         
             GLED = 1;              
             YLED = 1;              
             RLED = 1;              
        }                      
      else if(x==0)
        {                    
             GLED = 0;              
             YLED = 0;              
             RLED = 0;              
                
        }   
   }

   
   void Result()
   {
       double average= 0;
       for (int i = 0; i<8; i++)
        {
         result[i] = seconds[i]+(ticks[i]/244);
         average += result[i];
        }
       average = average/8;
       
       if(average<=4) //excellent hearing greenlight flash
        {
           while(1)
           {
               TimerCheck();
               if(sec%2==0)
               GLED = 1;
               
               if(sec%2==1)
               GLED = 0;
               
               /*if(INPUT ==1)
                 break;*/
           }
               
        }
       else if (average>=4 && average<=7) //slight hearing loss
        {
           while(1)
           {
               TimerCheck();
               if(sec%2==0)
               YLED = 1;
               
               if(sec%2==1)
               YLED = 0;
               
               /*if(INPUT ==1)
                   break;*/
           }
        }
       else if(average>7)        //bad hearing
        {
           while(1)
           {
               TimerCheck();
               if(sec%2==0)
               RLED = 1;
               
               if(sec%2==1)
               RLED = 0;
               
               /*if(INPUT ==1)
                   break;*/
           }
        }
       }
   
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
//
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
    // Bits 6:4 = 0b101     Prescaler is 1:128 (Timer increments every 128 cycles)
    // Bits 3:0 = 0b0000    Postscaler is 1:1 
    T4CON = 0xF0;
            
      T4TMR = 0;
    // TMR4IF is an interrupt flag, and is set whenever Timer2 expires.
    // We are not using interrupts, but it can still be checked manually.
    // It is cleared here as we start the timer.
    PIR4bits.TMR4IF = 0;
    
    
    return;
}


//meant to receive 0 < s < 255, then waits s/255, 255 being 1.04 seconds
 void wait(unsigned char s)
       {
     int p = sec;
           ConfigureTimer4(s);
           while(SECONDFLAG==0);
           ConfigureTimer4(242); 
           sec = p;
       }