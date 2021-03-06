#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <stdlib.h>
#define T uint8_t
using namespace std;

vector<T> csvToRam(ifstream&);
void printVec(vector<T>);
void charCodes(string str);
int charToNum(char);

int main(int argc, char* argv[])
{
    if (argc < 1) return 1;
    ifstream file;
    file.open(string(argv[1]));
    printVec(csvToRam(file));
    file.close();

    return 0;
}

vector<T> csvToRam(ifstream& file)
{
    vector<T> filevec;
    string line = "DEADBEEF";
    stringstream sstrm;
    string sub = "DEADBEEF";

    //skip first line
    getline(file,line);

    while(getline(file,line))
    {
        if (line!=" \r")
        {
            sub = line.substr(0,2);
            cout << endl;
            cout << "CHAR CODES:";
            cout << "LINE: " << dec <<line << endl;
            charCodes(line);
            cout << "SUB1:" << sub << endl;
            cout << "SUB1:0x" << hex << charToNum(sub[0]) << charToNum(sub[1]) << endl;
            filevec.push_back(16*charToNum(sub[0])+charToNum(sub[1]));
            if (line.size()>4)
            {
                sub = line.substr(3,line.size());
                cout << "SUB2:" << sub << endl;
                cout << "SUB2:0x" << hex << charToNum(sub[0]) << charToNum(sub[1]) << endl;
                filevec.push_back(16*charToNum(sub[0])+charToNum(sub[1]));
            }
        }
    }
    cout << endl;
    return filevec;
}


void printVec(vector<T> myVec)
{
    cout << "Ram Dump: " << endl;
    for (unsigned int i = 0; i < myVec.size(); i++)
    {
        if (!(i%16)&&i!=0) cout << endl;
        cout << setfill('0') << hex << " 0x" << setw(2) <<static_cast<unsigned>(myVec[i]) << " ";
    }
    cout << endl;

}
void charCodes(string str)
{
    for (unsigned int i = 0; i < str.size(); i++)
    {
        cout << int(str[i]) << " ";
    }
    cout << endl;
}
int charToNum(char c)
{
    switch (c)
    {
    case '0':
        return 0;
    case '1':
        return 1;
    case '2':
        return 2;
    case '3':
        return 3;
    case '4':
        return 4;
    case '5':
        return 5;
    case '6':
        return 6;
    case '7':
        return 7;
    case '8':
        return 8;
    case '9':
        return 9;
    case 'A':
    case 'a':
        return 10;
    case 'B':
    case 'b':
        return 11;
    case 'C':
    case 'c':
        return 12;
    case 'D':
    case 'd':
        return 13;
    case 'E':
    case 'e':
        return 14;
    case 'F':
    case 'f':
        return 15;
    default:
        return 0;
    }
}

