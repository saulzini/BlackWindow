#pragma once

#include <iostream>
#include <fstream>
class FileManager
{
private:
	std::streampos size;
	char* memblock;

public:
    
    
    FileManager(std::streampos mSize = 0){
        size = mSize;
        memblock = nullptr;
    }

    ~FileManager() {
        delete(memblock);
    }

	bool ReadFile(const char* path);

    std::streampos GetSize() const{
        return size;
    }

    char* GetFile() const {
        return memblock;
    }

};

