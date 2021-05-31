#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <cmath>
#include <algorithm>

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
#define FIELD_TYPE_BLOB 305

#define REFERENCE_RULE_RESTRICT 401
#define REFERENCE_RULE_CASCADE 402
#define REFERENCE_RULE_NOACTION 403
#define REFERENCE_RULE_SETNULL 404
#define REFERENCE_RULE_SETDEFAULT 405

class LinkedList{
    struct Node{
        void *data;
        Node *next;
    };
    Node head;
    Node *tail;
    Node *current;

public:
    LinkedList()
    {
        head = {NULL, NULL};
        tail = &head;
        current = NULL;
    }
    ~LinkedList()
    {
        deleteNode();
    }

    int addNode(void* data)
    {
        Node *node = new Node;
        node->data = data;
        node->next = NULL;
        tail->next = node;
        tail = node;

        return 0;
    }

    int deleteNode()
    {
        while(head.next != NULL)
        {
            current = head.next;
            head.next = current->next;
            delete current;
        }
        tail = &head;
        current = NULL;

        return 0;
    }

    void *getNodeData()
    {
        Node *result;
        if (current == NULL)
            current = head.next;
        result = current;
        if (current != NULL)
        {
            current = current->next;
            return result->data;
        }
        else
            return result;
    }

    void resetCurrent()
    {
        current = NULL;
        return;
    }
};

struct ForeignKey{
    std::string field;
    std::string referenceField;
    std::string referenceTable;

    // 401: RESTRICT  402: CASCADE  403:NO ACTION  404: SET NULL  405: SET DEFAULT
    int onDelete;
    int onUpdate;
};
struct Field{
    std::string name;
    // 301: NULL   302: INT    303: REAL   304: TEXT   305: BLOB
    int type;
    int typeLength;
    std::string typeString;

    bool nullable;
    std::string defaultData;    // save it as a string (regardless of integer/character)
    int collation;
    std::string collationString;
    bool autoIncrement;

    bool primaryKey;
    bool unique;
    std::string check;

    ForeignKey foreignKey;
};
struct Constraint{
    std::string name;
    std::string primaryKey;
    bool autoIncrement;

    std::string unique;
    std::string check;
    std::string defaultData;

    ForeignKey foreignKey;
};
struct Table{
    std::string name;
    long long rowid;
    int rootPage;
    std::string createSql;

    int numberOfFields;
    LinkedList fieldList;
    int numberOfConstraints;
    LinkedList constraintList;
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

    int seekg(int pageNumber, int size)
    {
        readFile.seekg(size * (pageNumber - 1));
        return 0;
    }

    std::string GetName()
    {
        return srcpath;
    }

};

class SqlParser
{;
    char *sql;
    char *head;
    char *content;

    char *field;
    char *constraint;

    LinkedList fieldSentence;
    LinkedList constraintSentence;

    int numberOfFields, numberOfConstraints;

    char *GetHead(char *input)
    {
        int size = strlen(input) - strlen(strchr(input, '('));
        char *parsed = new char[size + 1];
        memcpy(parsed, input, size);
        parsed[size] = '\0';
        return parsed;
    }

    char *GetContent(char *input)
    {
        input = strpbrk(input, "(") + 1;
        int size = strlen(input) - strlen(strrchr(input, ')'));
        char *parsed = new char[size + 1];
        memcpy(parsed, input, size);
        parsed[size] = '\0';
        return parsed;
    }

    std::string Trimming(std::string input)
    {
	    input.erase(std::remove(input.begin(), input.end(), 0x0d), input.end());    // '\r'
        input.erase(std::remove(input.begin(), input.end(), 0x0a), input.end());    // '\n'
        input.erase(std::remove(input.begin(), input.end(), 0x09), input.end());    // tab

        return input;
    }


public:

    void InputSql(std::string input)
    {
        input = Trimming(input);
        char *sql = new char[input.size() + 1];
        std::copy(input.begin(), input.end(), sql);
        sql[input.size()] = '\0';

        head = GetHead(sql);
        content = GetContent(sql);
    }
    
