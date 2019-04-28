#ifndef __BASICIO_H
#define __BASICIO_H

#include "std/types.h"

/* Read the byte found at port `port` */
extern uint8_t inbyte (uint16_t port);
/* Write the byte `data` to the port `port` */
extern void outbyte (uint16_t port, uint8_t data);

#endif
