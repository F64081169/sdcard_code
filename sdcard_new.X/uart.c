/*------------------------------------------------------------------------/
/  UART control module for PIC24F                          (C)ChaN, 2010
/-------------------------------------------------------------------------/
/
/  Copyright (C) 2010, ChaN, all right reserved.
/
/ * This software is a free software and there is NO WARRANTY.
/ * No restriction on use. You can use, modify and redistribute it for
/   personal, non-profit or commercial products UNDER YOUR RESPONSIBILITY.
/ * Redistributions of source code must retain the above copyright notice.
/
/-------------------------------------------------------------------------*/

#include <xc.h>

#include <string.h>

#include "uart.h"
#include "integer.h"

#define BUFFER_SIZE 128
#define BPS 		119200UL


static volatile int TxRun;		/* Tx running flag */
static volatile struct {
	int		ri, wi, ct;			/* Read index, Write index, Data counter */
	BYTE	buff[BUFFER_SIZE];	/* FIFO buffer */
} TxFifo, RxFifo;

#if 0
/* UART1 Rx interrupt ISR */
void __attribute__((interrupt, auto_psv)) _U1RXInterrupt (void)
{
	BYTE d;
	int i;

	d = (BYTE)RCREG;			/* Get received data */
	RCIF = 0;				/* Clear Rx interrupt flag */
	i = RxFifo.ct;				/* Number of bytes in the FIFO */
	if (i < BUFFER_SIZE) {		/* Skip if FIFO is full */
		RxFifo.ct = ++i;
		i = RxFifo.wi;
		RxFifo.buff[i++] = d;	/* Store data into the FIFO */
		RxFifo.wi = i % BUFFER_SIZE;	/* Next write ptr */
	}
}

/* UART1 Tx interrupt ISR */
void __attribute__((interrupt, auto_psv)) _U1TXInterrupt (void)
{
	int i;

	TXIF = 0;		/* Clear Tx interrupt flag */

	i = TxFifo.ct;		/* Number of data in the FIFO */
	if (i) {			/* If any data is available, pop a byte and send it. */
		TxFifo.ct = --i;
		i = TxFifo.ri;
		TXREG = TxFifo.buff[i++];		/* Send a byte */
		TxFifo.ri = i % BUFFER_SIZE;	/* Next read ptr */
	} else {			/* No data in the Tx FIFO */
		TxRun = 0;		/* Stop transmission sequense */
	}
}
#endif

void __interrupt(high_priority) isr_high (void)
{
    if(INTCONbits.INT0IF)
    {
        
    }
    if (RCIE && RCIF)           
    {
        BYTE d;
        int i;

        d = (BYTE)RCREG;			/* Get received data */
        RCIF = 0;				/* Clear Rx interrupt flag */
       
        TXREG = RCREG;
        
        i = RxFifo.ct;				/* Number of bytes in the FIFO */
        if (i < BUFFER_SIZE) {		/* Skip if FIFO is full */
            RxFifo.ct = ++i;
            i = RxFifo.wi;
            RxFifo.buff[i++] = d;	/* Store data into the FIFO */
            RxFifo.wi = i % BUFFER_SIZE;	/* Next write ptr */
        }           
        
        LATBbits.LB1 = PORTBbits.RB1 ^ 0x01;
    }   
}

int uart_gets (		/* 0:End of stream, 1:A line arrived */
	char* buff,	/* Pointer to the buffer */
	int len		/* Buffer length */
)
{
	int c, i;

	i = 0;
	for (;;) {
		c = RxFifo.buff[i];				/* Get a char from the incoming stream */
		if (!c) continue;//return 0;			/* End of stream? */
		if (c == '\r') break;		/* End of line? */
		if (c == '\b' && i) {		/* Back space? */
            RxFifo.buff[i] = '0';
			i--;            
			if (0)//(_LINE_ECHO) 
                printf("%c", c);
			continue;
		}
		if (c >= ' ' && i < len - 1) {	/* Visible chars */
			buff[i++] = c;
			if (0)//(_LINE_ECHO) 
                printf("%c", c);
		}
	}
	buff[i] = 0;	/* Terminate with a \0 */
	if (0)//(_LINE_ECHO) 
        printf("%c", '\n');
    
    memset(RxFifo.buff, 0x00, sizeof(RxFifo.buff));
    RxFifo.wi = 0; 
    RxFifo.ct = 0;

	return 1;
}

/* Check number of bytes in the Rx FIFO */

int uart_getRxCnt (void)
{
	return RxFifo.ct;	/* Returns number of bytes in the Rx FIFO */
}

// for use printf
void putch(unsigned char byte)
{
    while(!TXIF);
 
    TXREG = byte;
}

/* Initialize UART module */

