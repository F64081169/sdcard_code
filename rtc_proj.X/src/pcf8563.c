#include <xc.h>

#include <stdio.h>
#include <stdlib.h>
#include "myprintf.h"
//#include "math.h"
#include "pcf8563.h"
#include "i2c.h"

void setI2C(unsigned char reg, unsigned char data){
    I2C_Master_Start();             // Start condition

    I2C_Master_Write(RTC_ADDR);     // Select pcf8563 + Write
    I2C_Master_Write(reg);          // Write address select
    I2C_Master_Write(data);         // Write data

    I2C_Master_Stop();
}

unsigned char getI2C(unsigned char reg){
    unsigned char data;

    I2C_Master_Start();             // Start condition

    I2C_Master_Write(RTC_ADDR);     // Select pcf8563 + Write
    I2C_Master_Write(reg);          // Read address select

    I2C_Master_RepeatedStart();     // add, important!!!!

    I2C_Master_Write(RTC_ADDR + 1); // Select pcf8563 + read
    data = I2C_Master_Read(0);      // Read + acknowlege

    I2C_Master_Stop();              // Stop condition

    return data;
}

void PCF8563_setDate(unsigned short year, unsigned char mon, unsigned char day){
    year = changeIntToHex(year % 100);
    mon = changeIntToHex(mon) | ((year > 1999) ? 0x80 : 0x00);
    day = changeIntToHex(day);

    setI2C(REG_YEAR, year);
    setI2C(REG_MON, mon);
    setI2C(REG_DAY, day);
}

void PCF8563_getDate(unsigned int *year, unsigned int *mon, unsigned int *day){
    unsigned char buf[3];
    unsigned char century = 0;
    //unsigned int year, mon, day;

    // get buffer data
    buf[0] = getI2C(REG_YEAR);
    buf[1] = getI2C(REG_MON);
    buf[2] = getI2C(REG_DAY);

    // set century
    if(buf[1] & 0x80){
        century = 1;
    }

    // masked buffer
    buf[0] = buf[0] & 0xff;
    buf[1] = buf[1] & 0x1f;
    buf[2] = buf[2] & 0x3f;

    *year = (century == 1) ? (1900 + changeHexToInt(buf[0])) : (2000 + changeHexToInt(buf[0]));
    *mon = changeHexToInt(buf[1]);
    *day = changeHexToInt(buf[2]);

    //sprintf(ptr, "%4d-%2d-%2d ", year, mon, day);
}

void PCF8563_setTime(unsigned char hour, unsigned char min, unsigned char sec){
    setI2C(REG_HOUR, changeIntToHex(hour));
    setI2C(REG_MIN, changeIntToHex(min));
    setI2C(REG_SEC, changeIntToHex(sec));
}

void PCF8563_getTime(unsigned int *hour, unsigned int *min, unsigned int *sec){
    //unsigned char hour, min, sec;

    *hour = changeHexToInt(getI2C(REG_HOUR) & 0x3f);
    *min = changeHexToInt(getI2C(REG_MIN) & 0x7f);
    *sec = changeHexToInt(getI2C(REG_SEC) & 0x7f);

    //sprintf(ptr, "%2d:%2d:%2d", hour, min, sec);
}

void init_pcf8563(void){
    if((getI2C(0x0a)) != 0x08){
        setI2C(REG_CTRL_STATUS_1, 0x00);
        setI2C(REG_CTRL_STATUS_2, 0x00);
    }
}