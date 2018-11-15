#ifndef _REGBANK_H
#define _REGBANK_H
#include "../common.h"
//make this type easy to change
#define T uint8_t
using namespace std;

/*A numeric code for each register
*/
enum RegCode
{
    REG_A,
    REG_B,
    REG_C,
    REG_D,
    REG_PC,
    REG_FLAGS,
};

//what else do registers need or do? Maybe a lock to make it unwritable?
class Register
{
    T value;
public:
    Register(T val)
        : value(val)
    {}

    //default rule of five ones
    Register(const Register&) = default;               // Copy constructor
    Register(Register&&) = default;                    // Move constructor
    Register& operator=(const Register&) & = default;  // Copy assignment operator
    Register& operator=(Register&&) & = default;       // Move assignment operator
    virtual ~Register() { }                     // Destructor

    T Read();
    int Write(T);
    int Clear();
};

class RegBank
{
    Register A;
    Register B;
    Register C;
    Register D;
    Register PC;
    Register FLAGS;
    //Register Instruction
public:
    //useful ctors
    RegBank();
    RegBank(Register,Register,Register,Register,Register,Register);
    RegBank(T,T,T,T);
    RegBank(T,T,T,T,T,T);
    //rule of three(five?)
    RegBank(const RegBank&) = default;               // Copy constructor
    RegBank(RegBank&&) = default;                    // Move constructor
    RegBank& operator=(const RegBank&) & = default;  // Copy assignment operator
    RegBank& operator=(RegBank&&) & = default;       // Move assignment operator
    virtual ~RegBank() { }                     // Destructor

    //useful functions
    T ReadRegister(RegCode reg);
    int WriteRegister(RegCode reg,T val);
    int ClearRegisters();
    int LoadRegisters(T A, T B, T C, T D, T PC, T FLAGS);
    bool CarryFlag();
    bool ZeroFlag();
    bool SignFlag();
    bool OverflowFlag();
    bool LessFlag();
};
#endif
