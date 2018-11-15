//jump to one of two locations using a a jx style jump instruction
//test with jmp,jc,jz,js,jo,jl

//enter test values here
ldi a, 255
ldi b, 232

//test the values
cmp a,b

//perform the jump
//change this jump to match 
//which one you want to test
jli FlagIsSet

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
