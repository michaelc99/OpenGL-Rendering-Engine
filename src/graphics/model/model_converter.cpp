#include "model_converter.h"

namespace Utility {

ColladaModelConverter::ColladaModelConverter(const std::string& colladaFilePath) {
    this->colladaFilePath = colladaFilePath;
    this->modelDataPtr = createModelDataFromCollada(colladaFilePath);
}

Engine::ModelDataPtr ColladaModelConverter::getModelDataPtr() const {
#ifdef _DEBUG
    assert(modelDataPtr.get() != nullptr);
#endif
    return modelDataPtr;
}

Engine::ModelDataPtr ColladaModelConverter::createModelDataFromCollada(const std::string& colladaFilePath) {
    xmlParser = XmlParser(colladaFilePath);
    XmlNodePtr library_geometries = xmlParser.getTopNode()->getChild("library_geometries");
    XmlNodePtr library_effects = xmlParser.getTopNode()->getChild("library_effects");
    XmlNodePtr xmlMesh = library_geometries->getChild(0)->getChild(0); // Assume only one geometry node per file
    
    VectorPtr<VertexGroupData> vertexGroupDataList = std::make_shared<std::vector<VertexGroupData>>();
    VectorPtr<IndexMesh> indexMeshes = std::make_shared<std::vector<IndexMesh>>();
    parseColladaModel(xmlMesh, vertexGroupDataList, indexMeshes);
    
    // Get Materials for mesh
//    unsigned int meshNum = 0;
//    unsigned int startIndex = 0;
//    while(startIndex < xmlMesh->getNumChildNodes()) {
//        XmlNodePtr xmlTriangles = xmlMesh->getChild("triangles", startIndex, startIndex); // Note index modified here
//        
//        
//        meshNum++;
//        startIndex++;
//    }
    
    Engine::ModelDataPtr modelDataPtr;
    Engine::MeshGeometryDataPtr meshGeometryDataPtr = createMeshGeometryData(vertexGroupDataList);
    std::vector<Engine::Mesh> meshes;
    for(unsigned int i = 0; i < indexMeshes->size(); i++) {
        Engine::VectorPtr<unsigned int> indices = (*(indexMeshes.get()))[i].indices;
        Engine::MeshDataPtr meshDataPtr = std::make_shared<Engine::MeshData>(indices, meshGeometryDataPtr, "");
        
//        DEAL WITH TEXTURES/MATERIALS FROM COLLAD MODEL FILE
        Engine::TexturedMaterial texturedMaterial;
        Engine::UnTexturedMaterial unTexturedMaterial;
        
        Engine::Mesh mesh = Engine::Mesh(meshDataPtr, texturedMaterial, unTexturedMaterial);
        meshes.push_back(mesh);
    }
    
    modelDataPtr = std::make_shared<Engine::ModelData>(meshes);
    return modelDataPtr;
}

unsigned int ColladaModelConverter::addVertexGroup(const VectorPtr<VertexGroupData> vertexGroupDataList, VertexGroupData vertexGroupData) {
    for(unsigned int i = 0; i < vertexGroupDataList->size(); i++) {
        VertexGroupData currentVertexGroupData = (*(vertexGroupDataList.get()))[i];
        if(vertexGroupData.vertex == currentVertexGroupData.vertex && vertexGroupData.normal == currentVertexGroupData.normal
            && vertexGroupData.texCoord == currentVertexGroupData.texCoord
        ) {
            return i;
        }
    }
    vertexGroupDataList->push_back(vertexGroupData);
    return vertexGroupDataList->size() - 1;
}

void ColladaModelConverter::parseColladaModel(const XmlNodePtr xmlMesh, const VectorPtr<VertexGroupData> vertexGroupDataList, const VectorPtr<IndexMesh> indexMeshes) {
    VectorPtr<Engine::Math::Vec3f> unsortedPositions = std::make_shared<std::vector<Engine::Math::Vec3f>>();
    VectorPtr<Engine::Math::Vec3f> unsortedNormals = std::make_shared<std::vector<Engine::Math::Vec3f>>();
    VectorPtr<Engine::Math::Vec2f> unsortedTexCoords = std::make_shared<std::vector<Engine::Math::Vec2f>>();
    parseColladaVertexFloatArrays(xmlMesh, unsortedPositions, unsortedNormals, unsortedTexCoords);
    
    unsigned int numMeshes = 0;
    unsigned int startIndex = 0;
    while(startIndex < xmlMesh->getNumChildNodes()) {
        VectorPtr<Engine::Math::Vec3ui> indices = std::make_shared<std::vector<Engine::Math::Vec3ui>>();
        XmlNodePtr xmlTriangles = xmlMesh->getChild("triangles", startIndex, startIndex); // Note index modified here
        parseColladaIndexArray(xmlTriangles, indices);
        
        // Re-map indices to vertexGroups
        IndexMesh indexMesh;
        indexMesh.indices = std::make_shared<std::vector<unsigned int>>();
        for(unsigned int i = 0; i < indices->size(); i++) {
            VertexGroupData vertexGroupData;
            vertexGroupData.vertex = (*(unsortedPositions.get()))[(*(indices.get()))[i][0]];
            vertexGroupData.normal = (*(unsortedNormals.get()))[(*(indices.get()))[i][1]];
            vertexGroupData.texCoord = (*(unsortedTexCoords.get()))[(*(indices.get()))[i][2]];
            unsigned int vertexGroupIndex = addVertexGroup(vertexGroupDataList, vertexGroupData);
            indexMesh.indices->push_back(vertexGroupIndex);
        }
        indexMeshes->push_back(indexMesh);
        
        numMeshes++;
        startIndex++;
    }
}

void ColladaModelConverter::parseColladaVertexFloatArrays(const XmlNodePtr xmlMesh,
        VectorPtr<Engine::Math::Vec3f> unsortedPositions, VectorPtr<Engine::Math::Vec3f> unsortedNormals, VectorPtr<Engine::Math::Vec2f> unsortedTexCoords) {
    unsigned int startIndex = 0;
    XmlNodePtr xmlPositions = xmlMesh->getChild("source", startIndex, startIndex);
    startIndex++;
    XmlNodePtr xmlNormals = xmlMesh->getChild("source", startIndex, startIndex);
    startIndex++;
    XmlNodePtr xmlMeshMap = xmlMesh->getChild("source", startIndex, startIndex);
    
//    CONVERT FROM BLENDER "Z UP" TO OPENGL "Y UP"
//    CONVERT FROM BLENDER "Z UP" TO OPENGL "Y UP"
//    CONVERT FROM BLENDER "Z UP" TO OPENGL "Y UP"
//    CONVERT FROM BLENDER "Z UP" TO OPENGL "Y UP"
//    CONVERT FROM BLENDER "Z UP" TO OPENGL "Y UP"
//    CONVERT FROM BLENDER "Z UP" TO OPENGL "Y UP"
    // Positions
    std::stringstream positionValueDataStream = xmlPositions->getChild("float_array")->getDataAsStringStream();
    unsigned int numPositions = std::stoi(xmlPositions->getChild("technique_common")->getChild("accessor")->getAttributeValue("count"));
    for(unsigned int i = 0; i < numPositions; i++) {
        Engine::Math::Vec3f position;
        
        std::string valueString;
        // X
        valueString = "";
        if(positionValueDataStream.eof()) {
            throw ColladaFormatException("ERROR: Insufficient number of values for vertex in positions array in file \"" + xmlParser.getFilePath() + "\".");
        }
        positionValueDataStream >> valueString;
        position[0] = std::stof(valueString);
        
        // Y
        valueString = "";
        if(positionValueDataStream.eof()) {
            throw ColladaFormatException("ERROR: Insufficient number of values for vertex in positions array in file \"" + xmlParser.getFilePath() + "\".");
        }
        positionValueDataStream >> valueString;
        position[1] = std::stof(valueString);
        
        // Z
        valueString = "";
        if(positionValueDataStream.eof()) {
            throw ColladaFormatException("ERROR: Insufficient number of values for vertex in positions array in file \"" + xmlParser.getFilePath() + "\".");
        }
        positionValueDataStream >> valueString;
        position[2] = std::stof(valueString);
        
        unsortedPositions->push_back(position);
    }
    
    // Normals
    std::stringstream normalValueDataStream = xmlNormals->getChild("float_array")->getDataAsStringStream();
    unsigned int numNormals = std::stoi(xmlNormals->getChild("technique_common")->getChild("accessor")->getAttributeValue("count"));
    for(unsigned int i = 0; i < numNormals; i++) {
        Engine::Math::Vec3f normal;
        
        std::string valueString;
        // X
        valueString = "";
        if(normalValueDataStream.eof()) {
            throw ColladaFormatException("ERROR: Insufficient number of values for normal in normals array in file \"" + xmlParser.getFilePath() + "\".");
        }
        normalValueDataStream >> valueString;
        normal[0] = std::stof(valueString);
        
        // Y
        valueString = "";
        if(normalValueDataStream.eof()) {
            throw ColladaFormatException("ERROR: Insufficient number of values for normal in normals array in file \"" + xmlParser.getFilePath() + "\".");
        }
        normalValueDataStream >> valueString;
        normal[1] = std::stof(valueString);
        
        // Z
        valueString = "";
        if(normalValueDataStream.eof()) {
            throw ColladaFormatException("ERROR: Insufficient number of values for normal in normals array in file \"" + xmlParser.getFilePath() + "\".");
        }
        normalValueDataStream >> valueString;
        normal[2] = std::stof(valueString);
        
        unsortedNormals->push_back(normal);
    }
    
    // Texture Coordinates
    std::stringstream texCoordValueDataStream = xmlMeshMap->getChild("float_array")->getDataAsStringStream();
    unsigned int numTexCoords = std::stoi(xmlMeshMap->getChild("technique_common")->getChild("accessor")->getAttributeValue("count"));
    for(unsigned int i = 0; i < numTexCoords; i++) {
        Engine::Math::Vec2f texCoord;
        
        std::string valueString;
        // S
        valueString = "";
        if(texCoordValueDataStream.eof()) {
            throw ColladaFormatException(
                    "ERROR: Insufficient number of values for texture coordinate in mesh-map array in file \"" + xmlParser.getFilePath() + "\".");
        }
        texCoordValueDataStream >> valueString;
        texCoord[0] = std::stof(valueString);
        
        // T
        valueString = "";
        if(texCoordValueDataStream.eof()) {
            throw ColladaFormatException(
                    "ERROR: Insufficient number of values for texture coordinate in mesh-map array in file \"" + xmlParser.getFilePath() + "\".");
        }
        texCoordValueDataStream >> valueString;
        texCoord[1] = std::stof(valueString);
        
        unsortedTexCoords->push_back(texCoord);
    }
}

void ColladaModelConverter::parseColladaIndexArray(const XmlNodePtr xmlTriangles, const VectorPtr<Engine::Math::Vec3ui> indices) {
    std::stringstream indexValueDataStream = xmlTriangles->getChild("p")->getDataAsStringStream();
    unsigned int numIndices = std::stoi(xmlTriangles->getAttributeValue("count")) * 3;
    for(unsigned int i = 0; i < numIndices; i++) {
        Engine::Math::Vec3ui triangleIndex;
        for(unsigned int j = 0; j < 3; j++) { // Each index has a position, normal, and texture coordinate
            std::string valueString;
            if(indexValueDataStream.eof()) {
                throw ColladaFormatException("ERROR: Insufficient number of values for index in indices array in file \"" + xmlParser.getFilePath() + "\".");
            }
            indexValueDataStream >> valueString;
            triangleIndex[j] = (unsigned int)std::stoi(valueString);
        }
        indices->push_back(triangleIndex);
    }
}

Engine::MeshGeometryDataPtr ColladaModelConverter::createMeshGeometryData(const VectorPtr<VertexGroupData> vertexGroups) {
    unsigned int numVertices = vertexGroups->size();
    Engine::VectorPtr<Engine::Math::Vec3f> vertices = std::make_shared<std::vector<Engine::Math::Vec3f>>();
    Engine::VectorPtr<Engine::Math::Vec3f> normals = std::make_shared<std::vector<Engine::Math::Vec3f>>();
    Engine::VectorPtr<Engine::Math::Vec2f> textureCoords = std::make_shared<std::vector<Engine::Math::Vec2f>>();
    for(unsigned int i = 0; i < numVertices; i++) {
        vertices->push_back((*(vertexGroups.get()))[i].vertex);
        normals->push_back((*(vertexGroups.get()))[i].normal);
        textureCoords->push_back((*(vertexGroups.get()))[i].texCoord);
    }
    Engine::MeshGeometryDataPtr meshGeometryDataPtr = std::make_shared<Engine::MeshGeometryData>(vertices, normals, textureCoords);
    return meshGeometryDataPtr;
}

}
