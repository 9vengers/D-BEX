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

#define FILE_TYPE_DB 201
#define FILE_TYPE_EXCEL 202
#define FILE_TYPE_JSON 203

#define FIELD_TYPE_NULL 301
#define FIELD_TYPE_INT 302
#define FIELD_TYPE_REAL 303
#define FIELD_TYPE_TEXT 304
#define FIEDL_TYPE_BLOB 305

#define REFERENCE_RULE_RESTRICT 401
#define REFERENCE_RULE_CASCADE 402
#define REFERENCE_RULE_NOACTION 403
#define REFERENCE_RULE_SETNULL 404
#define REFERENCE_RULE_SETDEFAULT 405

struct ForeignKey{
    std::string field;
    std::string table;

    // 401: RESTRICT  402: CASCADE  403:NO ACTION  404: SET NULL  405: SET DEFAULT
    int onDelete;
    int onUpdate;
};
struct Field{
    // 301: NULL   302: INT    303: REAL   304: TEXT   305: BLOB
    int type;
    int typeLength;
    bool nullable;
    std::string defaultData;    // save it as a string (regardless of integer/character)
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
    std::string name;       // table name
    std::string createSql;  // create sentence - Sql
    int numberOfFields;     // count a number of fields
    Field* fields;          // array of fields
};



class FileContainer{
private:
    const unsigned char filetype;
    const std::string srcpath;
    std::ifstream readFile;
    std::ofstream writeFile;

    int FileOpen()
    {
        // Open the target file
        readFile.open(srcpath, std::ios::binary);
        if(readFile.is_open())
        {
            std::cout << "Created ";
            switch (filetype)
            {
                case FILE_TYPE_DB:
                std::cout << "DB";
                break;
                case FILE_TYPE_EXCEL:
                std::cout << "Excel";
                break;
                case FILE_TYPE_JSON:
                std::cout << "JSON";
                break;
            }
            std::cout << " container : " << srcpath << std::endl;
        }
        else
        {
            std::cout << "[error] file is not opened" << std::endl;
        }
        return 0;
    }

public:
    FileContainer(const unsigned char type, std::string path) :filetype(type), srcpath(path)
    {

    }
    ~FileContainer()
    {
        if(readFile.is_open()) readFile.close();
    }

    int Load()
    {
        switch(filetype)
        {
            case FILE_TYPE_DB:
                if(srcpath.substr(srcpath.find_last_of(".") + 1) == "db")
                    FileOpen();
                else
                    std::cout << "[error] Invalid file type" << std::endl;
            break;
            case FILE_TYPE_EXCEL:
                std::cout << "excel" << std::endl;
            break;
            case FILE_TYPE_JSON:
                if(srcpath.substr(srcpath.find_last_of(".") + 1) == "json")
                    FileOpen();
                else
                    std::cout << "[error] Invalid file type" << std::endl;
            break;
            default:
                std::cout << "[error] Invalid file type" << std::endl;
        }
        return 0;
    }

    int Read(char *buffer, std::streamsize size)
    {
        if(readFile.is_open())
            readFile.read(buffer, size);
        return 0;
    }

    int seekg()
    {
        readFile.seekg(0, std::ios::beg);

        return 0;
    }

};



class DBConverter{
private:
    int childPage;
    int numberOfTables;

    struct SqliteInfo{
        std::string fileName;
        int pageSize, changeCounter, inheaderSize, pageOfFreelist, numberOfFreelist, schemaCookie,
        schemaFormat, defaultCacheSize, vacuumRootPage, textEncoding, userVersion, incrementalVacuum,
        applicationID, versionValid, sqliteVersion;
        char writeVersion, readVersion, reservedSpace, maxPayload, minPayload, leafPayload;

        // PRAGMA is comming soon..
        // Auto Vacuum mode(auto optimize db size), FSYNC, Journal mode, Locking mode ...
        // ...

