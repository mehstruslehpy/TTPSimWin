//currently a stub file
#include "IO.h"
#define T uint8_t
using namespace std;


int IO::WriteText(ostream& out, string str)
{
    out << "COMP-SIM-OUTPUT: " + str << endl;
    return 1;
}
string IO::ReadText(istream& in, string str)
{
    in >> str;
    return str;
}

int IO::WriteNum(ostream& out, T num)
{
    out << "COMP-SIM-OUTPUT: " + num << endl;
    return 1;
}
int IO::WriteNumHex(ostream& out, T num)
{
    out << "COMP-SIM-OUTPUT: " + num << endl;
    return 1;
}
T IO::ReadNum(istream& in, T num)
{
    in >> num;
    return num;
}