    void DeleteSql()
    {
        numberOfFields = 0;
        numberOfConstraints = 0;
        fieldSentence.resetCurrent();
        fieldSentence.deleteNode();
        constraintSentence.resetCurrent();
        constraintSentence.deleteNode();
        delete[] head, content, sql;
    }


    std::string GetTableName() 
    {
        std::string tableName;
        char *pointer = strchr(head, '\"');
        if (pointer == NULL)
        {
            pointer = strstr(head, "sqlite_");
            if (pointer == NULL)
                std::cout << "Sqlite system table" << std::endl;
            else
                tableName = pointer;
        }
        else
        {
            pointer = pointer + 1;
            int size = strlen(pointer) - strlen(strrchr(pointer, '\"'));
            char *buffer = new char[size + 1];
            memcpy(buffer, pointer, size);
            buffer[size] = '\0';
            tableName = buffer;
            delete[] buffer;
        }
        return tableName;
    };

    int GetNumberOfField()
    {
        return numberOfFields;
    }

    int GetNumberOfConstraint()
    {
        return numberOfConstraints;
    }


    bool IsPrimaryKey()
    {
        std::string buffer = field;
        if (buffer.find("PRIMARY KEY") != std::string::npos)
        {
            return true;
        }
        else return false;
    }

    bool IsNullable()
    {
        std::string buffer = field;
        if (buffer.find("NOT NULL") != std::string::npos)
        {
            return false;
        }
        else return true;
    }

    bool IsAutoincrement()
    {
        std::string buffer = field;
        if (buffer.find("AUTOINCREMENT") != std::string::npos)
        {
            return true;
        }
        else return false;
    }

    bool IsUnique()
    {
        std::string buffer = field;
        if (buffer.find("UNIQUE") != std::string::npos)
        {
            return true;
        }
        else return false;
    }


    void Parse()
    {
        numberOfFields = 0;
        numberOfConstraints = 0;

        char *start = content;  // 검사할 문장 처음 위치
        char *end = NULL;       // 마지막 위치 ',' 기준
        int size = 0;           // 문장의 길이(크기)
        int searchComma = 0;    // 찾은 ',' 까지의 길이
        int searchChar1 = 0;    // 찾은 문자의 길이1
        int searchChar2 = 0;    // 찾은 문자의 길이2

        do
        {
            // 문장 앞부분은 항상 공백 검사
            while (*start == ' ')
                start++;

            if (*start == '[')  // field
            {
                numberOfFields++;

                end = strchr(start, ',');

                // () 소괄호를 찾으면 건너 뛰는 방법
                searchComma = strcspn(start, ",");
                searchChar1 = strcspn(start, "(");
                searchChar2 = strcspn(start, ")");

                if (searchChar2 > searchComma)
                {
                    while (searchChar1 < searchComma && searchChar2 > searchComma)
                    {
                        end = strchr(end + 1, ',');
                        searchComma += strcspn(start + searchComma + 1, ",") + 1;
                    }
                    searchChar1 += strcspn(start + searchChar1 + 1, "(") + 1;
                    searchChar2 += strcspn(start + searchChar2 + 1, ")") + 1;
                }

                size = strlen(start);
                if (end != NULL) size -= strlen(end);
                field = new char[size + 1];
                memcpy(field, start, size);
                field[size] = '\0';

                if (end != NULL) start += size +1;
                fieldSentence.addNode(field);

            }
            else
            {
                if (strncmp(start, "CONSTRAINT", strlen("CONSTRAINT")) == 0)
                {
                    //std::cout << "CONSTRAINT" << std::endl;
                    numberOfConstraints++;
                }
                else if (strncmp(start, "PRIMARY", strlen("PRIMARY")) == 0)
                {
                    //std::cout << "PRIMARY" << std::endl;
                    numberOfConstraints++;
                }
                else if (strncmp(start, "FOREIGN", strlen("FOREIGN")) == 0)
                {
                    //std::cout << "FOREIGN" << std::endl;
                    numberOfConstraints++;
                }
                else if (strncmp(start, "UNIQUE", strlen("UNIQUE")) == 0)
                {
                    //std::cout << "UNIQUE" << std::endl;
                    numberOfConstraints++;
                }
                else if (strncmp(start, "CHECK", strlen("CHECK")) == 0)
                {
                    //std::cout << "CHECK" << std::endl;
                    numberOfConstraints++;
                }
                else
                {
                    //std::cout << "[error] sentence parsing" << std::endl;
                }
                
                end = strchr(start, ',');
                // () 소괄호를 찾으면 건너 뛰는 방법
                searchComma = strcspn(start, ",");
                searchChar1 = strcspn(start, "(");
                searchChar2 = strcspn(start, ")");

                if (searchChar2 > searchComma)
                {
                    while (searchChar1 < searchComma && searchChar2 > searchComma)
                    {
                        end = strchr(end + 1, ',');
                        searchComma += strcspn(start + searchComma + 1, ",") + 1;
                    }
                    searchChar1 += strcspn(start + searchChar1 + 1, "(") + 1;
                    searchChar2 += strcspn(start + searchChar2 + 1, ")") + 1;
                }

                size = strlen(start);
                if (end != NULL) size -= strlen(end);
                constraint = new char[size + 1];
                memcpy(constraint, start, size);
                constraint[size] = '\0';

                if (end != NULL) start += size +1;
                constraintSentence.addNode(constraint);
            }

        } while (end != NULL);
    }

