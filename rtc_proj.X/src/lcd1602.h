#ifndef _LCD1602_H
#define _LCD1602_H

#define _XTAL_FREQ 1000000

#define rs LATAbits.LATA0
#define rw LATAbits.LATA1
#define en LATAbits.LATA2
#define data LATD
#define ctrl LATB
#define DELAYMS 4

#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

void lcd1602_initial();
void lcd1602_clear();
void lcd1602_home();
void lcd1602_noDisplay();
void lcd1602_display();
void lcd1602_setCursor(unsigned char col, unsigned char row);
void lcd1602_cursorNextLine();
void lcd1602_noCursor();
void lcd1602_cursor();
void lcd1602_noBlink();
void lcd1602_blink();
void lcd1602_scrollDisplayLeft();
void lcd1602_scrollDisplayRight();
void lcd1602_leftToRight();
void lcd1602_rightToLeft();
void lcd1602_autoscroll();
void lcd1602_noAutoscroll();
void lcd1602_print(unsigned char *str, int length);
void lcd1602_cmd(unsigned char cmd);
void lcd1602_displaychar(unsigned char value);

//unsigned char _addr;
unsigned char _displayfunction;
unsigned char _displaycontrol;
unsigned char _displaymode;
unsigned char _cols;
unsigned char _rows;
//unsigned char _charsize;
//unsigned char _backlightval;

#endif