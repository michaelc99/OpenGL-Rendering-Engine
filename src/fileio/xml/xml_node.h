#ifndef XML_NODE_H
#define XML_NODE_H

#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include <exceptions/io_exception.h>

namespace Utility {

class XmlNode {
    public:
        XmlNode() : XmlNode("", "", std::shared_ptr<XmlNode>(nullptr)) {}
        XmlNode(const std::string& name, const std::string& attributes, const std::shared_ptr<XmlNode> parentNode);
        
        //void addChild(std::shared_ptr<XmlNode> node) { childNodes.insert(std::make_pair(node->getKey(), node)); }
        void addChild(std::shared_ptr<XmlNode> node) { childNodes.push_back(node); }
        //std::map<std::string, std::shared_ptr<XmlNode>> getChildNodes() const { return childNodes; }
        std::vector<std::shared_ptr<XmlNode>> getChildNodes() const { return childNodes; }
        std::string getKey() const { return name + " " + attributes; }
        
        std::string getName() const { return name; }
        void setName(const std::string& name) { this->name = name; }
        std::string getAttributes() const { return attributes; }
        void setAttributes(const std::string& attributes) { this->attributes = attributes; }
        std::string getData() const { return data; }
        void setData(const std::string& data) { this->data = data; }
        std::shared_ptr<XmlNode> getParentNode() const { return parentNode; }
        void setParentNode(const std::shared_ptr<XmlNode> parentNode) { this->parentNode = parentNode; }
        
        std::string toString(const int depth = 0) const;
        friend std::ostream& operator<<(std::ostream& out, const XmlNode& node);
    private:
        std::string name;
        std::string attributes;
        std::string data;
        std::shared_ptr<XmlNode> parentNode;
        //std::map<std::string, std::shared_ptr<XmlNode>> childNodes;
        std::vector<std::shared_ptr<XmlNode>> childNodes;
};

}

#endif //XML_NODE_H
