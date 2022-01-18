#include <xc.h>

#include "lcd1602.h"



void lcd1602_initial(){
    // pinmide setting
    ADCON1 = 0x0F;
    LATD = 0x00;
    LATA = 0x00;
    TRISD = 0x00; // PORTD output
    TRISA = 0x00; // PORTA output

    _rows = 0;
    _cols = 0;

    __delay_ms(DELAYMS);
    _displayfunction = LCD_8BITMODE | LCD_2LINE | LCD_5x10DOTS;
    lcd1602_cmd(LCD_FUNCTIONSET | _displayfunction);

    _displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
    lcd1602_display();

    lcd1602_clear();

    _displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTINCREMENT;
    lcd1602_cmd(LCD_ENTRYMODESET | _displaymode);

    lcd1602_home();
}


void lcd1602_clear(){
    lcd1602_cmd(LCD_CLEARDISPLAY);
}

void lcd1602_home(){
    lcd1602_cmd(LCD_RETURNHOME);
}

void lcd1602_noDisplay(){
    _displaycontrol &= ~LCD_DISPLAYON;
    lcd1602_cmd(LCD_DISPLAYCONTROL | _displaycontrol);
}

void lcd1602_display(){
    _displaycontrol |= LCD_DISPLAYON;
    lcd1602_cmd(LCD_DISPLAYCONTROL | _displaycontrol);
}

void lcd1602_setCursor(unsigned char row, unsigned char col){
    int row_offsets[] = {0x00, 0x40, 0x14, 0x54};
    /*if(row > _rows){
        row = _rows - 1;
    }*/
    lcd1602_cmd(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

void lcd1602_cursorNextLine(){
    lcd1602_setCursor(1, 0);
}

void lcd1602_noCursor(){
    _displaycontrol &= ~LCD_CURSORON;
    lcd1602_cmd(LCD_DISPLAYCONTROL | _displaycontrol);
}

void lcd1602_cursor(){
    _displaycontrol |= LCD_CURSORON;
    lcd1602_cmd(LCD_DISPLAYCONTROL | _displaycontrol);
}

void lcd1602_noBlink(){
    _displaycontrol &= ~LCD_BLINKON;
    lcd1602_cmd(LCD_DISPLAYCONTROL | _displaycontrol);
}

void lcd1602_blink(){
    _displaycontrol |= LCD_BLINKON;
    lcd1602_cmd(LCD_DISPLAYCONTROL | _displaycontrol);
}

void lcd1602_scrollDisplayLeft(){
    lcd1602_cmd(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}

void lcd1602_scrollDisplayRight(){
    lcd1602_cmd(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

void lcd1602_leftToRight(){
    _displaymode |= LCD_ENTRYLEFT;
    lcd1602_cmd(LCD_ENTRYMODESET | _displaymode);
}

void lcd1602_rightToLeft(){
    _displaymode &= ~LCD_ENTRYLEFT;
    lcd1602_cmd(LCD_ENTRYMODESET | _displaymode);
}

void lcd1602_autoscroll(){
    _displaymode |= LCD_ENTRYSHIFTINCREMENT;
    lcd1602_cmd(LCD_ENTRYMODESET | _displaymode);
}

void lcd1602_noAutoscroll(){
    _displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
    lcd1602_cmd(LCD_ENTRYMODESET | _displaymode);
}

void lcd1602_print(unsigned char *str, int length){
    for(int i = 0; i < length; i++){
        lcd1602_displaychar(str[i]);
    }
}

void lcd1602_cmd(unsigned char cmd){
    data = cmd;
    //rs = 0;
    //rw = 0;
    //en = 1;
    PORTA = 0x04;
    __delay_ms(DELAYMS);
    //en = 0;
    PORTA = 0x00;
}

void lcd1602_displaychar(unsigned char value){
    data = value;
    //rs = 1;
    //rw = 0;
    //en = 1;
    PORTA = 0x05;
    __delay_ms(DELAYMS);
    //en = 0;
    PORTA = 0x01;
}