#pragma once
#include <iostream>
#include <fstream>

class FileContainer {
private:
    const unsigned char filetype;
    const std::string srcpath;
    std::ifstream readFile;
    std::ofstream writeFile;

    int FileOpen();

public:
    FileContainer(const unsigned char type, std::string path);
    ~FileContainer();
    int Load();
    int Read(char* buffer, std::streamsize size);
    int Write(std::string sentence, std::streamsize size);
    int seekg(int pageNumber, int size);
    std::string GetName();
};