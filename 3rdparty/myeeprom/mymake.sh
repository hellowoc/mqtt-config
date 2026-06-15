rm libmyeeprom.a
sync
CC=/home/jie/prebuilts-gcc-linux-x86-aarch64/gcc12/bin/aarch64-none-linux-gnu
$CC-gcc -c myeeprom.c -o myeeprom.o
$CC-ar rcs libmyeeprom.a myeeprom.o

