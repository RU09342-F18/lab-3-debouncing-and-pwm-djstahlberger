#include <msp430.h> 


/**
 * main.c
 */
void LEDSetup(void){

    P1SEL0 &= ~BIT0;             //Sets P1 to GPIO
    P1SEL1 &= ~BIT0;             //Sets P1 to GPIO
    P1DIR |= BIT0;          //Sets Pin 1.0 as an output
    P1OUT &= ~BIT0;          //Sets LED 1.0 on

    P2SEL0 &= ~BIT0;             //Sets P2 to GPIO
    P2SEL1 &= ~BIT0;             //Sets P2 to GPIO
    P2DIR |= BIT0;          //Sets Pin 2.0 as an output
    P2OUT &= ~BIT0;         //Pin 2.0 LED set off
}

void buttonSetup(void){

    P1SEL0 &= ~BIT1;         //Sets 1.1 to GPIO
    P1SEL1 &= ~BIT1;         //Sets 1.1 to GPIO
    P1DIR &= ~BIT1;         //Sets Pin 1.1 as an input
    P1REN |= BIT1;          //Enables a pull up/down resistor
    P1OUT |= BIT1;          //Sets Pin 1.1 to pull up
    P1IE  |= BIT1;            //enable interrupt on Pin 1.1
    P1IES |= BIT1;         //interrupt enables on falling edge
    P1IFG &= ~BIT1;         //clear interrupt flag
}

void timerB1Setup(void){

    TB0CTL = MC_1 + TBSSEL_2 + ID_2 + TBCLR + TBIE;         //Sets timer to up-mode on SMCLK with a divide by 4 and enables interrupt
    TB0CCR0 = 255;                                   //Sets pulse period to 255

    TB0CCR1 = 125;                                   //Starts with ~50% duty cycle
    TB0CCTL1 |= CCIE;                                //Enables Interrupt
}

void main(void){

    WDTCTL = WDTPW | WDTHOLD;       //stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;           // Unlocks GPIO ports

    LEDSetup();
    buttonSetup();
    timerB1Setup();

    _BIS_SR(LPM0_bits + GIE);   //global interrupts enabled
}




#pragma vector = TIMER0_B1_VECTOR;          //Setting the interrupt condition for the timer
__interrupt void Timer0B1 (void){

    switch(TB0IV){
            case 2:                         //Capture Compare detect for CCR0
                P1OUT &= ~BIT0;             //Turning off the LED for button state
                break;
            case 14:                        //Overflow detect
                P1OUT |= BIT0;
                break;
            }

}

#pragma vector = PORT1_VECTOR;              //Setting the interrupt condition for the button press
__interrupt void Port1 (void){

    P2OUT ^= BIT0;                         //toggle LED 2.0

    if(P1IES & BIT1){                      //If falling edge detected
        TB0CCR1 += 25;                     //Increases duty cycle by %10

        if(TB0CCR1 > 255){                 //If overflow happens
            TB0CCR1 = 0;                   //Value in CCR1 is set to 0
            P1OUT &= ~BIT0;                //Turn off 1.0
            TB0CTL = TBCLR;                //clear timer
        }
        else{
            TB0CTL = TBSSEL_2 + ID_2 + MC_1 + TBIE; // SMCLK, divide by 4, up mode, clear, enable interrupt
        }
    }

    P1IES ^= BIT1;                         //Toggles edge select
    P1IFG &= ~BIT1;                        //Clears interrupt flag

}
