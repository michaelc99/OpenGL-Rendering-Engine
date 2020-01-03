#include "xml_node.h"

namespace Utility {

XmlNode::XmlNode(const std::string& name, const std::string& attributes, const XmlNodePtr parentNode)
    : name(name), attributes(attributes), data(""), parentNode(parentNode) {
    
}

void XmlNode::addChild(XmlNodePtr node) {
    childNodes.push_back(node);
}

std::vector<XmlNodePtr> XmlNode::getChildNodes() const {
    return childNodes;
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
