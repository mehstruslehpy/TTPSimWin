#include "CompSim.h"
#include "Instructions.cpp"
//make changing this type easy
#define T uint8_t

//private funcs:
T CompSim::ExecInstr(InstrCode instr)
{
    //find the appropriate registers for those instructions that need it
    RegCode upper = UpperReg(ram.ReadRam(registers.ReadRegister(REG_PC)));
    RegCode lower = LowerReg(ram.ReadRam(registers.ReadRegister(REG_PC)));
    switch (instr)
    {
    case HALT_INSTR:
    {
        Halt();
        registers.WriteRegister(REG_PC, registers.ReadRegister(REG_PC)+1);
        break;
    }
    case NOP_INSTR:
    {
        Nop();
        registers.WriteRegister(REG_PC, registers.ReadRegister(REG_PC)+1);
        break;
    }
    case ADD_INSTR:
    {
        Add(upper,lower);
        registers.WriteRegister(REG_PC, registers.ReadRegister(REG_PC)+1);
        break;
    }
    case SUB_INSTR:
    {
        Sub(upper,lower);
        registers.WriteRegister(REG_PC, registers.ReadRegister(REG_PC)+1);
        break;
    }
    case CMP_INSTR:
    {
        Cmp(upper,lower);
        registers.WriteRegister(REG_PC, registers.ReadRegister(REG_PC)+1);
        break;
    }
    case AND_INSTR:
    {
        And(upper,lower);
        registers.WriteRegister(REG_PC, registers.ReadRegister(REG_PC)+1);
        break;
    }
    case OR_INSTR:
    {
        Or(upper,lower);
        registers.WriteRegister(REG_PC, registers.ReadRegister(REG_PC)+1);
        break;
    }
    case NOT_INSTR:
    {
        Not(upper);
        registers.WriteRegister(REG_PC, registers.ReadRegister(REG_PC)+1);
        break;
    }
    case RSH_INSTR:
    {
        Rsh(upper,lower);
        registers.WriteRegister(REG_PC, registers.ReadRegister(REG_PC)+1);
        break;
    }
    case INC_INSTR:
    {
        Inc(upper);
        registers.WriteRegister(REG_PC, registers.ReadRegister(REG_PC)+1);
        break;
    }
    case DEC_INSTR:
    {
        Dec(upper);
        registers.WriteRegister(REG_PC, registers.ReadRegister(REG_PC)+1);
        break;
    }
    case LDI_INSTR:
    {
        Ldi(lower);
        registers.WriteRegister(REG_PC, registers.ReadRegister(REG_PC)+1);
        break;
    }
    case ST_INSTR:
    {
        St(upper,lower);
        registers.WriteRegister(REG_PC, registers.ReadRegister(REG_PC)+1);
        break;
    }
    case LD_INSTR:
    {
        Ld(upper,lower);
        registers.WriteRegister(REG_PC, registers.ReadRegister(REG_PC)+1);
        break;
    }
    case CPR_INSTR:
    {
        Cpr(upper,lower);
        registers.WriteRegister(REG_PC, registers.ReadRegister(REG_PC)+1);
        break;
    }
    case JMP_INSTR:
    {
        Jmp(upper);
        break;
    }
    case JMPI_INSTR:
    {
        Jmpi();
        break;
    }
    case JC_INSTR:
    {
        Jc(upper);
        break;
    }
    case JCI_INSTR:
    {
        Jci();
        break;
    }
    case JZ_INSTR:
    {
        Jz(upper);
        break;
    }
    case JZI_INSTR:
    {
        Jzi();
        break;
    }
    case JS_INSTR:
    {
        Js(lower);
        break;
    }
    case JSI_INSTR:
    {
        Jsi();
        break;
    }
    case JO_INSTR:
    {
        Jo(lower);
        break;
    }
    case JOI_INSTR:
    {
        Joi();
        break;
    }
    case JL_INSTR:
    {
        Jl(lower);
        break;
    }
    case JLI_INSTR:
    {
        Jli();
        break;
    }
    case ADDI_INSTR:
    {
        Addi(lower);
        registers.WriteRegister(REG_PC, registers.ReadRegister(REG_PC)+1);
        break;
    }
    case STF_INSTR:
    {
        Stf();
        registers.WriteRegister(REG_PC, registers.ReadRegister(REG_PC)+1);
        break;
    }
    default:	//can't match operation to a function
    {
        Nop();
        registers.WriteRegister(REG_PC, registers.ReadRegister(REG_PC)+1);
    }
    }
    //the instruction is complete, increment the PC
    //registers.WriteRegister(REG_PC, registers.ReadRegister(REG_PC)+1);
    return 1;
}

