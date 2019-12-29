#ifndef FILEIO_H
#define FILEIO_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <exceptions/io_exception.h>

namespace Engine {

void readFile(std::string filename, std::string& fileString, const char* endLineDelimiter = "\n");

void readFile(std::string filename, std::stringstream& fileStringStream, const char* endLineDelimiter = "\n");

//readBinaryfile

};

#endif //FILEIO_H
