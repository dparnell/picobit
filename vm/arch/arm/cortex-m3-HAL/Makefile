# Cortex-M3 - STM32F100RB - HAL
#
# Author: Cecilia Carneiro e Silva <ceciliacarneiroesilva@gmail.com>
#
#

ifdef CONFIG_ARM_CPU_STM32F100RB
ldscript := stm32_flash.ld

ram_bytes := 0x2000
rom_bytes := 0x20000
endif

LINKER_path  := linker-file/
INCLUDE_path := -I$(arch)/include/cmsis/core/ \
	-I$(arch)/include/cmsis/device/ \
	-I$(arch)/include/hal/inc/ \
	-I$(arch)/include/picobit/ \
	-I$(arch)/include/picobit/periph/inc/ \
	-I$(arch)/include/cmsis/bkp/

CPPFLAGS += -DARCH_CPU_RAM_BYTES=$(ram_bytes) \
	-DARCH_CPU_ROM_BYTES=$(rom_bytes)

LDFLAGS  += -T$(ldscript)

arch-y += startup_stm32f10x_md_vl.s

# I changed here
HAL-sources = include/hal/src/stm32f10x_bkp.c \
	include/hal/src/stm32f10x_can.c \
	include/hal/src/stm32f10x_cec.c \
	include/hal/src/stm32f10x_crc.c \
	include/hal/src/stm32f10x_dbgmcu.c \
	include/hal/src/stm32f10x_exti.c \
	include/hal/src/stm32f10x_flash.c \
	include/hal/src/stm32f10x_fsmc.c \
	include/hal/src/stm32f10x_i2c.c \
	include/hal/src/stm32f10x_iwdg.c \
	include/hal/src/stm32f10x_pwr.c \
	include/hal/src/stm32f10x_rtc.c \
	include/hal/src/stm32f10x_sdio.c \
	include/hal/src/stm32f10x_spi.c \
	include/hal/src/stm32f10x_wwdg.c

HAL-files = include/hal/src/stm32f10x_rcc.c \
	include/hal/src/stm32f10x_gpio.c \
	include/hal/src/stm32f10x_adc.c \
	include/hal/src/stm32f10x_dma.c \
	include/hal/src/stm32f10x_tim.c \
	include/hal/src/stm32f10x_usart.c \
	include/hal/src/stm32f10x_dac.c \
	include/hal/src/misc.c \
	include/picobit/stm32f10x_it.c

PICOBIT-files =	include/picobit/periph/src/rcc.c  \
	include/picobit/periph/src/gpio.c \
	include/picobit/periph/src/adc.c  \
	include/picobit/periph/src/tim.c  \
	include/picobit/periph/src/usart.c \
	include/picobit/periph/src/crc16.c \
	include/picobit/periph/src/dac.c

arch-$(CONFIG_ARM_BOARD_STM32_VLDISCOVERY)   += include/picobit/main.c \
	include/cmsis/device/system_stm32f10x.c \
	$(HAL-files) \
	$(PICOBIT-files)

stdlib-$(CONFIG_ARM_BOARD_STM32_VLDISCOVERY) += include/picobit/main-scheme.scm #include/picobit/stdlib-vldiscovery.scm 

load-address := \#x8008000

arch-all: picobit.hex

ifdef CONFIG_ARM_COMPILER_GCC

# I changed here
STM32_includes =
CFLAGS += -mcpu=cortex-m3 -mthumb -g -mlittle-endian $(STM32_includes) -lm \

picobit.elf: $(obj-y) .config
	$(CROSS)gcc $(CFLAGS) -o $@ $(obj-y)
endif

picobit.hex: picobit.elf
	$(CROSS)objcopy -O ihex $^ $@
