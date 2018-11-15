#include "common.h"
#include "defs.h"
#define T uint8_t
using namespace std;

vector<T> csvToRam(ifstream&);
void printVec(vector<T>);
void charCodes(string str);
int charToNum(char);

int main(int argc, char* argv[])
{
    if (argc != 2) return 1;
    ifstream file;
    file.open(string(argv[1]));
    vector<T> filevec = csvToRam(file);
    file.close();
    printVec(filevec);
    CompSim comp;

    comp.LoadRam(filevec,256);
    comp.DumpState();
    comp.RunProgram();
    comp.DumpState();

    //printVec(filevec);
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
	if (line!=" \r"&&line[0]!=' ') //blank lines are the string " \r"
        {
            cout << "DEBUG: CASE 1" << endl;
			sub = line.substr(0,2);
            cout << endl;
            cout << "LINE: \"" << dec <<line <<"\""<< endl;
            cout << "CHAR CODES:";
            charCodes(line);
            cout << "SUB1:" << sub << endl;
            cout << "SUB1:0x" << hex << charToNum(sub[0]) << charToNum(sub[1]) << endl;
            filevec.push_back(16*charToNum(sub[0])+charToNum(sub[1]));
			cout << "DEBUG: line.size()=" << line.size() << endl;
            //if (line.size()==4)
            if (line.size()==5||line.size()==4)
            {
                sub = line.substr(3,line.size());
                cout << "SUB2:" << sub << endl;
                cout << "SUB2:0x" << hex << charToNum(sub[0]) << charToNum(sub[1]) << endl;
                if(line.size()==5)filevec.push_back(16*charToNum(sub[0])+charToNum(sub[1]));
                if(line.size()==4)filevec.push_back(charToNum(sub[0]));
            }
        }
        else if (line[0] == ' ' && line[1] != '\r') //byte instructions are a space followed by one or two digits
        {
			cout << "DEBUG: CASE 2" << endl;
            cout << endl;
            cout << dec << "LINE: \"" << line <<"\""<< endl;
            if(line.size()==2) //one digit
            {
                sub = line.substr(1,1);
                cout << "BYTE CHAR CODES:";
                charCodes(sub);
                cout << dec << "SUB: \"" << sub <<"\""<< endl;
                filevec.push_back(charToNum(sub[0]));
            }
            if(line.size()==4||line.size()==3) //two digits
            {
                sub = line.substr(1,2);
                cout << "BYTE CHAR CODES:";
                charCodes(sub);
                cout << dec << "SUB: \"" << sub <<"\""<< endl;
                filevec.push_back(16*charToNum(sub[0])+charToNum(sub[1]));
            }

            cout << endl;
        }
    }
    cout << endl;
    return filevec;
}


void printVec(vector<T> myVec)
{
    cout << "Vector Dump: " << endl;
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

