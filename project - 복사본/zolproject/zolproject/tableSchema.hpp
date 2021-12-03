#pragma once
#include "LinkedList.hpp"

struct ForeignKey {
    std::string field;
    std::string referenceField;
    std::string referenceTable;

    // 401: RESTRICT  402: CASCADE  403:NO ACTION  404: SET NULL  405: SET DEFAULT
    int onDelete;
    int onUpdate;
};
struct Field {
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
struct Constraint {
    std::string name;
    std::string primaryKey;
    bool autoIncrement;

    std::string unique;
    std::string check;
    std::string defaultData;

    ForeignKey foreignKey;
};
struct Table {
    std::string name;
    long long rowid;
    int rootPage;
    std::string createSql;

    int numberOfFields;
    int numberOfDatas;
    LinkedList fieldList;
    int numberOfConstraints;
    LinkedList constraintList;
};