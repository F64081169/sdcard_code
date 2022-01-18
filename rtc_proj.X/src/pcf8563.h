#ifndef _PCF8563_H
#define _PCF8563_H

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

#define changeIntToHex(dec)     ( ( ((dec)/10) << 4) + ((dec)%10) )
#define changeHexToInt(hex)     ( ( ((hex)>>4) *10) + ((hex%16)) )

#define RTC_ADDR (0x51 << 1)

#define REG_CTRL_STATUS_1       0x00
#define REG_CTRL_STATUS_2       0x01
#define REG_SEC                 0x02
#define REG_MIN                 0x03
#define REG_HOUR                0x04
#define REG_DAY                 0x05
#define REG_WEEK                0x06
#define REG_MON                 0x07
#define REG_YEAR                0x08

void setI2C(unsigned char reg, unsigned char data);
unsigned char getI2C(unsigned char reg);

void PCF8563_setDate(unsigned short year, unsigned char mon, unsigned char day);
void PCF8563_getDate(unsigned int *year, unsigned int *mon, unsigned int *day);
void PCF8563_setTime(unsigned char hour, unsigned char min, unsigned char sec);
void PCF8563_getTime(unsigned int *hour, unsigned int *min, unsigned int *sec);

void init_pcf8563(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _PCF8563_H */