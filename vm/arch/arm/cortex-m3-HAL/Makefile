ifdef CONFIG_ARM_CPU_STM32F100RB

# changed here
#ldscript  := stm32f100rb.ld
ldscript := stm32_flash.ld

ram_bytes := 0x2000
rom_bytes := 0x20000
endif

CPPFLAGS += -DARCH_CPU_RAM_BYTES=$(ram_bytes) \
	-DARCH_CPU_ROM_BYTES=$(rom_bytes)

LDFLAGS  += -T$(ldscript)

# changed here
#arch-y += init.s
arch-y += startup_stm32f10x_md_vl.s

arch-$(CONFIG_ARM_BOARD_STM32_VLDISCOVERY)   += board-vldiscovery.c system_stm32f1xx.c
stdlib-$(CONFIG_ARM_BOARD_STM32_VLDISCOVERY) += stdlib-vldiscovery.scm

load-address := \#x8008000

arch-all: picobit.hex

ifdef CONFIG_ARM_COMPILER_GCC
CFLAGS += -mcpu=cortex-m3 -mthumb -g  \

picobit.elf: $(obj-y) .config
	$(CROSS)gcc $(CFLAGS) -o $@ $(obj-y)
endif

ifdef CONFIG_ARM_COMPILER_KEIL
no-gcc := 1

CFLAGS += --cpu=Cortex-M3 --thumb -Ospace

keil-y = $(filter %.c,$(obj-y))

picobit.o: $(keil-y) .config
	wine $(CONFIG_ARM_KEIL_PATH)\\armcc.exe $(CFLAGS) -c $(keil-y)
	wine $(CONFIG_ARM_KEIL_PATH)\\armlink.exe --output $@ \
		--partial $(notdir $(patsubst %.c,%.o,$(keil-y)))

init.o: $(arch)/init.s
	$(CROSS)gcc $(CFLAGS) -c -o $@ $^

picobit.elf: picobit.o init.o
	$(CROSS)ld $(LDFLAGS) -o $@ init.o picobit.o
endif

picobit.hex: picobit.elf
	$(CROSS)objcopy -O ihex $^ $@
