#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <codecvt>
#include <locale>
#include <json/json.h>
#include <xlnt/xlnt.hpp>
#include "path_helper.hpp"

#define EXTENSION "xlsx"
#define TABLE "table"
#pragma warning(disable : 4996)
void ExcelToJson();
