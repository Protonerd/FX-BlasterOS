@echo Be sure to adjust for your correct COM port
@echo Uploading fimrware...
rem COM port can be adjusted by changing the -P<COMPORT> switch
rem Firmware to be uploaded can be adjusted by changing the hex file name -Uflash:<Firmware File Name>:i
rem Baud rate can be adjusted by changing the -b<Baud Rate> switch
avrdude -patmega328p -carduino -PCOM%1 -b57600 -D -Uflash:w:firmware/FXBlasterOS_Nano_scopeOled_newNanoBootloader.hex:i
@echo Upload attempt is complete.
pause
