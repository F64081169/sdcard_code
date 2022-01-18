/* 
 * File:   pwm.h
 * Author: USER
 *
 * Created on 2022?1?16?, ?? 2:37
 */

#ifndef PWM_H
#define	PWM_H


#ifdef	__cplusplus
extern "C" {
#endif

#define MIN_PWM_DUTY 0
#define MAX_PWM_DUTY 1023

void pwm_Init(unsigned char period){
    // CCP1/RC2 -> Output
    TRISC = 0;
    LATC = 0;
    
    /* CCP PWM mode */
    CCP1CON = 0X3C;   // CCP1CON REGISTER (ECCP1 MODULE, 40/44-PIN DEVICES)
                    // P1M1 P1M0 DC1B1 DC1B0 CCP1M3 CCP1M2 CCP1M1 CCP1M0
                    // 0     0    1      1     1     1       0       0
                    // P1M1:P1M0: Enhanced PWM Output Configuration bits
                        // 00 = Single output: P1A modulated; P1B, P1C, P1D assigned as port pins
                    // DC1B1:DC1B0: PWM Duty Cycle bit 1 and bit 0
                    // CCP1M3:CCP1M0: Enhanced CCP Mode Select bits
                        // 1100 = PWM mode; P1A, P1C active-high; P1B, P1D active-high
    
    /* Timer2 configuration *///62.5us
    PR2 = period;           //  configure timer2 period
    
    T2CON=0x04; // T2CON: TIMER2 CONTROL REGISTER
                // ? T2OUTPS3 T2OUTPS2 T2OUTPS1 T2OUTPS0 TMR2ON T2CKPS1 T2CKPS0
                // 0  0        0          0        0       1       0       0
                // Unimplemented: Read as ?0?
                // T2OUTPS3:T2OUTPS0: Timer2 Output Postscale Select bits
                // TMR2ON: Timer2 On bit
                // T2CKPS1:T2CKPS0: Timer2 Clock Prescale Select bits
                    // 00 = Prescaler is 1
}

void pwm_setDuty(unsigned int dutycycle)
{
    CCPR1L = dutycycle>>2;  //  PWM duty cycle - first 8-bits (MSb)
    CCP1CON &= 0xCF;        //  5,4 bits zeroed (DC1B1:DC1B0 = 00)
    CCP1CON |= ((dutycycle<<4) & 0x30);  //  PWM duty cycle - last 2-bits (LSb) in CCP1CON 5,4 bits    
}

void pwm_start(void)
{
    TMR2ON = 1;             //  timer2 on 
}

void pwm_stop(void)
{
    TMR2ON = 0;             //  timer2 off
}


#ifdef	__cplusplus
}
#endif

#endif	/* PWM_H */

