#pragma once
#include <iostream>
#include "tableSchema.hpp"
#include "json/json.h"
#include "fileContainer.hpp"

class DBConverter {
private:

    unsigned int sizeOfPage, numberOfPages, currentRootPage;
    FileContainer* srcFile;
    FileContainer* targetFile;

    struct SqliteInfo {
        std::string fileName;
        int pageSize, changeCounter, inheaderSize, pageOfFreelist, numberOfFreelist, schemaCookie,
            schemaFormat, defaultCacheSize, vacuumRootPage, textEncoding, userVersion, incrementalVacuum,
            applicationID, versionValid, sqliteVersion;
        char writeVersion, readVersion, reservedSpace, maxPayload, minPayload, leafPayload;

        // PRAGMA is comming soon..
        // Auto Vacuum mode(auto optimize db size), FSYNC, Journal mode, Locking mode ...
        // ...

        SqliteInfo(std::string inputfile) {
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
            pageOfFreelist = 0;
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
    Table* table;
    LinkedList tableList;

    Json::Value root;

public:
    DBConverter(FileContainer& db);
    DBConverter();
    ~DBConverter();
    int ReadDB();
    int ReadJSON(std::string filename);
    int MakeDB(std::string dstpath);
    int MakeJSON(FileContainer& json);
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
    int InteriorTable(unsigned char* buffer, int pageNumber);
    int LeafTable(unsigned char* buffer, int pageNumber);
    int InteriorIndex(unsigned char* buffer, int pageNumber);
    int LeafIndex(unsigned char* buffer, int pageNumber);

    void SqlParsing();
    void TestSchema();
    void TestRecord();

    std::string TransformNumber(int input);
    int GetVarintSize(unsigned char* buffer);
    unsigned short GetDataSize(long long value, unsigned short* getType);
    long long ByteStream(unsigned char* buffer, unsigned short sizeOfBytes);
    long long BitPattern(unsigned char* buffer, unsigned char* getSize);
    double Roundex(double value, int pos);


    int SearchTables(Json::Value json);
    void MakeSQLCreate();
    void InsertSQL(std::string dstpath);
};


class SqlParser
{
    char* sql;
    char* head;
    char* content;

    char* field;
    char* constraint;

    LinkedList fieldSentence;
    LinkedList constraintSentence;

    int numberOfFields, numberOfConstraints;

    char* GetHead(char* input);
    char* GetContent(char* input);
    std::string Trimming(std::string input);

public:

    SqlParser();
    ~SqlParser();

    void InputSql(std::string input);
    void DeleteSql();
    std::string GetTableName();
    int GetNumberOfField();
    int GetNumberOfConstraint();
    bool IsPrimaryKey();
    bool IsNullable();
    bool IsAutoincrement();
    bool IsUnique();
    void Parse();
    void GetField();
    std::string GetFieldName();
    std::string GetDataType();
    std::string GetCollate();
    std::string GetCheck();
    std::string GetDefault();
    std::string GetForeignKey();
    void GetConstraint();
    std::string GetConstraintName();
    std::string GetPrimaryKey();
    std::string GetUnique();
    std::string GetCheck2();
    std::string GetForeignKey2();
    std::string GetReferenceTable2();
    std::string GetReferenceField2();
    int DeleteRule();
    int UpdateRule();
    int GetNumberOfSystemField();
};