InstrCode CompSim::ReadInstr()
{
    InstrCode retVal = NOP_INSTR;
    T ramVal = 0;
    //check the PC for where we are in ram
    T progCount = registers.ReadRegister(REG_PC);
    //grab a value in ram
    ramVal = ram.ReadRam(progCount);
    //match value to the correct instruction
    retVal = MatchInstr(ramVal);
    cout << "NEXT-INSTRUCTION: " << InstrToStr(retVal,ramVal) << endl;
    return retVal;
}

//int CompSim::ReadInterpExec(Ram& r)
//{
//    ram = r;
//    ReadInterpExec();
//    return 1;
//}

//int CompSim::ReadInterpExec(vector<T> val)
//{
//    ram.LoadRam(val,ram.RamLength());
//    ReadInterpExec();
//    return 1;
//}

//Rule of three functions(five?)
int CompSim::ReadInterpExec()
{
    InstrCode cur_instr = ReadInstr();
    while (cur_instr != HALT_INSTR)
    {
        ExecInstr(cur_instr);
        cur_instr = ReadInstr();
        DumpState();
    }
    return 1;
}

//useful ctors
CompSim::CompSim()
//    : ram(DEFAULT_LEN,0), registers(0,0,0,0,0,0)
    : ram(255,0), registers(0,0,0,0,0,0)
{}
//CompSim::CompSim(RegBank re,Ram ra)
//    : ram(ra), registers(re)
//{}

//basic operation stuff
int CompSim::Poweron(RegBank re,Ram ra)
{
    ram = ra;
    registers = re;
    return 1;
}
int CompSim::Poweron()
{
    ram = Ram();
    registers = RegBank();
    return 1;
}
int CompSim::PowerOff()
{
    ram.ClearRam(0);
    registers.ClearRegisters();
    return 1;
}
int CompSim::DumpState()
{
    //system("clear");
    cout << endl;
    cout << "Reg A: 0x"  << hex << setw(2) << setfill('0')<< static_cast<unsigned>(registers.ReadRegister(REG_A)) << endl;
    cout << "Reg B: 0x"  << hex << setw(2) << setfill('0')<<static_cast<unsigned>(registers.ReadRegister(REG_B)) << endl;
    cout << "Reg C: 0x"  << hex << setw(2) << setfill('0')<<static_cast<unsigned>(registers.ReadRegister(REG_C)) << endl;
    cout << "Reg D: 0x"  << hex << setw(2) << setfill('0')<<static_cast<unsigned>(registers.ReadRegister(REG_D)) << endl;
    cout << "Reg PC: 0x" << hex << setw(2) << setfill('0')<<static_cast<unsigned>(registers.ReadRegister(REG_PC)) << endl;
    cout << "Reg FLAGS: 0x" << hex << setw(2) << setfill('0')<<static_cast<unsigned>(registers.ReadRegister(REG_FLAGS)) << endl;
    cout << "Ram Length: " << dec << ram.RamLength() << endl;
    cout << "Ram Dump: " << endl;
    for (int i = 0; i < ram.RamLength(); i++)
    {
        if (!(i%16)&&i!=0) cout << endl;
        cout << setfill('0') << hex << " 0x" << setw(2) <<static_cast<unsigned>(ram.ReadRam(i)) << " ";
    }
    cout << endl;
    return 1;
}
int CompSim::RunProgram()
{
    ReadInterpExec();
    DumpState();
    return 1;
}
//int CompSim::RunProgram(RegBank re,Ram ra)
//{
//    registers = re;
//    ReadInterpExec(ra);
//    return 1;
//}

//ram specific functions
int CompSim::LoadRam(vector<T>& vec,int len)
{
    ram.LoadRam(vec,len);
    return 1;
}
int CompSim::LoadRamLoc(T addr, T value)
{
    ram.WriteRam(addr,value);
    return 1;
}
int CompSim::ClearRam()
{
    ram.ClearRam(0);
    return 1;
}

