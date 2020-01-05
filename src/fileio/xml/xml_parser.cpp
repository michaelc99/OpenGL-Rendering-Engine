#include <fileio/xml/xml_parser.h>

namespace Utility {

XmlParser::XmlParser(std::string filePath) : filePath(filePath) {
    parseFile(this->filePath);
}

void XmlParser::parseFile(std::string filePath) {
    std::string fileString;
    Engine::readFile(filePath, fileString, "\n");
    // Clip XML info header node
    int xmlHeaderEnd = findCloseBracket(fileString);
    int xmlHeaderLength = xmlHeaderEnd + 1;
    fileString = fileString.erase(0, xmlHeaderLength);
    // Load XML file into tree
    int index = 0;
    topNode = constructNode(fileString, index);
}

XmlNodePtr XmlParser::constructNode(std::string& fileString, int& index, XmlNodePtr parentNode) {
    XmlNodePtr node = std::make_shared<XmlNode>("", "", parentNode);
    if(setNodeHeaderInfo(fileString, index, node)) {
        return node;
    }
    
    // Add children
    bool hasChildren = true;
    int numChildren = 0;
    while(hasChildren) {
        XmlNodePtr childNode = constructNode(fileString, index, node);
        if(childNode->getName() == "") { // Hit end of child nodes
            if(numChildren == 0) {
                hasChildren = false;
            }
            break;
        }
        node->addChild(childNode);
        numChildren++;
    }
    
    // Add data
    if(!hasChildren) {
        setNodeData(fileString, index, node);
    }
    
    // Move index to end of node
    index = findNodeTrailerEnd(fileString, index, node) + 1;
    return node;
}

bool XmlParser::setNodeHeaderInfo(std::string& fileString, int& index, XmlNodePtr node) {
    int nodeHeaderStart = findOpenBracket(fileString, index) + 1;
    int closeBracketIndex = findCloseBracket(fileString, nodeHeaderStart);
    int nodeHeaderEnd = closeBracketIndex - 1;
    if(nodeHeaderStart > nodeHeaderEnd) {
        throw XmlFormatException("ERROR: Invalid bracket structure.");
    }
    std::string nodeHeader = fileString.substr(nodeHeaderStart, closeBracketIndex - nodeHeaderStart);
    // Convert to stringstream to extract node name
    std::stringstream nodeHeaderStream;
    nodeHeaderStream << nodeHeader;
    std::string nodeName = "";
    nodeHeaderStream >> nodeName;
    if(nodeName == "") {
        throw XmlFormatException("ERROR: Parsing unnamed node.");
    }
    // Check if the trailer of the parent node was reached
    std::string parentName = node->getParentNode()->getName();
    if(nodeName == std::string("/") + parentName) {
        return true;
    }
    // This is an actual child node of the parent
    node->setName(nodeName);
    index = closeBracketIndex + 1;
    
    // Separate attributes
    std::string nodeAttributes = "";
    while(!nodeHeaderStream.eof()) {
        std::string attribute;
        nodeHeaderStream >> attribute;
        nodeAttributes += attribute;
        if(!nodeHeaderStream.eof()) {
            nodeAttributes += " ";
        }
    }
    // Drop closing '/' if the node is a header and trailer with no data or children nodes
    if(nodeAttributes.length() > 0 && nodeAttributes[nodeAttributes.length() - 1] == '/') {
        nodeAttributes.pop_back();
        node->setAttributes(nodeAttributes);
        return true;
    }
    // Regular header of a child node
    node->setAttributes(nodeAttributes);
    return false;
}

void XmlParser::setNodeData(std::string& fileString, int& index, XmlNodePtr node) {
    int dataStart = index;
    int openBracketIndex = findOpenBracket(fileString, index);
    int dataEnd = openBracketIndex - 1;
    if(dataStart > dataEnd) {
        return;
    }
    std::string dataString = fileString.substr(dataStart, openBracketIndex - dataStart);
    node->setData(dataString);
    index = openBracketIndex;
}

int XmlParser::findNodeTrailerEnd(std::string& fileString, const int& index, const XmlNodePtr node) {
    int startOfTrailer = findOpenBracket(fileString, index);
    int endOfTrailer = findCloseBracket(fileString, startOfTrailer);
    int trailerNameStart = startOfTrailer + 1;
    std::string trailerName = fileString.substr(trailerNameStart, endOfTrailer - trailerNameStart);
#ifdef _DEBUG
    assert(node->getName() != "");
#endif
    if(trailerName != std::string("/") + node->getName()) {
        throw XmlFormatException("ERROR: Unexpected end of node.");
    }
    return endOfTrailer;
}

int XmlParser::findOpenBracket(std::string& fileString, const int& index) {
    int foundIndex = fileString.find_first_of('<', index);
    if(foundIndex == (int)std::string::npos) {
        throw XmlFormatException("ERROR: Unexpected end of file.");
    }
    return foundIndex;
}

int XmlParser::findCloseBracket(std::string& fileString, const int& index) {
    int foundIndex = fileString.find_first_of('>', index);
    if(foundIndex == (int)std::string::npos) {
        throw XmlFormatException("ERROR: Unexpected end of file.");
    }
    return foundIndex;
}

}
