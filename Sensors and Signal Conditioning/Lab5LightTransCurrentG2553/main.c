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

    ADC10CTL1 |= INCH_7 + SHS_1; //Input on p1.7, sample and hold on
    ADC10AE0  |= BIT5 + BIT7; //P1.7 and P1.5 enabled for analog input
    ADC10CTL0 |= SREF_1 + ADC10SHT_2 + REFON + ADC10ON + ADC10IE;

    TA0CCR0 = 0x32768; //set CCR0 to 32k, so once per second
    TA0CTL = TASSEL_1|ID_0|MC_1;// ACLK, Divide by 1, Up mode, Interrupt enable
    TA0CCTL0 = CCIE;

    __bis_SR_register(GIE);// Enter Low power mode
    while(1);
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A(void)
{
    ADC10CTL0 |= ENC|ADC10SC; //read from adc

    UCA0TXBUF = ADC10MEM;
}

#pragma vector=ADC10_VECTOR
__interrupt void ADC10_Interrupt(void){
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
     //IE2 |= UCA0RXIE;                          // Enable USCI_A0 RX interrupt
}
