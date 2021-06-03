#pragma warning(disable: 4996)
#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <xlnt/xlnt.hpp>
#include <string>
#include "json/json.h"


#define ALPHASIZE 26
extern std::string alpha[ALPHASIZE];

struct cellDetails {
    std::string data;
    std::string style; //cell����
    std::string func; //�Լ���������
};

struct schema {
    std::string field; //�ʵ�
    std::string type; //Ÿ��
    std::string isNull; //null
    std::string defaultValue; //�⺻��
    std::string collation; //�ݷ��̼�
    std::string autoInc; //�ڵ�����
};

struct constraint {
    std::string name;
    std::string primaryKeyField; //�ֿ�Ű �ʵ�
    std::string uniqueKeyField; //����Ű �ʵ�
    std::string conditionField; //����, �ʵ�
    std::string ConditionalStatement; //����, ���ǽ�
    std::string foreignKeyField; //�ܷ�Ű �ʵ�
    std::string referTable; //���� ���̺�
    std::string referField; //���� �ʵ�
    std::string removeRule; //���� ������Ģ
    std::string modifyRule; //���� ������Ģ
};

struct sheetDetails {
    std::string sheetTitle; //sheet �̸�
    std::vector<std::vector<cellDetails*>*> cells;
    std::vector<schema*> schemas;
    std::vector<constraint*> constraints;
};

class excelInfo {
private:
    std::string title;
    std::vector<sheetDetails*> sheets;

public:

    std::vector<sheetDetails*> getSheets() {
        return this->sheets;
    }
    void setTitle(std::string title) {
        this->title = title;
    }
    std::string getTitle() {
        return this->title;
    }
    void setSheet(std::string sheetCount, Json::Value root, int index) { //"sheet1"
        sheetDetails* newSheetDetails = new sheetDetails();
        setTitle(root.get("name", "").asString() + ".xlsx");
        newSheetDetails->sheetTitle = root[sheetCount].get("tableName", "").asString();
        int columnNum = root[sheetCount].get("columnNum", "").asInt();

        for (int i = 0; i < columnNum; i++) {
            const Json::Value column = root[sheetCount][alpha[i]]; //"A"

            std::vector<cellDetails*>* cellTmp = new std::vector<cellDetails*>();
            for (int j = 0; j < column.size(); j++) {
                cellDetails* detailsTmp = new cellDetails;

                detailsTmp->data = column[j].asString();
                cellTmp->push_back(detailsTmp);
            }
            newSheetDetails->cells.push_back(cellTmp);


            // Schema
            std::string alphaSchema = alpha[i] + "Schema"; //"ASchema"
            //std::cout << alphaSchema << std::endl;
            const Json::Value columnSchema = root[sheetCount][alphaSchema];

            int indexSchema = 0;
            schema* schemaTmp = new schema;
            schemaTmp->field = columnSchema[indexSchema++].asString();
            schemaTmp->type = columnSchema[indexSchema++].asString();
            schemaTmp->isNull = columnSchema[indexSchema++].asString();
            schemaTmp->defaultValue = columnSchema[indexSchema++].asString();
            schemaTmp->collation = columnSchema[indexSchema++].asString();
            schemaTmp->autoInc = columnSchema[indexSchema++].asString();
            newSheetDetails->schemas.push_back(schemaTmp);

            //std::cout << schemaTmp->type << std::endl;

            // Constraint
        }

        int constraintsNum = root[sheetCount].get("ConstraintsNum", "").asInt();
        //std::cout << constraintsNum << std::endl;
        for (int i = 1; i <= constraintsNum; i++) {
            std::string constraintName = "Constraint" + std::to_string(i);
            const Json::Value constraintJson = root[sheetCount][constraintName];

            int indexConstraint = 0;
            constraint* constraintTmp = new constraint;
            constraintTmp->name = constraintJson[indexConstraint++].asString();
            constraintTmp->primaryKeyField = constraintJson[indexConstraint++].asString();; //�ֿ�Ű �ʵ�
            constraintTmp->uniqueKeyField = constraintJson[indexConstraint++].asString();; //����Ű �ʵ�
            constraintTmp->conditionField = constraintJson[indexConstraint++].asString();; //����, �ʵ�
            constraintTmp->ConditionalStatement = constraintJson[indexConstraint++].asString();; //����, ���ǽ�
            constraintTmp->foreignKeyField = constraintJson[indexConstraint++].asString();; //�ܷ�Ű �ʵ�
            constraintTmp->referTable = constraintJson[indexConstraint++].asString();; //���� ���̺�
            constraintTmp->referField = constraintJson[indexConstraint++].asString();; //���� �ʵ�
            constraintTmp->removeRule = constraintJson[indexConstraint++].asString();; //���� ������Ģ
            constraintTmp->modifyRule = constraintJson[indexConstraint++].asString();; //���� ������Ģ


            newSheetDetails->constraints.push_back(constraintTmp);
        }
        this->sheets.push_back(newSheetDetails);
    }
};

void setDataSchemaSheetForm(xlnt::worksheet ws, std::string cellName, std::string utf_string);
void setDataSchemaSheetContents(xlnt::worksheet ws, std::string cellName, std::string string);

int readJson(excelInfo* info, std::string filename);
void writeExcel(excelInfo* info, std::string absPath);
int writeXLSX(std::string filename, std::string absPath);