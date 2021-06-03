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
    std::string style; //cell서식
    std::string func; //함수값있으면
};

struct schema {
    std::string field; //필드
    std::string type; //타입
    std::string isNull; //null
    std::string defaultValue; //기본값
    std::string collation; //콜레이션
    std::string autoInc; //자동증가
};

struct constraint {
    std::string name;
    std::string primaryKeyField; //주요키 필드
    std::string uniqueKeyField; //고유키 필드
    std::string conditionField; //조건, 필드
    std::string ConditionalStatement; //조건, 조건식
    std::string foreignKeyField; //외래키 필드
    std::string referTable; //참조 테이블
    std::string referField; //참조 필드
    std::string removeRule; //참조 삭제규칙
    std::string modifyRule; //참조 수정규칙
};

struct sheetDetails {
    std::string sheetTitle; //sheet 이름
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
            constraintTmp->primaryKeyField = constraintJson[indexConstraint++].asString();; //주요키 필드
            constraintTmp->uniqueKeyField = constraintJson[indexConstraint++].asString();; //고유키 필드
            constraintTmp->conditionField = constraintJson[indexConstraint++].asString();; //조건, 필드
            constraintTmp->ConditionalStatement = constraintJson[indexConstraint++].asString();; //조건, 조건식
            constraintTmp->foreignKeyField = constraintJson[indexConstraint++].asString();; //외래키 필드
            constraintTmp->referTable = constraintJson[indexConstraint++].asString();; //참조 테이블
            constraintTmp->referField = constraintJson[indexConstraint++].asString();; //참조 필드
            constraintTmp->removeRule = constraintJson[indexConstraint++].asString();; //참조 삭제규칙
            constraintTmp->modifyRule = constraintJson[indexConstraint++].asString();; //참조 수정규칙


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