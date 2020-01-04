#include "xml_node.h"

namespace Utility {

XmlNode::XmlNode(const std::string& name, const std::string& attributes, const XmlNodePtr parentNode)
    : name(name), attributes(attributes), data(""), parentNode(parentNode) {}

void XmlNode::addChild(XmlNodePtr node) {
    childNodes.push_back(node);
}

size_t XmlNode::getNumChildNodes() {
    return childNodes.size();
}
        
XmlNodePtr XmlNode::getChild(const std::string& name, unsigned int& index, const unsigned int startIndex) const {
    for(unsigned int i = startIndex; i < childNodes.size(); i++) {
        if(childNodes[i]->name == name) {
            index = i;
            return childNodes[i];
        }
    }
    throw XmlFormatException("ERROR: Failed to find desired child node \"" + name + "\" of parent node \"" + this->name + "\" starting at index " + std::to_string(startIndex) + ".");
}

XmlNodePtr XmlNode::getChild(const std::string& name) const {
    unsigned int index;
    return getChild(name, index, 0);
}

XmlNodePtr XmlNode::getChildByKey(const std::string& key) const {
    for(unsigned int i = 0; i < childNodes.size(); i++) {
        if(childNodes[i]->getKey() == key) {
            return childNodes[i];
        }
    }
    throw XmlFormatException("ERROR: Failed to find child node with key \"" + key + "\" of parent node \"" + this->name + "\".");
}

XmlNodePtr XmlNode::getChild(const unsigned int index) const {
#ifdef _DEBUG
    assert(index < childNodes.size());
#endif
    return childNodes[index];
}

std::string XmlNode::getAttributeValue(const std::string& attributeName) const {
    std::stringstream attributeStream;
    attributeStream << this->attributes;
    while(!attributeStream.eof()) {
        std::string attributeString;
        attributeStream >> attributeString;
        unsigned int nameLength = attributeString.find("=");
        std::string currentAttributeName = attributeString.substr(0, nameLength);
        if(currentAttributeName == attributeName) {
            std::string attributeValue = attributeString.substr(nameLength + 1, attributeString.length() - (nameLength + 1));
            if(attributeValue[0] != '"' || attributeValue[attributeValue.length() - 1] != '"') {
                throw XmlFormatException("ERROR: Invalid attribute value format for attribute \"" + attributeName + "\" of node \"" + this->name + "\".");
            }
            attributeValue = attributeValue.substr(1, attributeValue.length() - 1);
            return attributeValue;
        }
    }
    throw XmlFormatException("ERROR: Failed to find attribute \"" + attributeName + "\" of node \"" + this->name + "\".");
}

std::string XmlNode::getKey() const {
    return name + " " + attributes;
}

std::stringstream XmlNode::getDataAsStringStream() const {
    // Convert to stringstream to extract node name
    std::stringstream dataStream;
    dataStream << data;
    return dataStream;
}

std::string XmlNode::toString(const unsigned int depth) const {
    std::string asString = "";
    std::string padding = "";
    for(unsigned int i = 0; i < depth; i++) {
        padding = padding + "    ";
    }
    asString += padding + "<" + name;
    if(attributes != "") {
        asString += " " + attributes;
    }
    asString += ">" + data;
    if(getChildNodes().size() > 0) {
        asString += "\n";
        for(size_t i = 0; i < getChildNodes().size(); i++) {
            asString += getChildNodes()[i]->toString(depth + 1);
            asString += "\n";
        }
        asString += padding;
    }
    asString += "</" + name + ">";
    return asString;
}

std::ostream& operator<<(std::ostream& out, const XmlNode& node) {
    out << node.toString();
    return out;
}

}
