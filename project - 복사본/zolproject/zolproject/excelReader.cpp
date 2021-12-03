#include "excelReader.hpp"
#include "excelWriter.hpp"
struct SchemaStruct;
struct RuleStruct;
struct FK;



/// <summary>
/// excel -> json 변환
/// </summary>
/// <param name="filepath"></param>
/// <param name="savepath"></param>
/// <returns></returns>
std::string excelReader(std::string filepath, std::string savepath, bool function, std::pair<T1, T2> table_pair) {

	xlnt::workbook wb;
	wb.load(filepath);

	auto ws = wb.active_sheet();

	Json::Value root;
	Json::Value function_root;
	std::string name = getFilename(filepath);

	root["name"] = name;
	function_root["name"] = name;

	int tablenum = wb.sheet_count();


	function_root["sheetNum"] = wb.sheet_count();


	//엑셀파일 순환하며 json파일에 넣기
	for (int y = 0; y < wb.sheet_count(); y++) {
		std::string tablename;
		int function_count = 1;
		Json::Value tablenow;
		Json::Value function_table;
		auto ws = wb.sheet_by_index(y);
		tablenow["tableName"] = ws.title();

		if (ws.title() != "DataSchema") {
			function_table["sheetName"] = ws.title();
			tablenow["columnNum"] = ws.highest_column().index;

			Json::Value cellName;
			Json::Value cellFormula;


			int x = 0;

			for (auto column : ws.columns(false)) {

				Json::Value columnnow;
				Json::Value function_column;
				bool isfield = true;
				std::string nowfield;

				for (auto cell : column) {

					if (isfield) {
						nowfield = cell.to_string();
						columnnow.append(nowfield);
						isfield = false;
					}
					else {
						if (function && cell.has_formula()) {
							cellFormula.append("=" + cell.formula());
							cellName.append(alpha[cell.column_index() - 1] + std::to_string(cell.row()));

						}

						if (table_pair.first[ws.title()][nowfield].datatype == "NVARCHAR" ||
							table_pair.first[ws.title()][nowfield].datatype == "CHAR") columnnow.append(cell.to_string());
						else if (table_pair.first[ws.title()][nowfield].datatype == "FLOAT") {
							float nowfloat = 0;
							std::stringstream ssFloat(cell.to_string());
							ssFloat >> nowfloat;
							columnnow.append(nowfloat);
						}
						else {
							int nowint = 0;
							std::stringstream ssInt(cell.to_string());
							ssInt >> nowint;
							columnnow.append(nowint);
						}
					}
				}
				tablenow[alpha[x++]] = columnnow;
			}
			tablename = "table" + std::to_string(y + 1);
			std::string sheetname = "sheet" + std::to_string(function_count);

			if (!cellName.isNull()) {
				function_table["cellName"] = cellName;
				function_table["cellFormula"] = cellFormula;
				function_root[sheetname] = function_table;
				function_count++;
			}


			//스키마 & 제약조건 (시트내 필드순환)
			int z = 0;
			auto schema_iter = table_pair.first[ws.title()].begin();
			while (schema_iter != table_pair.first[ws.title()].end()) {
				Json::Value schema;
				std::string schemaname = schema_iter->second.fieldcell + "Schema";

				schema.append(schema_iter->first);

				if (schema_iter->second.datatype_2 != "") schema.append(schema_iter->second.datatype + "(" + schema_iter->second.datatype_2 + ")");
				else schema.append(schema_iter->second.datatype);

				schema.append(schema_iter->second.null);
				schema.append(schema_iter->second.default_value);
				schema.append(schema_iter->second.collation);
				schema.append(schema_iter->second.up);

				tablenow[schemaname] = schema;
				schema_iter++;
			}

			//테이블별 제약조건

			tablenow["ConstraintsNum"] = table_pair.second[ws.title()].size();
			auto con_iter = table_pair.second[ws.title()].begin();
			int i = 1;

			while (con_iter != table_pair.second[ws.title()].end()) {
				Json::Value rule;
				std::string rulename = "Constraint" + to_string(i);

				rule.append(con_iter->rule_name);
				rule.append(con_iter->pk);
				rule.append(con_iter->uk);
				rule.append(con_iter->condition.first);
				rule.append(con_iter->condition.second);
				rule.append(con_iter->fk.fk);
				rule.append(con_iter->fk.fk_table);
				rule.append(con_iter->fk.fk_field);
				rule.append(con_iter->fk.delete_rule);
				rule.append(con_iter->fk.correction_rule);
				tablenow[rulename] = rule;

				con_iter++;
				i++;
			}

			root[tablename] = tablenow;

			//시트순환 end점
		}
		else tablenum--;
		root["tableNum"] = tablenum;
	}

	//엑셀파일 순환 끝

	//json 파일 생성
	std::ofstream file_id;
	savepath += "\\" + name + ".json";
	file_id.open(savepath);

	Json::StyledWriter styledWriter;
	file_id << styledWriter.write(root);

	file_id.close();

	if (function) {
		if (function_root != NULL) {
			std::ofstream file_id2;
			std::string function_savepath = getFilename2(savepath) + "Functions.json";
			file_id2.open(function_savepath);
			file_id2 << styledWriter.write(function_root);

			file_id2.close();
			wb.clear();
		}
	}
	return "정상적으로 변환되었습니다\n";


}

