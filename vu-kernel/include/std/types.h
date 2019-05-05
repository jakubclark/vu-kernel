#ifndef __TYPES_H
#define __TYPES_H

//  typedefs to standardise sizes across platforms.
typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long int uint64_t;

typedef signed char int8_t;
typedef signed short int int16_t;
typedef signed int int32_t;
typedef signed long long int int64_t;

typedef unsigned int size_t;

#define NULL 0L

#define MEM_BLOCK_SIZE 4096
#define PAGE_SIZE 4096
#define MEM_BLOCKS 1048576 /* enough for 4 GB */
#define PAGE_BITS 32

#define RESERVED 2
#define FREE 0
#define ALLOCATED 1

#define ALLOCATE 1

#define PAGES MEM_BLOCKS / PAGE_BITS

#define MEM_ERR_OK              0
#define MEM_ERR_BAD_MEM         1
#define MEM_ERR_RANGE           2
#define MEM_ERR_NOMEM           3

#endif
