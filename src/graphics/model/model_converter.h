#ifndef MODEL_CONVERTER_H
#define MODEL_CONVERTER_H

#include <graphics/model/model.h>
#include <fileio/xml/xml_parser.h>
#include <vector>

namespace Utility {

class ModelConverter {
    public:
        void createModelsFromCollada(const std::string& filePath);
    private:
        std::shared_ptr<Engine::ModelData> createModelFromCollada(XmlNodePtr xmlMesh);
        void addModel(std::shared_ptr<Engine::ModelData> staticModel);
        
        /*
         * Assumes that geometric information is formatted as triangles in Collada file.
         */
        Engine::MeshData createMeshDataFromTriangles(XmlNodePtr xmlPositions, XmlNodePtr xmlNormals, XmlNodePtr xmlIndices, XmlNodePtr triangles);
        
        XmlParser xmlParser;
        std::vector<Engine::ModelData> modelDataList;
        /*
        std::string elementType;
        std::string elementID;
        int count;
        std::string data;
         */
};

}

#endif //MODEL_CONVERTER_H
