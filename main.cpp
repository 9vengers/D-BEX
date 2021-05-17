#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <cmath>

#define SIZE_OF_DB_HEADER 100
#define SIZE_OF_INTERIOR_PAGE_HEAGER 12
#define SIZE_OF_LEAF_PAGE_HEAGER 8

struct ForeignKey{
    std::string field;
    std::string table;

    // 1: RESTRICT  2: CASCADE  3:NO ACTION  4: SET NULL  5: SET DEFAULT
    int onDelete;
    int onUpdate;
};
struct Field{
    int type;
    int typeLength;
    bool nullable;
    std::string defaultData;
    int collation;
    bool autoIncrement;


    bool primaryKey;
    bool unique;
    std::string check;

    ForeignKey foriegnKey;
};
struct Constraint{
    std::string primaryKey;
    bool autoIncrement;

    std::string check;
    std::string defaultData;

    ForeignKey foriegnKey;
};
struct Table{
    std::string name;
    std::string createSql;
    int numberOfFields;
    Field *fields;
    void addField(){
        fields = new Field[numberOfFields];
        return;
    }



};
class SqliteInfo{
public:
    std::string fileName;
    int pageSize, changeCounter, inheaderSize, pageOfFreelist, numberOfFreelist, schemaCookie,
    schemaFormat, defaultCacheSize, vacuumRootPage, textEncoding, userVersion, incrementalVacuum,
    applicationID, versionValid, sqliteVersion, numberOfTables;
    char writeVersion, readVersion, reservedSpace, maxPayload, minPayload, leafPayload;

    // PRAGMA 추후 고려
    // Auto Vacuum mode(auto optimize db size), FSYNC, Journal mode, Locking mode ...
    // ...

