#pragma once

#include <iostream>
#include <fstream>
class FileManager
{
public:
    
    
    FileManager(){
        size = 0;
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


private:
	std::streampos size;
	char* memblock;

};

