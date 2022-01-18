
#include <xc.h>

#include "i2c.h"

void init_i2c(unsigned long freq, unsigned long clock){
    I2C_Master_Init(freq, 100000);
}

void I2C_Master_Init(const unsigned long freq, const unsigned long clock){
    SSPCON1 = 0x38;     // SSPCON1: MSSP CONTROL REGISTER 1 (SPI MODE)
                        // WCOL SSPOV SSPEN CKP SSPM3 SSPM2 SSPM1 SSPM0
                        //
                        // WCOL: Write Collision Detect bit (Transmit mode only)
                        // SSPOV: Receive Overflow Indicator bit
                        // SSPEN: Master Synchronous Serial Port Enable bit
                        // CKP: Clock Polarity Select bit
                        // SSPM3:SSPM0: Synchronous Serial Port Mode Select bits
                            // 1000 = I2C Master mode, clock = FOSC/(4 * (SSPADD + 1))
    SSPCON2 = 0;
    SSPADD = (freq / (4 * clock)) - 1;
    SSPSTAT = 0xC0;     // SSPSTAT: MSSP STATUS REGISTER (I2C MODE)
                        // SMP CKE D/A P S R/W UA BF
                        //
                        // SMP: Slew Rate Control bit
                        // CKE: SMBus Select bit
                        // D/A: Data/Address bit
                        // P: Stop bit
                        // S: Start bit
                        // R/W: Read/Write Information bit (I2C mode only)
                        // UA: Update Address bit (10-bit Slave mode only)
                        // BF: Buffer Full Status bit
    TRISC3 = 1;
    TRISC4 = 1;
}

void I2C_Master_Wait(void){
    while((SSPSTAT & 0x04) || (SSPCON2 & 0x1F)); // transmitting? or ...
}

void I2C_Master_Start(void){
    I2C_Master_Wait();
    SEN = 1;
}

void I2C_Master_RepeatedStart(void){
    I2C_Master_Wait();
    RSEN = 1;
}

void I2C_Master_Stop(void){
    I2C_Master_Wait();
    PEN = 1;
}

void I2C_Master_Write(unsigned char d){
    I2C_Master_Wait();
    SSPBUF = d;
}

unsigned char I2C_Master_Read(unsigned char a){
    unsigned char temp;

    I2C_Master_Wait();
    RCEN = 1;
    I2C_Master_Wait();
    temp = SSPBUF;
    I2C_Master_Wait();
    ACKDT = a ? 0 : 1;      // ACKDT: Acknowledge Data bit (Master Receive mode only)
                                // 1 = Not Acknowledge
                                // 0 = Acknowledge
    ACKEN = 1;

    return temp;
}
