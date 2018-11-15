#include "RegBank.h"
//make this type easy to change
#define T uint8_t

using namespace std;
//register class functions
T Register::Read()
{
    return value;
}
int Register::Write(T val)
{
    value = val;
    return 1;
}
int Register::Clear()
{
    value = 0;
    return 1;
}

//regbank class functions
RegBank::RegBank()
    : A(0),B(0),C(0),D(0),PC(0),FLAGS(0)
{}
RegBank::RegBank(Register a, Register b, Register c, Register d, Register pc, Register flags)
    : A(a),B(b),C(c),D(d),PC(pc),FLAGS(flags)
{}
RegBank::RegBank(T a, T b, T c, T d)
    : A(a),B(b),C(c),D(d),PC(0),FLAGS(0)
{}
RegBank::RegBank(T a, T b, T c, T d, T pc, T flags)
    : A(a),B(b),C(c),D(d),PC(pc),FLAGS(flags)
{}

T RegBank::ReadRegister(RegCode reg)
{
    switch(reg)
    {
    case REG_A:
    {
        return A.Read();
    }
    case REG_B:
    {
        return B.Read();
    }
    case REG_C:
    {
        return C.Read();
    }
    case REG_D:
    {
        return D.Read();
    }
    case REG_PC:
    {
        return PC.Read();
    }
    case REG_FLAGS:
    {
        return FLAGS.Read();
    }
    default:
    {
        cout << "ERROR: T RegBank::ReadRegister(RegCode reg)" << endl;
        abort();
    }
    }
}
int RegBank::WriteRegister(RegCode reg,T val)
{
    switch(reg)
    {
    case REG_A:
    {
        A.Write(val);
        break;
    }
    case REG_B:
    {
        B.Write(val);
        break;
    }
    case REG_C:
    {
        C.Write(val);
        break;
    }
    case REG_D:
    {
        D.Write(val);
        break;
    }
    case REG_PC:
    {
        PC.Write(val);
        break;
    }
    case REG_FLAGS:
    {
        FLAGS.Write(val);
        break;
    }
    default:
    {
        cout << "ERROR: int RegBank::WriteRegister(RegCode reg,T val)" << endl;
        abort();
    }
    }
    return 1;
}
int RegBank::ClearRegisters()
{
    A.Clear();
    B.Clear();
    C.Clear();
    D.Clear();
    PC.Clear();
    FLAGS.Clear();
    return 1;
}
int RegBank::LoadRegisters(T a, T b, T c, T d, T pc, T flags)
{
    A.Write(a);
    B.Write(b);
    C.Write(c);
    D.Write(d);
    PC.Write(pc);
    FLAGS.Write(flags);
    return 1;
}

//convenience function adapted from stack overflow
static T checkbit(T var, T pos)
{
    return ((var) & (1<<(pos)));//could probably just return this in each function below
}
//the flags will be ordered as CZSOL
bool RegBank::CarryFlag()
{
    if (checkbit(FLAGS.Read(),0))
    {
        return true;
    }
    return false;
}
bool RegBank::ZeroFlag()
{
    if (checkbit(FLAGS.Read(),1))
    {
        return true;
    }
    return false;
}
bool RegBank::SignFlag()
{
    if (checkbit(FLAGS.Read(),2))
    {
        return true;
    }
    return false;
}

bool RegBank::OverflowFlag()
{
    if (checkbit(FLAGS.Read(),3))
    {
        return true;
    }
    return false;
}
bool RegBank::LessFlag()
{
    if (checkbit(FLAGS.Read(),4))
    {
        return true;
    }
    return false;
}

