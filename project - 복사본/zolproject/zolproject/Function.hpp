#pragma once
#include <iostream>
#include "DBConverter.hpp"
#include "ExcelConverter.hpp"
#include "typedef.hpp"


int DBtoExcel(std::string srcpath, std::string dstpath, bool function);
int ExceltoDB(std::string srcpath, std::string dstpath, bool function, std::pair<T1, T2> table_map);
int DBtoDB(std::string srcpath, std::string dstpath);