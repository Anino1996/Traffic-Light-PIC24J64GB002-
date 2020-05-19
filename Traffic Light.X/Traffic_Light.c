/*
 * File:   Traffic_Light.c
 * Author: Albert Aninagyei Ofori
 *
 * Created on March 8, 2020, 10:29 PM
 */


#include "xc.h"
#include <p24FJ64GB002.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>

#define Green_1 0x8221
#define Green_2 0x820C
#define Green_3 0x80A4
#define Green_4 0x2224  
#define Yellow_1 0x8222
#define Yellow_2 0x8214
#define Yellow_3 0x8124
#define Yellow_4 0x4224
#define OneSecondDelay 0x7A12

_CONFIG1(JTAGEN_OFF)
_CONFIG3(SOSCSEL_IO)


volatile int idx,Cnt,Delay,GreenDelayMain,YellowDelay,GreenDelaySmall;
volatile int lights[8] = {Green_1, Yellow_1, Green_2, Yellow_2,Green_3, Yellow_3,Green_4, Yellow_4};


void SetNormalDelay(){
    GreenDelayMain = 15;
    GreenDelaySmall = 10;
    YellowDelay = 5;
}

volatile void SetOfficeHoursDelay(){
    GreenDelayMain = 10;
    GreenDelaySmall = 6;
    YellowDelay = 3;
}


void setupTimer2()
{   
    TMR2 = 0;
    PR2 = OneSecondDelay;
    IPC1bits.T2IP = 0x7;
    IFS0bits.T2IF = 0;
    IEC0bits.T2IE = 1;
    T2CON = 0x8020;   
}

void __attribute__((__interrupt__, __shadow__)) _T2Interrupt(void){
    
    Cnt++; 
    if(Cnt >= Delay){
        idx++;
        if(idx >= 8){
            idx = 0;
    }
                
        if(idx%2 == 0 && idx < 4){
            Delay = GreenDelayMain;
        }
        else if(idx%2 == 0 && idx >= 4) {
            Delay = GreenDelaySmall;
        }
        else {
            Delay = YellowDelay;
        }
            
        LATB = lights[idx];
        Cnt = 0;
              
}
    IFS0bits.T2IF = 0;
    TMR2=0;  
}

int main(int argc, char** argv) { 
    SetNormalDelay();
    TRISB = 0x0080; 
    Cnt = 0;
    idx = 0;
    LATB = lights[idx];
    Delay = GreenDelayMain;
    setupTimer2();
           
    IPC4bits.CNIP = 0x5;  
    IFS1 = 0;          
    CNPU2bits.CN30PUE =1;
    CNEN2bits.CN30IE = 1; 
    IEC1 = 0x0008;    
    while(1){}
              
    return (0); 

}

