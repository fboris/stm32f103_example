file stm32_executable.elf
target remote :3333
monitor halt
monitor flash write_image erase stm32_bin_image.bin
monitor verify_image stm32_bin_image.bin
monitor reset halt
c