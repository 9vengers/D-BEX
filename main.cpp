#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <cmath>

#define SIZE_OF_DB_HEADER 100
#define SIZE_OF_INTERIOR_PAGE_HEADER 12
#define SIZE_OF_LEAF_PAGE_HEADER 8

#define DATA_TYPE_NULL 101
#define DATA_TYPE_SIGNED_INT8 102
#define DATA_TYPE_SIGNED_INT16 103
#define DATA_TYPE_SIGNED_INT24 104
#define DATA_TYPE_SIGNED_INT32 105
#define DATA_TYPE_SIGNED_INT48 106
#define DATA_TYPE_SIGNED_INT64 107
#define DATA_TYPE_FLOAT64 108
#define DATA_TYPE_ZERO 109
#define DATA_TYPE_ONE 110
#define DATA_TYPE_RESERVED 111
#define DATA_TYPE_BLOB 112
#define DATA_TYPE_TEXT 113

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

__int64 BitPattern(unsigned char *buffer, unsigned char *getSize)
{
    __int64 result = 0;
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

    *getSize = sizeOfBytes;

    return result;
}
/*
int GetVarintSize(unsigned char *buffer)
{
    int count = 0;
    int sizeOfBytes = 1;
    while (count < 8)
    {
        if(buffer[count] < 128) break;
        sizeOfBytes++;
        count++;
    }
    return sizeOfBytes;
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
*/
unsigned short GetDataSize(__int64 value){

    char type;
    switch (value)
    {
        case 0:
            type = DATA_TYPE_NULL;
            return 0;
            break;
        case 1:
            type = DATA_TYPE_SIGNED_INT8;
            return 1;
            break;
        case 2:
            type = DATA_TYPE_SIGNED_INT16;
            return 2;
            break;
        case 3:
            type = DATA_TYPE_SIGNED_INT24;
            return 3;
            break;
        case 4:
            type = DATA_TYPE_SIGNED_INT32;
            return 4;
            break;
        case 5:
            type = DATA_TYPE_SIGNED_INT48;
            return 6;
            break;
        case 6:
            type = DATA_TYPE_SIGNED_INT64;
            return 8;
            break;
        case 7:
            type = DATA_TYPE_FLOAT64;
            return 8;
            break;
        case 8:
            type = DATA_TYPE_ZERO;
            return 0;
            break;
        case 9:
            type = DATA_TYPE_ONE;
            return 0;
            break;
        case 10:
            type = DATA_TYPE_RESERVED;
            return 0;
            break;
        case 11:
            type = DATA_TYPE_RESERVED;
            return 0;
            break;
        default:
            if((value >= 12) && (value % 2) == 0)      // value is even
            {
                type = DATA_TYPE_BLOB;
                return (value - 12) / 2;
            }
            else if((value >= 13) && (value % 2) == 1) // value is odd
            {
                type = DATA_TYPE_TEXT;
                return (value - 13) / 2;
            }
            break;
    }
    return 0;
}

