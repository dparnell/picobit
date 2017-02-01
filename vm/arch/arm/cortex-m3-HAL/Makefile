ifdef CONFIG_ARM_CPU_STM32F100RB

# I changed here
#ldscript  := stm32f100rb.ld
ldscript := stm32_flash.ld

ram_bytes := 0x2000
rom_bytes := 0x20000
endif

CPPFLAGS += -DARCH_CPU_RAM_BYTES=$(ram_bytes) \
	-DARCH_CPU_ROM_BYTES=$(rom_bytes)

LDFLAGS  += -T$(ldscript)

# I changed here
#arch-y += init.s
arch-y += startup_stm32f10x_md_vl.s

# I changed here
HAL-sources = include/stm32-hal/src/misc.c \
	include/stm32-hal/src/stm32f10x_adc.c \
	include/stm32-hal/src/stm32f10x_bkp.c \
	include/stm32-hal/src/stm32f10x_can.c \
	include/stm32-hal/src/stm32f10x_cec.c \
	include/stm32-hal/src/stm32f10x_crc.c \
	include/stm32-hal/src/stm32f10x_dac.c \
	include/stm32-hal/src/stm32f10x_dbgmcu.c \
	include/stm32-hal/src/stm32f10x_dma.c \
	include/stm32-hal/src/stm32f10x_exti.c \
	include/stm32-hal/src/stm32f10x_flash.c \
	include/stm32-hal/src/stm32f10x_fsmc.c \
	include/stm32-hal/src/stm32f10x_gpio.c \
	include/stm32-hal/src/stm32f10x_i2c.c \
	include/stm32-hal/src/stm32f10x_iwdg.c \
	include/stm32-hal/src/stm32f10x_pwr.c \
	include/stm32-hal/src/stm32f10x_rcc.c \
	include/stm32-hal/src/stm32f10x_rtc.c \
	include/stm32-hal/src/stm32f10x_sdio.c \
	include/stm32-hal/src/stm32f10x_spi.c \
	include/stm32-hal/src/stm32f10x_tim.c \
	include/stm32-hal/src/stm32f10x_usart.c \
	include/stm32-hal/src/stm32f10x_wwdg.c

arch-$(CONFIG_ARM_BOARD_STM32_VLDISCOVERY)   += board-vldiscovery.c abc.c \
	include/stm32-hal/src/stm32f10x_rcc.c \
	include/stm32-hal/src/stm32f10x_gpio.c
	#include/stm32-hal/src/misc.c \
	#system_stm32f1xx.c # $(HAL-sources)
stdlib-$(CONFIG_ARM_BOARD_STM32_VLDISCOVERY) += stdlib-vldiscovery.scm

load-address := \#x8008000

arch-all: picobit.hex

ifdef CONFIG_ARM_COMPILER_GCC

# I changed here
STM32_includes =
CFLAGS += -mcpu=cortex-m3 -mthumb -g -mlittle-endian $(STM32_includes) -lm \

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
