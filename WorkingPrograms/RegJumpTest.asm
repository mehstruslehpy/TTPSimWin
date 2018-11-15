//enter test values here
ldi a, 65
ldi b, 57

//if the flag is set the program will jump
//to this location
ldi c, FlagIsSet

//test the values
cmp a,b

//perform the jump
//change this jump to match 
//which one you want to test
jmp c

//the fallthrough case
jmpi FlagWasNotSet

//the PC registers value will be different
//based on which jump occured
FlagIsSet:
halt

//padding to make the PC value differ more
byte 00
byte 00
byte 00
byte 00

FlagWasNotsET:
halt
