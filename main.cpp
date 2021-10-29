#pragma once
#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <cmath>
#include <algorithm>
#include "parson.h"
#include "excelWriter.hpp"

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

#define FILE_TYPE_DB_READ 201
#define FILE_TYPE_EXCEL_READ 202
#define FILE_TYPE_JSON_READ 203
#define FILE_TYPE_DB_WRITE  204
#define FILE_TYPE_EXCEL_WRITE 205
#define FILE_TYPE_JSON_WRITE 206

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

    int numberOfDatas;
    LinkedList dataList;
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
        switch (filetype)
        {
            case FILE_TYPE_DB_READ:
            readFile.open(srcpath, std::ios::binary);
            if (readFile.is_open())
            {
                std::cout << "Created DB container : " << srcpath << std::endl;
                return 0;
            }
            else
            {
                return -1;
            }
            break;

            case FILE_TYPE_EXCEL_READ:
            readFile.open(srcpath);
            if (readFile.is_open())
            {
                std::cout << "Created Excel container : " << srcpath << std::endl;
                return 0;
            }
            else
            {
                return -1;
            }
            break;

            case FILE_TYPE_JSON_READ:
            readFile.open(srcpath);
            if (readFile.is_open())
            {
                std::cout << "Created JSON container : " << srcpath << std::endl;
                return 0;
            }
            else
            {
                return -1;
            }
            break;

            case FILE_TYPE_DB_WRITE:
            writeFile.open(srcpath, std::ios::binary);
            if (writeFile.is_open())
            {
                std::cout << "Created DB container : " << srcpath << std::endl;
                return 0;
            }
            else
            {
                return -1;
            }
            break;

            case FILE_TYPE_EXCEL_WRITE:
            writeFile.open(srcpath);
            if (writeFile.is_open())
            {
                std::cout << "Created Excel container : " << srcpath << std::endl;
                return 0;
            }
            else
            {
                return -1;
            }
            break;

            case FILE_TYPE_JSON_WRITE:
            writeFile.open(srcpath);
            if (writeFile.is_open())
            {
                std::cout << "Created JSON container : " << srcpath << std::endl;
                return 0;
            }
            else
            {
                return -1;
            }
            break;
            default:
            std::cout << "[error] Incorrect file type argument" << std::endl;
        }
        return -1;
    }

