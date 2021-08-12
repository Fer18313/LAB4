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
#include "LCD.h"
#define _XTAL_FREQ 8000000 

//FUNCTION PROTOTYPES
void initCONFIG(void);
void startLCD(void);
void I2C_command(void);
void read_2_LCD (uint16_t z);

// VARIABLES
uint16_t var0=0;
uint8_t unit0;
uint8_t c;
uint16_t temp;
uint8_t dec0;
uint8_t dec1;


void initCONFIG(void); 
void char_2_dec(uint16_t s);
void I2C_Command(void);

void main(void) {
    initCONFIG(); 
    Lcd_Init();                     
    Lcd_Clear();  
    Lcd_Set_Cursor(1,1); 
    Lcd_Write_String(" S1:   S2:   S3:"); 
    while(1){  
        I2C_Command();
        var0= var0*1.961; 
        char_2_dec(var0);             
        Lcd_Set_Cursor(2,1);        
        Lcd_Write_Char(unit0);
        Lcd_Write_Char(46);
        Lcd_Write_Char(dec0);
        Lcd_Write_Char(dec1);
        Lcd_Write_String("  ");        
        char_2_dec(c);           
        Lcd_Write_Char(unit0);
        Lcd_Write_Char(dec0);
        Lcd_Write_Char(dec1);
        Lcd_Write_String("  ");        
        char_2_dec(temp);                  
        Lcd_Write_Char(unit0);
        Lcd_Write_Char(dec0);
        Lcd_Write_Char(dec1);
    }
    return;
}
void I2C_Command(void){
        I2C_Master_Start();         
        I2C_Master_Write(0x71);     
        var0 = I2C_Master_Read(0); 
        I2C_Master_Stop();        
        __delay_ms(200);       
        I2C_Master_Start();         
        I2C_Master_Write(0x81);     
        c = I2C_Master_Read(0); 
        I2C_Master_Stop();         
        __delay_ms(200);       
        I2C_Master_Start();         
        I2C_Master_Write(0x90);     
        I2C_Master_Write(0xEE);     
        I2C_Master_Stop();          
        __delay_ms(200);        
        I2C_Master_Start();         
        I2C_Master_Write(0x90);     
        I2C_Master_Write(0xAA);    
        I2C_Master_Stop();        
        __delay_ms(200);        
        I2C_Master_Start();         
        I2C_Master_Write(0x91);     
        temp = I2C_Master_Read(0); 
        I2C_Master_Stop();
        __delay_ms(200);        
}
void char_2_dec(uint16_t z){      
    uint16_t zeta;
    zeta = z;                  
    unit0 = (zeta/100) ;              
    zeta = (zeta - (unit0*100));
    dec0 = (zeta/10);             
    zeta = (zeta - (dec0*10));
    dec1 = (zeta);              
    unit0 = unit0 + 48;     
    dec0 = dec0 + 48;
    dec1 = dec1 + 48;   
}
void initCONFIG(void){    
    OSCCONbits.IRCF2 = 1; 
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 1;
    OSCCONbits.SCS = 1;
    ANSELH = 0;
    ANSEL = 0;   
    TRISA = 0; 
    TRISB = 0;
    TRISC = 0; 
    TRISD = 0; 
    TRISE = 0;   
    PORTA = 0; 
    PORTB = 0;
    PORTC = 0;    
    PORTD = 0;
    PORTE = 0;
    INTCONbits.GIE = 0; 
    INTCONbits.PEIE = 0;           
    I2C_Master_Init(100000); 
}