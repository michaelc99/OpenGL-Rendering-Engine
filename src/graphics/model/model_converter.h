#ifndef MODEL_CONVERTER_H
#define MODEL_CONVERTER_H

#include <graphics/model/model.h>
#include <fileio/xml/xml_parser.h>
#include <vector>

namespace Utility {

class ModelConverter {
    public:
        /*
         * 
         */
        Engine::ModelDataPtr createModelDataFromCollada(const std::string& savingFilePath, const std::string& colladaFilePath);
    private:
        /*
         * Assumes mesh geometry in Collada file has positions, normals, and a mesh map.
         */
        Engine::MeshGeometryDataPtr createMeshGeometryDataFromCollada(const XmlNodePtr xmlMesh);
        
        /*
         * Assumes that mesh indices in Collada file are formatted to form triangles.
         */
        Engine::MeshDataPtr createMeshDataFromTriangles(const Engine::MeshGeometryDataPtr meshGeometryDataPtr, const XmlNodePtr xmlTriangles);
        
        XmlParser xmlParser;
};

}

#endif //MODEL_CONVERTER_H
