#ifndef XML_NODE_H
#define XML_NODE_H

#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include <exceptions/io_exception.h>

namespace Utility {

class XmlNode;
typedef std::shared_ptr<XmlNode> XmlNodePtr;

class XmlNode {
    public:
        XmlNode() : XmlNode("", "", XmlNodePtr(nullptr)) {}
        XmlNode(const std::string& name, const std::string& attributes, const XmlNodePtr parentNode);
        
        void addChild(XmlNodePtr node);
        std::vector<XmlNodePtr> getChildNodes() const;
        
        /*
         * Returns number of children nodes.
         */
        size_t numChildNodes();
        
        /*
         * Returns pointer to the first child with a name that matches name starting at startIndex.
         * Moves index to the index the child was found at.
         * Throws ColladaFormatException if child is not found.
         */
        XmlNodePtr getChild(const std::string& name, unsigned int& index, const unsigned int startIndex) const;
        
        /*
         * Returns pointer to the first child with name and attribute signature that matches key.
         * Throws ColladaFormatException if child is not found.
         */
        XmlNodePtr getChild(const std::string& key) const;
        
        /*
         * Returns pointer to the child at index in vector of children.
         * Throws ColladaFormatException if child is not found.
         */
        XmlNodePtr getChild(const unsigned int index) const;
        
        /*
         * Returns value assigned to attribute in Xml tree. Returns empty string "" if not found.
         */
        std::string getAttributeValue(const std::string& attribute) const;
        
        /*
         * Returns signature of node consisting of it's name and attributes with value list.
         */
        std::string getKey() const;
        
        std::string getName() const { return name; }
        void setName(const std::string& name) { this->name = name; }
        std::string getAttributes() const { return attributes; }
        void setAttributes(const std::string& attributes) { this->attributes = attributes; }
        std::string getData() const { return data; }
        void setData(const std::string& data) { this->data = data; }
        XmlNodePtr getParentNode() const { return parentNode; }
        void setParentNode(const XmlNodePtr parentNode) { this->parentNode = parentNode; }
        
        std::string toString(const unsigned int depth = 0) const;
        friend std::ostream& operator<<(std::ostream& out, const XmlNode& node);
    private:
        std::string name;
        std::string attributes;
        std::string data;
        XmlNodePtr parentNode;
        std::vector<XmlNodePtr> childNodes;
};

}

#endif //XML_NODE_H