    void GetField()
    {
        field = (char *)fieldSentence.getNodeData();
    }
    
    std::string GetFieldName()
    {
        char *fieldName;
        fieldName = strtok(field, " ");
        field += strlen(fieldName) + 1;
        return fieldName;
    }

    std::string GetDataType()
    {
        char *dataType;
        dataType = strtok(field, " ");
        field += strlen(dataType) + 1;
        return dataType;
    }

    std::string GetCollate()
    {
        char *data = strstr(field, "COLLATE");
        if (data != NULL)
        {
            //strtok(data, " ");
            //data = strtok(NULL, " ");

            return "Field - Collation";
        }
        return "none";
    }

    std::string GetCheck()
    {
        char *data = strstr(field, "CHECK");
        if (data != NULL)
        {
            //strtok(data, " ");
            //data = strtok(NULL, " ");

            return "Field - Check";
        }
        return "none";
    }

    std::string GetDefault()
    {
        char *data = strstr(field, "DEFAULT");
        if (data != NULL)
        {
            //strtok(data, " ");
            //data = strtok(NULL, " ");

            return "Field - Default";
        }
        return "none";
    }

    std::string GetForeignKey()
    {
        int size = strlen(field) + 1;
        char *copy = new char[size];
        memcpy(copy, field, size);

        char *data = strstr(copy, "FOREIGN KEY");
        if (data != NULL)
        {
            //strtok(data, " ");
            //strtok(NULL, " ");
            //data = strtok(NULL, " ");
            delete[] copy;
            return "Field - Foreign key";
        }
        delete[] copy;
        return data;
    }


    void GetConstraint()
    {
        constraint = (char *)constraintSentence.getNodeData();
    }

    std::string GetConstraintName()
    {
        if ( strstr(constraint, "CONSTRAINT") != NULL)
        {
            //strtok(data, " ");
            //data = strtok(NULL, " ");

            strtok(constraint, " ");
            constraint += strlen(constraint) + 1;
            char *constraintName = strtok(NULL, " ");
            constraint += strlen(constraint) + 1;
            return constraintName;
        }
        return "none";
    }

    std::string GetPrimaryKey()
    {
        if (strstr(constraint, "PRIMARY KEY") != NULL)
        {
            return "    ";

        }
        return "none";
    }

    std::string GetUnique()
    {
        if (strstr(constraint, "UNIQUE") != NULL)
        {
            return "    ";

        }
        return "none";
    }

    std::string GetCheck2()
    {
        if (strstr(constraint, "CHECK") != NULL)
        {
            return "    ";

        }
        return "none";
    }

