#ifndef MODEL_CONVERTER_H
#define MODEL_CONVERTER_H

#include "model.h"
#include <fileio/xml/xml_parser.h>
#include <vector>

namespace Utility {

class ModelConverter {
    public:
        void createModelsFromCollada(const std::string& filePath);
    private:
        enum ColladaMeshPolygonType {TRIANGLES, POLYLIST};
        
        std::shared_ptr<Engine::Model> createModelFromCollada(XmlNodePtr xmlMesh);
        void addModel(std::shared_ptr<Engine::Model> staticModel);
        
        Engine::Mesh createMeshFromTriangles(XmlNodePtr xmlPositions, XmlNodePtr xmlNormals, XmlNodePtr xmlIndices, XmlNodePtr triangles);
        Engine::Mesh createMeshFromPolyList(XmlNodePtr xmlPositions, XmlNodePtr xmlNormals, XmlNodePtr xmlIndices, XmlNodePtr polylist);
        
        XmlParser xmlParser;
        std::vector<Engine::Model> staticModels;
        /*
        std::string elementType;
        std::string elementID;
        int count;
        std::string data;
         */
};

}

#endif //MODEL_CONVERTER_H
