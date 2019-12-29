#ifndef XML_PARSER_H
#define XML_PARSER_H

#include <map>
#include <string>
#include <sstream>
#include <memory>
#include <cassert>
#include <exceptions/io_exception.h>
#include <fileio/fileio.h>
#include "xml_node.h"

namespace Utility {

class XmlParser {
    public:
        XmlParser() {}
        XmlParser(std::string filename);
        ~XmlParser() { topNode.reset(); }
        
        void parseFile(std::string filename);
        
        //std::shared_ptr<XmlNode> getChildNode();
        
        std::shared_ptr<XmlNode> getTopNode() { return topNode; }
        
        /*
         * Constructs a node from the XML file with data or recursively constructs any children nodes.
         * Moves index to just after the closing bracket of the node trailer.
         */
        std::shared_ptr<XmlNode> constructNode(std::string& fileString, int& index, std::shared_ptr<XmlNode> parentNode = std::make_shared<XmlNode>());
    private:
        /*
         * Fills in node name and attributes. Returns true if node's parent has no children (ie this node doesn't exist) or node
         * is simple (i.e. opens and closes in the same set of braces) and false otherwise.
         * Moves index to just after the closing bracket of the node header.
         */
        bool setNodeHeaderInfo(std::string& fileString, int& index, std::shared_ptr<XmlNode> node);
        
        /*
         * Copies data from XML file as a string into node.
         * Moves index to the opening bracket of node trailer.
         */
        void setNodeData(std::string& fileString, int& index, std::shared_ptr<XmlNode> node);
        
        /*
         * Starts searching at input index and returns index of closing bracket of trailer of the node with name node->getName().
         * Throws exception if the index can't be found or if the name of next trailer doesn't match node->getName().
         */
        int findNodeTrailerEnd(std::string& fileString, const int& index, const std::shared_ptr<XmlNode> node);
        
        /*
         * Starts searching at input index and returns index of opening carrot bracket.
         * Throws exception if the index can't be found.
         */
        int findOpenBracket(std::string& fileString, const int& index = 0);
        
        /*
         * Starts searching at input index and returns index of closing carrot bracket.
         * Throws exception if the index can't be found.
         */
        int findCloseBracket(std::string& fileString, const int& index = 0);
        
        std::shared_ptr<XmlNode> topNode;
};

}

#endif //XML_PARSER_H
