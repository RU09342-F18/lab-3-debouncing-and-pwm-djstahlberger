# Hardware PWM
This code uses a hardware pulse width modulation signal to change LED brightness. This only works for certain pins connected to the CCR registers.

## How it works
A 1MHz clock is divided to arround 250 kHz to let the counter loop 1,000 times a second. When the button is pressed it increases the duty cycle by 10% and when it reaches 100% it loops back to 0.

## Where does it work
This code works on the G2553 and the FR2311 with changes in the timer name and the pin names.
