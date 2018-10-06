#include <msp430.h> 


/**
 * main.c
 */
void LEDSetup(void){

       P1DIR |= BIT0;              // Sets P1.0 to output


       P2DIR |= BIT0;              // Sets P2.0 to output
       P2SEL0 |= BIT0;              // Connects P2.1 to the timer
       P2SEL1 &= ~BIT1;            // Connects P2.1 to the timer
}

void buttonSetup(void){

        P1DIR &= ~BIT1;             // Sets P1.1 in the input direction
        P1REN |= BIT1;              // P1.1 pullup/pulldown resistor enabled
        P1OUT |= BIT1;              // P1.1 pullup set

        P1IE |= BIT1;               // P1.1 interrupt enabled
        P1IES |= BIT1;              // P1.1 interrupt flag is set to trigger on falling edge
        P1IFG &= ~BIT1;             // P1.1 interrupt flag is cleared
}

void timerB1Setup(void){

        TB1CCR0 = 1000; //Set period
        TB1CCTL1 = OUTMOD_7; //set PWM to reset/set
        TB1CCR1 = 500; //ON period
        TB1CTL = TBSSEL_2 + MC_1 + ID_2; //SMCLK, up mode, divide by 4
}

void main(void){

    WDTCTL = WDTPW | WDTHOLD;       //stop watchdog timer

    PM5CTL0 &= ~LOCKLPM5; //unlock GPIO

    LEDSetup();
    buttonSetup();
    timerB1Setup();

    _BIS_SR(LPM0_bits + GIE);   //global interrupts enabled
}





#pragma vector =PORT1_VECTOR //button interrupt
__interrupt void PORT1(void)
{
    if (P1IES & BIT1){              //if falling edge
        P1OUT |= BIT0; //turn on LED
        P1IES &= ~BIT1;//change to detect falling edge
    }
    else{ //if rising edge
        P1OUT &= ~BIT0;//turn off LED
        P1IES |= BIT1;//change to rising edge detect
    }

    if(TB1CCR1 < 1000 && TB1CCR1 >= 0){ //if CCR1 is between 0 and 1000
        TB1CCR1 +=100; //increase duty cycle by 10%
    }

    else if (TB1CCR1 >= 1000){ //if duty cyle is 100%
        TB1CCR1 = 0; //clear duty cycle to 0%
    }
    else{
        TB1CCR1 = 100; //value outside range gets 10%
    }

    P1IFG &= ~BIT1; //clear button interrupt flag
}
