#pragma warning(disable: 4996)
#include "DBConverter.hpp"
#include "fileContainer.hpp"
#include "typedef.hpp"
#include "sqlite3.h"
#include "parson.h"

extern std::string alpha[26];

DBConverter::DBConverter(FileContainer& db)  // db->excel  or  db->db
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
DBConverter::DBConverter()   // only excel->db
{
    sizeOfPage = 0;
    numberOfPages = 0;
    currentRootPage;
    srcFile = NULL;
    targetFile = NULL;
    sqliteInfo = NULL;

    numberOfTables = 0;
    table = NULL;
    root = NULL;
}
DBConverter::~DBConverter()
{
    delete sqliteInfo;

    // Delete tables, fields, constraints
    Field* field;
    Constraint* constraint;
    tableList.resetCurrent();
    table = (Table*)tableList.getNodeData();
    long long* datai = NULL;
    char* datac = NULL;
    double* dataf = NULL;
    bool* datab = NULL;

    for (unsigned int i = 0; i < numberOfTables; i++)
    {
        table->fieldList.resetCurrent();
        for (int j = 0; j < table->numberOfFields; j++)
        {
            field = (Field*)table->fieldList.getNodeData();
            const char* datatype = field->typeString.c_str();
            if (strstr(datatype, "NVARCHAR") != NULL)
            {
                for (int k = 0; k < field->numberOfDatas; k++)
                {
                    datac = (char*)field->dataList.getNodeData();
                    delete[] datac;
                }
            }
            else if (strstr(datatype, "INTEGER") != NULL)
            {
                for (int k = 0; k < field->numberOfDatas; k++)
                {
                    datai = (long long*)field->dataList.getNodeData();
                    delete datai;
                }
            }
            else if (strstr(datatype, "NUMERIC") != NULL)
            {
                for (int k = 0; k < field->numberOfDatas; k++)
                {
                    dataf = (double*)field->dataList.getNodeData();
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
            constraint = (Constraint*)table->constraintList.getNodeData();
            delete constraint;
        }
        table->constraintList.~LinkedList();
        delete table;
        table = (Table*)tableList.getNodeData();
    }
    // delete table List
    tableList.~LinkedList();
}

int DBConverter::ReadDB()
{
    unsigned int nextPage = 0;
    // Database Header 100 Bytes
    unsigned char* buffer = new unsigned char[SIZE_OF_DB_HEADER];
    srcFile->Read((char*)buffer, SIZE_OF_DB_HEADER);
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
            table = (Table*)tableList.getNodeData();
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
int DBConverter::ReadJSON(std::string filename)
{
    Json::Value root;
    Json::Reader reader;
    std::ifstream json(filename, std::ifstream::binary);

    bool parsingSuccessful = reader.parse(json, root);

    if (parsingSuccessful == false) {
        std::cout << "[error] Failed to parse configuration\n" << reader.getFormattedErrorMessages();
        return -1;
    }
    std::cout << "-----------------READ JSON---------------" << std::endl;

    SearchTables(root);

    return 0;
}


int DBConverter::SearchTables(Json::Value json)
{
    numberOfTables = json.get("tableNum", "").asInt();

    for (int i = 0; i < numberOfTables; i++) {

        table = new Table;
        Json::Value temp = json.get("table" + std::to_string(i + 1), "");
        table->name = temp.get("tableName", "").asString();
        table->numberOfFields = temp.get("columnNum", "").asInt();



        //필드 스키마
        for (int j = 0; j < table->numberOfFields; j++) {
            Field* field;
            field = new Field;

            Json::Value temp2 = temp.get(alpha[j] + "Schema", "");
            Json::Value temp3 = temp.get(alpha[j], "");


            //field->name = temp2[0].asString();
            field->name = temp3[0].asString();
            field->name.pop_back();
            field->name.erase(0, 1);
            //field->typeString = temp2[1].asString();
            field->typeString = "NVARCHAR";
            field->numberOfDatas = temp3.size() - 1;
            table->numberOfDatas = field->numberOfDatas;
            std::string* data = NULL;
            for (int k = 0; k < field->numberOfDatas; k++) {

                data = new std::string;
                *data = temp3[k + 1].asString();
                size_t idx = (*data).find('\'');
                while (idx != std::string::npos)
                {
                    (*data).replace(idx, 1, "\'\'");
                    idx = (*data).find('\'', idx + 2);
                }

                field->dataList.addNode(data);
            }

            for (int k = 0; k < temp2.size(); k++) {
                field->name = temp2[0].asString();
                field->typeString = temp2[1].asString();
                field->nullable = temp2[2].asBool();
                field->defaultData = temp2[3].asString();
                field->collationString = temp2[4].asString();
                field->autoIncrement = temp2[5].asBool();

            }


            table->fieldList.addNode(field);
        }


        //제약조건 설정

        int constaintNum = temp.get("ConstraintsNum", "").asInt();

        table->numberOfConstraints = constaintNum;

        for (int j = 0; j < table->numberOfConstraints; j++) {

            Json::Value constraintJson = temp.get("Constraint" + std::to_string(j + 1), "");

            Constraint* constraint;
            constraint = new Constraint;

            constraint->name = constraintJson[0].asString();
            constraint->primaryKey = constraintJson[1].asString();
            constraint->unique = constraintJson[2].asString();
            constraint->check = constraintJson[3].asString();// check 확인필요.
            constraint->foreignKey.field = constraintJson[5].asString();
            constraint->foreignKey.referenceTable = constraintJson[6].asString();
            constraint->foreignKey.referenceField = constraintJson[7].asString();



            if (constraintJson[8].asString() == "CASCADE") {
                constraint->foreignKey.onDelete = REFERENCE_RULE_CASCADE;
            }
            else if (constraintJson[8].asString() == "NO ACTION") {
                constraint->foreignKey.onDelete = REFERENCE_RULE_NOACTION;
            }
            else if (constraintJson[8].asString() == "RESTRICT") {
                constraint->foreignKey.onDelete = REFERENCE_RULE_RESTRICT;
            }
            else if (constraintJson[8].asString() == "SET DEFAULT") {
                constraint->foreignKey.onDelete = REFERENCE_RULE_SETDEFAULT;
            }
            else if (constraintJson[8].asString() == "SET NULL") {
                constraint->foreignKey.onDelete = REFERENCE_RULE_SETNULL;
            }
            else {
                constraint->foreignKey.onDelete = 403;//확인필요
            }

            if (constraintJson[9].asString() == "CASCADE") {
                constraint->foreignKey.onUpdate = REFERENCE_RULE_CASCADE;
            }
            else if (constraintJson[9].asString() == "NO ACTION") {
                constraint->foreignKey.onUpdate = REFERENCE_RULE_NOACTION;
            }
            else if (constraintJson[9].asString() == "RESTRICT") {
                constraint->foreignKey.onUpdate = REFERENCE_RULE_RESTRICT;
            }
            else if (constraintJson[9].asString() == "SET DEFAULT") {
                constraint->foreignKey.onUpdate = REFERENCE_RULE_SETDEFAULT;
            }
            else if (constraintJson[9].asString() == "SET NULL") {
                constraint->foreignKey.onUpdate = REFERENCE_RULE_SETNULL;
            }
            else {
                constraint->foreignKey.onUpdate = 403;//확인필요
            }

            table->constraintList.addNode(constraint);

        }
        tableList.addNode(table);
    }
    return 0;
}

int DBConverter::MakeDB(std::string dstpath)
{
    std::cout << "----------------CREATE DATA---------------" << std::endl;
    MakeSQLCreate();
    std::cout << "----------------INSERT DATA---------------" << std::endl;
    std::cout << "                    ..." << std::endl;
    InsertSQL(dstpath);
    std::cout << "---------------------OK-------------------" << std::endl;

    return 0;
}
int DBConverter::MakeJSON(FileContainer& json)
{
    //---------------------------------------------
    // Use parson-master library in this Function.
    //---------------------------------------------
    JSON_Value* rootValue;
    JSON_Object* rootObject;
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
        table = (Table*)tableList.getNodeData();
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
            field = (Field*)table->fieldList.getNodeData();
            getData = TransformNumber(j + 1);
            json_object_set_value(tableObject, getData.c_str(), json_value_init_array());
            arrayPointer = json_object_get_array(tableObject, getData.c_str());
            getData = '[' + field->name + ']';
            json_array_append_string(arrayPointer, getData.c_str());
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
int DBConverter::GetVarintSize(unsigned char* buffer)
{
    int count = 0;
    int sizeOfBytes = 1;
    while (count < 8)
    {
        if (buffer[count] < 128) break;
        sizeOfBytes++;
        count++;
    }
    return sizeOfBytes;
}
int DBConverter::ReadDBheader(unsigned char* buffer)
{
    // temporary Function.
    // Only read the size of a page and the number of pages in the database.
    // It will store variety variables TBD.
    char signature[16] = "SQLite format 3";
    if (strncmp((const char*)buffer, signature, 16) == 0)
    {
        // correct file
        sizeOfPage = buffer[16] * pow(2, 8) + buffer[17];   // 16-17 offset in DB Header
        numberOfPages = buffer[28] * pow(2, 24) + buffer[29] * pow(2, 16) + buffer[30] * pow(2, 8) + buffer[31]; // 28-31 offset in DB Header

        return 1;
    }
    else
    {
        // wrong file
        std::cout << "[error] This file is not SQLite Format!!!" << std::endl;
        return -1;
    }
};
long long DBConverter::BitPattern(unsigned char* buffer, unsigned char* getSize)
{
    long long result = 0;
    int count = 0;
    int sizeOfBytes = 1;

    // get a byte_size
    while (count < 8)
    {
        if (buffer[count] >= 128)
            sizeOfBytes++;
        else break;
        count++;
    }
    // get result
    for (count = 0; count < sizeOfBytes - 1; count++)
    {
        result += buffer[count];
        if (buffer[count] >= 128) result -= 128;
        result <<= 7;
    }
    if (sizeOfBytes == 9)   result <<= 1;
    result += buffer[sizeOfBytes - 1];

    *getSize = sizeOfBytes;

    return result;
};
unsigned short DBConverter::GetDataSize(long long value, unsigned short* getType)
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
        if ((value >= 12) && (value % 2) == 0)      // value is even
        {
            *getType = DATA_TYPE_BLOB;
            return (value - 12) / 2;
        }
        else if ((value >= 13) && (value % 2) == 1) // value is odd
        {
            *getType = DATA_TYPE_TEXT;
            return (value - 13) / 2;
        }
        break;
    }
    return 0;
};
long long DBConverter::ByteStream(unsigned char* buffer, unsigned short sizeOfBytes)
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
    unsigned char* buffer = 0;

    buffer = new unsigned char[sizeOfPage];
    srcFile->seekg(pageNumber, sizeOfPage);
    srcFile->Read((char*)buffer, sizeOfPage);

    //std::cout << "Page" << pageNumber << ": ";

    // Only First Page
    if (pageNumber == 1) offset = SIZE_OF_DB_HEADER;

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
int DBConverter::InteriorTable(unsigned char* buffer, int pageNumber)
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
    unsigned short* cellOffset = new unsigned short[numberOfCells];
    for (int count = 0; count < numberOfCells; count++)
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

    for (int count = 0; count < numberOfCells; count++)
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
int DBConverter::LeafTable(unsigned char* buffer, int pageNumber)
{
    int offset = 0;
    if (pageNumber == 1)  offset = SIZE_OF_DB_HEADER;
    std::cout << "page: " << pageNumber << std::endl;
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
    unsigned short* cellOffset = new unsigned short[numberOfCells];
    for (int count = 0; count < numberOfCells; count++)
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
    LeafCell* cell = new LeafCell[numberOfCells];
    int bytesCount, fieldBytesCount;

    unsigned char varintSize = 0;
    long long content = 0;
    long long dataSize = 0;
    unsigned short dataType = 0;

    long long* datai = NULL;
    char* datac = NULL;
    double* dataf = NULL;

    int sqliteObjType = 0;  // temporary variable.  (1: table, 2: index)

    Field* field;

    for (int count = 0; count < numberOfCells; count++)
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
            if (currentRootPage == 1)    // Sqlite Schema
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
                field = (Field*)table->fieldList.getNodeData();
                field->numberOfDatas++;
                switch (dataType)
                {
                case DATA_TYPE_NULL:
                    field->type = FIELD_TYPE_NULL;
                    break;

                case DATA_TYPE_SIGNED_INT8:
                case DATA_TYPE_SIGNED_INT16:
                case DATA_TYPE_SIGNED_INT24:
                case DATA_TYPE_SIGNED_INT32:
                case DATA_TYPE_SIGNED_INT48:
                case DATA_TYPE_SIGNED_INT64:
                case DATA_TYPE_ZERO:
                case DATA_TYPE_ONE:
                    field->type = FIELD_TYPE_INT;
                    break;

                case DATA_TYPE_FLOAT64:
                    field->type = FIELD_TYPE_REAL;
                    break;

                case DATA_TYPE_RESERVED:
                    // wrong
                    field->type = -1;
                    break;

                case DATA_TYPE_BLOB:
                    field->type = FIELD_TYPE_BLOB;
                    break;

                case DATA_TYPE_TEXT:
                    field->type = FIELD_TYPE_TEXT;
                    break;

                default:
                    // wrong
                    field->type = -1;
                }

                switch (field->type)
                {
                case FIELD_TYPE_NULL:
                    // addNode(NULL)만 입력할 수 있게 수정
                    if (strstr(field->typeString.c_str(), "INTEGER") != NULL)
                    {
                        datai = new long long;
                        *datai = field->numberOfDatas;
                        field->dataList.addNode(datai);
                    }
                    else
                        field->dataList.addNode(NULL);
                    break;
                case FIELD_TYPE_REAL:
                    dataf = new double;
                    *reinterpret_cast<unsigned long long*>(dataf) = (double)ByteStream(buffer + offset + cell[count].dataHeaderSize + fieldBytesCount, dataSize);
                    //*dataf = Roundex(*dataf, 2);
                    fieldBytesCount += dataSize;
                    field->dataList.addNode(dataf);
                    break;
                case FIELD_TYPE_INT:
                case FIELD_TYPE_BLOB:
                    datai = new long long;
                    *datai = ByteStream(buffer + offset + cell[count].dataHeaderSize + fieldBytesCount, dataSize);
                    fieldBytesCount += dataSize;
                    field->dataList.addNode(datai);
                    break;
                case FIELD_TYPE_TEXT:
                    datac = new char[dataSize + 1];
                    for (int i = 0; i < dataSize; i++)
                    {
                        datac[i] = *(buffer + offset + cell[count].dataHeaderSize + fieldBytesCount);
                        fieldBytesCount++;
                    }
                    datac[dataSize] = '\0';
                    field->dataList.addNode(datac);
                    break;
                default:
                    // wrong
                    break;
                }
            }
        }
    }
    delete[] cell;
    delete[] cellOffset;
    return 0;
};
int DBConverter::InteriorIndex(unsigned char* buffer, int pageNumber)
{
    return 0;
};
int DBConverter::LeafIndex(unsigned char* buffer, int pageNumber)
{
    return 0;
};
void DBConverter::SqlParsing()
{
    Field* field;
    Constraint* constraint;
    tableList.resetCurrent();
    SqlParser parser;
    for (int i = 0; i < numberOfTables; i++)
    {
        table = (Table*)tableList.getNodeData();

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
                field->primaryKey = false;
                table->fieldList.addNode(field);

                field = new Field;
                field->name = "seq";
                field->typeString = "INTEGER";
                field->nullable = true;
                field->numberOfDatas = 0;
                field->primaryKey = false;
                table->fieldList.addNode(field);
            }
            else    // consider this table "sqlite_statN"
            {
                field = new Field;
                field->name = "tbl";
                field->typeString = "NVARCHAR";
                field->nullable = true;
                field->numberOfDatas = 0;
                field->primaryKey = false;
                table->fieldList.addNode(field);

                field = new Field;
                field->name = "idx";
                field->typeString = "NVARCHAR";
                field->nullable = true;
                field->numberOfDatas = 0;
                field->primaryKey = false;
                table->fieldList.addNode(field);

                field = new Field;
                field->name = "stat";
                field->typeString = "NVARCHAR";
                field->nullable = true;
                field->numberOfDatas = 0;
                field->primaryKey = false;
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



        // (5) Primary Key
        table->fieldList.resetCurrent();
        for (int j = 0; j < table->numberOfFields; j++)
        {
            field = (Field*)table->fieldList.getNodeData();
            if (field->primaryKey)
            {
                table->constraintList.resetCurrent();
                constraint = (Constraint*)table->constraintList.getNodeData();
                if (constraint != NULL)
                {
                    if (constraint->primaryKey == "")
                    {
                        constraint->primaryKey = field->name;
                    }
                }
                else
                {
                    constraint = new Constraint;
                    constraint->primaryKey = field->name;
                    table->constraintList.addNode(constraint);
                    table->numberOfConstraints++;
                }
            }
        }


        // (6) Exit parsing
        parser.DeleteSql();
    }

}
void DBConverter::TestSchema()
{
    // console output test

    Table* table;
    Field* field;
    Constraint* constraint;
    tableList.resetCurrent();
    for (int i = 0; i < numberOfTables; i++)
    {
        table = (Table*)tableList.getNodeData();
        std::cout << "-------------------------[table" << i + 1 << "] " << table->name << "---------------------------" << std::endl;
        std::cout << "Id(" << table->rowid << ") RootPage(" << table->rootPage << ")" << std::endl;
        std::cout << "SQL: " << table->createSql << std::endl;
        std::cout << "Number of fields: " << table->numberOfFields << " / Number of constraints: " << table->numberOfConstraints << std::endl;

        table->fieldList.resetCurrent();
        for (int j = 0; j < table->numberOfFields; j++)
        {
            field = (Field*)table->fieldList.getNodeData();
            std::cout << " Field[" << j + 1 << "] " << field->name << std::endl;
            std::cout << " Type: " << field->typeString << std::endl;
            //std::cout << " Constraint(field level): ";
            std::cout << " ";
            if (field->primaryKey == true) std::cout << " PK";
            if (field->autoIncrement == true) std::cout << " AI";
            if (field->nullable == true);
            else std::cout << " NOT NULL";
            if (field->foreignKey.field != "")
            {
                std::cout << "\n  FK " << field->foreignKey.field << ": " << field->foreignKey.referenceTable << "." << field->foreignKey.referenceField;
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
            constraint = (Constraint*)table->constraintList.getNodeData();
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
                std::cout << "  FK " << constraint->foreignKey.field << ": " << constraint->foreignKey.referenceTable << "." << constraint->foreignKey.referenceField;
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
double DBConverter::Roundex(double value, int pos)
{
    double b = pow(10, pos);
    double temp = value * pow(10, pos);
    temp = floor(temp + 0.5);
    temp /= pow(10, pos);
    return temp;
}
void DBConverter::MakeSQLCreate()
{
    tableList.resetCurrent();

    for (int i = 0; i < numberOfTables; i++)
    {
        table = (Table*)tableList.getNodeData();
        std::string sqlstr = "CREATE TABLE IF NOT EXISTS ";
        sqlstr += table->name;
        sqlstr += "(";

        table->fieldList.resetCurrent();
        Field* field = NULL;
        for (int j = 0; j < table->numberOfFields; j++) {
            field = (Field*)table->fieldList.getNodeData();
            sqlstr = sqlstr + field->name + " " + field->typeString + " "; // +", ";

            if (field->nullable == false) {
                sqlstr += "NOT NULL";
            }
            else {
            }
            if (field->defaultData == "") {
            }
            else {
                //sqlstr += "DEFAULT " + '\'' + field->defaultData + "\', ";
                sqlstr += "DEFAULT" + field->defaultData + ", ";
            }
            if (field->collationString == "") {
            }
            else {
                sqlstr += "COLATE" + field->collationString + ", ";
            }
            /*
            if (field->autoIncrement == true) {
                sqlstr += "AUTOINCREMENT ";
            else {
            }
            integer primary key 와 사용됨 무조건... 아놔.. ㅠㅠㅠ
            }*/

            sqlstr += ", ";
        }

        //**************************** 수정 해야할 듯**************************//
        Constraint* constraint;
        table->constraintList.resetCurrent();

        if (table->numberOfConstraints == 0) {
        }

        else {
            for (int j = 0; j < table->numberOfConstraints; j++) {
                constraint = (Constraint*)table->constraintList.getNodeData();
                if (constraint->name == "") {
                    sqlstr += "\nCONSTRAINT cons" + std::to_string(j + 1) + " ";
                }
                else {
                    sqlstr += +"\nCONSTRAINT " + constraint->name + " ";
                }

                if (constraint->primaryKey == "") {
                }
                else {
                    sqlstr += "PRIMARY KEY(" + constraint->primaryKey + " ";

                    if (constraint->autoIncrement == true) {
                        sqlstr += "AUTOINCREMENT";
                    }
                    else {
                    }
                    sqlstr += "), ";
                }

                if (constraint->unique == "") {
                }
                else {
                    sqlstr += "UNIQUE " + constraint->unique + " ";
                }
                if (constraint->check == "") {
                }
                else {
                    sqlstr += "CHECK " + constraint->check + " ";
                }

                if (constraint->foreignKey.field == "") {
                }
                else {
                    sqlstr += "FOREIGN KEY(" + constraint->foreignKey.field + ") REFERENCES " +
                        constraint->foreignKey.referenceTable + "(" + constraint->foreignKey.referenceField
                        + ") ";
                    sqlstr += "ON DELETE ";
                    switch (constraint->foreignKey.onDelete)
                    {
                    case REFERENCE_RULE_CASCADE:
                        sqlstr += "CASCADE ";
                        break;
                    case REFERENCE_RULE_NOACTION:
                        sqlstr += "NO ACTION ";
                        break;
                    case REFERENCE_RULE_RESTRICT:
                        sqlstr += "RESTRICT ";
                        break;
                    case REFERENCE_RULE_SETDEFAULT:
                        sqlstr += "SET DEFAULT ";
                        break;
                    case REFERENCE_RULE_SETNULL:
                        sqlstr += "SET NULL ";
                        break;
                    default:
                        sqlstr += "NO ACTION ";
                    }
                    sqlstr += "On UPDATE ";
                    switch (constraint->foreignKey.onUpdate)
                    {
                    case REFERENCE_RULE_CASCADE:
                        sqlstr += "CASCADE  ";
                        break;
                    case REFERENCE_RULE_NOACTION:
                        sqlstr += "NO ACTION  ";
                        break;
                    case REFERENCE_RULE_RESTRICT:
                        sqlstr += "RESTRICT  ";
                        break;
                    case REFERENCE_RULE_SETDEFAULT:
                        sqlstr += "SET DEFAULT  ";
                        break;
                    case REFERENCE_RULE_SETNULL:
                        sqlstr += "SET NULL  ";
                        break;
                    default:
                        sqlstr += "NO ACTION  ";
                    }
                }

            }

        }

        sqlstr.pop_back();
        sqlstr.pop_back();


        sqlstr += ");";
        std::cout << sqlstr << std::endl;
        table->createSql = sqlstr;
    }
}
void DBConverter::InsertSQL(std::string dstpath)
{
    sqlite3* db;
    int rc;
    std::string sqlstr;
    std::string path = dstpath + "\\test_dbex.db";
    rc = sqlite3_open(path.c_str(), &db);

    char* err_msg = 0;

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    tableList.resetCurrent();

    for (int i = 0; i < numberOfTables; i++)
    {
        table = (Table*)tableList.getNodeData();
        if (table->name.find("sqlite_stat") != std::string::npos || table->name.find("sqlite_sequence") != std::string::npos)
            continue;
        rc = sqlite3_exec(db, table->createSql.c_str(), 0, 0, &err_msg);
        if (rc != SQLITE_OK)
        {
            fprintf(stderr, "SQL error: %s\n", err_msg);
            sqlite3_free(err_msg);
            sqlite3_close(db);
            return;
        }
        std::cout << table->name << std::endl;

        for (int k = 0; k < table->numberOfDatas; k++)
        {
            sqlstr = "INSERT OR IGNORE INTO " + table->name + " VALUES (";
            table->fieldList.resetCurrent();
            Field* field;
            for (int j = 0; j < table->numberOfFields; j++)
            {
                field = (Field*)table->fieldList.getNodeData();
                if (field->typeString.find("INTEGER") != std::string::npos) {
                    sqlstr = sqlstr + *(std::string*)field->dataList.getNodeData() + ", ";
                }
                else if (field->typeString.find("DATETIME") != std::string::npos) {
                    sqlstr = sqlstr + "\'" + *(std::string*)field->dataList.getNodeData() + "\', ";
                }
                else if (field->typeString.find("NUMERIC") != std::string::npos) {
                    sqlstr = sqlstr + *(std::string*)field->dataList.getNodeData() + ", ";
                }
                else if (field->typeString.find("NVARCHAR") != std::string::npos) {
                    sqlstr = sqlstr + "\'" + *(std::string*)field->dataList.getNodeData() + "\', ";
                }
            }
            sqlstr.pop_back();
            sqlstr.pop_back();
            sqlstr += ");";
            std::cout << sqlstr << std::endl;

            rc = sqlite3_exec(db, sqlstr.c_str(), 0, 0, &err_msg);
            if (rc != SQLITE_OK)
            {
                fprintf(stderr, "SQL error: %s\n", err_msg);
                sqlite3_free(err_msg);
                sqlite3_close(db);
                return;
            }
        }
    }
    sqlite3_close(db);
    return;
}


char* SqlParser::GetHead(char* input) // ok
{
    int size = strlen(input) - strlen(strchr(input, '('));
    char* parsed = new char[size + 1];
    memcpy(parsed, input, size);
    parsed[size] = '\0';
    return parsed;
}

char* SqlParser::GetContent(char* input) // ok
{
    input = strpbrk(input, "(") + 1;
    int size = strlen(input) - strlen(strrchr(input, ')'));
    char* parsed = new char[size + 1];
    memcpy(parsed, input, size);
    parsed[size] = '\0';
    return parsed;
}

std::string SqlParser::Trimming(std::string input)
{
    input.erase(std::remove(input.begin(), input.end(), 0x0d), input.end());    // '\r'
    input.erase(std::remove(input.begin(), input.end(), 0x0a), input.end());    // '\n'
    input.erase(std::remove(input.begin(), input.end(), 0x09), input.end());    // tab

    return input;
}

SqlParser::SqlParser()
{
    sql = NULL;
    head = NULL;
    content = NULL;
    field = NULL;
    constraint = NULL;

    numberOfFields = 0;
    numberOfConstraints = 0;
}
SqlParser::~SqlParser()
{
    DeleteSql();
}

void SqlParser::InputSql(std::string input)
{
    input = Trimming(input);
    char* sql = new char[input.size() + 1];
    std::copy(input.begin(), input.end(), sql);
    sql[input.size()] = '\0';

    head = GetHead(sql);
    content = GetContent(sql);
}

void SqlParser::DeleteSql()
{
    fieldSentence.resetCurrent();
    for (int i = 0; i < numberOfFields; i++)
    {
        field = (char*)fieldSentence.getNodeData();
        delete[] field;
    }
    fieldSentence.deleteNode();

    constraintSentence.resetCurrent();
    for (int i = 0; i < numberOfConstraints; i++)
    {
        constraint = (char*)constraintSentence.getNodeData();
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


std::string SqlParser::GetTableName()
{
    std::string tableName;
    /*char *pointer = strchr(head, '\"');
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
    }*/
    const char* search = "CREATE TABLE";
    char* pointer = strstr(head, search);
    if (pointer == NULL)
        return "tableNameError";
    else
    {
        pointer = pointer + strlen(search);
        char* buffer = new char[strlen(pointer) + 1];
        //strcpy(buffer, pointer);
        strcpy_s(buffer, strlen(pointer), pointer);
        tableName = buffer;
        delete[] buffer;

        return tableName;
    }

};

int SqlParser::GetNumberOfField()
{
    return numberOfFields;
}

int SqlParser::GetNumberOfConstraint()
{
    return numberOfConstraints;
}


bool SqlParser::IsPrimaryKey()
{
    std::string buffer = field;
    if (buffer.find("PRIMARY KEY") != std::string::npos)
    {
        return true;
    }
    else return false;
}

bool SqlParser::IsNullable()
{
    std::string buffer = field;
    if (buffer.find("NOT NULL") != std::string::npos)
    {
        return false;
    }
    else return true;
}

bool SqlParser::IsAutoincrement()
{
    std::string buffer = field;
    if (buffer.find("AUTOINCREMENT") != std::string::npos)
    {
        return true;
    }
    else return false;
}

bool SqlParser::IsUnique()
{
    std::string buffer = field;
    if (buffer.find("UNIQUE") != std::string::npos)
    {
        return true;
    }
    else return false;
}


void SqlParser::Parse()
{
    numberOfFields = 0;
    numberOfConstraints = 0;

    char* sentence = NULL;
    char* start = content;  // 검사할 문장 처음 위치
    char* end = NULL;       // 마지막 위치 ',' 기준
    int size = 0;           // 문장의 길이(크기)
    int searchComma = 0;    // 찾은 ',' 까지의 길이
    int searchChar1 = 0;    // 찾은 문자의 길이1
    int searchChar2 = -1;    // 찾은 문자의 길이2

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

            if (end != NULL) start += size + 1;
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

            if (end != NULL) start += size + 1;
            constraintSentence.addNode(constraint);
        }



        // 문장을 일단 다 쪼개기

        // " " () [] 괄호를 찾으면 건너 뛰는 방법
        /*searchComma = strcspn(start, ",");
        if (searchComma != strlen(start))   // , 가 있을 때
        {
            searchChar1 = strcspn(start, "([\""); // (, [, "

            while (searchChar1 < searchComma)   // ,는  (이나 [이나 "보다 오른쪽이라면
            {
                switch (*(start + searchChar1))
                {
                case '(':
                    searchChar2 += strcspn(start + searchChar2 + 1, ")") + 1;
                    break;

                case '[':
                    searchChar2 += strcspn(start + searchChar2 + 1, "]") + 1;
                    break;

                case '\"':
                    searchChar2 += strcspn(start + searchChar2 + 1, "\"") + 1;
                    break;

                    // 그 외의 경우는 있어선 안됨. sql문법 오류임. error
                }

                while (searchChar2 > searchComma)  //  )이나 ]이나 "까지 무시
                {
                    // next comma
                    searchComma += strcspn(start + searchComma + 1, ",") + 1;
                }
                searchChar1 += strcspn(start + searchChar1 + 1, "([\"") + 1;
            }
            end = start + searchComma;
        }
        else // ,가 없을 때
        {
            end = NULL;
        }

        size = strlen(start);
        if (end != NULL) size -= strlen(end);

        // 동적할당 sentence
        sentence = new char[size + 1];
        memcpy(sentence, start, size);
        sentence[size] = '\0';

        if (strstr(sentence, "NULL") != NULL)
        {

        }


        // 다음 sentence
        if (end != NULL) start = end + 1;

        // 동적할당 sentence 노드 추가
        constraintSentence.addNode(constraint);*/

    } while (end != NULL);
}

void SqlParser::GetField()
{
    field = (char*)fieldSentence.getNodeData();
}

std::string SqlParser::GetFieldName()
{
    std::string fieldName;
    fieldName = strtok_s(field, " ", &field);
    fieldName.erase(remove(fieldName.begin(), fieldName.end(), '['), fieldName.end());
    fieldName.erase(remove(fieldName.begin(), fieldName.end(), ']'), fieldName.end());

    return fieldName;
}

std::string SqlParser::GetDataType()
{
    char* dataType;
    dataType = strtok_s(field, " ", &field);
    return dataType;
}

std::string SqlParser::GetCollate()
{
    char* data = strstr(field, "COLLATE");
    if (data != NULL)
    {
        return "miguhyun";
    }
    return "";
}

std::string SqlParser::GetCheck()
{
    char* data = strstr(field, "CHECK");
    if (data != NULL)
    {
        return "miguhyun";
    }
    return "";
}

std::string SqlParser::GetDefault()
{
    char* data = strstr(field, "DEFAULT");
    if (data != NULL)
    {
        //strtok(data, " ");
        //data = strtok(NULL, " ");

        return "miguhyun";
    }
    return "";
}

std::string SqlParser::GetForeignKey()
{
    char* src = strstr(field, "FOREIGN KEY");
    char* dest;
    std::string fk;
    if (src != NULL)
    {
        src = strchr(src, '(') + 1;
        char* end = strchr(src, ')');
        int size = strlen(src) - strlen(end);
        dest = new char[size + 1];
        memcpy(dest, src, size);
        dest[size] = '\0';
        fk = dest;
        delete[] dest;

        fk.erase(remove(fk.begin(), fk.end(), '['), fk.end());
        fk.erase(remove(fk.begin(), fk.end(), ']'), fk.end());
        return fk;
    }
    return "";
}


void SqlParser::GetConstraint()
{
    constraint = (char*)constraintSentence.getNodeData();
}

std::string SqlParser::GetConstraintName()
{
    if (strstr(constraint, "CONSTRAINT") != NULL)
    {
        strtok_s(constraint, " ", &constraint);
        char* constraintName = strtok_s(NULL, " ", &constraint);
        std::string getName = constraintName;
        getName.erase(remove(getName.begin(), getName.end(), '['), getName.end());
        getName.erase(remove(getName.begin(), getName.end(), ']'), getName.end());
        return getName;
    }
    return "";
}

std::string SqlParser::GetPrimaryKey()
{
    char* src = strstr(constraint, "PRIMARY KEY");
    char* dest = NULL;
    std::string pk;
    if (src != NULL)
    {
        src = strchr(src, '(') + 1;
        char* end = strchr(src, ')');
        int size = strlen(src) - strlen(end);
        dest = new char[size + 1];
        memcpy(dest, src, size);
        dest[size] = '\0';
        pk = dest;
        delete[] dest;

        pk.erase(remove(pk.begin(), pk.end(), '['), pk.end());
        pk.erase(remove(pk.begin(), pk.end(), ']'), pk.end());
        return pk;
    }
    return "";
}

std::string SqlParser::GetUnique()
{
    if (strstr(constraint, "UNIQUE") != NULL)
    {
        return "miguhyun";

    }
    return "";
}

std::string SqlParser::GetCheck2()
{
    if (strstr(constraint, "CHECK") != NULL)
    {
        return "miguhyun";

    }
    return "";
}

std::string SqlParser::GetForeignKey2()
{
    char* src = strstr(constraint, "FOREIGN KEY");
    char* dest;
    std::string fk;
    if (src != NULL)
    {
        src = strchr(src, '(') + 1;
        char* end = strchr(src, ')');
        int size = strlen(src) - strlen(end);
        dest = new char[size + 1];
        memcpy(dest, src, size);
        dest[size] = '\0';
        fk = dest;
        delete[] dest;
        fk.erase(remove(fk.begin(), fk.end(), '['), fk.end());
        fk.erase(remove(fk.begin(), fk.end(), ']'), fk.end());
        return fk;
    }
    return "";
}

std::string SqlParser::GetReferenceTable2()
{
    char* src = strstr(constraint, "REFERENCES");
    char* dest;
    std::string refer;
    if (src != NULL)
    {
        src = strchr(src, '\"') + 1;
        char* end = strchr(src, '\"');
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

std::string SqlParser::GetReferenceField2()
{
    char* src = strstr(constraint, "REFERENCES");
    char* dest;
    std::string refer;
    if (src != NULL)
    {
        src = strchr(src, '(') + 1;
        char* end = strchr(src, ')');
        int size = strlen(src) - strlen(end);
        dest = new char[size + 1];
        memcpy(dest, src, size);
        dest[size] = '\0';
        refer = dest;
        delete[] dest;
        refer.erase(remove(refer.begin(), refer.end(), '['), refer.end());
        refer.erase(remove(refer.begin(), refer.end(), ']'), refer.end());
        return refer;
    }
    return "";
}

int SqlParser::DeleteRule()
{
    char dest[23];
    int rule = 0;
    char* src = strstr(constraint, "ON DELETE");
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

int SqlParser::UpdateRule()
{
    char dest[23];
    int rule = 0;
    char* src = strstr(constraint, "ON UPDATE");
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

int SqlParser::GetNumberOfSystemField()
{
    // ',' 개수 + 1
    char* a = content;
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
