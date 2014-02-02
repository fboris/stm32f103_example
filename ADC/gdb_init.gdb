file stm32_executable.elf
tar extended-remote :4242
load
c
target extended-remote localhost:4242
kill
printf "Please set your break point"
