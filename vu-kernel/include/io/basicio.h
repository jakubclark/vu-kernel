#ifndef __BASICIO_H
#define __BASICIO_H

#include "std/types.h"

/* Read the byte found at port `_port` */
extern uint8_t inbyte(uint16_t port);
/* Write the byte `_data` to the port `_port` */
extern void outbyte(uint16_t _port, uint8_t _data);
/* Read the word fount at port `_port` */
extern uint32_t inword(uint16_t _port);
/* Write the byte `_data` to the port `_port` */
extern void outword(uint16_t _port, uint32_t _data);

#endif
