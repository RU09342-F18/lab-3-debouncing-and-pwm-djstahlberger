#include <msp430.h> 

int main(void){
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer

    PM5CTL0 &= ~LOCKLPM5;

    P1DIR |= BIT0;              // Sets P1.0 to output
    P1OUT &= ~BIT0;             // Clears P1.0 output register value

    P1DIR &= ~BIT1;             // Sets P1.1 to input
    P1REN |= BIT1;              // P1.1 pullup/pulldown resistor enabled
    P1OUT |= BIT1;              // P1.1 pullup

    P1IE |= BIT1;               // P1.1 interrupt enabled
    P1IES |= BIT1;              // P1.1 interrupt flag on falling edge
    P1IFG &= ~BIT1;             // P1.1 interrupt flag is cleared

    TB0CCTL0 |= CCIE;           // Capture/compare interrupt enabled
    TB0CCR0 = 0x31;             // Sets the compare register to be triggered every 1 ms

    _BIS_SR(LPM0_bits + GIE);   // low power mode and global interrupts
    return 0;
}

#pragma vector = PORT1_VECTOR
__interrupt void button_interrupt(void){
    TB0CTL = TBSSEL_2 + MC_2 + TBCLR;
                                // Sets timer to A clock, sets the clock to
                                // continuous mode and clears the clock
    P1IFG &= ~BIT1;             // P1.1 interrupt flag is cleared
    P1IES ^= BIT1;              // Toggles the edge triggers the interrupt
}

#pragma vector = TIMER0_B0_VECTOR
__interrupt void timer0_b_interrupt(void){
    if (!(P1IN & BIT1)){
        P1OUT ^= BIT0;              // Toggles the state of P1.0
    }
    TB0CTL = MC_0;              // Halts the timer
}