    SqliteInfo(std::string file){
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


// temporary Function.
// Only read the size of a page and the number of pages in the database.
// It will store variety variables TBD.
int ReadDBheader(unsigned char *buffer, unsigned int *sizeOfPage, unsigned int *numberOfPages)
{
    char signature[16] = "SQLite format 3";
    if ( strncmp((const char *)buffer, signature, 16) == 0)
    {
        // 정상 파일
        *sizeOfPage = buffer[16] * pow(2,8) + buffer[17];   // 16-17 offset in DB Header
        *numberOfPages = buffer[28] * pow(2,24) + buffer[29] * pow(2,16) + buffer[30] * pow(2,8) + buffer[31]; // 28-31 offset in DB Header

        return 1;
    }
    else
    {
        // 비정상 파일
        std::cout << "This is not SQLite Format!!!" << std::endl;
        return 0;
    }
}

int BitPattern(unsigned char *buffer)
{
    unsigned int result = 0;
    int count = 0;
    int sizeOfBytes = 1;

    // get a byte_size
    while(count < 8)
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
    if (sizeOfBytes == 9)   result <<= 1;
    result += buffer[sizeOfBytes - 1];

    return result;
}

int GetVarintSize(unsigned char *buffer)
{
    int count = 0;
    int sizeOfBytes = 1;
    while (count < 8)
    {
        if(buffer[count] < 128) break;
        sizeOfBytes++;
    }
}
int BitPatternSize(unsigned char *buffer)
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
    return sizeOfBytes;
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
void ReadPage(unsigned char *buffer, int firstPageFlag)
{
    // ---------------------
    // Initialize
    // ---------------------
    int pageHeaderSize = 0;
    int offset = 0;
    int freeblockOffset = 0;
    int numberOfCells = 0;
    int firstCellOffset = 0;
    int fragmentedFreeBytes = 0;
    int rightMostChildPage = 0;

    // -----------------------------------------------
    // Only First Page
    // -----------------------------------------------
    if (firstPageFlag == 1) offset = SIZE_OF_DB_HEADER;

    // -----------------------------------------------
    // Page header
    // -----------------------------------------------
    //
    // PageFlag                             1Byte
    // Internal Table   : 0x05
    // Leaf     Table   : 0x0D
    switch(buffer[offset]){
        case 0x02:
            std::cout << "Page type : Internal page of B-tree index" << std::endl;
            return;
            break;

        case 0x05:
            pageHeaderSize = 12;
            std::cout << "Page type : Internal page of B-tree table" << std::endl;
            break;

        case 0x0A:
            std::cout << "Page type : Leaf page of B-tree index" << std::endl;
            return;
            break;

        case 0x0D:
            pageHeaderSize = 8;
            std::cout << "Page type : Leaf page of B-tree table" << std::endl;
            break;

        // exception, error
        default:
            return;
            break;
    }
    offset += 1;
    // Offset of freeblock                  2Bytes
    freeblockOffset = ByteStream(buffer + offset, 2);
    offset += 2;
    // Number of cells                      2Bytes
    numberOfCells = ByteStream(buffer + offset, 2);
    offset += 2;
    // Offset of the first Cell             2Bytes
    firstCellOffset = ByteStream(buffer + offset, 2);
    offset += 2;
    // Size of fragmented Free bytes        1Byte
    fragmentedFreeBytes = ByteStream(buffer + offset, 1);
    offset += 1;
    // The number of right-most child page  4Bytes
    // (Only Internal Page)

    if(pageHeaderSize == 12)
    {
        rightMostChildPage = ByteStream(buffer + offset, 4);
        offset += 4;
    }

    // --------------------
    // Console Output
    // --------------------
    std::cout << "Number of Cells : " << numberOfCells << std::endl;
    std::cout << "Fragmented Free Bytes : " << fragmentedFreeBytes << std::endl;
    if(pageHeaderSize == 12)
        std::cout << "The right-most child page : " << rightMostChildPage << std::endl;

    std::cout << std::showbase << std::uppercase << std::hex;
    std::cout << "First of Freeblock Offset : " << std::hex << freeblockOffset << std::endl;
    std::cout << "First of Cellblock Offset : " << firstCellOffset << std::dec << std::endl;






    // -----------------------------------------------
    // Cell Offsets
    // -----------------------------------------------
    unsigned short *cellOffset = new unsigned short [numberOfCells];
    for(int count = 0; count < numberOfCells; count++)
    {
        *(cellOffset + count) = ByteStream(buffer + offset, 2);
        offset += 2;
        // --------------------
        // Console Output
        // --------------------
        std::cout << "Cell Offset[" << count << "] : " << std::hex << 1024*(firstPageFlag-1) + cellOffset[count] << std::dec << std::endl;
    }
    std::cout << std::noshowbase << std::nouppercase << std::dec;
    





    // -----------------------------------------------
    // Cell Contents
    // ------------------
    unsigned char *cellContents = new unsigned char[numberOfCells];
    unsigned char *cellVarInt = new unsigned char[numberOfCells];
    if (*buffer == 0x53){
        for (int count = 0; count < numberOfCells ; count++){
            *(cellContents + count) = ByteStream(&buffer[cellOffset[count]] ,4);
            *(cellVarInt + count) = ByteStream(&buffer[cellOffset[count]+ 4] , BitPatternSize(&buffer[cellOffset[count]]));
            std::cout<< "Cell Contents[" << count << "] : " << std::hex << cellContents[count] << cellVarInt[count] << std::dec << std::endl;        
        }
        
    }else if(pageHeaderSize == 12){
        for (int count = 0; count < numberOfCells ; count++){
            *(cellContents + count) = ByteStream(&buffer[cellOffset[count]] ,4);
            *(cellVarInt + count) = ByteStream(&buffer[cellOffset[count] + 4] , BitPatternSize(&buffer[cellOffset[count]]));
            std::cout<< "Cell Contents[" << count << "] : " << std::hex << (int)cellContents[count] << (int)cellVarInt[count] << std::dec << std::endl;        
        }
    }else if(pageHeaderSize == 8 ){
        int lengthOfRecord = 0;
        int rowID = 0;
        int lengthOfDataHeader = 0;
        int sizeOfField[lengthOfDataHeader];
        int dataOfField[lengthOfDataHeader];


        for (int count = 0; count < numberOfCells ; count++){
            offset = cellOffset[count];
            lengthOfRecord = BitPatternSize(buffer[offset]);
            offset += BitPatternSize(buffer[offset]);
            rowID = BitPattern(buffer[offset]);
            offset += BitPatternSize(buffer[offset]);
            lengthOfDataHeader = BitPattern(buffer[offset]);
            offset += BitPatternSize(buffer[offset]);
            for [int i = 1; i <= lengthOfDataHeader; i++]{
                sizeOfField[i] BitPattern(buffer[offset]);
                offset += BitPatternSize(buffer[offset]);
            }

            
        }
    }
    
    delete[] cellContents;
    delete[] cellVarInt;

    delete[] cellOffset;

    /*if (pageHeaderSize == 12) // interior page
    {
        for (int count = 0; count < numberOfCells ; count++)
        {
            offset = cellOffset[count];
            childPage = ByteStream(buffer + offset, 4); // left child page number  : 4 Bytes
            offset += 4;
            varintSize = GetVarintSize(buffer + offset);
            content = BitPattern(buffer + offset);
            std::cout << "Cell Content[" << count << "] : "  << content << ", size : " << varintSize << ", child page : " << childPage << std::endl;
        }
    }
    else if(pageHeaderSize == 8) // leaf page
    {
        
    }


    delete[] cellOffset;*/

};


int main ()
{
    std::ifstream readFile;
    std::ofstream writeFile;

    // Open the target file
    readFile.open("D-BEX/chinook.db", std::ios::binary);


    if(readFile.is_open())
    {
        char *buffer = 0;
        unsigned int sizeOfPage, numberOfPages;

        // Database Header 100 Bytes
        buffer = new char [SIZE_OF_DB_HEADER];
        readFile.read(buffer, SIZE_OF_DB_HEADER);
        if (ReadDBheader((unsigned char*)buffer, &sizeOfPage, &numberOfPages))
        {
            printf("Page size : %d, Number of pages : %d\n", sizeOfPage, numberOfPages);
            delete[] buffer;

            readFile.seekg(0, std::ios::beg);

            // Read Pages
            buffer = new char[sizeOfPage];
            for(int count = 1 ; count <= 10; count++ )
            {
                std::cout << "------------Page" << count << "-----------" << std::endl;
                readFile.read(buffer, sizeOfPage);
                ReadPage((unsigned char*)buffer, count);
            }
        }
        delete[] buffer;
    } 
    readFile.close();

    return 0;
}