void uart_init (void)
{
	/* Disable UART1 Tx/Rx interrupts */
	RCIE = 0;
	TXIE = 0;

	/* Initialize UART1 */
    TXSTA = 0x24;         // CSRC TX9 TXEN SYNC SENDB BRGH TRMT TX9D
                          // 0    0   1    0    0     1    0    0
                          // CSRC: Clock Source Select bit
                          // TX9: 9-bit Transmit Enable bit
                          // TXEN: Transmit Enable bit
                          // SYNC: EUSART Mode Select bit
                          // SENDB: Send Break Character bit
                          // BRGH: High Baud Rate Select bit
                          // TRMT: Transmit Shift Register Status bit
                          // TX9D: 9th bit of Transmit Data
    
    RCSTA = 0x90;         // SPEN RX9 SREN CREN ADDEN FERR OERR RX9D
                          // 1    0   0    0    0     0    0    0   
                          // SPEN: Serial Port Enable bit
                          // RX9: 9-bit Receive Enable bit
                          // SREN: Single Receive Enable bit
                          // CREN: Continuous Receive Enable bit
                          // ADDEN: Address Detect Enable bit
                          // FERR: Framing Error bit
                          // OERR: Overrun Error bit
                          // RX9D: 9th bit of Received Data
    
    SPBRG = 129;           // 4MHz, 19200

	/* Clear Tx/Rx FIFOs */
	TxFifo.ri = 0; TxFifo.wi = 0; TxFifo.ct = 0;
	RxFifo.ri = 0; RxFifo.wi = 0; RxFifo.ct = 0;
	TxRun = 0;

	/* Enable UART Tx/Rx interruptrs */
    RCIP = 1; // EUSART Receive Interrupt Priority bit
    TXIP = 0; // EUSART Transmit Interrupt Priority bit
    
    RCIF = 0;
    TXIF = 0;
    
	RCIE = 1;
	//TXIE = 1;

    GIE  = 0x01;                    
    PEIE = 0x01;    
}

/*----------------------------------------------*/
/* Dump a line of binary dump                   */
/*----------------------------------------------*/

void put_dump (
	const void* buff,		/* Pointer to the array to be dumped */
	unsigned long addr,		/* Heading address value */
	int len,				/* Number of items to be dumped */
	int width				/* Size of the items (DW_CHAR, DW_SHORT, DW_LONG) */
)
{
	int i;
	const unsigned char *bp;
	const unsigned short *sp;
	const unsigned long *lp;

	printf("addr: %08lX |", addr);		/* address */

	switch (width) {
	case DW_CHAR:
		bp = buff;
		for (i = 0; i < len; i++)		/* Hexdecimal dump */
			printf(" %02X", bp[i]);
		printf(" ");
		for (i = 0; i < len; i++)		/* ASCII dump */
			printf("%c", (bp[i] >= ' ' && bp[i] <= '~') ? bp[i] : '.');
		break;
	case DW_SHORT:
		sp = buff;
		do								/* Hexdecimal dump */
			printf(" %04X", *sp++);
		while (--len);
		break;
	case DW_LONG:
		lp = buff;
		do								/* Hexdecimal dump */
			printf(" %08LX", *lp++);
		while (--len);
		break;
	}

	printf("\r\n");
}

/*----------------------------------------------*/
/* Get a value of the string                    */
/*----------------------------------------------*/
/*	"123 -5   0x3ff 0b1111 0377  w "
	    ^                           1st call returns 123 and next ptr
	       ^                        2nd call returns -5 and next ptr
                   ^                3rd call returns 1023 and next ptr
                          ^         4th call returns 15 and next ptr
                               ^    5th call returns 255 and next ptr
                                  ^ 6th call fails and returns 0
*/

int xatoi (			/* 0:Failed, 1:Successful */
	char **str,		/* Pointer to pointer to the string */
	long *res		/* Pointer to the valiable to store the value */
)
{
	unsigned long val;
	unsigned char c, r, s = 0;

	*res = 0;

	while ((c = **str) == ' ') (*str)++;	/* Skip leading spaces */

	if (c == '-') {		/* negative? */
		s = 1;
		c = *(++(*str));
	}
 
	if (c == '0') {
		c = *(++(*str));
		switch (c) {
		case 'x':		/* hexdecimal */
			r = 16; c = *(++(*str));
			break;
		case 'b':		/* binary */
			r = 2; c = *(++(*str));
			break;
		default:
			if (c <= ' ') return 1;	/* single zero */
			if (c < '0' || c > '9') return 0;	/* invalid char */
			r = 8;		/* octal */
		}
	} else {
		if (c < '0' || c > '9') return 0;	/* EOL or invalid char */
		r = 10;			/* decimal */
	}

	val = 0;
	while (c > ' ') {
		if (c >= 'a') 
            c -= 0x20;
		c -= '0';
		if (c >= 17) {
			c -= 7;
			if (c <= 9) return 0;	/* invalid char */
		}
		if (c >= r) return 0;		/* invalid char for current radix */
		val = val * r + c;
		c = *(++(*str));

	}
	if (s) val = 0 - val;			/* apply sign if needed */

	*res = val;
	return 1;
}