#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
using namespace std;
#define SIZE_OF_DB_HEADER 100

// temporary Function.
// Only read the size of a page and the number of pages in the database.
// It will store variety variables TBD.
void ReadDBheader(unsigned char *buffer, unsigned int *sizeOfPage, unsigned int *numberOfPages)
{
    *sizeOfPage = buffer[16] * pow(2,8) + buffer[17];   // 16-17 offset in DB Header
    *numberOfPages = buffer[28] * pow(2,24) + buffer[29] * pow(2,16) + buffer[30] * pow(2,8) + buffer[31]; // 28-31 offset in DB Header

    return;
}

int BitPattern(unsigned char *buffer)
{
    unsigned int result = 0;
    int count = 0;
    int sizeOfBytes = 1;

    // get a byte_size
    while(count < 9)
    {
        if(buffer[count] >= 128)
            sizeOfBytes++;
        else break;
        count++;
    }
    // get result
    for (count = 0; count < sizeOfBytes - 1; count++)
    {
        result += buffer[count];
        if(buffer[count] >= 128) result -= 128;
        result <<= 7;
    }
    result += buffer[sizeOfBytes - 1];

    return result;
}

/*int DataFieldSize(int a){
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
}*/


// Get the data of bytestream as much as you want
int ByteStream(unsigned char *buffer, int sizeOfBytes)
{
    unsigned int result = 0;
    int count = 0;

    for (count = 0; count < sizeOfBytes - 1; count++)
    {
        result += buffer[count];
        result <<= 8;
    }
    result += buffer[sizeOfBytes - 1];

    return result;
}

// Read Only One Page in pageSize
void ReadPage(unsigned char *buffer)
{
    // ---------------
    // HEADER
    // ---------------
    int pageHeaderSize = 0;

    // PageFlag     1Byte
    // Internal   : 0x05
    // Leaf       : 0x0D
    switch(*buffer){
        case 0x02:
            return;
            break;

        case 0x05:
            pageHeaderSize = 12;
            cout << "Page type : Internal page of B-tree table" << endl;
            break;

        case 0x0A:
            return;
            break;

        case 0x0D:
            pageHeaderSize = 8;
            cout << "Page type : leaf page of B-tree table" << endl;
            break;

        // exception, error
        default:
            return;
            break;
    }
    buffer += 1;

    int freeblockOffset = ByteStream(buffer, 2);
    buffer += 2;
    int numberOfCells = ByteStream(buffer, 2);
    buffer += 2;
    int firstCellOffset = ByteStream(buffer, 2);
    buffer += 2;
    int fragmentedFreeBytes = ByteStream(buffer, 1);
    buffer += 1;
    int rightMostChildPage = 0;
    if(pageHeaderSize == 12) rightMostChildPage = ByteStream(buffer, 4);
    buffer += 4;

    // test command out line

    cout << "Number of Cells : " << numberOfCells << endl;
    cout << "Fragmented Free Bytes : " << fragmentedFreeBytes << endl;
    cout << "The right-most child page : " << rightMostChildPage << endl;

    cout << showbase << uppercase << hex;
    cout << "First of Freeblock Offset : " << hex << freeblockOffset << endl;
    cout << "First of Cellblock Offset : " << firstCellOffset << dec << endl;
    // ------------------
    // Cell Offsets
    // ------------------
    unsigned short *cellOffset = new unsigned short [numberOfCells];
    for(int count = 0; count < numberOfCells; count++)
    {
        *(cellOffset + count) = ByteStream(buffer, 2);
        buffer += 2;
        cout << "Cell Offset[" << count << "] : " << hex << cellOffset[count] << dec << endl;
    }
    cout << noshowbase << nouppercase << dec;
    

    // ------------------
    // Cell Contents
    // ------------------



    delete[] cellOffset;

}
struct ForeignKey{
    string field;
    string table;

    // 1: RESTRICT  2: CASCADE  3:NO ACTION  4: SET NULL  5: SET DEFAULT
    int onDelete;
    int onUpdate;
};
struct Field{
    int type;
    int typeLength;
    bool nullable;
    string defaultData;
    int collation;
    bool autoIncrement;


    bool primaryKey;
    bool unique;
    string check;

    ForeignKey foriegnKey;
};
struct Constraint{
    string primaryKey;
    bool autoIncrement;

    string check;
    string defaultData;

    ForeignKey foriegnKey;
};
struct Table{
    string name;
    string createSql;
    int numberOfFields;
    Field *fields;
    void addField(){
        fields = new Field[numberOfFields];
        return;
    }



};

class SqliteInfo{
public:
    string fileName;
    int pageSize, changeCounter, inheaderSize, pageOfFreelist, numberOfFreelist, schemaCookie,
    schemaFormat, defaultCacheSize, vacuumRootPage, textEncoding, userVersion, incrementalVacuum,
    applicationID, versionValid, sqliteVersion, numberOfTables;
    char writeVersion, readVersion, reservedSpace, maxPayload, minPayload, leafPayload;

    // PRAGMA 추후 고려
    // Auto Vacuum mode(auto optimize db size), FSYNC, Journal mode, Locking mode ...
    // ...

    SqliteInfo(string file){
        fileName = file;
        pageSize = 0;
        writeVersion = 1;
        readVersion = 1;
        reservedSpace = 0;
        maxPayload = 64;
        minPayload = 32;
        leafPayload = 32;
        changeCounter = 0;
        inheaderSize = 1;
        pageOfFreelist= 0;
        numberOfFreelist = 0;
        schemaCookie = 0;
        schemaFormat = 0;
        defaultCacheSize;
        vacuumRootPage = 0;
        textEncoding = 1;
        userVersion = 0;
        incrementalVacuum = 0;
        applicationID = 0;
        versionValid = 0;
        sqliteVersion = 0;
    }
private:


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
        printf("Page size : %d, Number of pages : %d\n", sizeOfPage, numberOfPage);
        delete[] buffer;

        //only first page
        cout << "------------1Page-----------" << endl;
        buffer = new char[sizeOfPage-SIZE_OF_DB_HEADER];
        readFile.read(buffer, sizeOfPage-SIZE_OF_DB_HEADER);

        ReadPage((unsigned char*)buffer);
        delete[] buffer;

    } 


    readFile.close();

    return 0;
}