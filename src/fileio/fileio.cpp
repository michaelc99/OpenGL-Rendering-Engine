#include <fileio/fileio.h>

namespace Engine {

void readFile(std::string filePath, std::string& fileString, const char* endLineDelimiter) {
    std::ifstream inFile;
    fileString = "";
    inFile = std::ifstream(filePath, std::ios_base::in);
    if(inFile.fail()) {
        throw FileIOException("ERROR: Failed to open file: \"" + filePath + "\"");
    }
    while(!inFile.eof()) {
        std::string line;
        getline(inFile, line);
        if(!inFile.fail()) {
            fileString = fileString + line + endLineDelimiter;
        }
    }
    inFile.close();
}

void readFile(std::string filePath, std::stringstream& fileStringStream, const char* endLineDelimiter) {
    std::ifstream inFile;
    fileStringStream = std::stringstream();
    inFile = std::ifstream(filePath, std::ios_base::in);
    if(inFile.fail()) {
        throw FileIOException("ERROR: Failed to open file: \"" + filePath + "\"");
    }
    while(!inFile.eof()) {
        std::string line;
        getline(inFile, line);
        if(!inFile.fail()) {
            fileStringStream << line + endLineDelimiter;
        }
    }
    inFile.close();
}

}
