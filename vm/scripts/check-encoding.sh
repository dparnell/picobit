#!/bin/bash

function get_symbol() {
	echo "0x"$(($(${CROSS}objdump picobit.elf -t | grep "$1" | cut -f1 -d' ')))
}

function get_macro() {
	echo $(($(echo "$2" | ${CROSS}cpp ${CPPFLAGS} --include "$1" | tail -n1)))
}

REAL_HEAP_SIZE=$(($(get_macro heap.h ARCH_CPU_RAM_BYTES) - \
	($(get_symbol __picobit_heap) - $(get_symbol __stack_begin__))))

DEFINED_HEAP_SIZE=$(get_macro heap.h ARCH_RAM_BYTES)

if [ "$REAL_HEAP_SIZE" != "$DEFINED_HEAP_SIZE" ]; then
	echo
	echo "  HEAP SIZE MISMATCH!"
	echo "    Number of heap bytes based on linker data: ${REAL_HEAP_SIZE}"
	echo "    Number of heap bytes based on headers:     ${DEFINED_HEAP_SIZE}"
	echo "  To see variables consuming the .bss, try:"
	echo "    ${CROSS}objdump -t picobit.elf"
	echo
	exit 1
fi
