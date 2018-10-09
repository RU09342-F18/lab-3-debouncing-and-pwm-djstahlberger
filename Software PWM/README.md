# Software PWM
This code uses a software pulse width modulation signal to change LED brightness.

## How does it work
A 1MHz clock is divided to around 250 kHz to let the counter loop 1,000 times a second. When the button is pressed it increases the duty cycle by 10% and when it reaches 100% it loops back to 0.

## Where does it work
This code works with the FR2311 and and the F5529 with changing timer names and pin names
