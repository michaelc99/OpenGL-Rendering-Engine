#include "model_converter.h"

namespace Utility {

Engine::ModelDataPtr ModelConverter::createModelDataFromCollada(const std::string& savingFilePath, const std::string& colladaFilePath) {
    xmlParser = XmlParser(colladaFilePath);
    XmlNodePtr library_geometries = xmlParser.getTopNode()->getChild("library_geometries");
    XmlNodePtr library_effects = xmlParser.getTopNode()->getChild("library_effects");
    XmlNodePtr xmlMesh = library_geometries->getChild(0)->getChild(0); // Assume only one geometry node per file
    
    Engine::ModelDataPtr modelDataPtr;
    Engine::MeshGeometryDataPtr meshGeometryDataPtr = createMeshGeometryDataFromCollada(xmlMesh);
    std::vector<Engine::Mesh> meshes;
    unsigned int index = 0;
    while(index < xmlMesh->getNumChildNodes()) {
        Engine::MeshDataPtr meshDataPtr; // Each set of triangles is broken into another mesh with shared mesh geometry
        XmlNodePtr xmlTriangles = xmlMesh->getChild("triangles", index, index);
        meshDataPtr = createMeshDataFromTriangles(meshGeometryDataPtr, xmlTriangles);
        
        Engine::TexturedMaterial texturedMaterial;
        Engine::UnTexturedMaterial unTexturedMaterial;
        Engine::Mesh mesh = Engine::Mesh(meshDataPtr, texturedMaterial, unTexturedMaterial);
        meshes.push_back(mesh);
        index++;
    }
    modelDataPtr = std::make_shared<Engine::ModelData>(meshes);
    return modelDataPtr;
}

Engine::MeshGeometryDataPtr ModelConverter::createMeshGeometryDataFromCollada(const XmlNodePtr xmlMesh) {
    unsigned int index = 0;
    XmlNodePtr xmlPositions = xmlMesh->getChild("source", index, index);
    index++;
    XmlNodePtr xmlNormals = xmlMesh->getChild("source", index, index);
    index++;
    XmlNodePtr xmlMeshMap = xmlMesh->getChild("source", index, index);
    
    // Vertices
    Engine::VectorPtr<Engine::Math::Vec3f> vertices = std::make_shared<std::vector<Engine::Math::Vec3f>>();
    std::stringstream vertexValueDataStream = xmlPositions->getChild("float_array")->getDataAsStringStream();
    unsigned int numVertices = std::stoi(xmlPositions->getChild("technique_common")->getChild("accessor")->getAttributeValue("count"));
    for(unsigned int i = 0; i < numVertices; i++) {
        Engine::Math::Vec3f vertex;
        for(int j = 0; j < 3; j++) {
            std::string valueString;
            if(vertexValueDataStream.eof()) {
                throw ColladaFormatException("ERROR: Insufficient number of values for vertex in vertex array from file.");
            }
            vertexValueDataStream >> valueString;
            vertex[j] = std::stof(valueString);
        }
        vertices->push_back(vertex);
    }
    
    // Normals
    Engine::VectorPtr<Engine::Math::Vec3f> normals = std::make_shared<std::vector<Engine::Math::Vec3f>>();
    std::stringstream normalValueDataStream = xmlNormals->getChild("float_array")->getDataAsStringStream();
    unsigned int numNormals = std::stoi(xmlNormals->getChild("technique_common")->getChild("accessor")->getAttributeValue("count"));
    for(unsigned int i = 0; i < numNormals; i++) {
        Engine::Math::Vec3f normal;
        for(int j = 0; j < 3; j++) {
            std::string valueString;
            if(normalValueDataStream.eof()) {
                throw ColladaFormatException("ERROR: Insufficient number of values for normal in normal array from file.");
            }
            normalValueDataStream >> valueString;
            normal[j] = std::stof(valueString);
        }
        normals->push_back(normal);
    }
    
    // Texture Coordinates
    Engine::VectorPtr<Engine::Math::Vec2f> textureCoords = std::make_shared<std::vector<Engine::Math::Vec2f>>();
    std::stringstream textureCoordValueDataStream = xmlMeshMap->getChild("float_array")->getDataAsStringStream();
    unsigned int numTextureCoords = std::stoi(xmlMeshMap->getChild("technique_common")->getChild("accessor")->getAttributeValue("count"));
    for(unsigned int i = 0; i < numTextureCoords; i++) {
        Engine::Math::Vec2f textureCoord;
        for(int j = 0; j < 2; j++) {
            std::string valueString;
            if(textureCoordValueDataStream.eof()) {
                throw ColladaFormatException("ERROR: Insufficient number of values for texture coordinate in mesh map array from file.");
            }
            textureCoordValueDataStream >> valueString;
            textureCoord[j] = std::stof(valueString);
        }
        textureCoords->push_back(textureCoord);
    }
    
    Engine::MeshGeometryDataPtr meshGeometryDataPtr = std::make_shared<Engine::MeshGeometryData>(vertices, normals, textureCoords);
    return meshGeometryDataPtr;
}

Engine::MeshDataPtr ModelConverter::createMeshDataFromTriangles(const Engine::MeshGeometryDataPtr meshGeometryDataPtr, const XmlNodePtr xmlTriangles) {
    // Indices
    Engine::VectorPtr<Engine::Math::Vec3ui> indices = std::make_shared<std::vector<Engine::Math::Vec3ui>>();
    std::stringstream indexValueDataStream = xmlTriangles->getChild("p")->getDataAsStringStream();
    unsigned int numIndices = std::stoi(xmlTriangles->getAttributeValue("count")) / 3;
    for(unsigned int i = 0; i < numIndices; i++) {
        Engine::Math::Vec3ui index;
        for(int j = 0; j < 3; j++) {
            std::string valueString;
            if(indexValueDataStream.eof()) {
                throw ColladaFormatException("ERROR: Insufficient number of values for index in indices array from file.");
            }
            indexValueDataStream >> valueString;
            index[j] = (unsigned int)std::stoi(valueString);
        }
        indices->push_back(index);
    }
    
    Engine::MeshDataPtr meshDataPtr = std::make_shared<Engine::MeshData>(indices, meshGeometryDataPtr, "");
    return meshDataPtr;
}

}
