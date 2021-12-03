#pragma warning(disable: 4996)
#include "Function.hpp"

int DBtoExcel(std::string srcpath, std::string dstpath, bool function)
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
    if (writeXLSX(srcpath, dstpath, false) == -1)
    {
        std::cout << "[error] Failed to make Excel file." << std::endl;
        return -1;
    }

    std::cout << "---------------------------------------------" << std::endl;
    std::cout << "--------------SUCCESS CONVERTING-------------" << std::endl;
    std::cout << "---------------------------------------------" << std::endl;
    return 0;
}

int ExceltoDB(std::string srcpath, std::string dstpath, bool function, std::pair<T1, T2> table_map)
{
    // Excel
    excelReader(srcpath, dstpath, 0, table_map);

    // filename.xlsx ---> filename.json
    srcpath = srcpath.substr(0, srcpath.find(".xlsx", 0)) + ".json";

    // Json
    DBConverter dbConverter;
    dbConverter.ReadJSON(srcpath);


    // DB
    dbConverter.MakeDB(dstpath);
    return 0;
}

int DBtoDB(std::string srcpath, std::string dstpath)
{
    return 0;
}
/*
int main (int argc, char *argv[])
{
    std::string path1 = "C:\\Users\\82109\\Desktop\\test\\chinook.xlsx";
    std::string path2 = "C:\\Users\\82109\\Desktop\\test";

    switch ('2')
    {
    case '1':
        if (DBtoExcel(path1, path2) == -1)
        {
            std::cout << "[error] Failed to DB to Excel Process" << std::endl;
            return -1;
        }
        break;
    case '2':
        if (ExceltoDB(path1, path2) == -1)
        {
            std::cout << "[error] Failed to Excel to DB Process" << std::endl;
            return -1;
        }
        break;
    case '3':
        if (DBtoDB(path1, path2) == -1)
        {
            std::cout << "[error] Failed to DB to DB Process" << std::endl;
            return -1;
        }
        break;
    }

    return 0;
}
*/