//register specific functions
int CompSim::LoadRegister(RegCode code,T val)
{
    registers.WriteRegister(code,val);
    return 1;
}
int CompSim::LoadRegisters(T a, T b, T c, T d, T pc, T flags)
{
    registers = RegBank(a,b,c,d,pc,flags);
    return 1;
}
int CompSim::LoadRegisters(T a, T b, T c, T d)
{
    registers = RegBank(a,b,c,d);
    return 1;
}
int CompSim::ClearRegisters()
{
    registers.ClearRegisters();
    return 1;
}
InstrCode CompSim::MatchInstr(T ramVal)
{
    //switch on most significant 4 bits then switch on least significant bits
    T msbits = ramVal&0xF0;
    switch (msbits)
    {
    case 0x00: //nop, halt
    {
        if ((ramVal&0x0F)==0x00) //if the last four bits are 0
            return NOP_INSTR;
        else if ((ramVal&0x0F)==0x01) //if the last bits are 0001
            return HALT_INSTR;
        else //failed to find a matching instruction
            return NOP_INSTR;
    }
    //case 0x10: //these are unimplemented
    //{break;}
    //case 0x20:
    //{break;}
    case 0x30: //addi
    {
        return ADDI_INSTR;
    }
    case 0x40: //jmpi, jli, joi, jsi, jci, jzi, stf
    {
        if (ramVal==0x40) return JMPI_INSTR; //these all have constant opcodes
        if (ramVal==0x41) return JLI_INSTR;
        if (ramVal==0x42) return JOI_INSTR;
        if (ramVal==0x43) return JSI_INSTR;
        if (ramVal==0x44) return JCI_INSTR;
        if (ramVal==0x45) return JZI_INSTR;
        if (ramVal==0x46) return STF_INSTR;
        return NOP_INSTR; //failed to find a matching instruction
    }
    case 0x50: //cpr
    {
        return CPR_INSTR;
    }
    case 0x60: //jl, jo, js, ldi
    {
        //the second to last pair of bits determines which opcode
        //the last two determine the register
        if ((ramVal&0x0C)==0x00) return JL_INSTR;
        if ((ramVal&0x0C)==0x04) return JO_INSTR;
        if ((ramVal&0x0C)==0x08) return JS_INSTR;
        if ((ramVal&0x0C)==0x0C) return LDI_INSTR;
        return NOP_INSTR; //failed to find a matching instruction
    }
    case 0x70: //ld
    {
        return LD_INSTR;
    }
    case 0x80: //add
    {
        return ADD_INSTR;
    }
    case 0x90: //sub
    {
        return SUB_INSTR;
    }
    case 0xA0: //rsh
    {
        return RSH_INSTR;
    }
    case 0xB0: //not, jmp, jc, jz
    {
        //the last two bits of each byte determine the opcode
        //preserve the last two bits for comparison
        if ((ramVal&0x03)==0x00)return NOT_INSTR;
        if ((ramVal&0x03)==0x01)return JMP_INSTR;
        if ((ramVal&0x03)==0x02)return JC_INSTR;
        if ((ramVal&0x03)==0x03)return JZ_INSTR;
        return NOP_INSTR; //failed to find a matching instruction
    }
    case 0xC0: //and
    {
        return AND_INSTR;
    }
    case 0xD0: //or, inc
    {
        //for the LSB if xxxx then inc if xxyy then or
        //so mask off the 0000bb00 and mask off 000000bb
        //then compare 0000bb00 to (000000bb<<2)
        //this will determine a match
        if ((ramVal&0x0C)==((ramVal&0x03)<<2)) return INC_INSTR;
        if ((ramVal&0x0C)!=((ramVal&0x03)<<2)) return OR_INSTR;
        return NOP_INSTR;
    }
    case 0xE0: //cmp, dec
    {
        //exactly the same as or/inc but this time with dec/cmp
        if ((ramVal&0x0C)==((ramVal&0x03)<<2)) return DEC_INSTR;
        if ((ramVal&0x0C)!=((ramVal&0x03)<<2)) return CMP_INSTR;
        return NOP_INSTR;
    }
    case 0xF0: //st
    {
        return ST_INSTR;
    }
    default://what to do if we don't recognize an instruction?
    {
        return NOP_INSTR;
    }
    }
}
RegCode CompSim::UpperReg(T ramVal)
{
    //mask out the unwanted bits
    T masked  = ramVal&0x0C;
    switch (masked)
    {
    case 0x00:
    {
        return REG_A;
    }
    case 0x04:
    {
        return REG_B;
    }
    case 0x08:
    {
        return REG_C;
    }
    case 0x0C:
    {
        return REG_D;
    }
    default:
        return REG_A; //match failed return REG_A by default
    }
}
RegCode CompSim::LowerReg(T ramVal)
{
    //mask out the unwanted bits
    T masked  = ramVal&0x03;
    switch (masked)
    {
    case 0x00:
    {
        return REG_A;
    }
    case 0x01:
    {
        return REG_B;
    }
    case 0x02:
    {
        return REG_C;
    }
    case 0x03:
    {
        return REG_D;
    }
    default:
        return REG_A; //match failed return REG_A by default
    }
}
string CompSim::RegToStr(T ramVal)
{
    //mask out the unwanted bits
    T masked  = ramVal&0x03;
    switch (masked)
    {
    case 0x00:
    {
        return "a";
    }
    case 0x01:
    {
        return "b";
    }
    case 0x02:
    {
        return "c";
    }
    case 0x03:
    {
        return "d";
    }
    default:
        return "<NO REGISTER MATCH>";
    }

}
string CompSim::InstrToStr(InstrCode instr,T ramval)
{
    switch (instr)
    {
    case HALT_INSTR:
    {
        return "halt";
    }
    case NOP_INSTR:
    {
        return "nop";
    }
    case ADD_INSTR:
    {
        return "add " + RegToStr(UpperReg(ramval)) +", "+ RegToStr(LowerReg(ramval));
    }
    case SUB_INSTR:
    {
        return "sub " + RegToStr(UpperReg(ramval)) +", "+ RegToStr(LowerReg(ramval));
    }
    case CMP_INSTR:
    {
        return "cmp " + RegToStr(UpperReg(ramval)) +", "+ RegToStr(LowerReg(ramval));
    }
    case AND_INSTR:
    {
        return "and " + RegToStr(UpperReg(ramval)) +", "+ RegToStr(LowerReg(ramval));
    }
    case OR_INSTR:
    {
        return "or " + RegToStr(UpperReg(ramval)) +", "+ RegToStr(LowerReg(ramval));
    }
    case NOT_INSTR:
    {
        return "not " + RegToStr(UpperReg(ramval));
    }
    case RSH_INSTR:
    {
        return "rsh " + RegToStr(UpperReg(ramval)) +", "+ RegToStr(LowerReg(ramval));
    }
    case INC_INSTR:
    {
        return "inc " + RegToStr(UpperReg(ramval));
    }
    case DEC_INSTR:
    {
        return "dec " + RegToStr(UpperReg(ramval));
    }
    case LDI_INSTR:
    {
        return "ldi " + RegToStr(LowerReg(ramval)) +", "+ to_string(ram.ReadRam(registers.ReadRegister(REG_PC)+1))+"u";
    }
    case ST_INSTR:
    {
        return "st (" + RegToStr(LowerReg(ramval)) +"), "+ RegToStr(UpperReg(ramval));
    }
    case LD_INSTR:
    {
        return "ld " + RegToStr(UpperReg(ramval)) +", ("+ RegToStr(LowerReg(ramval))+")";
    }
    case CPR_INSTR:
    {
        return "cpr " + RegToStr(UpperReg(ramval)) +", "+ RegToStr(LowerReg(ramval));
    }
    case JMP_INSTR:
    {
        return "jmp " + RegToStr(UpperReg(ramval));
    }
    case JMPI_INSTR:
    {
        return "jmpi "+ to_string(ram.ReadRam(registers.ReadRegister(REG_PC)+1))+"u";
    }
    case JC_INSTR:
    {
        return "jc " + RegToStr(UpperReg(ramval));
    }
    case JCI_INSTR:
    {
        return "jci "+ to_string(ram.ReadRam(registers.ReadRegister(REG_PC)+1))+"u";
    }
    case JZ_INSTR:
    {
        return "jz " + RegToStr(UpperReg(ramval));
    }
    case JZI_INSTR:
    {
        return "jzi "+ to_string(ram.ReadRam(registers.ReadRegister(REG_PC)+1))+"u";
    }
    case JS_INSTR:
    {
        return "js " + RegToStr(LowerReg(ramval));
    }
    case JSI_INSTR:
    {
        return "jsi "+ to_string(ram.ReadRam(registers.ReadRegister(REG_PC)+1))+"u";
    }
    case JO_INSTR:
    {
        return "jo " + RegToStr(LowerReg(ramval));
    }
    case JOI_INSTR:
    {
        return "joi "+ to_string(ram.ReadRam(registers.ReadRegister(REG_PC)+1))+"u";
    }
    case JL_INSTR:
    {
        return "jl " + RegToStr(LowerReg(ramval));
    }
    case JLI_INSTR:
    {
        return "jli "+ to_string(ram.ReadRam(registers.ReadRegister(REG_PC)+1))+"u";
    }
    case ADDI_INSTR:
    {
        return "addi " + RegToStr(LowerReg(ramval)) + ", "+ to_string(ram.ReadRam(registers.ReadRegister(REG_PC)+1))+"u";
    }
    case STF_INSTR:
    {
        return "stf";
    }
    default:
    {
        return "<NO INSTRUCTION MATCHED>";
    }
    }
}