        SqliteInfo(std::string inputfile){
            fileName = inputfile;
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
    };
    SqliteInfo* sqliteInfo;

public:
    DBConverter(std::string inputfile)
    {
        sqliteInfo = new SqliteInfo(inputfile);
    }
    ~DBConverter()
    {
        delete sqliteInfo;
    }
    int ReadDB(FileContainer &db)
    {
        char *buffer = 0;
        unsigned int sizeOfPage, numberOfPages;

        // Database Header 100 Bytes
        buffer = new char [SIZE_OF_DB_HEADER];
        db.Read(buffer, SIZE_OF_DB_HEADER);
        if (ReadDBheader((unsigned char*)buffer, &sizeOfPage, &numberOfPages))
        {
            printf("Page size : %d, Number of pages : %d\n", sizeOfPage, numberOfPages);
            delete[] buffer;
            db.seekg();
            // Read Pages
            buffer = new char[sizeOfPage];
            for(int count = 1 ; count <= 3; count++ )
            {
                std::cout << "------------------Page" << count << "-----------------" << std::endl;
                db.Read(buffer, sizeOfPage);
                ReadPage((unsigned char*)buffer, count);
            }
        }
        else
        {
            delete[] buffer;
            return 1;
        }
        delete[] buffer;
        return 0;
    }
    int MakeJSON(FileContainer &file)
    {
        return 0;
    }
    int ReadJSON(FileContainer &file)
    {
        return 0;
    }
    int MakeDB(FileContainer &file)
    {
        return 0;
    }

private:
    int ReadDBheader(unsigned char *buffer, unsigned int *sizeOfPage, unsigned int *numberOfPages);
    int ReadPage(unsigned char *buffer, int pageNumber);
    int InteriorTable(unsigned char *buffer, int pageNumber);
    int LeafTable(unsigned char *buffer, int pageNumber);
    int InteriorIndex(unsigned char *buffer, int pageNumber);
    int LeafIndex(unsigned char *buffer, int pageNumber);

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
    unsigned short GetDataSize(long long value);
    long long ByteStream(unsigned char *buffer, unsigned short sizeOfBytes);
    long long BitPattern(unsigned char *buffer, unsigned char *getSize);
};

int DBConverter::ReadDBheader(unsigned char *buffer, unsigned int *sizeOfPage, unsigned int *numberOfPages)
{
    // temporary Function.
    // Only read the size of a page and the number of pages in the database.
    // It will store variety variables TBD.
    char signature[16] = "SQLite format 3";
    if ( strncmp((const char *)buffer, signature, 16) == 0)
    {
        // correct file
        *sizeOfPage = buffer[16] * pow(2,8) + buffer[17];   // 16-17 offset in DB Header
        *numberOfPages = buffer[28] * pow(2,24) + buffer[29] * pow(2,16) + buffer[30] * pow(2,8) + buffer[31]; // 28-31 offset in DB Header

        return 1;
    }
    else
    {
        // wrong file
        std::cout << "This is not SQLite Format!!!" << std::endl;
        return 0;
    }
};
long long DBConverter::BitPattern(unsigned char *buffer, unsigned char *getSize)
{
    long long result = 0;
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
};
unsigned short DBConverter::GetDataSize(long long value)
{
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
};
long long DBConverter::ByteStream(unsigned char *buffer, unsigned short sizeOfBytes)
{
    // Get the data of bytestream as much as you want
    long long result = 0;
    int count = 0;

    for (count = 0; count < sizeOfBytes - 1; count++)
    {
        result += buffer[count];
        result <<= 8;
    }
    result += buffer[sizeOfBytes - 1];

    return result;
};
int DBConverter::ReadPage(unsigned char *buffer, int pageNumber)
{
    int offset = 0;

    // Only First Page
    if (pageNumber == 1) offset = SIZE_OF_DB_HEADER;


    // -----------------------------------------------
    // Page header
    // -----------------------------------------------
    // PageFlag           1Byte
    // Internal Table   : 0x05
    // Leaf     Table   : 0x0D
    // -----------------------------------------------
    switch(buffer[offset]){
        case 0x02:
            std::cout << "Page type : Internal page of B-tree index" << std::endl;
            InteriorIndex(buffer, pageNumber);
            break;

        case 0x05:
            std::cout << "Page type : Internal page of B-tree table" << std::endl;
            InteriorTable(buffer, pageNumber);
            break;

        case 0x0A:
            std::cout << "Page type : Leaf page of B-tree index" << std::endl;
            LeafIndex(buffer, pageNumber);
            break;

        case 0x0D:
            std::cout << "Page type : Leaf page of B-tree table" << std::endl;
            LeafTable(buffer, pageNumber);
            break;

        // exception, error
        default:
            return 0;
    }
    return 0;
};
int DBConverter::InteriorTable(unsigned char *buffer, int pageNumber)
{
    int offset = 0;
    if (pageNumber == 1)  offset = SIZE_OF_DB_HEADER;
    int freeblockOffset = 0;
    int numberOfCells = 0;
    int firstCellOffset = 0;
    int fragmentedFreeBytes = 0;
    int rightMostChildPage = 0;

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
    rightMostChildPage = ByteStream(buffer + offset, 4);
    offset += 4;

    // --------------------
    // Console Output
    // --------------------
    std::cout << "Number of Cells : " << numberOfCells << std::endl;
    std::cout << "Fragmented Free Bytes : " << fragmentedFreeBytes << std::endl;
    std::cout << "The right-most child page : " << rightMostChildPage << std::endl;
    std::cout << std::showbase << std::uppercase << std::hex;
    std::cout << "First of Freeblock Offset : ";
    if (!freeblockOffset) std::cout << "None" << std::endl;
    else std::cout << 1024 * (pageNumber - 1) + freeblockOffset << std::endl;
    std::cout << "First of Cellblock Offset : " << 1024*(pageNumber-1) + firstCellOffset << std::dec << std::endl;



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
        std::cout << "Cell Offset[" << count << "] : " << std::hex << 1024*(pageNumber-1) + cellOffset[count] << std::dec << std::endl;
    }
    std::cout << std::noshowbase << std::nouppercase << std::dec;
    



