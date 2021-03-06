#pragma once

#include <iostream>
#include <fstream>
class FileManager
{
private:
	std::streampos size;
	char* memblock;

public:
    
    FileManager(){
        size = 0;
        memblock = nullptr;
    }

    ~FileManager() {
        delete(memblock);
        memblock = nullptr;
    }

	bool ReadFile(const char* path);

    std::streampos GetSize() const{
        return size;
    }

    char* GetFile() const {
        return memblock;
    }

};

