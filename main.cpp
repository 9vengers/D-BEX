#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
using namespace std;
#define SIZE_OF_DB_HEADER 100

// temporary Function.
// Only read the size of a page and the number of pages in the database.
// It will store variety variables TBD.
void ReadDBheader(unsigned char *buffer, unsigned int *sizeOfPage, unsigned int *numberOfPage)
{
    *sizeOfPage = buffer[16] * pow(2,8) + buffer[17];   // 16-17 offset in DB Header
    *numberOfPage = buffer[28] * pow(2,24) + buffer[29] * pow(2,16) + buffer[30] * pow(2,8) + buffer[31]; // 28-31 offset in DB Header

    return;
}

int BitPattern(unsigned char *buffer)
{
    unsigned int result = 0;
    int count = 0;
    int sizeOfByte = 1;

    // get a byte_size
    while(count < 9)
    {
        if(buffer[count] >= 128)
            sizeOfByte++;
        else break;
        count++;
    }
    // get result
    for (count = 0; count < sizeOfByte - 1; count++)
    {
        result += buffer[count];
        if(buffer[count] >= 128) result -= 128;
        result <<= 7;
    }
    result += buffer[sizeOfByte - 1];

    return result;
}

int DataFieldSize(int a){
    if (a == 0){
        return 0;
    }
    else if(a > 0 && a <= 4){
        return a;
    }
    else if(a == 5){
        return 6;
    }
    else if(a == 6){
        return 8;
    }
    else if(a == 7){
        return 8;
    } 
    else if(a > 12){
        return (a-12)/2;
    } 
    else if(a > 13){
        return (a-13)/2;
    }
}


// Get the data of bytestream as much as you want
int GetByteStream(unsigned char *buffer, int sizeOfByte)
{
    unsigned int result = 0;
    int count = 0;

    for (count = 0; count < sizeOfByte - 1; count++)
    {
        result += buffer[count];
        result <<= 8;
    }
    result += buffer[sizeOfByte - 1];


    // test command out line
    cout << result << endl;

    return result;
}

void ReadPage(unsigned char *buffer)
{
    //header
    int sizeOfPageHeader = 0;
    //PageFlag 1Byte(Internal : 0x05, Leaf:0x0D)
    switch(*buffer){
        case 0x02:
            return;
            break;

        case 0x05:
            sizeOfPageHeader = 12;
            break;

        case 0x0A:
            return;
            break;

        case 0x0D:
            sizeOfPageHeader = 8;
            break;

        default:
            return;
            break;
    }
    buffer += 1;

    int offsetOfFreeSpace = GetByteStream((unsigned char*)buffer, 2);
    buffer += 2;
    int numberOfCells = GetByteStream((unsigned char*)buffer, 2);
    buffer += 2;
    int offsetOfFirstByte = GetByteStream((unsigned char*)buffer, 2);
    buffer += 2;
    int fragmentedFreeBytes = GetByteStream((unsigned char*)buffer, 1);
    buffer += 1;
    
    if(sizeOfPageHeader == 12) int childPage = GetByteStream((unsigned char*)buffer, 4);

}

struct DbexData{
    string fileName;
    int numberOfTable;
    struct table{
        
        struct attribute{
            string name;
            int type;
            int length;
            bool nullable;

            };
        
        string tableName;
        string rowNumber;
        int numberOfColumn;
        
    };
};

class DBConverter{
    public:
    int sizeOfPageHeader_;
    int offsetOfFreeSpace_;
    int numberOfCells_;
    int offsetOfFirstByte_;
    int fragmentedFreeBytes_;
    int childPage_;
    int cellOffset_[];


};




int main ()
{

    ifstream readFile;
    ofstream writeFile;

    //open the target file
    readFile.open("D-BEX/chinook.db", ios::binary);

    if(readFile.is_open())
    {
        unsigned int sizeOfPage, numberOfPage;
        char *buffer = 0;
        buffer = new char [SIZE_OF_DB_HEADER];
        readFile.read(buffer, SIZE_OF_DB_HEADER);
        ReadDBheader((unsigned char*)buffer, &sizeOfPage, &numberOfPage);
        printf("page size is : %d, the number of pages : %d\n", sizeOfPage, numberOfPage);
        delete[] buffer;

        buffer = new char[sizeOfPage-SIZE_OF_DB_HEADER];
        readFile.read(buffer, sizeOfPage-SIZE_OF_DB_HEADER);

        ReadPage((unsigned char*)buffer);


    } 


    readFile.close();

    return 0;
}