#include <xc.h>

#include <stdio.h>
#include <stdlib.h>
//#include "myprintf.h"
#include "configuration_bits.h"
#include "main.h"
#include "pcf8563.h"
#include "i2c.h"
#include "lcd1602.h"

#define MODE    PORTBbits.RB0
#define UP      PORTBbits.RB1
#define DOWN    PORTBbits.RB2
#define DEBOUNCING 1000

unsigned char temp[16] = {};
unsigned int year = 2022;
unsigned int month = 1;
unsigned int day = 17;
unsigned int hour = 21;
unsigned int minute = 5;
unsigned int second = 3;
const int month_day[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

void printDate(){
    PCF8563_setDate(year, month, day);
    sprintf(temp, "%04d-%02d-%02d ", year, month, day);
    lcd1602_noBlink();
    lcd1602_home();
    lcd1602_print(temp, 10);
    __delay_ms(DEBOUNCING);
}

void printTime(){
    PCF8563_setTime(hour, minute, second);
    sprintf(temp, "%02d:%02d:%02d", hour, minute, second);
    lcd1602_noBlink();
    lcd1602_cursorNextLine();
    lcd1602_print(temp, 8);
    __delay_ms(DEBOUNCING);
}

void myISR(){
    // set year
    while(MODE == 1){
        lcd1602_setCursor(0, 3);
        lcd1602_blink();
            if(UP == 0 && year <= 2022){
                year++;
                printDate();
            }else if(DOWN == 0 && year >= 2022){
                year--;
                printDate();
            }
    }
    MODE = 1;
    __delay_ms(DEBOUNCING);
    // set month
    while(MODE == 1){
        lcd1602_setCursor(0, 6);
        lcd1602_blink();
            if(UP == 0 && month <= 11){
                month++;
                printDate();
            }else if(DOWN == 0 && month >= 2){
                month--;
                printDate();
            }
    }
    MODE = 1;
    __delay_ms(DEBOUNCING);
    // set day
    while(MODE == 1){
        lcd1602_setCursor(0, 9);
        lcd1602_blink();
            if(UP == 0 && day <= month_day[month] - 1){
                day++;
                printDate();
            }else if(DOWN == 0 && day >= 2){
                day--;
                printDate();
            }
    }
    MODE = 1;
    __delay_ms(DEBOUNCING);
    // set hour
    while(MODE == 1){
        lcd1602_setCursor(1, 1);
        lcd1602_blink();
            if(UP == 0 && hour <= 22){
                hour++;
                printTime();
            }else if(DOWN == 0 && hour >= 1){
                hour--;
                printTime();
            } 
    }
    MODE = 1;
    __delay_ms(DEBOUNCING);
    // set minute
    while(MODE == 1){
        lcd1602_setCursor(1, 4);
        lcd1602_blink();
            if(UP == 0 && minute <= 59){
                minute++;
                printTime();
            }else if(DOWN == 0 && minute >= 1){
                minute--;
                printTime();
            }
    }
    MODE = 1;
    __delay_ms(DEBOUNCING);
    // set second
    while(MODE == 1){
        lcd1602_setCursor(1, 7);
        lcd1602_blink();
            if(UP == 0 && second <= 59){
                second++;
                printTime();
            }else if(DOWN == 0 && second >= 1){
                second--;
                printTime();
            }
    }
    MODE = 1;
    __delay_ms(DEBOUNCING);
    lcd1602_home();
    lcd1602_noBlink();
}

void __interrupt(high_priority) Hi_ISR(void)
{
    INTCONbits.INT0IE = 0;
    myISR();
    INTCONbits.INT0IF = 0;
    INTCONbits.INT0IE = 1;
}

void main(void){
    OSCCONbits.IRCF = 0b110;    // 4MHz
    ADCON1 = 0x0F;
    LATB = 0;
    TRISB = 0xFF; 
    // INT0 interrupt enable
    INTCONbits.INT0F = 0;
    INTCONbits.INT0IE = 1;
    INTCONbits.GIE = 1;
    
    init_i2c(_XTAL_FREQ, 100000);
    init_pcf8563();
    
    lcd1602_initial();
    lcd1602_clear();
    PCF8563_getDate(&year, &month, &day);
    PCF8563_getTime(&hour, &minute, &second);
    while(1){
        lcd1602_home();
        PCF8563_getDate(&year, &month, &day);
        sprintf(temp, "%04d-%02d-%02d", year, month, day);
        lcd1602_print(temp, 10);
        lcd1602_cursorNextLine();
        PCF8563_getTime(&hour, &minute, &second);
        sprintf(temp, "%02d:%02d:%02d", hour, minute, second);
        lcd1602_print(temp, 8);
        __delay_ms(1000);
    }

    return;
}