#ifndef __DMA_H
#define __DMA_H

#include "std/types.h"

extern void dma_xfer(unsigned char channel, unsigned long address, unsigned int length, unsigned char read);

#endif
