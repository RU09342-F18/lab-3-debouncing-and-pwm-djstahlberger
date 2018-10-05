#include <msp430.h> 


/**
 * main.c
 */
void LEDSetup(void){

    P1SEL &= ~BIT0;             //Sets P1 to GPIO
    P1DIR |= BIT0;          //Sets Pin 1.0 as an output
    P1OUT &= ~BIT0;          //Sets LED 1.0 on

    P4SEL &= ~BIT7;             //Sets P4 to GPIO
    P4DIR |= BIT7;          //Sets Pin 4.7 as an output
    P4OUT &= ~BIT7;         //Pin 4.7 LED set off
}

void buttonSetup(void){

    P2SEL &= ~BIT1;         //Sets 2.1 to GPIO
    P2DIR &= ~BIT1;         //Sets Pin 2.1 as an input
    P2REN |= BIT1;          //Enables a pull up/down resistor
    P2OUT |= BIT1;          //Sets Pin 2.1 to pull up
    P2IE  |= BIT1;            //enable interrupt on Pin 2.1
    P2IES |= BIT1;         //interrupt enables on falling edge
    //P2IFG &= ~BIT1;         //clear interrupt flag
}

void timerA0Setup(void){

    TA0CTL = MC_1 + TASSEL_2 + ID_2 + TACLR + TAIE;         //Sets timer to up-mode on SMCLK with a divide by 4 and enables interrupt
    TA0CCR0 = 255;                                   //Sets pulse period to 255

    TA0CCR1 = 125;                                   //Starts with ~50% duty cycle
    TA0CCTL1 |= CCIE;                                //Enables Interrupt

    //TA0CCTL0 &= ~TAIFG;                              //Clears flag
   // TA0CCTL1 &= ~TAIFG;                              //Clears flag
}

void main(void){

    WDTCTL = WDTPW | WDTHOLD;       //stop watchdog timer

    LEDSetup();
    buttonSetup();
    timerA0Setup();

    _BIS_SR(LPM0_bits + GIE);   //global interrupts enabled
}




#pragma vector = TIMER0_A1_VECTOR;          //Setting the interrupt condition for the timer
__interrupt void Timer0A0 (void){

    switch(TA0IV){
            case 2:                         //Capture Compare detect for CCR0
                P1OUT &= ~BIT0;             //Turning off the LED for button state
                break;
            case 14:                        //Overflow detect
                P1OUT |= BIT0;
                break;
            }

}

#pragma vector = PORT2_VECTOR;              //Setting the interrupt condition for the button press
__interrupt void Port1 (void){

    P4OUT ^= BIT7;                         //toggle LED 4.7

    if(P2IES & BIT1){                      //If falling edge detected
        TA0CCR1 += 25;                     //Increases duty cycle by %10

        if(TA0CCR1 > 255){                 //If overflow happens
            TA0CCR1 = 0;                   //Value in CCR1 is set to 0
            P1OUT &= ~BIT0;                //Turn off 1.0
            TA0CTL = TACLR;                //clear timer
        }
        else{
            TA0CTL = TASSEL_2 + ID_2 + MC_1 + TAIE; // SMCLK, divide by 4, up mode, clear, enable interrupt
        }
    }

    P2IES ^= BIT1;                         //Toggles edge select
    P2IFG &= ~BIT1;                        //Clears interrupt flag

}
