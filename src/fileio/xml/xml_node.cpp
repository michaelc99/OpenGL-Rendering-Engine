#include "xml_node.h"

namespace Utility {

XmlNode::XmlNode(const std::string& name, const std::string& attributes, const std::shared_ptr<XmlNode> parentNode)
    : name(name), attributes(attributes), data(""), parentNode(parentNode) {
    
}

std::string XmlNode::toString(const int depth) const {
    std::string asString = "";
    std::string padding = "";
    for(int i = 0; i < depth; i++) {
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
