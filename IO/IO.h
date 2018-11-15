#ifndef _IO_H
#define _IO_H
#include "../common.h"
#define T uint8_t
using namespace std;

//not exactly sure how to implement this
//ideally as time progresses I can add io components here
//and then incorporate them into the asm language
class IO
{
public:
    int WriteText(ostream&,string);
    string ReadText(istream&,string);
    int WriteNum(ostream&,T);
    int WriteNumHex(ostream&,T);
    T ReadNum(istream&,T);

    //default rule of five ones
    IO() = default;               // Constructor
    IO(const IO&) = default;               // Copy constructor
    IO(IO&&) = default;                    // Move constructor
    IO& operator=(const IO&) & = default;  // Copy assignment operator
    IO& operator=(IO&&) & = default;       // Move assignment operator
    virtual ~IO() { }                     // Destructor

};
#endif