// Get the data of bytestream as much as you want
__int64 ByteStream(unsigned char *buffer, unsigned short sizeOfBytes)
{
    __int64 result = 0;
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
    std::cout << "First of Freeblock Offset : " << std::hex << 1024*(firstPageFlag-1) + freeblockOffset << std::endl;
    std::cout << "First of Cellblock Offset : " << 1024*(firstPageFlag-1) + firstCellOffset << std::dec << std::endl;






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
    /*
    unsigned int *cellContents = new unsigned int[numberOfCells];
    unsigned long *cellVarInt = new unsigned long[numberOfCells];

    if(pageHeaderSize == 12)
    {
        for (int count = 0; count < numberOfCells ; count++){
            *(cellContents + count) = ByteStream(&buffer[cellOffset[count]] ,4);
            *(cellVarInt + count) = ByteStream(&buffer[cellOffset[count] + 4] , BitPatternSize(&buffer[cellOffset[count]]));
            std::cout<< "Cell Contents[" << count << "] : " << std::hex << cellContents[count] << cellVarInt[count] << std::dec << std::endl;        
        }
    }
    else if(pageHeaderSize == 8 )
    {
        int lengthOfRecord = 0;
        int rowID = 0;
        int lengthOfDataHeader = 0;
        int sizeOfField[lengthOfDataHeader];
        int dataOfField[lengthOfDataHeader];


        for (int count = 0; count < numberOfCells ; count++)
        {
            offset = cellOffset[count];
            lengthOfRecord = BitPatternSize(buffer[offset]);
            offset += BitPatternSize(buffer[offset]);
            rowID = BitPattern(buffer[offset]);
            offset += BitPatternSize(buffer[offset]);
            lengthOfDataHeader = BitPattern(buffer[offset]);
            offset += BitPatternSize(buffer[offset]);

            for(int i = 1; i <= lengthOfDataHeader; i++)
            {
                sizeOfField[i] BitPattern(buffer[offset]);
                offset += BitPatternSize(buffer[offset]);
            }

            
        }
    }
    
    delete[] cellContents;
    delete[] cellVarInt;

    delete[] cellOffset;
    */
    __int64 childPage = 0;
    unsigned char varintSize = 0;
    __int64 content = 0;
    __int64 data = 0;

    struct LeafCell
    {
        int cellHeaderSize = 0;
        int recordSize = 0;
        int rowId = 0;

        int dataHeaderSize = 0;
        int lengthOfDataHeaderSize = 0;
        int numberOfFields = 0;
    };

    if (pageHeaderSize == SIZE_OF_INTERIOR_PAGE_HEADER)
    {
        for (int count = 0; count < numberOfCells ; count++)
        {
            offset = cellOffset[count];
            childPage = ByteStream(buffer + offset, 4); // left child page number  : 4 Bytes
            offset += 4;
            content = BitPattern(buffer + offset, &varintSize);
            std::cout << "Cell Content[" << count << "] : rowid : "  << content << ", size : " << varintSize << ", child page : " << childPage << std::endl;
        }
    }
    else if(pageHeaderSize == SIZE_OF_LEAF_PAGE_HEADER)
    {
        struct LeafCell cell[numberOfCells];
        int bytesCount, fieldBytesCount;
        for (int count = 0; count < numberOfCells ; count++)
        {
            bytesCount = 0;
            fieldBytesCount = 0;
            offset = cellOffset[count];
            
            //
            // Cell Header
            //
            cell[count].recordSize = BitPattern(buffer + offset, &varintSize);
            offset += varintSize;
            cell[count].cellHeaderSize += varintSize;

            cell[count].rowId = BitPattern(buffer + offset, &varintSize);
            offset += varintSize;
            cell[count].cellHeaderSize += varintSize;

            cell[count].dataHeaderSize = BitPattern(buffer + offset, &varintSize);
            cell[count].lengthOfDataHeaderSize = varintSize;

            std::cout << "Cell Content[" << count << "] : rowid : " << cell[count].rowId << std::endl;
            //
            // Data Header
            // Repeat 'Size of Field' datas until the end of the Data Header
            while (cell[count].lengthOfDataHeaderSize + bytesCount < cell[count].dataHeaderSize)
            {
                // PART A - Size of Field
                content = BitPattern(buffer + offset + cell[count].lengthOfDataHeaderSize + bytesCount, &varintSize);
                bytesCount += varintSize;
                cell[count].numberOfFields++;

                content = GetDataSize(content);

                std::cout << "    field[" << cell[count].numberOfFields << "] : (" << content << "bytes) - ";
                // PART B - Data of Field
                data = ByteStream(buffer + offset + cell[count].dataHeaderSize + fieldBytesCount, content);
                std::cout << std::showbase << std::uppercase << std::hex;
                std::cout << data << std::endl;
                std::cout << std::noshowbase << std::nouppercase << std::dec;
                fieldBytesCount += content;
            }

            /*if (cell[count].lengthOfDataHeaderSize + bytesCount == cell[count].dataHeaderSize)
            {
                std::cout << "incorrect size calculate!!!" << std::endl;
            }*/
        }

    }
    else
    {
    }
    delete[] cellOffset;

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
            for(int count = 1 ; count <= 3; count++ )
            {
                std::cout << "------------------Page" << count << "-----------------" << std::endl;
                readFile.read(buffer, sizeOfPage);
                ReadPage((unsigned char*)buffer, count);
            }
        }
        delete[] buffer;
    } 
    readFile.close();

    return 0;
}