    // -----------------------------------------------
    // Cell Contents
    // -----------------------------------------------
    long long childPage = 0;
    unsigned char varintSize = 0;
    long long content = 0;

    for (int count = 0; count < numberOfCells ; count++)
    {
        offset = cellOffset[count];
        childPage = ByteStream(buffer + offset, 4); // left child page number  : 4 Bytes
        offset += 4;
        content = BitPattern(buffer + offset, &varintSize);
        std::cout << "Cell Content[" << count << "] : rowid : "  << content << ", size : " << (int)varintSize << ", child page : " << childPage << std::endl;
    }

    delete[] cellOffset;

    return 0;

};
int DBConverter::LeafTable(unsigned char *buffer, int pageNumber)
{
    int offset = 0;
    if (pageNumber == 1)  offset = SIZE_OF_DB_HEADER;
    int freeblockOffset = 0;
    int numberOfCells = 0;
    int firstCellOffset = 0;
    int fragmentedFreeBytes = 0;
    int rightMostChildPage = 0;

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

    // --------------------
    // Console Output
    // --------------------
    std::cout << "Number of Cells : " << numberOfCells << std::endl;
    std::cout << "Fragmented Free Bytes : " << fragmentedFreeBytes << std::endl;
    std::cout << std::showbase << std::uppercase << std::hex;
    std::cout << "First of Freeblock Offset : " << std::hex << 1024*(pageNumber-1) + freeblockOffset << std::endl;
    std::cout << "First of Cellblock Offset : " << 1024*(pageNumber-1) + firstCellOffset << std::dec << std::endl;



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
        std::cout << "Cell Offset[" << count << "] : " << std::hex << 1024*(pageNumber-1) + cellOffset[count] << std::dec << std::endl;
    }
    std::cout << std::noshowbase << std::nouppercase << std::dec;
    



    // -----------------------------------------------
    // Cell Contents
    // ------------------

    unsigned char varintSize = 0;
    long long content = 0;
    long long data = 0;

    struct LeafCell
    {
        int cellHeaderSize = 0;
        int recordSize = 0;
        int rowId = 0;

        int dataHeaderSize = 0;
        int lengthOfDataHeaderSize = 0;
        int numberOfFields = 0;
    };


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


    delete[] cellOffset;
    return 0;

};
int DBConverter::InteriorIndex(unsigned char *buffer, int pageNumber)
{
    return 0;
};
int DBConverter::LeafIndex(unsigned char *buffer, int pageNumber)
{
    return 0;
};



/*class ExcelConverter{

};*/


int DBtoExcel(std::string srcpath)
{
    // DB
    DBConverter dbConverter(srcpath);
    FileContainer srcFile(FILE_TYPE_DB, srcpath);
    srcFile.Load();
    dbConverter.ReadDB(srcFile);

    //FileContainer jsonFile(FILE_TYPE_JSON, srcpath);
    //dbConverter.MakeJSON(&jsonFile);



    // Excel

    return 0;
}

int main ()
{
    //std::string path;
    //std::cout << "input file path: ";   // D-BEX/chinook.db
    //std::cin >> path;
    DBtoExcel("D-BEX/chinook.db");
    //ExceltoDB();

    //DBtoDB();

    return 0;
}