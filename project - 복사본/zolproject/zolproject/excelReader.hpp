#pragma warning(disable: 4996)
#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <xlnt/xlnt.hpp>
#include <list>
#include <fstream>
#include <cctype>
#include <map>
#include <set>

#include "json/json.h"


#define T1 std::map<std::string, std::map<std::string, SchemaStruct>>
#define T2 std::map <std::string, std::vector<RuleStruct>>

struct SchemaStruct;
struct RuleStruct;
struct FK;

struct SchemaStruct {
public:
	std::string fieldcell;
	std::string datatype;
	std::string datatype_2;
	bool null = false;
	bool up = false;
	bool pk = true;
	std::string default_value;
	std::string collation;

	SchemaStruct(std::string datatype_, std::string fieldcell_) : datatype(datatype_), fieldcell(fieldcell_){}
	SchemaStruct() {};
};



struct FK {
	std::string fk="";
	std::string fk_table="";
	std::string fk_field="";
	std::string delete_rule="";
	std::string correction_rule="";

	FK(
		std::string fk_table_,
		std::string fk_field_,
		std::string delete_rule_,
		std::string correction_rule_) :
		fk_table(fk_table_), fk_field(fk_field_),
		delete_rule(delete_rule_), correction_rule(correction_rule_)
	{}

	FK(
		std::string fk_table_,
		std::string fk_field_)
		:
		fk_table(fk_table_), fk_field(fk_field_), delete_rule("NO ACTION"), correction_rule("NO ACTION")
	{}

	FK() {};
};

struct RuleStruct {
	std::string rule_name = "";
	std::string pk = "";
	std::string uk = "";
	std::pair<std::string, std::string> condition;
	FK fk;

	RuleStruct(std::string pk_) : pk(pk_) {}
	RuleStruct() {};
};

std::string excelReader(std::string filepath_, std::string savepath, bool function, std::pair<T1, T2> table_map);
std::string getFilename(std::string filepath);
std::string getFilename2(std::string filepath);
std::map<std::string, std::map<std::string, struct SchemaStruct>> table_list(std::string filepath, std::string savepath);
//시트이름, 키이름, 키옵션
std::vector<std::string> split(std::string str, char delimiter);