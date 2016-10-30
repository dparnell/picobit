#ifndef PICOBIT_ARCH_ARM_CORTEX_M3_MEMORY_H
#define PICOBIT_ARCH_ARM_CORTEX_M3_MEMORY_H

#include <picobit.h>

/*
 * Addresses 0-0x8000 are reserved for code.
 */
#define CODE_START 0x8000

#define ARCH_ROM_BYTES (ARCH_CPU_ROM_BYTES - CODE_START)

/*
 * Extra RAM bytes taken by static variables in certain configurations.
 */
#ifdef CONFIG_ARM_CPU_STM32F100RB
#define ARCH_RAM_EXTRA_BYTES 8
#else
#define ARCH_RAM_EXTRA_BYTES 0
#endif /* CONFIG_ARM_CPU_STM32F100RB */

/*
 * 0x100 is the size of stack.
 * 44 is the size of interpreter variables.
 * Adjust ARCH_RAM_EXTRA_BYTES according to your needs.
 */
#define ARCH_RAM_BYTES (ARCH_CPU_RAM_BYTES - 0x100 - 44 - ARCH_RAM_EXTRA_BYTES)

extern uint8 __picobit_heap;

#define ram_get(a) ((uint8*)&__picobit_heap)[a]
#define ram_set(a,x) ((uint8*)&__picobit_heap)[a] = (x)

#define rom_get(a) (((uint8*) 0)[a])

#endif
