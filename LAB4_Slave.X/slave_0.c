/*
 * File:   slave_0.c
 * Author: Fernando Sandoval
 * Carne: 18313
 * Digital 2
 * Created on August 10, 2021, 12:39 PM
 */
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)
// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// LIBRARIES
#include <xc.h>
#include <stdint.h>
#include "I2C.h"
#define _XTAL_FREQ 8000000 

// VARIABLES
uint8_t var0; 
uint8_t ADC0; 
uint8_t cont=0;

// FUNCTION PROTOTYPES
void initCONFIG(void); 

// MAIN CODE
void main(void) {
    initCONFIG(); 
    ADCON0bits.GO = 1;
    while(1){
        cont++;
    }
    return;
}

void __interrupt()isr(void){
    di();                   
     if (ADIF == 1){                            
        ADC0 = ADRESH;
        ADIF = 0; 
        __delay_us(60);
        ADCON0bits.GO = 1;
    }
       if(PIR1bits.SSPIF == 1){ 
        SSPCONbits.CKP = 0;
        if ((SSPCONbits.SSPOV) || (SSPCONbits.WCOL)){
            var0 = SSPBUF;                 
            SSPCONbits.SSPOV = 0;      
            SSPCONbits.WCOL = 0;       
            SSPCONbits.CKP = 1;         
        }

        if(!SSPSTATbits.D_nA && !SSPSTATbits.R_nW) {
            var0 = SSPBUF;                 
            PIR1bits.SSPIF = 0;        
            SSPCONbits.CKP = 1;         
            while(!SSPSTATbits.BF);     
            __delay_us(250);           
        }else if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW){
            var0 = SSPBUF;
            BF = 0;
            SSPBUF = ADC0;
            SSPCONbits.CKP = 1;
            __delay_us(250);
            while(SSPSTATbits.BF);
        }
        PIR1bits.SSPIF = 0;    
    }
    ei();                        
}

void initCONFIG(void){
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 1;
    OSCCONbits.SCS = 1;
    ANSELH = 0;
    ANSEL = 0b00000001;
    TRISA = 0b00000001; 
    TRISB = 0;
    TRISC = 8; 
    TRISD = 0; 
    TRISE = 0; 
    PORTA = 0;
    PORTB = 0;
    PORTC = 0;    
    PORTD = 0;
    PORTE = 0;
    ADCON1bits.ADFM = 0; 
    ADCON1bits.VCFG0 = 0; 
    ADCON1bits.VCFG1 = 0; 
    ADCON0bits.ADCS = 0b10; 
    ADCON0bits.CHS = 0;           
    ADCON0bits.ADON = 1;
    __delay_us(50); 
    ADCON0bits.GO = 1;
    INTCONbits.GIE = 1;  
    INTCONbits.PEIE = 1;           
    I2C_Slave_Init(0x70); 
}