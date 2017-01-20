ifdef CONFIG_ARM_CORTEX_M3
arch := $(arch)/cortex-m3
endif

ifdef CONFIG_CMSIS_ARM_CORTEX_M3
arch := $(arch)/cortex-m3-CMSIS
endif

ifdef CONFIG_HAL_ARM_CORTEX_M3
arch := $(arch)/cortex-m3-HAL
endif

include $(arch)/Makefile
