#ifndef PICOBIT_ARCH_HOST_MEMORY_H
#define PICOBIT_ARCH_HOST_MEMORY_H

#define CODE_START 0x8000

#define ARCH_RAM_BYTES 0x2000
#define ARCH_ROM_BYTES 0x8000

extern uint8 ram_mem[];
#define ram_get(a) ram_mem[a]
#define ram_set(a,x) ram_mem[a] = (x)

extern uint8 rom_mem[];
#define rom_get(a) (rom_mem[a-CODE_START])

#endif
