ifdef CONFIG_ARM_CPU_STM32F100RB
ldscript  := stm32f100rb.ld
ram_bytes := 0x2000
rom_bytes := 0x20000
endif

LINKER_path  := lib/
INCLUDE_path :=

CPPFLAGS += -DARCH_CPU_RAM_BYTES=$(ram_bytes) \
	-DARCH_CPU_ROM_BYTES=$(rom_bytes)

LDFLAGS  += -T$(ldscript)

arch-y += init.s

arch-$(CONFIG_ARM_BOARD_STM32_VLDISCOVERY)   += board-vldiscovery.c
stdlib-$(CONFIG_ARM_BOARD_STM32_VLDISCOVERY) += stdlib-vldiscovery.scm

load-address := \#x8008000

arch-all: picobit.hex

ifdef CONFIG_ARM_COMPILER_GCC
CFLAGS += -mcpu=cortex-m3 -mthumb -g  \

picobit.elf: $(obj-y) .config
	$(CROSS)gcc $(CFLAGS) -o $@ $(obj-y)
endif

picobit.hex: picobit.elf
	$(CROSS)objcopy -O ihex $^ $@
