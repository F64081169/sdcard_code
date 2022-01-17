#ifndef _COMMFUNC
#define _COMMFUNC

void uart_init (void);
int uart_getRxCnt (void);
int uart_gets (char* buff,	int len);
void put_dump (const void* buff, unsigned long addr, int len, int width);
int xatoi (char** str, long* res);

#define DW_CHAR		sizeof(char)
#define DW_SHORT	sizeof(short)
#define DW_LONG		sizeof(long)

#endif

