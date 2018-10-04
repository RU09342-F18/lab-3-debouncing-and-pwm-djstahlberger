#include <msp430.h> 


/**
 * main.c
 */

int tempValue; //Temp value for making sure the port interrupt runs when it's supposed to
void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    P1SEL = 0; //sets pin function to I/O

    P1DIR |= BIT0; //Sets P1.0 to output
    P1OUT &= ~BIT0; //Turns LED off initially

    P1DIR &= ~BIT3; //Sets P1.3 to input
    P1REN |= BIT3; //Enables the pull up-down resistor
    P1IN &= ~BIT3; //Sets the pull down resistor

    P1IE |= BIT3; //Enables the interrupt for P1.3
    P1IES |= BIT3; //Enables falling edge for P1.3
    P1IFG &= ~BIT3; //Clears the flag

    // Enter LPM0 and enable global interrupts
    _BIS_SR(GIE);

    //Setting up TimerA module
    TA0CTL = TASSEL_1 + MC_0 + TAIE;
    TA0CCR0 = 10000;


    tempValue = 1;


    while(1){
    }
}

#pragma vector = TIMER0_A1_VECTOR;
    __interrupt void Timer0(void){
        switch(TA0IV){
        case 2:
            break;
        case 4:
            break;
        case 10:
            tempValue = 1;
            TA0CTL = TACLR;
            TA0CTL = TASSEL_1 + MC_0;
            break;
        }

    }

    #pragma vector = PORT1_VECTOR;
    __interrupt void Port_1(void){
        if(tempValue){
            P1OUT ^= BIT0;
            TA0CTL = TACLR;
            TA0CTL = TASSEL_1 + MC_1 + TAIE;
            tempValue = 0;
        }
        P1IFG &= ~BIT3;
    }
