#include <msp430.h> 


/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	PM5CTL0 &= ~LOCKLPM5;
	//UART
    CSCTL0_H = CSKEY_H;                                     // Unlock CS registers
    CSCTL1 = DCOFSEL_3 | DCORSEL;                           // Set DCO to 8MHz
    CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK;
    CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;                   // Set all dividers
    CSCTL0_H = 0;                                           // Lock CS registers

    P2SEL0 &= ~(BIT0 | BIT1);                               //P2.0 => RXD
    P2SEL1 |= BIT0 + BIT1;                                  //P2.1 => TXD

                                                            // Configure USCI_A0 for UART mode
    UCA0CTLW0 = UCSWRST;                                    // Put eUSCI in reset
    UCA0CTLW0 |= UCSSEL__SMCLK;                             // CLK = SMCLK
    UCA0BRW = 52;                                           // 8000000/16/9600
    UCA0MCTLW |= UCOS16 | UCBRF_1 | 0x4900;                 //modulation
    UCA0CTLW0 &= ~UCSWRST;                                  // Initialize UCA
    UCA0IE |= UCRXIE;                                       // Enable USCI_A0 RX interrupt
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //Input to the ADC
    //P1OUT  &= ~BIT0;                                      // Clear LED to start
    P1SEL1 |= BIT3;                                         // Configure P1.3 for ADC
    P1SEL0 |= BIT3;

    //Timer A
    TA0CCTL0 = CCIE;                                        //Timer A interrupt enable
    TA0CCR0 = 4096-1;                                     //Set period
    TA0CTL = TASSEL_1 + MC_1 + ID_3;                        //ACLK, UPMODE, DIV 8
    //ADC
    REFCTL0 |= REFON + REFTCOFF + REFVSEL_0;                //Enable reference voltage at 1.2v
    ADC12CTL0 |= ADC12SHT0_2 + ADC12ON;                     // Set sample time
    ADC12CTL1 |= ADC12SHP;                  // Enable sample timer
    ADC12CTL2 |= ADC12RES_2;                                // 12-bit conversion results
    ADC12MCTL0 = ADC12INCH_3 | ADC12VRSEL_1;                // Vref+ , Input channel A3
    ADC12IER0 |= ADC12IE0;                                  // Enable ADC conv complete interrupt
    P1OUT = BIT0;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    while (REFCTL0 & REFGENBUSY);                           // If ref generator busy, WAIT
    REFCTL0 |= REFVSEL_0 + REFON;                           // Enable ADC internal reference and set to 1.2V

    while (!(REFCTL0 & REFGENRDY));                         // Wait for reference generator
    __enable_interrupt();                                   //Enable all interrupts.

    while (1)
    {
        __bis_SR_register(LPM0 + GIE);                      // Go into LPM0 + general interrupt enable
        __no_operation();                                   // For debugger
    }

	return 0;
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR(void)
{
    ADC12CTL0 |= ADC12SC + ADC12ENC;

}

#pragma vector=ADC12_B_VECTOR
__interrupt void ADC12ISR(void)
{
    while (!(UCA0IFG&UCTXIFG));                             //While the Transmit Flag is clear
    UCA0TXBUF = ADC12MEM0;
}