public:
    FileContainer(const unsigned char type, std::string path) :filetype(type), srcpath(path)
    {

    }
    ~FileContainer()
    {
        if(readFile.is_open()) readFile.close();
        if(writeFile.is_open()) writeFile.close();
    }

    int Load()
    {
        switch(filetype)
        {
            case FILE_TYPE_DB_READ:
                if (srcpath.substr(srcpath.find_last_of(".") + 1) == "db")
                {
                    if (FileOpen() == -1)
                    {
                        std::cout << "[error] The input file does not exist." << std::endl;
                        return -1;
                    }
                }
                else
                {
                    std::cout << "[error] The input file is not '*.db'" << std::endl;
                    return -1;
                }
            break;

            case FILE_TYPE_DB_WRITE:
                std::cout << "[error] FILE_TYPE_DB_WRITE" << std::endl;
                return -1;
            break;

            case FILE_TYPE_EXCEL_READ:
            case FILE_TYPE_EXCEL_WRITE:
                std::cout << "[error] FILE_TYPE_EXCEL" << std::endl;
                return -1;
            break;

            case FILE_TYPE_JSON_READ:
                    std::cout << "[error] FILE_TYPE_JSON_READ" << std::endl;
                    return -1;
            break;

            case FILE_TYPE_JSON_WRITE:
                if(srcpath.substr(srcpath.find_last_of(".") + 1) == "json")
                {
                    if (FileOpen() == -1)
                    {
                        std::cout << "[error] The json file does not exist." << std::endl;
                        return -1;
                    }
                }
                else
                {
                    std::cout << "[error] The JSON file is not named '*.json'" << std::endl;
                    return -1;
                }
            break;

            default:
                std::cout << "[error] Incorrect file type argument" << std::endl;
                return -1;
        }
        return 0;
    }

    int Read(char *buffer, std::streamsize size)
    {
        if(readFile.is_open())
            readFile.read(buffer, size);
        return 0;
    }

    int Write(std::string sentence, std::streamsize size)
    {
        if(writeFile.is_open())
            writeFile.write(sentence.c_str(), size);
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

    SqlParser()
    {
        sql = NULL;
        head = NULL;
        content = NULL;
        field = NULL;
        constraint = NULL;

        numberOfFields = 0;
        numberOfConstraints = 0;
    }
    ~SqlParser()
    {
        DeleteSql();
    }

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
        fieldSentence.resetCurrent();
        for (int i = 0; i < numberOfFields; i++)
        {
            field = (char *)fieldSentence.getNodeData();
            delete[] field;
        }
        fieldSentence.deleteNode();

        constraintSentence.resetCurrent();
        for (int i = 0; i < numberOfConstraints; i++)
        {
            constraint = (char *)constraintSentence.getNodeData(); 
            delete[] constraint;
        }
        constraintSentence.deleteNode();

        numberOfFields = 0;
        numberOfConstraints = 0;
        delete[] head, content, sql;
        head = NULL;
        content = NULL;
        sql = NULL;
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

            if (*start == '[')  // Field
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
            else    // Constraint
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
        fieldName = strtok_s(field, " ", &field);
        return fieldName;
    }

    std::string GetDataType()
    {
        char *dataType;
        dataType = strtok_s(field, " ", &field);
        return dataType;
    }

    std::string GetCollate()
    {
        char *data = strstr(field, "COLLATE");
        if (data != NULL)
        {
            return "miguhyun";
        }
        return "";
    }

    std::string GetCheck()
    {
        char *data = strstr(field, "CHECK");
        if (data != NULL)
        {
            return "miguhyun";
        }
        return "";
    }

    std::string GetDefault()
    {
        char *data = strstr(field, "DEFAULT");
        if (data != NULL)
        {
            //strtok(data, " ");
            //data = strtok(NULL, " ");

            return "miguhyun";
        }
        return "";
    }

    std::string GetForeignKey()
    {
        char *src = strstr(field, "FOREIGN KEY");
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
        return "";
    }


    void GetConstraint()
    {
        constraint = (char *)constraintSentence.getNodeData();
    }

    std::string GetConstraintName()
    {
        if ( strstr(constraint, "CONSTRAINT") != NULL)
        {
            strtok_s(constraint, " ", &constraint);
            char *constraintName = strtok_s(NULL, " ", &constraint);
            return constraintName;
        }
        return "";
    }

    std::string GetPrimaryKey()
    {
        if (strstr(constraint, "PRIMARY KEY") != NULL)
        {
            return "miguhyun";

        }
        return "";
    }

    std::string GetUnique()
    {
        if (strstr(constraint, "UNIQUE") != NULL)
        {
            return "miguhyun";

        }
        return "";
    }

    std::string GetCheck2()
    {
        if (strstr(constraint, "CHECK") != NULL)
        {
            return "miguhyun";

        }
        return "";
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
        return "";
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
        return "";
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
        return "";
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

    int GetNumberOfSystemField()
    {
        // ',' 개수 + 1
        char *a = content;
        while (*a != '\0')
        {
            if (*a == ',')
            {
                numberOfFields++;
            }
            a++;
        }
        numberOfFields++;

        return numberOfFields;
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
    /*DBConverter()   // only excel->db
    {

    }*/
    ~DBConverter()
    {
        delete sqliteInfo;

        // Delete tables, fields, constraints
        Field *field;
        Constraint *constraint;
        tableList.resetCurrent();
        table = (Table *)tableList.getNodeData();
        long long *datai = NULL;
        char *datac = NULL;
        double *dataf = NULL;
        bool *datab = NULL;

        for (unsigned int i = 0; i < numberOfTables; i++)
        {
            table->fieldList.resetCurrent();
            for (int j = 0; j < table->numberOfFields; j++)
            {
                field = (Field *)table->fieldList.getNodeData();
                const char *datatype = field->typeString.c_str();
                if (strstr(datatype, "NVARCHAR") != NULL)
                {
                    for (int k = 0; k < field->numberOfDatas; k++)
                    {
                        datac = (char *)field->dataList.getNodeData();
                        delete[] datac;
                    }
                }
                else if (strstr(datatype, "INTEGER") != NULL)
                {
                    for (int k = 0; k < field->numberOfDatas; k++)
                    {
                        datai = (long long *)field->dataList.getNodeData();
                        delete datai;
                    }
                }
                else if (strstr(datatype, "NUMERIC") != NULL)
                {
                    for (int k = 0; k < field->numberOfDatas; k++)
                    {
                        dataf = (double *)field->dataList.getNodeData();
                        delete dataf;
                    }
                }
                // DATETIME type?

                field->dataList.~LinkedList();
                delete field;
            }
            table->fieldList.~LinkedList();
            table->constraintList.resetCurrent();
            for (int j = 0; j < table->numberOfConstraints; j++)
            {
                constraint = (Constraint *)table->constraintList.getNodeData();
                delete constraint;
            }
            table->constraintList.~LinkedList();
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
            std::cout << "------------------[SCHEMA]-------------------" << std::endl;
            printf(" One Page size : %d, Total number of pages : %d\n", sizeOfPage, numberOfPages);
            delete[] buffer;

            // Read and create a schema
            if (ReadPage(currentRootPage) == -1)
            {
                return -1;
            }
            printf(" Total number of Tables: %d\n", numberOfTables);
            std::cout << "---------------------------------------------" << std::endl;

            // Build the table schema (SQL Parser)
            SqlParsing();
            //TestSchema();

            // Read Pages and input data
            tableList.resetCurrent();
            for (unsigned int i = 0; i < numberOfTables; i++)
            {
                table = (Table *)tableList.getNodeData();
                currentRootPage = table->rootPage;
                if (ReadPage(currentRootPage) == -1)
                {
                    return -1;
                }
            }
        }
        else
        {
            delete[] buffer;
            return -1;
        }
        return 1;
    }
    int ReadJSON(FileContainer &file)
    {
        return 0;
    }
    int MakeDB(FileContainer &file)
    {
        return 0;
    }
    int MakeJSON(FileContainer &json)
    {
        //---------------------------------------------
        // Use parson-master library in this Function.
        //---------------------------------------------
        JSON_Value *rootValue;
        JSON_Object *rootObject;
        JSON_Value* tableValue;
        JSON_Object* tableObject;
        JSON_Array* arrayPointer;

        rootValue = json_value_init_object();
        rootObject = json_value_get_object(rootValue);

        std::string getData;
        long long* datai;
        double* dataf;
        char* datac;
        Field* field;
        Constraint* constraint;


        tableList.resetCurrent();
        

        getData = strrchr(json.GetName().c_str(), '\\') + 1;
        getData = getData.substr(0, getData.find(".json", 0));
        json_object_set_string(rootObject, "name", getData.c_str());
        json_object_set_number(rootObject, "tableNum", numberOfTables);

        for (int i = 0; i < numberOfTables; i++)
        {
            table = (Table *)tableList.getNodeData();
            tableValue = json_value_init_object();
            tableObject = json_value_get_object(tableValue);
            getData = "table" + std::to_string(i + 1);
            json_object_set_value(rootObject, getData.c_str(), tableValue);

            json_object_set_string(tableObject, "tableName", table->name.c_str());
            json_object_set_number(tableObject, "columnNum", table->numberOfFields);

            // Records
            table->fieldList.resetCurrent();
            for (int j = 0; j < table->numberOfFields; j++)
            {
                field = (Field *)table->fieldList.getNodeData();
                getData = TransformNumber(j + 1);
                json_object_set_value(tableObject, getData.c_str(), json_value_init_array());
                arrayPointer = json_object_get_array(tableObject, getData.c_str());
                json_array_append_string(arrayPointer, field->name.c_str());
                field->dataList.resetCurrent();

                if (strstr(field->typeString.c_str(), "INTEGER") != NULL)
                {
                    for (int k = 0; k < field->numberOfDatas; k++)
                    {

                        datai = (long long*)field->dataList.getNodeData();
                        if (datai == NULL)
                            json_array_append_null(arrayPointer);
                        else json_array_append_number(arrayPointer, *datai);
                    }
                }
                else if (strstr(field->typeString.c_str(), "NVARCHAR") != NULL || strstr(field->typeString.c_str(), "DATETIME") != NULL)
                {
                    for (int k = 0; k < field->numberOfDatas; k++)
                    {
                        datac = (char*)field->dataList.getNodeData();
                        if (datac == NULL)
                            json_array_append_null(arrayPointer);
                        else json_array_append_string(arrayPointer, datac);
                    }
                }
                else if (strstr(field->typeString.c_str(), "NUMERIC") != NULL)
                {
                    for (int k = 0; k < field->numberOfDatas; k++)
                    {
                        dataf = (double*)field->dataList.getNodeData();
                        if (dataf == NULL)
                            json_array_append_null(arrayPointer);
                        else json_array_append_number(arrayPointer, *dataf);
                    }
                }
            }


            // Field Schemas
            table->fieldList.resetCurrent();
            for (int j = 0; j < table->numberOfFields; j++)
            {
                field = (Field*)table->fieldList.getNodeData();
                getData = TransformNumber(j + 1) + "Schema";
                json_object_set_value(tableObject, getData.c_str(), json_value_init_array());
                arrayPointer = json_object_get_array(tableObject, getData.c_str());
                json_array_append_string(arrayPointer, field->name.c_str());
                json_array_append_string(arrayPointer, field->typeString.c_str());
                json_array_append_boolean(arrayPointer, field->nullable);
                json_array_append_string(arrayPointer, field->defaultData.c_str());
                json_array_append_string(arrayPointer, field->collationString.c_str());
                json_array_append_boolean(arrayPointer, field->autoIncrement);
            }


            // Constraints
            table->constraintList.resetCurrent();
            json_object_set_number(tableObject, "ConstraintsNum", table->numberOfConstraints);
            for (int j = 0; j < table->numberOfConstraints; j++)
            {
                constraint = (Constraint*)table->constraintList.getNodeData();
                getData = "Constraint" + std::to_string(j + 1);
                json_object_set_value(tableObject, getData.c_str(), json_value_init_array());
                arrayPointer = json_object_get_array(tableObject, getData.c_str());
                json_array_append_string(arrayPointer, constraint->name.c_str());
                json_array_append_string(arrayPointer, constraint->primaryKey.c_str());
                json_array_append_string(arrayPointer, constraint->unique.c_str());
                json_array_append_string(arrayPointer, constraint->check.c_str());
                json_array_append_string(arrayPointer, constraint->check.c_str());
                json_array_append_string(arrayPointer, constraint->foreignKey.field.c_str());
                json_array_append_string(arrayPointer, constraint->foreignKey.referenceTable.c_str());
                json_array_append_string(arrayPointer, constraint->foreignKey.referenceField.c_str());
                switch (constraint->foreignKey.onDelete)
                {
                case REFERENCE_RULE_CASCADE:
                    getData = "CASCADE";
                    break;
                case REFERENCE_RULE_NOACTION:
                    getData = "NO ACTION";
                    break;
                case REFERENCE_RULE_RESTRICT:
                    getData = "RESTRICT";
                    break;
                case REFERENCE_RULE_SETDEFAULT:
                    getData = "SET DEFAULT";
                    break;
                case REFERENCE_RULE_SETNULL:
                    getData = "SET NULL";
                    break;
                default:
                    getData = "";
                }
                json_array_append_string(arrayPointer, getData.c_str());
                switch (constraint->foreignKey.onUpdate)
                {
                case REFERENCE_RULE_CASCADE:
                    getData = "CASCADE";
                    break;
                case REFERENCE_RULE_NOACTION:
                    getData = "NO ACTION";
                    break;
                case REFERENCE_RULE_RESTRICT:
                    getData = "RESTRICT";
                    break;
                case REFERENCE_RULE_SETDEFAULT:
                    getData = "SET DEFAULT";
                    break;
                case REFERENCE_RULE_SETNULL:
                    getData = "SET NULL";
                    break;
                default:
                    getData = "";
                }
                json_array_append_string(arrayPointer, getData.c_str());
            }
        }

        json_serialize_to_file_pretty(rootValue, json.GetName().c_str());
        json_value_free(rootValue);

        return 0;
    }
    /*int MakeJSON2(FileContainer& json)
    {
        std::string getData;
        targetFile = &json;

        // Initialize
        tableList.resetCurrent();

        // JSON File Start
        targetFile->Write("{\n", 2);

        // Name
        getData = "\t\"name\" : \"";
        targetFile->Write(getData, getData.size());
        getData = srcFile->GetName();
        targetFile->Write(getData, getData.size());
        targetFile->Write("\",\n", 3);

        // Number of tables
        getData = "\t\"tableNum\" : ";
        targetFile->Write(getData, getData.size());
        getData = std::to_string(numberOfTables);
        targetFile->Write(getData, getData.size());
        targetFile->Write(",\n", 2);

        // Table Datas
        for (unsigned int i = 0; i < numberOfTables; i++)
        {
            table = (Table*)tableList.getNodeData();
            getData = "\t\"table";
            targetFile->Write(getData, getData.size());
            getData = std::to_string(i + 1);
            targetFile->Write(getData, getData.size());
            getData = "\" : {\n";
            targetFile->Write(getData, getData.size());
            getData = "\t\t\"tableName\" : \"";
            targetFile->Write(getData, getData.size());
            getData = table->name;
            targetFile->Write(getData, getData.size());
            getData = "\",\n\t\t\"columnNum\" : ";
            targetFile->Write(getData, getData.size());
            getData = std::to_string(table->numberOfFields);
            targetFile->Write(getData, getData.size());

            table->fieldList.resetCurrent();
            Field* field;
            long long* datai;
            char* datac;
            double* dataf;
            //bool *datab;
            Constraint* constraint;

            for (int j = 0; j < table->numberOfFields; j++)
            {
                field = (Field*)table->fieldList.getNodeData();
                targetFile->Write(",\n\t\t\"", 5);
                getData = std::to_string(j + 1);
                targetFile->Write(getData, getData.size());
                targetFile->Write("\" : [\"", 6);
                getData = field->name;
                targetFile->Write(getData, getData.size());
                targetFile->Write("\"", 1);

                field->dataList.resetCurrent();
                if (strstr(field->typeString.c_str(), "INTEGER") != NULL || strstr(field->typeString.c_str(), "DATETIME") != NULL)
                {
                    for (int k = 0; k < field->numberOfDatas; k++)
                    {
                        targetFile->Write(", \"", 3);
                        datai = (long long*)field->dataList.getNodeData();
                        if (datai == NULL) getData = "";
                        else getData = std::to_string(*datai);
                        targetFile->Write(getData, getData.size());
                        targetFile->Write("\"", 1);
                    }
                }
                else if (strstr(field->typeString.c_str(), "NVARCHAR") != NULL)
                {
                    for (int k = 0; k < field->numberOfDatas; k++)
                    {
                        targetFile->Write(", \"", 3);
                        datac = (char*)field->dataList.getNodeData();
                        if (datac == NULL) getData = "";
                        else getData = datac;
                        targetFile->Write(getData, getData.size());
                        targetFile->Write("\"", 1);
                    }
                }
                else if (strstr(field->typeString.c_str(), "NUMERIC") != NULL)
                {
                    for (int k = 0; k < field->numberOfDatas; k++)
                    {
                        targetFile->Write(", \"", 3);
                        dataf = (double*)field->dataList.getNodeData();
                        if (dataf == NULL) getData = "";
                        else getData = std::to_string(*dataf);
                        targetFile->Write(getData, getData.size());
                        targetFile->Write("\"", 1);
                    }
                }

                targetFile->Write("]", 1);
            }

            // Schema
            table->fieldList.resetCurrent();
            for (int j = 0; j < table->numberOfFields; j++)
            {
                field = (Field*)table->fieldList.getNodeData();
                targetFile->Write(",\n\t\t\"", 5);
                getData = std::to_string(j + 1);
                targetFile->Write(getData, getData.size());
                targetFile->Write("Schema\" : [\"", 12);
                getData = field->name;
                targetFile->Write(getData, getData.size());
                targetFile->Write("\", \"", 4);
                getData = field->typeString;
                targetFile->Write(getData, getData.size());
                targetFile->Write("\", \"", 4);
                if (field->nullable) getData = "YES";
                else getData = "NO";
                targetFile->Write(getData, getData.size());
                targetFile->Write("\", \"", 4);
                getData = field->defaultData;
                targetFile->Write(getData, getData.size());
                targetFile->Write("\", \"", 4);
                getData = field->collationString;
                targetFile->Write(getData, getData.size());
                targetFile->Write("\", \"", 4);
                if (field->autoIncrement) getData = "YES";
                else getData = "NO";
                targetFile->Write(getData, getData.size());
                targetFile->Write("\"]", 2);

            }


            getData = ",\n\t\t\"ConstraintsNum\" : ";
            targetFile->Write(getData, getData.size());
            getData = std::to_string(table->numberOfConstraints);
            targetFile->Write(getData, getData.size());
            table->constraintList.resetCurrent();
            for (int j = 0; j < table->numberOfConstraints; j++)
            {
                constraint = (Constraint*)table->constraintList.getNodeData();
                getData = ",\n\t\t\"Constraint";
                targetFile->Write(getData, getData.size());
                getData = std::to_string(j + 1);
                targetFile->Write(getData, getData.size());
                targetFile->Write("\" : [\"", 6);
                getData = constraint->name;
                targetFile->Write(getData, getData.size());
                targetFile->Write("\", \"", 4);
                getData = constraint->primaryKey;
                targetFile->Write(getData, getData.size());
                targetFile->Write("\", \"", 4);
                getData = constraint->unique;
                targetFile->Write(getData, getData.size());
                targetFile->Write("\", \"", 4);
                getData = constraint->check;
                targetFile->Write(getData, getData.size());
                targetFile->Write("\", \"", 4);
                getData = constraint->check;
                targetFile->Write(getData, getData.size());
                targetFile->Write("\", \"", 4);
                getData = constraint->foreignKey.field;
                targetFile->Write(getData, getData.size());
                targetFile->Write("\", \"", 4);
                getData = constraint->foreignKey.referenceTable;
                targetFile->Write(getData, getData.size());
                targetFile->Write("\", \"", 4);
                getData = constraint->foreignKey.referenceField;
                targetFile->Write(getData, getData.size());
                targetFile->Write("\", \"", 4);
                switch (constraint->foreignKey.onDelete)
                {
                case REFERENCE_RULE_CASCADE:
                    getData = "CASCADE";
                    break;
                case REFERENCE_RULE_NOACTION:
                    getData = "NO ACTION";
                    break;
                case REFERENCE_RULE_RESTRICT:
                    getData = "RESTRICT";
                    break;
                case REFERENCE_RULE_SETDEFAULT:
                    getData = "SET DEFAULT";
                    break;
                case REFERENCE_RULE_SETNULL:
                    getData = "SET NULL";
                    break;
                default:
                    getData = "";
                }
                targetFile->Write(getData, getData.size());
                targetFile->Write("\", \"", 4);
                switch (constraint->foreignKey.onUpdate)
                {
                case REFERENCE_RULE_CASCADE:
                    getData = "CASCADE";
                    break;
                case REFERENCE_RULE_NOACTION:
                    getData = "NO ACTION";
                    break;
                case REFERENCE_RULE_RESTRICT:
                    getData = "RESTRICT";
                    break;
                case REFERENCE_RULE_SETDEFAULT:
                    getData = "SET DEFAULT";
                    break;
                case REFERENCE_RULE_SETNULL:
                    getData = "SET NULL";
                    break;
                default:
                    getData = "";
                }
                targetFile->Write(getData, getData.size());
                targetFile->Write("\"]", 2);
            }

            getData = "\n\t},\n";
            targetFile->Write(getData, getData.size());
        }

        targetFile->Write("\n}", 2);
        return 0;
    }*/


private:
    int ReadDBheader(unsigned char* buffer);
    int ReadPage(unsigned int pageNumber);
    int InteriorTable(unsigned char *buffer, int pageNumber);
    int LeafTable(unsigned char *buffer, int pageNumber);
    int InteriorIndex(unsigned char *buffer, int pageNumber);
    int LeafIndex(unsigned char *buffer, int pageNumber);

    void SqlParsing();
    void TestSchema();
    void TestRecord();

    std::string TransformNumber(int input);
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
        std::cout << "[error] This file is not SQLite Format!!!" << std::endl;
        return -1;
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
    if (*(buffer + offset) == 0x02)
    {
        InteriorIndex(buffer, pageNumber);
    }
    else if (*(buffer + offset) == 0x05)
    {
        InteriorTable(buffer, pageNumber);
    }
    else if (*(buffer + offset) == 0x0A)
    {
        LeafIndex(buffer, pageNumber);
    }
    else if (*(buffer + offset) == 0x0D)
    {
        LeafTable(buffer, pageNumber);
    }
    else
    {
        std::cout << "[error] Invalid page type(Index or Table)" << std::endl;
        return -1;
    }

    delete[] buffer;
    return 1;
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

    long long *datai = NULL;
    char *datac = NULL;
    double *dataf = NULL;
    bool *datab = NULL;

    int sqliteObjType = 0;  // temporary variable.  (1: table, 2: index)

    Field *field;

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
                        if (sqliteObjType == 1)
                        {
                            table->rootPage = ByteStream(buffer + offset + cell[count].dataHeaderSize + fieldBytesCount, dataSize);
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
                field = (Field *)table->fieldList.getNodeData();
                field->numberOfDatas++;
                switch (dataType)
                {
                    case DATA_TYPE_NULL:
                        //std::cout << "NULL" << std::endl;
                        if (strstr(field->typeString.c_str(), "INTEGER") != NULL)
                        {
                            datai = new long long;
                            *datai = field->numberOfDatas;
                            field->dataList.addNode(datai);
                        }
                        else
                            field->dataList.addNode(NULL);
                    break;
                    case DATA_TYPE_FLOAT64:
                        dataf = new double;
                        *dataf = (double)ByteStream(buffer + offset + cell[count].dataHeaderSize + fieldBytesCount, dataSize);
                        fieldBytesCount += dataSize;
                        //std::cout << dataf << std::endl;
                        field->dataList.addNode(dataf);
                    break;
                    case DATA_TYPE_ZERO:
                        datab = new bool;
                        *datab = 0;
                        //std::cout << "0" << std::endl;
                        field->dataList.addNode(datab);
                    break;
                    case DATA_TYPE_ONE:
                        datab = new bool;
                        *datab = 1;
                        //std::cout << "1" << std::endl;
                        field->dataList.addNode(datab);
                    break;
                    case DATA_TYPE_RESERVED:
                        std::cout << "[error] Invalid data type" << std::endl;
                    break;
                    case DATA_TYPE_BLOB:
                        datai = new long long;
                        *datai = ByteStream(buffer + offset + cell[count].dataHeaderSize + fieldBytesCount, dataSize);
                        std::cout << "BLOB type" << std::endl;
                        fieldBytesCount += dataSize;
                        field->dataList.addNode(datai);
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
                        field->dataList.addNode(datac);
                    break;
                    default:
                        datai = new long long;
                        *datai = ByteStream(buffer + offset + cell[count].dataHeaderSize + fieldBytesCount, dataSize);
                        /*std::cout << *datai << " (";
                        std::cout << std::showbase << std::uppercase << std::hex;
                        std::cout << *datai << ")" << std::endl;
                        std::cout << std::noshowbase << std::nouppercase << std::dec;*/
                        fieldBytesCount += dataSize;
                        //std::cout << *datai << std::endl;
                        field->dataList.addNode(datai);
                    break;
                }
            }
        }
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
        if (table->numberOfFields == 0) // consider this table system table
        {
            table->numberOfFields = parser.GetNumberOfSystemField();

            if (strstr(table->name.c_str(), "sqlite_sequence") != NULL) // consider this table "sqlite_sequence"
            {
                field = new Field;
                field->name = "name";
                field->typeString = "NVARCHAR";
                field->nullable = true;
                field->numberOfDatas = 0;
                table->fieldList.addNode(field);

                field = new Field;
                field->name = "seq";
                field->typeString = "INTEGER";
                field->nullable = true;
                field->numberOfDatas = 0;
                table->fieldList.addNode(field);
            }
            else    // consider this table "sqlite_statN"
            {
                field = new Field;
                field->name = "tbl";
                field->typeString = "NVARCHAR";
                field->nullable = true;
                field->numberOfDatas = 0;
                table->fieldList.addNode(field);

                field = new Field;
                field->name = "idx";
                field->typeString = "NVARCHAR";
                field->nullable = true;
                field->numberOfDatas = 0;
                table->fieldList.addNode(field);

                field = new Field;
                field->name = "stat";
                field->typeString = "NVARCHAR";
                field->nullable = true;
                field->numberOfDatas = 0;
                table->fieldList.addNode(field);
            }
        }
        else
        {
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
                field->foreignKey.field = parser.GetForeignKey();

                field->numberOfDatas = 0;
                table->fieldList.addNode(field);
            }
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
void DBConverter::TestSchema()
{
    // console output test

    Table *table;
    Field *field;
    Constraint *constraint;
    tableList.resetCurrent();
    for (int i = 0; i < numberOfTables; i++)
    {
        table = (Table *)tableList.getNodeData();
        std::cout << "-------------------------[table" << i + 1 << "] " << table->name << "---------------------------" << std::endl;
        std::cout << "Id(" << table->rowid << ") RootPage(" << table->rootPage << ")" << std::endl;
        std::cout << "SQL: " << table->createSql << std::endl;
        std::cout << "Number of fields: " << table->numberOfFields << " / Number of constraints: " << table->numberOfConstraints << std::endl;

        table->fieldList.resetCurrent();
        for (int j = 0; j < table->numberOfFields; j++)
        {
            field = (Field *)table->fieldList.getNodeData();
            std::cout << " Field[" << j + 1 << "] " << field->name << std::endl;
            std::cout << " Type: " << field->typeString << std::endl;
            //std::cout << " Constraint(field level): ";
            std::cout << " ";
            if (field->primaryKey == true) std::cout << " PK";
            if (field->autoIncrement == true) std::cout << " AI";
            if (field->nullable == true) ;
            else std::cout << " NOT NULL";
            if (field->foreignKey.field != "")
            {
                std::cout << "\n  FK " << field->foreignKey.field << ": " << field->foreignKey.referenceTable << "."<< field->foreignKey.referenceField;
                switch (field->foreignKey.onDelete)
                {
                    case REFERENCE_RULE_RESTRICT:
                        std::cout << " ON DELETE RESTRICT";
                    break;

                    case REFERENCE_RULE_CASCADE:
                        std::cout << " ON DELETE CASCADE";
                    break;

                    case REFERENCE_RULE_NOACTION:
                        std::cout << " ON DELETE NO ACTION";
                    break;

                    case REFERENCE_RULE_SETNULL:
                        std::cout << " ON DELETE SET NULL";
                    break;

                    case REFERENCE_RULE_SETDEFAULT:
                        std::cout << " ON DELETE SET DEFAULT";
                    break;
                }
                switch (field->foreignKey.onUpdate)
                {
                    case REFERENCE_RULE_RESTRICT:
                        std::cout << " ON UPDATE RESTRICT";
                    break;

                    case REFERENCE_RULE_CASCADE:
                        std::cout << " ON UPDATE CASCADE";
                    break;

                    case REFERENCE_RULE_NOACTION:
                        std::cout << " ON UPDATE NO ACTION";
                    break;

                    case REFERENCE_RULE_SETNULL:
                        std::cout << " ON UPDATE SET NULL";
                    break;

                    case REFERENCE_RULE_SETDEFAULT:
                        std::cout << " ON UPDATE SET DEFAULT";
                    break;
                }
            }
            std::cout << std::endl;
        }

        table->constraintList.resetCurrent();
        for (int j = 0; j < table->numberOfConstraints; j++)
        {
            constraint = (Constraint *)table->constraintList.getNodeData();
            std::cout << " Constraint[" << j + 1 << "] ";
            if (constraint->name == "none") std::cout << std::endl;
            else std::cout << constraint->name << std::endl;
            if (constraint->primaryKey != "")
            {
                std::cout << " PK";
                if (constraint->autoIncrement == true) std::cout << "(AI)";
                std::cout << ": " << constraint->primaryKey << std::endl;
            }
            if (constraint->foreignKey.field != "")
            {
                std::cout << "  FK " << constraint->foreignKey.field << ": " << constraint->foreignKey.referenceTable << "."<< constraint->foreignKey.referenceField;
                switch (constraint->foreignKey.onDelete)
                {
                    case REFERENCE_RULE_RESTRICT:
                        std::cout << " ON DELETE RESTRICT";
                    break;

                    case REFERENCE_RULE_CASCADE:
                        std::cout << " ON DELETE CASCADE";
                    break;

                    case REFERENCE_RULE_NOACTION:
                        std::cout << " ON DELETE NO ACTION";
                    break;

                    case REFERENCE_RULE_SETNULL:
                        std::cout << " ON DELETE SET NULL";
                    break;

                    case REFERENCE_RULE_SETDEFAULT:
                        std::cout << " ON DELETE SET DEFAULT";
                    break;
                }
                switch (constraint->foreignKey.onUpdate)
                {
                    case REFERENCE_RULE_RESTRICT:
                        std::cout << " ON UPDATE RESTRICT";
                    break;

                    case REFERENCE_RULE_CASCADE:
                        std::cout << " ON UPDATE CASCADE";
                    break;

                    case REFERENCE_RULE_NOACTION:
                        std::cout << " ON UPDATE NO ACTION";
                    break;

                    case REFERENCE_RULE_SETNULL:
                        std::cout << " ON UPDATE SET NULL";
                    break;

                    case REFERENCE_RULE_SETDEFAULT:
                        std::cout << " ON UPDATE SET DEFAULT";
                    break;
                }
            }
            std::cout << std::endl;
        }
    }
}
void DBConverter::TestRecord()
{
    // console output test

    /*Table *table = NULL;
    Field *field = NULL;
    long long *datai = NULL;
    char *datac = NULL;
    double *dataf = NULL;
    bool *datab = NULL;
    tableList.resetCurrent();
    for (int i = 0; i < 1; i++)
    {
        table = (Table *)tableList.getNodeData();
        std::cout << "-------------------------[table" << i + 1 << "] " << table->name << "---------------------------" << std::endl;
        std::cout << "Id(" << table->rowid << ") RootPage(" << table->rootPage << ")" << std::endl;
        std::cout << "Number of fields: " << table->numberOfFields << " / Number of constraints: " << table->numberOfConstraints << std::endl;
        
        table->fieldList.resetCurrent();
        for (int j = 0; j < table->numberOfFields; j++)
        {
            field = (Field *)table->fieldList.getNodeData();
            std::cout << " " << field->name << "(" << field->typeString << "), ";
            field->dataList.resetCurrent();
        }
        std::cout << field->numberOfDatas << std::endl;


        table->fieldList.resetCurrent();
        for (int j = 0; j < table->numberOfFields; j++)
        {
            field = (Field *)table->fieldList.getNodeData();

            const char *datatype = field->typeString.c_str();
            if (strstr(datatype, "NVARCHAR") != NULL)
            {
                for (int k = 0; k < field->numberOfDatas; k++)
                {
                    datac = (char *)field->dataList.getNodeData();
                    std::cout << datac << std::endl;
                }
            }
            else if (strstr(datatype, "INTEGER") != NULL)
            {
                for (int k = 0; k < field->numberOfDatas; k++)
                {
                    datai = (long long *)field->dataList.getNodeData();
                    if ( datai == NULL ) std::cout << "NULL" << std::endl;
                    else printf(" %lld\n", *datai);
                }
            }
            else if (strstr(datatype, "NUMERIC") != NULL)
            {
                for (int k = 0; k < field->numberOfDatas; k++)
                {
                    dataf = (double *)field->dataList.getNodeData();
                    printf(" %f\n", *dataf);
                }
            }
            else
            {
                for (int k = 0; k < field->numberOfDatas; k++)
                {
                    std::cout << "datatime?" << std::endl;
                }
            }
        }
    }*/
}
std::string DBConverter::TransformNumber(int input) {

    std::string output;
    int buffer;
    int rest;

    if (input > 702 && input < 16384) {
        buffer = (input - 702) / 676;
        rest = (input - 702) % 676;
        if (rest < 27 && rest != 0) {
            output += (char)buffer + 65;
            output += (char)65;
            output += (char)rest + 64;
        }
        else if (rest == 0) {
            output += (char)buffer + 64;
            output += (char)90;
            output += (char)90;
        }
        else if (rest > 26 && rest < 676) {
            output += (char)buffer + 65;
            buffer = rest / 26;
            rest = rest % 26;
            if (rest == 0) {
                output += (char)buffer + 64;
                output += (char)90;
            }
            else if (rest != 0) {
                output += (char)buffer + 65;
                output += (char)rest + 64;
            }
        }
    }
    else if (input > 26 && input < 703) {
        buffer = input / 26;
        rest = input % 26;
        if (buffer >= 1 && rest != 0) {
            output += (char)buffer + 64;
            output += (char)rest + 64;
        }
        else if (buffer >= 1 && rest == 0) {
            output += (char)buffer + 63;
            output += (char)90;
        }


    }
    else if (input < 27 && input > 0) {
        output += (char)input + 64;

    }
    else {
        output = "too many columns";
    }
    return output;
}

class ExcelConverter{

};

int DBtoExcel(std::string srcpath, std::string dstpath, bool hasFunctions)
{
    // DB
    FileContainer srcFile(FILE_TYPE_DB_READ, srcpath);
    if (srcFile.Load() == -1)
    {
        std::cout << "[error] Failed to load database file." << std::endl;
        return -1;
    }
    DBConverter dbConverter(srcFile);
    if (dbConverter.ReadDB() == -1)
    {
        std::cout << "[error] Failed to read database file." << std::endl;
        return -1;
    }
    std::cout << "--------------SUCCESS READ DB----------------" << std::endl;
    std::cout << "---------------------------------------------" << std::endl;
    // JSON
    // filename.db ---> filename.json
    srcpath = srcpath.substr(0, srcpath.find(".db", 0)) + ".json";


    FileContainer jsonFile(FILE_TYPE_JSON_WRITE, srcpath);
    if (dbConverter.MakeJSON(jsonFile) == -1)
    {
        std::cout << "[error] Failed to make json file." << std::endl;
        return -1;
    }
    std::cout << "-------------SUCCESS MAKE JSON---------------" << std::endl;



    // EXCEL
    if (writeXLSX(srcpath, dstpath, hasFunctions) == -1)
    {
        std::cout << "[error] Failed to make Excel file." << std::endl;
        return -1;
    }

    std::cout << "---------------------------------------------" << std::endl;
    std::cout << "--------------SUCCESS CONVERTING-------------" << std::endl;
    std::cout << "---------------------------------------------" << std::endl;
    return 0;
}

int main (int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cout << "[error] Invalid argument!!" << std::endl;
        std::cout << "press any key.. " << std::endl;
        system("pause > nul");
        return -1;
    }
    std::string path1 = argv[1];
    std::string path2 = argv[2];


    //DBtoExcel("D:\\Documents\\Visual Studio 2019\\projects\\D-BEX\\qvengers\\D-BEX\\chinook.db", "D:\\Documents\\Visual Studio 2019\\projects\\D-BEX\\qvengers\\D-BEX");
    if (DBtoExcel(path1, path2, false) == -1)
    {
        std::cout << "[error] Failed to DB to Excel Process" << std::endl;
        return -1;
    }

    //ExceltoDB();

    //DBtoDB();

    return 0;
}
