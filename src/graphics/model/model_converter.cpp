#include "model_converter.h"

namespace Utility {

void ModelConverter::createModelsFromCollada(const std::string& filePath) {
    xmlParser = XmlParser(filePath);
    XmlNodePtr library_geometries = xmlParser.getTopNode()->getChild("library_geometries");
    
    for(size_t i = 0; i < library_geometries->numChildNodes(); i++) {
        XmlNodePtr xmlMesh = library_geometries->getChild(i)->getChild(0);
        std::shared_ptr<Engine::Model> model = createModelFromCollada(xmlMesh);
        addModel(model);
    }
}

std::shared_ptr<Engine::Model> ModelConverter::createModelFromCollada(XmlNodePtr xmlMesh) {
    std::shared_ptr<Engine::Model> model;
    unsigned int index = 0;
    XmlNodePtr xmlPositions = xmlMesh->getChild("source", index, index);
    index++;
    XmlNodePtr xmlNormals = xmlMesh->getChild("source", index, index);
    index++;
    XmlNodePtr xmlIndices = xmlMesh->getChild("source", index, index);
    index++;
    for(unsigned int startIndex = index; startIndex < xmlMesh->numChildNodes();) {
        Engine::Mesh mesh;
        ColladaMeshPolygonType polygonType = ColladaMeshPolygonType::TRIANGLES;
        // Try catch since the polygon type might be POLYLIST
        try {
            XmlNodePtr triangles = xmlMesh->getChild("triangles", index, startIndex);
            mesh = createMeshFromTriangles(xmlPositions, xmlNormals, xmlIndices, triangles);
        }
        catch(ColladaFormatException& e) {
            polygonType = ColladaMeshPolygonType::POLYLIST;
        }
        
        if(polygonType != ColladaMeshPolygonType::TRIANGLES) {
            XmlNodePtr polylist = xmlMesh->getChild("polylist", index, startIndex);
            mesh = createMeshFromPolyList(xmlPositions, xmlNormals, xmlIndices, polylist);
        }
        model->addMesh(mesh);
        startIndex = index + 1;
    }
    return model;
}

void ModelConverter::addModel(std::shared_ptr<Engine::Model> model) {
    
}

Engine::Mesh ModelConverter::createMeshFromTriangles(XmlNodePtr xmlPositions, XmlNodePtr xmlNormals, XmlNodePtr xmlIndices, XmlNodePtr triangles) {
    
}

Engine::Mesh ModelConverter::createMeshFromPolyList(XmlNodePtr xmlPositions, XmlNodePtr xmlNormals, XmlNodePtr xmlIndices, XmlNodePtr polylist) {
    
}

}
