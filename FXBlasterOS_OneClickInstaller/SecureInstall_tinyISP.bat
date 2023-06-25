@rem Erase chip (This will also clear lock bits and allow read/write access)
avrdude -p atmega328p -c usbtiny -B32 -e
@java -jar Sleep.jar 1000

@rem Set High Fuse Byte to disable bootloader and just run the sketch directly
avrdude -p atmega328p -c usbtiny -B32 -D -u -U hfuse:w:0xDF:m
@java -jar Sleep.jar 1000

@rem Set Low Fuse byte exactly as Arduino does (all disabled, apparently)
avrdude -p atmega328p -c usbtiny -B32 -D -u -U lfuse:w:0xFF:m
@java -jar Sleep.jar 1000

@rem Set extended fuse to enable brownout detection at ~2.7V (Exactly the same as Arduino defaults)
avrdude -p atmega328p -c usbtiny -B32 -D -u -U efuse:w:0x05:m
@java -jar Sleep.jar 1000

@rem Upload the program
@rem avrdude -p atmega328p -c usbtiny -B32 -D -U flash:w:D:\sloeber\workspace\Lightsaber\BlinkTest\Release\BlinkTest.hex:i
avrdude -p atmega328p -c usbtiny -D -U flash:w:%~dp0\firmware\DIYinoStardustV3.hex:i
@java -jar Sleep.jar 1000

@rem Lock down the flash to disable read/write
avrdude -p atmega328p -c usbtiny -V -B32 -D -u -U lock:w:0x0C:m
pause
