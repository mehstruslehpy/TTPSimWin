//currently a stub file
#include "Ram.h"

//make it easy to change this on the fly
#define T uint8_t

//ctors
Ram::Ram(int length, T initval)
    : RamRep(length,initval), RamLen(length)
{}

Ram::Ram()
    : RamRep(0,0), RamLen(0)
{}

int Ram::WriteRegion(T start, T end, T value)
{
    for (int i = start; i < end; i++)
    {
        RamRep[i%RamLength()] = value;
    }
    return 1;
}
int Ram::WriteRam(T addr,T val)
{
    RamRep[addr%RamLength()] = val;
    return 1;
}
T Ram::ReadRam(T addr)
{
    return RamRep[addr%RamLen];
}
int Ram::ClearRam(T val)
{
    WriteRegion(0,RamLength(),val);
    return 1;
}
int Ram::LoadRam(vector<T>& vec, int len)
{
    RamRep.clear();
    RamLen = 0;

    //cout << dec << "DEBUG: RamLen:" << RamLen << endl;
    for (unsigned int i = 0; i < vec.size(); i++)
    {
        RamRep.push_back(vec[i%vec.size()]);
        //cout << dec << "DEBUG: RamLen:" << RamLen << endl;
        //cout << dec << "DEBUG: len:" << len << endl;
    }
    RamLen = vec.size();

    while (RamLen < len)
    {

        //cout <<"DEBUG: RamLen:" << RamLen << endl;
        //cout <<"DEBUG: len:" << len << endl;
        RamLen++;
        RamRep.push_back(0);
    }
    //cout <<"LoadRam(): RamRep.size():" << RamLength() << endl;
    return 1;
}
int Ram::SetRamLength(T length)
{
    RamLen = length;
    return 1;
}
int Ram::RamLength()
{
    return RamLen;
}