    std::string GetForeignKey2()
    {
        char *src = strstr(constraint, "FOREIGN KEY");
        char *dest;
        std::string fk;
        if (src != NULL)
        {
            src = strchr(src, '(') + 1;
            char *end = strchr(src, ')');
            int size = strlen(src) - strlen(end);
            dest = new char[size + 1];
            memcpy(dest, src, size);
            dest[size] = '\0';
            fk = dest;
            delete[] dest;
            return fk;
        }
        return "none";
    }

    std::string GetReferenceTable2()
    {
        char *src = strstr(constraint, "REFERENCES");
        char *dest;
        std::string refer;
        if (src != NULL)
        {
            src = strchr(src, '\"') + 1;
            char *end = strchr(src, '\"');
            int size = strlen(src) - strlen(end);
            dest = new char[size + 1];
            memcpy(dest, src, size);
            dest[size] = '\0';
            refer = dest;
            delete[] dest;
            return refer;
        }
        return "none";
    }

    std::string GetReferenceField2()
    {
        char *src = strstr(constraint, "REFERENCES");
        char *dest;
        std::string refer;
        if (src != NULL)
        {
            src = strchr(src, '(') + 1;
            char *end = strchr(src, ')');
            int size = strlen(src) - strlen(end);
            dest = new char[size + 1];
            memcpy(dest, src, size);
            dest[size] = '\0';
            refer = dest;
            delete[] dest;
            return refer;
        }
        return "none";
    }

    int DeleteRule()
    {
        char dest[23];
        int rule = 0;
        char *src = strstr(constraint, "ON DELETE");
        if (src != NULL)
        {
            memcpy(dest, src, 23);
            dest[22] = '\0';
            if (strstr(dest, "ON RESTRICT") != NULL)
                rule = REFERENCE_RULE_RESTRICT;
            if (strstr(dest, "ON CASCADE") != NULL)
                rule = REFERENCE_RULE_CASCADE;
            if (strstr(dest, "NO ACTION") != NULL)
                rule = REFERENCE_RULE_NOACTION;
            if (strstr(dest, "SET NULL") != NULL)
                rule = REFERENCE_RULE_SETNULL;
            if (strstr(dest, "SET DEFAULT") != NULL)
                rule = REFERENCE_RULE_SETDEFAULT;
        }
        return rule;
    }

    int UpdateRule()
    {
        char dest[23];
        int rule = 0;
        char *src = strstr(constraint, "ON UPDATE");
        if (src != NULL)
        {
            memcpy(dest, src, 23);
            dest[22] = '\0';
            if (strstr(dest, "ON RESTRICT") != NULL)
                rule = REFERENCE_RULE_RESTRICT;
            if (strstr(dest, "ON CASCADE") != NULL)
                rule = REFERENCE_RULE_CASCADE;
            if (strstr(dest, "NO ACTION") != NULL)
                rule = REFERENCE_RULE_NOACTION;
            if (strstr(dest, "SET NULL") != NULL)
                rule = REFERENCE_RULE_SETNULL;
            if (strstr(dest, "SET DEFAULT") != NULL)
                rule = REFERENCE_RULE_SETDEFAULT;
        }
        return rule;
    }
};

class DBConverter{
private:

    unsigned int sizeOfPage, numberOfPages, currentRootPage;
    FileContainer *srcFile;
    FileContainer *targetFile;

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

