#ifndef PICOBIT_ARCH_ARM_CORTEX_M3_TYPES_H
#define PICOBIT_ARCH_ARM_CORTEX_M3_TYPES_H

#ifdef CONFIG_ARM_COMPILER_KEIL
#pragma diag_suppress 1,1295,951,223
#endif

#ifdef CONFIG_ARM_COMPILER_GCC
#pragma GCC diagnostic ignored "-Wimplicit-function-declaration"
#endif

/* Define Picobit basic types */

typedef unsigned char  uint8;
typedef unsigned short uint16;
typedef unsigned int   uint32;

#endif
