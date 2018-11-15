#include "common.h"
#include "CompSim.h"
#define T uint8_t
//T2 has a wider bitwidth than T
#define T2 uint16_t
//Helper Functions

//Utility Type:
//does nothing
T CompSim::Nop() // {} 0000 0000
{
    return 1;
}
//this shouldn't actually run the ReadInterpExec
//method should actually catches this before entering here
//if it fails the program will loop endlessly here which is fine
//too
T CompSim::Halt() // for(;;) 0000 0001
{
    for(;;) {}
    return 1;
}

//ALU Type (changes flags register):
//convenience macros that I probably don't need
#define MSB(x) (x & 0x80)
bool CompSim::ComputeCarryFlag(T op1,T op2,InstrCode code)
{
    //cout << "DEBUG: ComputeCarryFlag(): T(op1+op2)=" << T(op1+op2) << endl;
    //cout << "DEBUG: ComputeCarryFlag(): T2(op1+op2)" << T(op1+op2) << endl;
    //cout << "DEBUG: ComputeCarryFlag(): T(op1-op2)" << T(op1-op2) << endl;
    //cout << "DEBUG: ComputeCarryFlag(): T2(op1-op2)" << T2(op1-op2) << endl;
    if (code == ADD_INSTR)
    {
        if (T2(op1+op2)>T(op1+op2)) return true;
        return false;
    }
    else if (code == SUB_INSTR || code == CMP_INSTR)
    {
        if (T2(op1-op2)>T(op1-op2)) return true;
        return false;
    }
    else
    {
        cout << "ERROR: T CompSim::ComputeCarryFlag()" << endl;
        return false;
    }
}
bool CompSim::ComputeZeroFlag(T result)
{
    if (result == 0) return true;
    else return false;
}
bool CompSim::ComputeSignFlag(T result)
{
    if (MSB(result)) return true;
    else return false;
}
bool CompSim::ComputeOverflowFlag(T op1,T op2, T result,InstrCode code)
{
    if (code == ADD_INSTR)
    {
        //o=!x!ys+xy!s
        if((!ComputeSignFlag(op1)&&!ComputeSignFlag(op2)&&ComputeSignFlag(result))
                ||(ComputeSignFlag(op1)&&ComputeSignFlag(op2)&&!ComputeSignFlag(result)))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else if (code == SUB_INSTR || code == CMP_INSTR)
    {
        //o=x!y!d+!xyd
        if((ComputeSignFlag(op1)&&!ComputeSignFlag(op2)&&!ComputeSignFlag(result))
                ||(!ComputeSignFlag(op1)&&ComputeSignFlag(op2)&&ComputeSignFlag(result)))
        {
            return true;
        }
        else
        {
            return false;
        }

    }
    else
    {
        cout << "ERROR: CompSim::ComputeOverflowFlag()" << endl;
        return false;
    }
}
bool CompSim::ComputeLessFlag(T op1,T op2,T result,InstrCode code)
{
    if (code == ADD_INSTR)
    {
        if( (ComputeSignFlag(result)&&!ComputeOverflowFlag(op1,op2,result,code))
                ||(!ComputeSignFlag(result)&&ComputeOverflowFlag(op1,op2,result,code)))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else if (code == SUB_INSTR || code == CMP_INSTR)
    {
        if( (ComputeSignFlag(result)&&!ComputeOverflowFlag(op1,op2,result,code))
                ||(!ComputeSignFlag(result)&&ComputeOverflowFlag(op1,op2,result,code)))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        cout << "ERROR: CompSim::ComputeLessFlag()" << endl;
        return false;
    }
}
int CompSim::ComputeFlags(T op1,T op2,T result,InstrCode opcode)
{
    switch (opcode)
    {
    case ADD_INSTR:
    case SUB_INSTR:
    case CMP_INSTR:
    {
        T flags = 0;
        //cflag: if the number is not representable in 8 bits
        if (ComputeCarryFlag(op1,op2,opcode)) flags += 0x01;
        //zflag: if the result is 0
        if (ComputeZeroFlag(result)) flags += 0x02;
        //sflag: if the msb of the result is set
        if (ComputeSignFlag(result)) flags += 0x04;
        //oflag: depends on version this is the add version which checks msb's
        if (ComputeOverflowFlag(op1,op2,result,opcode)) flags += 0x08;
        //lflag: S!O+!SO
        if (ComputeLessFlag(op1,op2,result,opcode)) flags += 0x10;
        registers.WriteRegister(REG_FLAGS,flags);
        break;
    }
    case AND_INSTR:
    case OR_INSTR:
    case NOT_INSTR:
    case RSH_INSTR:
    {
        T flags = 0;
        //zflag: if the result is 0
        if (ComputeZeroFlag(result)) flags += 0x02;
        //sflag: if the msb of the result is set
        if (ComputeSignFlag(result)) flags += 0x04;
        registers.WriteRegister(REG_FLAGS,flags);

        //lflag: use the sub version?
        //if (ComputeLessFlag(op1,op2,result,SUB_INSTR)) flags += 0x10;

        //ttp recomputes zero and sign flags then xors sign and overflow
        //to compute the less flag
        if ((registers.SignFlag()||registers.OverflowFlag())
                && !(registers.SignFlag()&&registers.OverflowFlag()))
            flags += 0x10;

        registers.WriteRegister(REG_FLAGS,flags);
        break;
    }
    default:
    {}
    }
    return 1;
}

T CompSim::Add(RegCode xc, RegCode yc) // x+=y 1000 xxyy
{
    T op1 = registers.ReadRegister(xc);
    T op2 = registers.ReadRegister(yc);
    T result = op1+op2;
    registers.WriteRegister(xc,result);
    ComputeFlags(op1,op2,result,ADD_INSTR);
    return 1;
}
T CompSim::Sub(RegCode xc,RegCode yc) // x-=y 1001 xxyy
{
    T op1 = registers.ReadRegister(xc);
    T op2 = registers.ReadRegister(yc);
    T result = op1-op2;
    registers.WriteRegister(xc,result);
    ComputeFlags(op1,op2,result,SUB_INSTR);
    return 1;
}
T CompSim::Cmp(RegCode xc,RegCode yc) // x-y 1110 xxyy
{
    T op1 = registers.ReadRegister(xc);
    T op2 = registers.ReadRegister(yc);
    T result = op1-op2;
    ComputeFlags(op1,op2,result,CMP_INSTR);
    return 1;
}
T CompSim::And(RegCode xc,RegCode yc) // x&=y 1100 xxyy
{
    T op1 = registers.ReadRegister(xc);
    T op2 = registers.ReadRegister(yc);
    T result = op1&op2;
    registers.WriteRegister(xc,result);
    ComputeFlags(op1,op2,result,AND_INSTR);
    return 1;
}
T CompSim::Or(RegCode xc,RegCode yc) // x|=y 1101 xxyy
{
    T op1 = registers.ReadRegister(xc);
    T op2 = registers.ReadRegister(yc);
    T result = op1|op2;
    registers.WriteRegister(xc,result);
    ComputeFlags(op1,op2,result,OR_INSTR);
    return 1;
}
T CompSim::Not(RegCode xc) // x=~x 1011 xx00
{
    T op1 = registers.ReadRegister(xc);
    T result = ~op1;
    registers.WriteRegister(xc,result);
    //the docs say not updates the flags register
    //but the flags register never gets enabled when
    //you trace the instruction in logisim
    //ComputeFlags(op1,~op1,result,NOT_INSTR);
    return 1;
}
T CompSim::Rsh(RegCode xc,RegCode yc) // x>>=y 1010 xxyy
{
    //the ttp takes ONLY the 3 least significant bits of operand 2
    //everything else gets ignored, this is undocumented
    //we can achieve this with a bitmask of 0x07 on op2
    T op1 = registers.ReadRegister(xc);
    T op2 = registers.ReadRegister(yc);
    op2 &= 0x07;
    T result = op1>>op2;
    registers.WriteRegister(xc,result);
    ComputeFlags(op1,op2,result,RSH_INSTR);
    return 1;
}

//ALU Type (no changes flags register):
T CompSim::Inc(RegCode xc) // ++x 1101 xxxx
{
    T op1 = registers.ReadRegister(xc);
    T result = ++op1;
    registers.WriteRegister(xc,result);
    return 1;
}
T CompSim::Dec(RegCode xc) // --x 1110 xxxx
{
    T op1 = registers.ReadRegister(xc);
    T result = --op1;
    registers.WriteRegister(xc,result);
    return 1;
}

//Copying Type:
T CompSim::Ldi(RegCode xc) // x=*(PC++) or x=(*PC++) 0110 11xx
{
    //cout << "DEBUG: T CompSim::Ldi(RegCode xc)" << endl;
    //get the value right after the just read instruction by incrementing the pc once
    //and then reading from ram
    T pc = registers.ReadRegister(REG_PC);
    pc++;
    registers.WriteRegister(REG_PC,pc);

    //assign the new value from
    T val = ram.ReadRam(pc);
    //put it in a register
    registers.WriteRegister(xc,val);
    return 1;
}
T CompSim::St(RegCode xc,RegCode yc) // *y=x 1111 xxyy
{
    T addr = registers.ReadRegister(yc);
    T val = registers.ReadRegister(xc);
    ram.WriteRam(addr,val);
    return 1;
}
T CompSim::Ld(RegCode xc,RegCode yc) // x=*y 0111 xxyy
{
    T addr = registers.ReadRegister(yc);
    T val = ram.ReadRam(addr);
    registers.WriteRegister(xc,val);
    return 1;
}
T CompSim::Cpr(RegCode xc,RegCode yc) // x=y 0101 xxyy
{
    registers.WriteRegister(xc,registers.ReadRegister(yc));
    return 1;
}

//Jump Type:
T CompSim::Jmp(RegCode xc) // PC=x 1011 xx01
{
    T val = registers.ReadRegister(xc);
    registers.WriteRegister(REG_PC,val);
    return 1;
}
//is this right?
T CompSim::Jmpi() // PC=i or PC=*PC 0100 0000
{
    //increment the program counter past the instruction opcode
    T pc = 0;
    pc = registers.ReadRegister(REG_PC);
    //cout << "DEBUG: pc=" << pc << endl;
    registers.WriteRegister(REG_PC,ram.ReadRam(pc+1));
    //pc++;
    //registers.WriteRegister(REG_PC,pc);
    //read the immediate operand into the program counter
    //T val = ram.ReadRam(pc);
    //registers.WriteRegister(REG_PC,val);
    return 1;
}
T CompSim::Jc(RegCode xc) // PC=(CFlag?XReg:PC) 1011 xx10
{
    if (registers.CarryFlag())
    {
        T val = registers.ReadRegister(xc);
        registers.WriteRegister(REG_PC,val);
    }
    else
    {
        T pc = registers.ReadRegister(REG_PC);
        pc++;
        registers.WriteRegister(REG_PC,pc);
    }
    return 1;
}
T CompSim::Jci() // PC=(CFlag?*PC:PC+1) 0100 0100
{
    //increment the program counter past the instruction opcode
    T pc = registers.ReadRegister(REG_PC);
    pc++;
    registers.WriteRegister(REG_PC,pc);

    if (registers.CarryFlag())
    {
        T val = ram.ReadRam(pc);
        registers.WriteRegister(REG_PC,val);
    }
    else
    {
        pc++;
        registers.WriteRegister(REG_PC,pc);
    }
    return 1;
}
T CompSim::Jz(RegCode xc) // ZFlag 1011 xx11
{
    if (registers.ZeroFlag())
    {
        T val = registers.ReadRegister(xc);
        registers.WriteRegister(REG_PC,val);
    }
    else
    {
        T pc = registers.ReadRegister(REG_PC);
        pc++;
        registers.WriteRegister(REG_PC,pc);
    }
    return 1;
}
T CompSim::Jzi() // ZFlag 0100 0101
{
    //increment the program counter past the instruction opcode
    T pc = registers.ReadRegister(REG_PC);
    pc++;
    registers.WriteRegister(REG_PC,pc);

    if (registers.ZeroFlag())
    {
        T val = ram.ReadRam(pc);
        registers.WriteRegister(REG_PC,val);
    }
    else
    {
        pc++;
        registers.WriteRegister(REG_PC,pc);
    }

    return 1;
}
T CompSim::Js(RegCode xc) // SFlag 0110 10xx
{
    if (registers.SignFlag())
    {
        T val = registers.ReadRegister(xc);
        registers.WriteRegister(REG_PC,val);
    }
    else
    {
        T pc = registers.ReadRegister(REG_PC);
        pc++;
        registers.WriteRegister(REG_PC,pc);
    }
    return 1;
}
T CompSim::Jsi() // SFlag 0100 0011
{
    //increment the program counter past the instruction opcode
    T pc = registers.ReadRegister(REG_PC);
    pc++;
    registers.WriteRegister(REG_PC,pc);

    if (registers.SignFlag())
    {
        T val = ram.ReadRam(pc);
        registers.WriteRegister(REG_PC,val);
    }
    else
    {
        pc++;
        registers.WriteRegister(REG_PC,pc);
    }

    return 1;
}
T CompSim::Jo(RegCode xc) // OFlag 0110 01xx
{
    if (registers.OverflowFlag())
    {
        T val = registers.ReadRegister(xc);
        registers.WriteRegister(REG_PC,val);
    }
    else
    {
        T pc = registers.ReadRegister(REG_PC);
        pc++;
        registers.WriteRegister(REG_PC,pc);
    }
    return 1;
}
T CompSim::Joi() // OFlag 0100 0010
{
    //increment the program counter past the instruction opcode
    T pc = registers.ReadRegister(REG_PC);
    pc++;
    registers.WriteRegister(REG_PC,pc);

    if (registers.OverflowFlag())
    {
        T val = ram.ReadRam(pc);
        registers.WriteRegister(REG_PC,val);
    }
    else
    {
        pc++;
        registers.WriteRegister(REG_PC,pc);
    }

    return 1;
}
T CompSim::Jl(RegCode xc) // LFlag 0110 00xx
{
    if (registers.LessFlag())
    {
        T val = registers.ReadRegister(xc);
        registers.WriteRegister(REG_PC,val);
    }
    else
    {
        T pc = registers.ReadRegister(REG_PC);
        pc++;
        registers.WriteRegister(REG_PC,pc);
    }
    return 1;
}
T CompSim::Jli() // LFlag 0100 0001
{
    //increment the program counter past the instruction opcode
    T pc = registers.ReadRegister(REG_PC);
    pc++;
    registers.WriteRegister(REG_PC,pc);

    if (registers.LessFlag())
    {
        T val = ram.ReadRam(pc);
        registers.WriteRegister(REG_PC,val);
    }
    else
    {
        pc++;
        registers.WriteRegister(REG_PC,pc);
    }
    return 1;
}

//"Memory Allocation" Type:
//I don't think this is implementable?
//T Byte(); //alloc and initialize a byte in RAM

//Misc Type:
//these don't actually exist on TTP
T CompSim::Addi(RegCode xc) // add imediate to reg 0011 00xx
{
    //increment the program counter past the instruction opcode
    T pc = registers.ReadRegister(REG_PC);
    pc++;
    registers.WriteRegister(REG_PC,pc);

    //read the immediate operand
    T ival = ram.ReadRam(pc);
    //read the register value
    T rval = registers.ReadRegister(xc);
    //add them
    T result = rval + ival;
    //compute flags
    ComputeFlags(rval,ival,result,ADD_INSTR);
    //assign result
    registers.WriteRegister(xc,result);
    return 1;
}
T CompSim::Stf() //store FLAGS reg to where D reg points in ram 0100 0010
{
    ram.WriteRam(registers.ReadRegister(REG_D),registers.ReadRegister(REG_FLAGS));
    return 1;
}
