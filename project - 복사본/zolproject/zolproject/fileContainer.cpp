#include "fileContainer.hpp"
#include "typedef.hpp"

int FileContainer::FileOpen()
{
    switch (filetype)
    {
    case FILE_TYPE_DB_READ:
        readFile.open(srcpath, std::ios::binary);
        if (readFile.is_open())
        {
            std::cout << "Created DB container : " << srcpath << std::endl;
            return 0;
        }
        else
        {
            return -1;
        }
        break;

    case FILE_TYPE_EXCEL_READ:
        readFile.open(srcpath);
        if (readFile.is_open())
        {
            std::cout << "Created Excel container : " << srcpath << std::endl;
            return 0;
        }
        else
        {
            return -1;
        }
        break;

    case FILE_TYPE_JSON_READ:
        readFile.open(srcpath);
        if (readFile.is_open())
        {
            std::cout << "Created JSON container : " << srcpath << std::endl;
            return 0;
        }
        else
        {
            return -1;
        }
        break;

    case FILE_TYPE_DB_WRITE:
        writeFile.open(srcpath, std::ios::binary);
        if (writeFile.is_open())
        {
            std::cout << "Created DB container : " << srcpath << std::endl;
            return 0;
        }
        else
        {
            return -1;
        }
        break;

    case FILE_TYPE_EXCEL_WRITE:
        writeFile.open(srcpath);
        if (writeFile.is_open())
        {
            std::cout << "Created Excel container : " << srcpath << std::endl;
            return 0;
        }
        else
        {
            return -1;
        }
        break;

    case FILE_TYPE_JSON_WRITE:
        writeFile.open(srcpath);
        if (writeFile.is_open())
        {
            std::cout << "Created JSON container : " << srcpath << std::endl;
            return 0;
        }
        else
        {
            return -1;
        }
        break;
    default:
        std::cout << "[error] Incorrect file type argument" << std::endl;
    }
    return -1;
}


FileContainer::FileContainer(const unsigned char type, std::string path) :filetype(type), srcpath(path)
{

}
FileContainer::~FileContainer()
{
    if (readFile.is_open()) readFile.close();
    if (writeFile.is_open()) writeFile.close();
}

int FileContainer::Load()
{
    switch (filetype)
    {
    case FILE_TYPE_DB_READ:
        if (srcpath.substr(srcpath.find_last_of(".") + 1) == "db")
        {
            if (FileOpen() == -1)
            {
                std::cout << "[error] The input file does not exist." << std::endl;
                return -1;
            }
        }
        else
        {
            std::cout << "[error] The input file is not '*.db'" << std::endl;
            return -1;
        }
        break;

    case FILE_TYPE_DB_WRITE:
        std::cout << "[error] FILE_TYPE_DB_WRITE" << std::endl;
        return -1;
        break;

    case FILE_TYPE_EXCEL_READ:
    case FILE_TYPE_EXCEL_WRITE:
        std::cout << "[error] FILE_TYPE_EXCEL" << std::endl;
        return -1;
        break;

    case FILE_TYPE_JSON_READ:
        std::cout << "[error] FILE_TYPE_JSON_READ" << std::endl;
        return -1;
        break;

    case FILE_TYPE_JSON_WRITE:
        if (srcpath.substr(srcpath.find_last_of(".") + 1) == "json")
        {
            if (FileOpen() == -1)
            {
                std::cout << "[error] The json file does not exist." << std::endl;
                return -1;
            }
        }
        else
        {
            std::cout << "[error] The JSON file is not named '*.json'" << std::endl;
            return -1;
        }
        break;

    default:
        std::cout << "[error] Incorrect file type argument" << std::endl;
        return -1;
    }
    return 0;
}

int FileContainer::Read(char* buffer, std::streamsize size)
{
    if (readFile.is_open())
        readFile.read(buffer, size);
    return 0;
}

int FileContainer::Write(std::string sentence, std::streamsize size)
{
    if (writeFile.is_open())
        writeFile.write(sentence.c_str(), size);
    return 0;
}

int FileContainer::seekg(int pageNumber, int size)
{
    readFile.seekg(size * (pageNumber - 1));
    return 0;
}

std::string FileContainer::GetName()
{
    return srcpath;
}