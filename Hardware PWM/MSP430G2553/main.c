#include <msp430.h> 


/**
 * main.c
 */
void LEDSetup(void){

    P2DIR |= BIT1;              // Sets P2.1 to output
       P2SEL |= BIT1;              // Connects P2.1 to the timer
       P2SEL2 &= ~BIT1;            // Connects P2.1 to the timer
}

void buttonSetup(void){

        P1DIR &= ~BIT3;             // Sets P1.3 in the input direction
        P1REN |= BIT3;              // P1.3 pullup/pulldown resistor enabled
        P1OUT |= BIT3;              // P1.3 pullup set

        P1IE |= BIT3;               // P1.3 interrupt enabled
        P1IES |= BIT3;              // P1.3 interrupt flag is set to trigger on falling edge
        P1IFG &= ~BIT3;             // P1.3 interrupt flag is cleared
}

void timerB1Setup(void){

        TA1CCR0 = 512-1;            // Sets the PWM Period
        TA1CCTL1 = OUTMOD_7;        // CCR1 reset/set
        TA1CCR1 = 0;                // CCR1 PWM duty cycle
        TA1CTL = TASSEL_2 + MC_1;   // SMCLK, up mode
}

void main(void){

    WDTCTL = WDTPW | WDTHOLD;       //stop watchdog timer

    LEDSetup();
    buttonSetup();
    timerB1Setup();

    _BIS_SR(LPM0_bits + GIE);   //global interrupts enabled
}





#pragma vector = PORT1_VECTOR;              //Setting the interrupt condition for the button press
__interrupt void Port1 (void){

    if (TA1CCR1 <= 510){        // If duty cycle is not 100%
           TA1CCR1 += 51;          // Increase duty cycle by 10%
       } else{                     // If not
           TA1CCR1 = 0;            // Sets duty cycle to 0%
       }
       P1IFG &= ~BIT3;             // P1.3 interrupt flag is cleared
   }