    unsigned int numberOfTables;
    Table *table;
    LinkedList tableList;

public:
    DBConverter(FileContainer &db)  // db->excel  or  db->db
    {
        sqliteInfo = new SqliteInfo(db.GetName());
        srcFile = &db;
        targetFile = NULL;

        sizeOfPage = 0;
        numberOfPages = 0;
        currentRootPage = 1;
        numberOfTables = 0;
        table = NULL;
    }
    DBConverter()   // only excel->db
    {

    }
    ~DBConverter()
    {
        // table output
        // Console output
        // test
        /*for (int i = 0; i < numberOfTables; i++)
        {
            std::cout << "[Table " << i + 1 << "]" << std::endl;
            table = (Table *)tableList.getNodeData();
            std::cout << "name: " << table->name << std::endl;
            std::cout << "rowid: " << table->rowid << std::endl;
            std::cout << "root page: " << table->rootPage << std::endl;
            std::cout << "sql: " << table->createSql << std::endl << std::endl;
        }*/

        delete sqliteInfo;

        // Delete tables, fields, constraints
        Field *field;
        Constraint *constraint;
        tableList.resetCurrent();
        table = (Table *)tableList.getNodeData();
        for (int i = 0; i < numberOfTables; i++)
        {
            table->fieldList.resetCurrent();
            for (int j = 0; j < table->numberOfFields; j++)
            {
                field = (Field *)table->fieldList.getNodeData();
                delete field;
            }
            table->constraintList.resetCurrent();
            for (int j = 0; j < table->numberOfConstraints; j++)
            {
                constraint = (Constraint *)table->constraintList.getNodeData();
                delete constraint;
            }
            delete table;
            table = (Table *)tableList.getNodeData();
        }
        // delete table List
        tableList.~LinkedList();
    }
    int ReadDB()
    {
        unsigned int nextPage = 0;
        // Database Header 100 Bytes
        unsigned char *buffer = new unsigned char [SIZE_OF_DB_HEADER];
        srcFile->Read((char *)buffer, SIZE_OF_DB_HEADER);
        if (ReadDBheader(buffer))
        {
            std::cout << "----------------[schema]-----------------" << std::endl;
            printf(" One Page size : %d, Total number of pages : %d\n", sizeOfPage, numberOfPages);
            delete[] buffer;

            // Read and create a schema
            ReadPage(currentRootPage);
            printf(" Total number of Tables: %d\n", numberOfTables);

            // Build the table schema (SQL Parser)
            SqlParsing();

            // Read Pages and input data
            tableList.resetCurrent();
            /*for (int i = 0; i < numberOfTables; i++)
            {
                //std::cout << "-----------------[table" << i + 1 << "]-----------------" << std::endl;
                table = (Table *)tableList.getNodeData();
                currentRootPage = table->rootPage;
                ReadPage(currentRootPage);
            }*/
        }
        else
        {
            delete[] buffer;
            return 1;
        }
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
    int ReadDBheader(unsigned char* buffer);
    int ReadPage(unsigned int pageNumber);
    int InteriorTable(unsigned char *buffer, int pageNumber);
    int LeafTable(unsigned char *buffer, int pageNumber);
    int InteriorIndex(unsigned char *buffer, int pageNumber);
    int LeafIndex(unsigned char *buffer, int pageNumber);

    void SqlParsing();

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
    unsigned short GetDataSize(long long value, unsigned short *getType);
    long long ByteStream(unsigned char *buffer, unsigned short sizeOfBytes);
    long long BitPattern(unsigned char *buffer, unsigned char *getSize);
};
int DBConverter::ReadDBheader(unsigned char *buffer)
{
    // temporary Function.
    // Only read the size of a page and the number of pages in the database.
    // It will store variety variables TBD.
    char signature[16] = "SQLite format 3";
    if ( strncmp((const char *)buffer, signature, 16) == 0)
    {
        // correct file
        sizeOfPage = buffer[16] * pow(2,8) + buffer[17];   // 16-17 offset in DB Header
        numberOfPages = buffer[28] * pow(2,24) + buffer[29] * pow(2,16) + buffer[30] * pow(2,8) + buffer[31]; // 28-31 offset in DB Header

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
unsigned short DBConverter::GetDataSize(long long value, unsigned short *getType)
{
    switch (value)
    {
        case 0:
            *getType = DATA_TYPE_NULL;
            return 0;
            break;
        case 1:
            *getType = DATA_TYPE_SIGNED_INT8;
            return 1;
            break;
        case 2:
            *getType = DATA_TYPE_SIGNED_INT16;
            return 2;
            break;
        case 3:
            *getType = DATA_TYPE_SIGNED_INT24;
            return 3;
            break;
        case 4:
            *getType = DATA_TYPE_SIGNED_INT32;
            return 4;
            break;
        case 5:
            *getType = DATA_TYPE_SIGNED_INT48;
            return 6;
            break;
        case 6:
            *getType = DATA_TYPE_SIGNED_INT64;
            return 8;
            break;
        case 7:
            *getType = DATA_TYPE_FLOAT64;
            return 8;
            break;
        case 8:
            *getType = DATA_TYPE_ZERO;
            return 0;
            break;
        case 9:
            *getType = DATA_TYPE_ONE;
            return 0;
            break;
        case 10:
            *getType = DATA_TYPE_RESERVED;
            return 0;
            break;
        case 11:
            *getType = DATA_TYPE_RESERVED;
            return 0;
            break;
        default:
            if((value >= 12) && (value % 2) == 0)      // value is even
            {
                *getType = DATA_TYPE_BLOB;
                return (value - 12) / 2;
            }
            else if((value >= 13) && (value % 2) == 1) // value is odd
            {
                *getType = DATA_TYPE_TEXT;
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
int DBConverter::ReadPage(unsigned int pageNumber)
{
    int offset = 0;
    unsigned char *buffer = 0;
    
    buffer = new unsigned char[sizeOfPage];
    srcFile->seekg(pageNumber, sizeOfPage);
    srcFile->Read((char *)buffer, sizeOfPage);
    //std::cout << "Page" << pageNumber << ": ";

    // Only First Page
    if ( pageNumber == 1) offset = SIZE_OF_DB_HEADER;

    // -----------------------------------------------
    // PageFlag           1Byte
    // Internal Table   : 0x05
    // Leaf     Table   : 0x0D
    // -----------------------------------------------
    switch(buffer[offset]){
        case 0x02:
            //std::cout << "Internal index" << std::endl;
            InteriorIndex(buffer, pageNumber);
            break;

        case 0x05:
            //std::cout << "Internal table" << std::endl;
            InteriorTable(buffer, pageNumber);
            break;

        case 0x0A:
            //std::cout << "Leaf index" << std::endl;
            LeafIndex(buffer, pageNumber);
            break;

        case 0x0D:
            //std::cout << "Leaf table" << std::endl;
            LeafTable(buffer, pageNumber);
            break;

        // exception, error
        default:
            return 0;
    }
    delete[] buffer;
    return 0;
};
int DBConverter::InteriorTable(unsigned char *buffer, int pageNumber)
{
    int offset = 0;
    if (pageNumber == 1)  offset = SIZE_OF_DB_HEADER;

    // -----------------------------------------------
    // Page header
    // -----------------------------------------------
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


    // -----------------------------------------------
    // Cell Offsets
    // -----------------------------------------------
    unsigned short *cellOffset = new unsigned short [numberOfCells];
    for(int count = 0; count < numberOfCells; count++)
    {
        *(cellOffset + count) = ByteStream(buffer + offset, 2);
        offset += 2;
    }
    
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
        ReadPage(childPage);
    }

    delete[] cellOffset;
    ReadPage(rightMostChildPage);

    return 0;

};
int DBConverter::LeafTable(unsigned char *buffer, int pageNumber)
{
    int offset = 0;
    if (pageNumber == 1)  offset = SIZE_OF_DB_HEADER;

    // -----------------------------------------------
    // Page header
    // -----------------------------------------------
    int freeblockOffset = 0;
    int firstCellOffset = 0;
    int numberOfCells = 0;
    int fragmentedFreeBytes = 0;
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


    // -----------------------------------------------
    // Cell Offsets
    // -----------------------------------------------
    unsigned short *cellOffset = new unsigned short [numberOfCells];
    for(int count = 0; count < numberOfCells; count++)
    {
        *(cellOffset + count) = ByteStream(buffer + offset, 2);
        offset += 2;
    }

    // -----------------------------------------------
    // Cell Contents
    // -----------------------------------------------

    struct LeafCell
    {
        int cellHeaderSize = 0;
        int recordSize = 0;
        int rowId = 0;

        int dataHeaderSize = 0;
        int lengthOfDataHeaderSize = 0;
        int numberOfFields = 0;
    };
    LeafCell *cell = new LeafCell[numberOfCells];
    int bytesCount, fieldBytesCount;

    unsigned char varintSize = 0;
    long long content = 0;
    long long dataSize = 0;
    unsigned short dataType = 0;
    long long datai = 0;
    char *datac = 0;
    double dataf = 0;

    int sqliteObjType = 0;  // temporary variable.  (1: table, 2: index)

    for (int count = 0; count < numberOfCells ; count++)
    {
        bytesCount = 0;
        fieldBytesCount = 0;
        offset = cellOffset[count];

        sqliteObjType = 0;  // temporary vaiable.
        
        //---------------------
        // Cell Header
        //---------------------
        cell[count].recordSize = BitPattern(buffer + offset, &varintSize);
        offset += varintSize;
        cell[count].cellHeaderSize += varintSize;

        cell[count].rowId = BitPattern(buffer + offset, &varintSize);
        offset += varintSize;
        cell[count].cellHeaderSize += varintSize;

        cell[count].dataHeaderSize = BitPattern(buffer + offset, &varintSize);
        cell[count].lengthOfDataHeaderSize = varintSize;

        //
        // Data Header
        // Repeat 'Size of Field' datas until the end of the Data Header
        while (cell[count].lengthOfDataHeaderSize + bytesCount < cell[count].dataHeaderSize)
        {
            // PART A - Size of Field
            content = BitPattern(buffer + offset + cell[count].lengthOfDataHeaderSize + bytesCount, &varintSize);
            bytesCount += varintSize;
            cell[count].numberOfFields++;

            dataSize = GetDataSize(content, &dataType);

            // PART B - Data of Field
            //
            // current root page가 1일 경우 : ( sqlite_schema  B-tree Page )
            // 우선 Table인 데이터만 저장.
            // Index 등의 경우,   Table 객체보다 큰 상위 객체를 만들어야 함.
            // 그리고, 객체 타입을  table, index, ..로 지정해야함.
            if(currentRootPage == 1)    // Sqlite Schema
            {
                if (dataType == DATA_TYPE_TEXT)
                {
                    datac = new char[dataSize + 1];
                    for (int i = 0; i < dataSize; i++)
                    {
                        datac[i] = *(buffer + offset + cell[count].dataHeaderSize + fieldBytesCount);
                        fieldBytesCount++;
                    }
                    datac[dataSize] = '\0';
                }

                switch (cell[count].numberOfFields)
                {
                    case 1:
                        if (strcmp("table", datac) == 0)
                        {
                            sqliteObjType = 1;
                            numberOfTables++;
                            table = new Table;
                            table->rowid = cell[count].rowId;
                            tableList.addNode(table);
                        }
                        else if (strcmp("index", datac) == 0)
                        {
                            sqliteObjType = 2;
                        }
                    break;

                    case 2:
                        if (sqliteObjType == 1)
                        {
                            table->name = datac;
                        }
                    break;

                    case 3:

                    break;

                    case 4:
                        datai = ByteStream(buffer + offset + cell[count].dataHeaderSize + fieldBytesCount, dataSize);
                        if (sqliteObjType == 1)
                        {
                            table->rootPage = datai;
                        }
                        
                        fieldBytesCount += dataSize;
                    break;

                    case 5:
                        if (sqliteObjType == 1)
                        {
                            table->createSql = datac;
                        }
                    break;
                    default:
                        std::cout << "[error] Invalid sqlite schema" << std::endl;
                    break;
                }
                if (dataType == DATA_TYPE_TEXT)
                    delete[] datac;
            }
            else
            {
                switch (dataType)
                {
                    case DATA_TYPE_NULL:
                        //std::cout << "NULL" << std::endl;
                    break;
                    case DATA_TYPE_FLOAT64:
                        dataf = (double)ByteStream(buffer + offset + cell[count].dataHeaderSize + fieldBytesCount, dataSize);
                        fieldBytesCount += dataSize;
                    break;
                    case DATA_TYPE_ZERO:
                        //std::cout << "0" << std::endl;
                    break;
                    case DATA_TYPE_ONE:
                        //std::cout << "1" << std::endl;
                    break;
                    case DATA_TYPE_RESERVED:
                        //std::cout << "[error] Invalid data type" << std::endl;
                    break;
                    case DATA_TYPE_BLOB:
                        datai = ByteStream(buffer + offset + cell[count].dataHeaderSize + fieldBytesCount, dataSize);
                        //std::cout << "BLOB type" << std::endl;
                        fieldBytesCount += dataSize;
                    break;
                    case DATA_TYPE_TEXT:
                        datac = new char[dataSize + 1];
                        for (int i = 0; i < dataSize; i++)
                        {
                            datac[i] = *(buffer + offset + cell[count].dataHeaderSize + fieldBytesCount);
                            fieldBytesCount++;
                        }
                        datac[dataSize] = '\0';
                        
                        //std::cout << datac << std::endl;
                        delete[] datac;
                    break;
                    default:
                        datai = ByteStream(buffer + offset + cell[count].dataHeaderSize + fieldBytesCount, dataSize);
                        /*std::cout << datai << " (";
                        std::cout << std::showbase << std::uppercase << std::hex;
                        std::cout << datai << ")" << std::endl;
                        std::cout << std::noshowbase << std::nouppercase << std::dec;*/
                        fieldBytesCount += dataSize;
                    break;
                }
            }
        }
        table = NULL;
    }
    delete[] cell;
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
void DBConverter::SqlParsing()
{
    Field *field;
    Constraint *constraint;
    tableList.resetCurrent();
    SqlParser parser;
    for (int i = 0; i < numberOfTables; i++)
    {
        table = (Table *)tableList.getNodeData();
        
        // (0) Input Sql ---> Trimming ---> Split head
        parser.InputSql(table->createSql);

        // (1) Table name
        //std::cout << "----Table[" << i + 1 << "]----" << std::endl;
        //std::cout << "Table name: " << parser.GetTableName() << std::endl;

        // (2) Parse Senetences - Fields & Constraints
        parser.Parse();

        // (3) Field datas
        table->numberOfFields = parser.GetNumberOfField();
        for (int j = 0; j < table->numberOfFields; j++)
        {
            field = new Field;
            parser.GetField();

            field->name = parser.GetFieldName();
            field->typeString = parser.GetDataType();
            
            field->primaryKey = parser.IsPrimaryKey();
            field->autoIncrement = parser.IsAutoincrement();
            field->nullable = parser.IsNullable();
            field->unique = parser.IsUnique();

            field->defaultData = parser.GetDefault();
            field->check = parser.GetCheck();
            field->collationString = parser.GetCollate();
            table->fieldList.addNode(field);
        }


        // (4) Constraints
        table->numberOfConstraints = parser.GetNumberOfConstraint();
        for (int j = 0; j < table->numberOfConstraints; j++)
        {
            constraint = new Constraint;
            parser.GetConstraint();

            constraint->name = parser.GetConstraintName();
            constraint->primaryKey = parser.GetPrimaryKey();
            constraint->unique = parser.GetUnique();
            constraint->check = parser.GetCheck2();
            constraint->foreignKey.field = parser.GetForeignKey2();
            constraint->foreignKey.referenceTable = parser.GetReferenceTable2();
            constraint->foreignKey.referenceField = parser.GetReferenceField2();
            constraint->foreignKey.onDelete = parser.DeleteRule();
            constraint->foreignKey.onUpdate = parser.UpdateRule();
            table->constraintList.addNode(constraint);
        }

        // (5) Exit parsing
        parser.DeleteSql();
    }

}

class ExcelConverter{

};

int DBtoExcel(std::string srcpath)
{
    // DB
    FileContainer srcFile(FILE_TYPE_DB, srcpath);
    srcFile.Load();
    DBConverter dbConverter(srcFile);
    dbConverter.ReadDB();

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