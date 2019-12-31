#include "model_converter.h"

namespace Utility {

void ModelConverter::createModelsFromCollada(const std::string& filePath) {
    xmlParser = XmlParser(filePath);
    XmlNodePtr library_geometries = xmlParser.getTopNode()->getChild("library_geometries");
    
    for(size_t i = 0; i < library_geometries->numChildNodes(); i++) {
        XmlNodePtr xmlMesh = library_geometries->getChild(i)->getChild(0);
        std::shared_ptr<Engine::ModelData> model = createModelFromCollada(xmlMesh);
        addModel(model);
    }
}

std::shared_ptr<Engine::ModelData> ModelConverter::createModelFromCollada(XmlNodePtr xmlMesh) {
    std::shared_ptr<Engine::ModelData> modelData;
    unsigned int index = 0;
    /*XmlNodePtr xmlPositions = xmlMesh->getChild("source", index, index);
    index++;
    XmlNodePtr xmlNormals = xmlMesh->getChild("source", index, index);
    index++;
    XmlNodePtr xmlIndices = xmlMesh->getChild("source", index, index);
    index++;*/
    for(unsigned int startIndex = index; startIndex < xmlMesh->numChildNodes();) {
        Engine::MeshData meshData;
        XmlNodePtr triangles = xmlMesh->getChild("triangles", index, startIndex);
        meshData = createMeshDataFromTriangles(xmlPositions, xmlNormals, xmlIndices, triangles);
        modelData->addMeshData(meshData);
        startIndex = index + 1;
    }
    return modelData;
}

void ModelConverter::addModel(std::shared_ptr<Engine::ModelData> model) {
    
}

Engine::MeshData ModelConverter::createMeshDataFromTriangles(XmlNodePtr xmlPositions, XmlNodePtr xmlNormals, XmlNodePtr xmlIndices, XmlNodePtr triangles) {
    
}

}
