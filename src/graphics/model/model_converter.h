#ifndef MODEL_CONVERTER_H
#define MODEL_CONVERTER_H

#include <graphics/model/model.h>
#include <fileio/xml/xml_parser.h>
#include <vector>
#include <cassert>

namespace Utility {

class ColladaModelConverter {
    public:
        ColladaModelConverter() {}
        ColladaModelConverter(const std::string& colladaFilePath);
        
        std::string getColladaFilePath() const { return colladaFilePath; }
        Engine::ModelDataPtr getModelDataPtr() const;
    private:
        template<typename T>
        using VectorPtr = std::shared_ptr<std::vector<T>>;
        
        struct VertexGroupData {
            Engine::Math::Vec3f vertex;
            Engine::Math::Vec3f normal;
            Engine::Math::Vec2f texCoord;
        };
        
        struct IndexMesh {
            VectorPtr<unsigned int> indices;
        };
        
        /*
         * Assumes that mesh indices in Collada file are formatted to form triangles.
         * Assumes mesh geometry in Collada file has positions, normals, and a mesh map.
         * 
         * 
         */
        Engine::ModelDataPtr createModelDataFromCollada(const std::string& colladaFilePath);
        
        /*
         * Searches vertexGroupDataList for the vertex group and adds it if not found. Returns the index of the vertex group in the list.
         */
        unsigned int addVertexGroup(const VectorPtr<VertexGroupData> vertexGroupDataList, VertexGroupData vertexGroupData);
        
        /*
         * Appends re-mapped data from XML node tree file to vertexGroupDataList and indexMeshes.
         */
        void parseColladaModel(const XmlNodePtr xmlMesh, VectorPtr<VertexGroupData> vertexGroupDataList, VectorPtr<IndexMesh> indexMeshes);
        
        /*
         * Appends data from XML node tree file to data vectors.
         */
        void parseColladaVertexFloatArrays(const XmlNodePtr xmlMesh, VectorPtr<Engine::Math::Vec3f> positions, VectorPtr<Engine::Math::Vec3f> normals, VectorPtr<Engine::Math::Vec2f> texCoords);
        
        /*
         * Appends index data from XML node tree file to index array.
         */
        void parseColladaIndexArray(const XmlNodePtr xmlTriangles, VectorPtr<Engine::Math::Vec3ui> indices);
        
        /*
         * 
         */
        Engine::MeshGeometryDataPtr createMeshGeometryData(const VectorPtr<VertexGroupData> vertexGroups);
        
        std::string colladaFilePath;
        XmlParser xmlParser;
        Engine::ModelDataPtr modelDataPtr;
};

}

#endif //MODEL_CONVERTER_H
