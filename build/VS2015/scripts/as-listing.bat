:: 1st arg 	- toolchain direcory
:: 2nd arg	- output directory
:: 3rd arg	- output file name
:: 4rd arg	- output file name without extension
@echo off
%1\bin\avr-objdump.exe -h -S  %2\%3 > %2\%4.lss