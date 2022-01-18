#ifndef _MYPRINTF_H
#define _MYPRINTF_H

#include <xc.h>

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

void init_uart(void);
void putch(unsigned char byte);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _MYPRINTF_H */