std::string getFilename(std::string filepath) {

	for (int x = filepath.length() - 1; x >= 0; x--) {

		if (filepath[x] == '\\') {
			return getFilename2(filepath.substr(x + 1, filepath.length() - 1 - x));
		}

	}
	return "";
}

std::string getFilename2(std::string filepath) {

	for (int x = filepath.length() - 1; x >= 0; x--) {

		if (filepath[x] == '.') {
			return filepath.substr(0, x);
			break;
		}

	}
	return "";
}

/// <summary>
/// 테이블이름, <필드네임, 필드정보>
/// </summary>
/// <param name="filepath"></param>
/// <param name="savepath"></param>
/// <returns></returns>
std::map<std::string, std::map<std::string, struct SchemaStruct>> table_list(std::string filepath, std::string savepath) {

	std::map<std::string, std::map<std::string, SchemaStruct>> map;

	xlnt::workbook wb;
	wb.load(filepath);

	auto ws = wb.active_sheet();

	for (int y = 0; y < wb.sheet_count(); y++) {
		std::map<std::string, SchemaStruct> now_map;
		auto ws = wb.sheet_by_index(y);
		std::string sheet_name = ws.title();

		std::set<std::string> pktest;

		if (sheet_name != "DataSchema") {
			int x = 0;
			for (auto column : ws.columns(false)) {
				bool field = true;
				bool br = false;
				std::string now_field;
				for (auto cell : column) {

					std::string key = cell.to_string();


					if (field) {
						SchemaStruct option = SchemaStruct("NVARCHAR", alpha[x++]);
						now_map.insert(make_pair(key, option));
						now_field = key;
						field = false;
					}

					else {
						if (pktest.find(key) == pktest.end() && !key.empty()) pktest.insert(key);
						else {
							now_map[now_field].pk = false;
							br = true;
						}
					}

					if (br) break;
				}
				pktest.clear();
			}
		}
		map.insert(make_pair(sheet_name, now_map));
	}
	return map;

}

std::vector<string> split(std::string str, char delimiter)
{
	std::vector<string> vs;
	std::string temp;

	for (int i = 0; !(str[i] == 13 || str[i] == '\n' || str[i] == 0); i++)
	{
		if (str[i] == delimiter)
		{
			vs.push_back(temp);
			temp.clear();

			continue;
		}

		temp.push_back(str[i]);
	}
	vs.push_back(temp);

	return vs;
}