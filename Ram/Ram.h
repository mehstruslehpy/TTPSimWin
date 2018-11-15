#ifndef _RAM_H
#define _RAM_H
#include "../common.h"
//make this type easy to change

#define T uint8_t
const int DEFAULT_LEN = 0xFF;
using namespace std;

class Ram
{
    //ttp by default supports ram addresses from 0 to FF of bytes
    vector<T> RamRep;
    int RamLen;
public:
    //ctors
    Ram(int length, T initval);
    Ram();

    //default rule of five ones
    Ram(const Ram&) = default;               // Copy constructor
    Ram(Ram&&) = default;                    // Move constructor
    Ram& operator=(const Ram&) & = default;  // Copy assignment operator
    Ram& operator=(Ram&&) & = default;       // Move assignment operator
    virtual ~Ram() { }                     // Destructor

    int SetRamLength(T);
    int WriteRegion(T start, T end, T value);
    int WriteRam(T,T);
    T ReadRam(T);
    int ClearRam(T);
    int LoadRam(vector<T>&,int);
    int RamLength();
};
#endif
