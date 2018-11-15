#ifndef _COMPSIM_H
#define _COMPSIM_H
#include "common.h"
#include "defs.h"
//make changing this type easy
#define T uint8_t
enum InstrCode
{
    HALT_INSTR,
    NOP_INSTR,
    ADD_INSTR,
    SUB_INSTR,
    CMP_INSTR,
    AND_INSTR,
    OR_INSTR,
    NOT_INSTR,
    RSH_INSTR,
    INC_INSTR,
    DEC_INSTR,
    LDI_INSTR,
    ST_INSTR,
    LD_INSTR,
    CPR_INSTR,
    JMP_INSTR,
    JMPI_INSTR,
    JC_INSTR,
    JCI_INSTR,
    JZ_INSTR,
    JZI_INSTR,
    JS_INSTR,
    JSI_INSTR,
    JO_INSTR,
    JOI_INSTR,
    JL_INSTR,
    JLI_INSTR,
    ADDI_INSTR,
    STF_INSTR
};

class CompSim
{
    //Parts:
    Ram ram;
    RegBank registers;
    IO io;

    //Functions:
    int ReadInterpExec(Ram&);
    int ReadInterpExec(vector<T>);
    int ReadInterpExec();

    InstrCode ReadInstr();
    T ExecInstr(InstrCode);
    InstrCode MatchInstr(T);
    //tells you a register from an opcode
    RegCode UpperReg(T); //most significant bits
    RegCode LowerReg(T);
    string RegToStr(T);
    string InstrToStr(InstrCode,T);
    int ComputeFlags(T,T,T,InstrCode);
    bool ComputeLessFlag(T,T,T,InstrCode);
    bool ComputeOverflowFlag(T,T,T,InstrCode);
    bool ComputeSignFlag(T);
    bool ComputeZeroFlag(T);
    bool ComputeCarryFlag(T,T,InstrCode);
    //RTFM for which method matches which instruction
    //TTP Instructions: <operation in c> <opcode>
    //Utility Type:
    T Nop(); // for(;;) 0000 0001
    T Halt(); // {} 0000 0000

    //ALU Type (changes flags register):
    T Add(RegCode,RegCode); // x+=y 1000 xxyy
    T Sub(RegCode,RegCode); // x-=y 1001 xxyy
    T Cmp(RegCode,RegCode); // x-y 1110 xxyy
    T And(RegCode,RegCode); // x&=y 1100 xxyy
    T Or(RegCode,RegCode); // x|=y 1101 xxyy
    T Not(RegCode); // x=~x 1011 xx00
    T Rsh(RegCode,RegCode); // x>>=y 1010 xxyy

    //ALU Type (no changes flags register):
    T Inc(RegCode); // ++x 1101 xxxx
    T Dec(RegCode); // --x 1110 xxxx

    //Copying Type:
    T Ldi(RegCode); // x=*(PC++) or x=(*PC++) 0110 11xx
    T St(RegCode,RegCode); // *y=x 1111 xxyy
    T Ld(RegCode,RegCode); // x=*y 0111 xxyy
    T Cpr(RegCode,RegCode); // x=y 0101 xxyy

    //Jump Type:
    T Jmp(RegCode); // PC=x 1011 xx01
    T Jmpi(); // PC=i or PC=*PC 0100 0000
    T Jc(RegCode); // PC=(CFlag?XReg:PC) 1011 xx10
    T Jci(); // PC=(CFlag?*PC:PC+1) 0100 0100
    T Jz(RegCode); // ZFlag 1011 xx11
    T Jzi(); // ZFlag 0100 0101
    T Js(RegCode); // SFlag 0110 10xx
    T Jsi(); // SFlag 0100 0011
    T Jo(RegCode); // OFlag 0110 01xx
    T Joi(); // OFlag 0100 0010
    T Jl(RegCode); // LFlag 0110 00xx
    T Jli(); // LFlag 0100 0001

    //"Memory Allocation" Type:
    //I don't think this is implementable?
    //T Byte(); //alloc and initialize a byte in RAM

    //Misc Type:
    //these don't actually exist on TTP
    T Addi(RegCode); // add imediate to reg 0011 00xx
    T Stf(); //store FLAGS reg to where D reg points in ram 0100 0010
public:
    //useful ctors
    CompSim();
    CompSim(RegBank,Ram);
    virtual ~CompSim() {};

    //Rule of three functions(five?)
    //CompSim's are not meant to be copied moved or shared just to be used in main
    CompSim(const CompSim&) = delete; //no copying of Computer Simulators for right now
    CompSim(CompSim&&) = delete; // no move constructor
    CompSim& operator=(const CompSim&) & = delete; // no copy assignment operator
    CompSim& operator=(CompSim&&) & = delete; // no move assignment operator

    //basic operation stuff
    int Poweron(RegBank,Ram);
    int Poweron();
    int PowerOff();
    int DumpState();
    int RunProgram();
    int RunProgram(RegBank,Ram);

    //ram specific functions
    int LoadRam(vector<T>&,int);
    int LoadRamLoc(T addr, T value);
    int ClearRam();

    //register specific functions
    int LoadRegister(RegCode,T);
    int LoadRegisters(T a, T b, T c, T d, T pc, T flags);
    int LoadRegisters(T a, T b, T c, T d);
    int ClearRegisters();

};
#endif
