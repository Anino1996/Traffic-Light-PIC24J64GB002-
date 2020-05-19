/*
 * File:   Traffic_Light.c
 * Author: Albert Aninagyei Ofori
 *
 * Created on March 8, 2020, 10:29 PM
 */



#include <p24FJ64GB002.h>



/* Port B pin states for all 8 sequences.*/
#define Green_1 0x8221          //M1 Green and all other lights red
#define Green_2 0x820C          //M2 Green and all other lights red
#define Green_3 0x80A4          //S1 Green and all other lights red
#define Green_4 0x2224          //S2 Green and all other lights red
#define Yellow_1 0x8222         //M1 Yellow and all other lights red
#define Yellow_2 0x8214         //M2 Yellow and all other lights red
#define Yellow_3 0x8124         //S1 Yellow and all other lights red
#define Yellow_4 0x4224         //S2 Yellow and all other lights red

#define OneSecondDelay 0x7A12   //Number of cycles required to achieve a 1s timer delay for prescaler value of 1:64 

/* Config words required to give I/O PORTS A and B precedence over other functions at the same pin. This eliminates competition, enabling all RA or RB pins to be used as I/O */
_CONFIG1(JTAGEN_OFF)            
_CONFIG3(SOSCSEL_IO)            

/* Values initialized as volatile to enable manipulation inside ISR and other subroutines */
volatile int idx,Cnt,Delay,GreenDelayMainStreet,YellowDelay,GreenDelaySmallStreet;                      // Define array index, delay counter, and traffic light wait times respectively.
volatile int lights[8] = {Green_1, Yellow_1, Green_2, Yellow_2,Green_3, Yellow_3,Green_4, Yellow_4};    //Array of defined traffic light states in sequential order.

/* Define subroutine to set delay for normal operating hours */
volatile void SetNormalDelay()
{
    GreenDelayMainStreet = 10;  //Traffic lights on main street stay green for 10 seconds
    GreenDelaySmallStreet = 7;  //Traffic lights on small street stay green for 7 seconds
    YellowDelay = 3;            //Yellow transition between green and red lasts 3 seconds
}

/* Define subroutine to set reduced delay for office hours */
volatile void SetOfficeHoursDelay()
{
    GreenDelayMainStreet = 5;   //Traffic lights on main street stay green for 5 seconds
    GreenDelaySmallStreet = 4;  //Traffic lights on small street stay green for 4 seconds
    YellowDelay = 3;            //Yellow transition between green and red lasts 3 seconds
}

/* Define subroutine to set up Timer 2 interrupt */ 
void setupTimer2()
{   
    TMR2 = 0;                   // Reset Timer 2 value
    PR2 = OneSecondDelay;       // Set Timer 2 period register such that T2 interrupt is triggered after 1 second.
    IPC1bits.T2IP = 0x6;        // Set Timer 2 interrupt priority to 6
    IFS0bits.T2IF = 0;          // Reset Timer 2 interrupt flag
    IEC0bits.T2IE = 1;          // Enable Timer 2 interrupt
    T2CON = 0x8020;             // Enable Timer 2 with a prescaler ratio of 1:64
}

/* Define subroutine to set up change notification interrupt */
void configure_CN()
{
    TRISAbits.TRISA4 = 1;       // Set pin 12 as an input.
    IPC4bits.CNIP = 0x7;        // Set CNInterrupt priority to 7
    IFS1bits.CNIF = 0;          // Reset CNInterrupt flag
    CNPU1bits.CN0PUE =1;        // Enable pull-up resistor for CN0 (PIN 12)
    CNEN1bits.CN0IE = 1;        // Enable interrupt for state change at CN0 (PIN 12)
    IEC1bits.CNIE = 1;          // Enable change notification interrupts
}

/* CN Interrupt service routine to enable shorter delay for office hours */
void __attribute__ ((__interrupt__)) _CNInterrupt(void)
{ 
    SetOfficeHoursDelay();      // Call subroutine to set shorter delay    
    IFS1bits.CNIF = 0;          // Reset CNInterrupt flag
}

/* Timer 2 interrupt service routine for traffic signal change */
void __attribute__((__interrupt__, __shadow__)) _T2Interrupt(void)
{   Cnt++;                      // Increment delay counter value after every second.
    if(Cnt >= Delay){
        idx++;                  // Increment array index value to next signal
        if(idx >= 8){
            idx = 0;            // Start from first array value if index is greater than index of the last sequence.
        }
/* Determine delay based on street and signal type */                
        if(idx%2 == 0 && idx < 4){
            Delay = GreenDelayMainStreet;   
        }
        else if(idx%2 == 0 && idx >= 4) {
            Delay = GreenDelaySmallStreet;
        }
        else {
            Delay = YellowDelay;
        }
            
        LATB = lights[idx];     // Display next signal in array.
        Cnt = 0;                // Reset delay counter value.
              
}
    IFS0bits.T2IF = 0;          // Reset interrupt flag
    TMR2=0;                     // Reset Timer 2
}
   
   
    
int main(int argc, char** argv) { 
    TRISB = 0x0000;             // Initialize port B as output.
    SetNormalDelay();           // Set delay values for Normal hours
    Delay = GreenDelayMainStreet;   // Set delay for first signal
    Cnt = 0;                        
    idx = 0;                        
    LATB = lights[idx];             // Display first traffic signal    
    setupTimer2();                  // Initialize timer 2 and its interrupt.
    configure_CN();                 // Initialize Change notification interrupt            
    while(1);
              
    return (0); 

            

}
