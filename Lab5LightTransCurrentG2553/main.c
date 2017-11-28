#include <msp430.h> 


/**
 * main.c
 */
#include <msp430.h>


/**
 * main.c
 * based heavily on code by Manpreet Singh Minhas,
 * posted at: https://learningmsp430.wordpress.com/2014/01/01/thermometer-using-lm35/
 */

void UARTInit(void);

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    UARTInit(); //enable UART

    ADC10CTL0 |= ADC10ON + SREF1; //Turn ADC on
    ADC10CTL1 |= INCH_0|ADC10SSEL_1|CONSEQ_1; //Input on p1.0, using aclk, sequence of channels, no repeat
    ADC10AE0  |= BIT0|BIT1; //P1.0 and P1.1 enabled for analog input
    ADC10CTL0 |= ENC|ADC10SC; //conversion enabled, sample and conversion started
    TA0CCR0 = 0x32768; //set CCR0 to 32k, so once per second
    TA0CTL = TASSEL_1|ID_0|MC_1|TAIE;// ACLK, Divide by 1, Up mode, Interrupt enable
    _BIS_SR(LPM3_bits + GIE);// Enter Low power mode

    return 0;
}

#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer_A(void)
{
    ADC10CTL0 |= ENC|ADC10SC; //read from adc
    int temp = (ADC10MEM*35)/100; //multiply adc reading by 35, divide by 100. This puts results into degrees
    TA0CTL &= ~(TAIFG);
    UCA0TXBUF = temp;
    __no_operation();
}

void UARTInit(void){
    if (CALBC1_1MHZ==0xFF)                 // If calibration constant erased
     {
       while(1);                               // do not load, trap CPU!!
     }
     DCOCTL = 0;                               // Select lowest DCOx and MODx settings
     BCSCTL1 = CALBC1_1MHZ;                    // Set DCO
     DCOCTL = CALDCO_1MHZ;
     P1SEL = BIT1 + BIT2 ;                     // P1.1 = RXD, P1.2=TXD
     P1SEL2 = BIT1 + BIT2 ;                    // P1.1 = RXD, P1.2=TXD
     UCA0CTL1 |= UCSSEL_2;                     // SMCLK
     UCA0BR0 = 104;                            // 1MHz 9600
     UCA0BR1 = 0;                              // 1MHz 9600
     UCA0MCTL = UCBRS0;                        // Modulation UCBRSx = 1
     UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
     IE2 |= UCA0RXIE;                          // Enable USCI_A0 RX interrupt
}
