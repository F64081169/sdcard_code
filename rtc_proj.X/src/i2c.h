
#ifndef _I2C_H
#define _I2C_H

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

void init_i2c(unsigned long freq, unsigned long clock);

void I2C_Master_Init(const unsigned long freq, const unsigned long clock);
void I2C_Master_Wait(void);
void I2C_Master_Start(void);
void I2C_Master_RepeatedStart(void);
void I2C_Master_Stop(void);
void I2C_Master_Write(unsigned char d);
unsigned char I2C_Master_Read(unsigned char a);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  // _I2C_H
