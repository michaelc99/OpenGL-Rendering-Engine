#include "model_converter.h"

namespace Utility {

Engine::ModelDataPtr ModelConverter::createModelFromCollada(const std::string& savingFilePath, const std::string& colladaFilePath) {
    xmlParser = XmlParser(colladaFilePath);
    XmlNodePtr library_geometries = xmlParser.getTopNode()->getChild("library_geometries");
    XmlNodePtr library_effects = xmlParser.getTopNode()->getChild("library_effects");
    XmlNodePtr xmlMesh = library_geometries->getChild(0)->getChild(0); // Assume only one geometry node per file
    Engine::ModelDataPtr modelDataPtr;
    
    Engine::MeshGeometryDataPtr meshGeometryDataPtr = CreateMeshGeometryFromCollada(xmlMesh);
    unsigned int index = 0;
    for(unsigned int startIndex = index; startIndex < xmlMesh->numChildNodes();) {
        Engine::MeshDataPtr meshDataPtr; // Each set of triangles is broken into another mesh
        XmlNodePtr xmlTriangles = xmlMesh->getChild("triangles", index, startIndex);
        unsigned int numTriangles = xmlTriangles->getAttributeValue("count");
        std::string meshMaterial = xmlTriangles->getAttributeValue("material");
        
        modelData->addMeshData(meshData);
        startIndex = index + 1;
    }
    return modelData;
}

Engine::MeshGeometryDataPtr ModelConverter::CreateMeshGeometryFromCollada(const XmlNodePtr xmlMesh) {
    unsigned int index = 0;
    XmlNodePtr xmlPositions = xmlMesh->getChild("source", index, index);
    index++;
    XmlNodePtr xmlNormals = xmlMesh->getChild("source", index, index);
    index++;
    XmlNodePtr xmlIndices = xmlMesh->getChild("source", index, index);
    index++;
    Engine::VectorPtr<Engine::Math::Vec3f> vertices = std::make_shared<std::vector<Engine::Math::Vec3f>>();
    std::stringstream dataStream = xmlPositions->getChild("float_array")->getDataAsStringStream();
    std::string floatValue;
    dataStream >> floatValue;
    while(floatValue != "") {
        vertices->push_back(floatValue);
        dataStream >> floatValue;
    }
    
    Engine::VectorPtr<Engine::Math::Vec3f> normals = std::make_shared<std::vector<Engine::Math::Vec3f>>();
    Engine::VectorPtr<Engine::Math::Vec2f> textureCoords = std::make_shared<std::vector<Engine::Math::Vec2f>>();
    Engine::MeshGeometryDataPtr meshGeometryDataPtr = std::make_shared<Engine::MeshGeometryDataPtr>();
}

void ModelConverter::addModel(std::shared_ptr<Engine::ModelData> model) {
    
}

Engine::MeshData ModelConverter::createMeshDataFromTriangles(XmlNodePtr xmlPositions, XmlNodePtr xmlNormals, XmlNodePtr xmlIndices, XmlNodePtr triangles) {
    
}

}
