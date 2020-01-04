#include <graphics/model/model_converter.h>
#include <fileio/xml/xml_parser.h>
#include <iostream>

int main() {
    int errorNum = 0;
    
    std::cout << "CONVERTER UTILITY" << std::endl;
    
    try {
        Utility::XmlParser parser;
        ADD_ERROR_INFO(parser = Utility::XmlParser("wolf_test.dae"));
        std::cout << parser.getTopNode()->getChildNodes()[6]->toString();
    }
    catch(Engine::GeneralException& e) {
        std::cerr << e.getMessage() << std::endl;
        errorNum = -1;
    }
    catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
        errorNum = -1;
    }
    catch(...) {
        std::cerr << "ERROR: Unknown exception occurred." << std::endl;
        errorNum = -1;
    }
    
    std::cout << "DONE CONVERSION" << std::endl;
    
    //std::cin.get();
    
    return errorNum;
}

