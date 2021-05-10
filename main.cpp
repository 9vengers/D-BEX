#include <iostream>
#include <string>
#include <fstream>
using namespace std;

int main ()
{
    int length;
    char *buffer = 0;
    ifstream readFile;
    ofstream writeFile;

    readFile.open("D-BEX/chinook.db", ios::binary);
    writeFile.open("D-BEX/chinook.json", ios::binary);

    if(readFile.is_open())
    {
        readFile.seekg(0, ios::end);
        length = readFile.tellg();
        readFile.seekg(0, ios::beg);

        buffer = new char [length];
        readFile.read(buffer, length);
        writeFile.write(buffer, length);

        if(writeFile.is_open()){
            unsigned char page_size[2];
            unsigned char page_num[2];
            
        }
    } 
    delete[] buffer;
    readFile.close();
    writeFile.close();

    return 0;
}