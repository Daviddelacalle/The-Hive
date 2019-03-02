#include "BinaryParser.hpp"
#include <fstream>
#include <experimental/filesystem>
#include <ComponentArch/Components/ComponentHeaders.hpp>
#include <ComponentArch/ObjectManager.hpp>


template<typename T>
std::istream & GG_Read(std::istream& _istream, T& value){
    return _istream.read(reinterpret_cast<char*>(&value), sizeof(T));
}

void BinaryParser::ReadNavmeshData(
    const std::string &BinaryFile,
    std::vector<Node> &GRAPH,
    std::vector<std::vector<Connection>> &Connections,
    std::vector<Face> &SQUARE_FACES
){

    std::ifstream Navmesh(BinaryFile, std::ios::binary);

    uint16_t GRAPH_SIZE;
    GG_Read(Navmesh, GRAPH_SIZE);

    GRAPH.reserve(GRAPH_SIZE);
    for(uint16_t i = 0; i < GRAPH_SIZE; ++i){

        uint16_t ID;
        GG_Read(Navmesh, ID);

        uint16_t NODE_1;
        GG_Read(Navmesh, NODE_1);

        uint16_t NODE_2;
        GG_Read(Navmesh, NODE_2);

        gg::Vector3f Position;
        GG_Read(Navmesh, Position);

        float Radius;
        GG_Read(Navmesh, Radius);

        GRAPH.emplace_back(ID, NODE_1, NODE_2, Position, Radius);
    }

    uint16_t CONNECTIONS_SIZE;
    GG_Read(Navmesh, CONNECTIONS_SIZE);
    Connections.reserve(CONNECTIONS_SIZE);
    Connections.resize(CONNECTIONS_SIZE);

    for(uint16_t i = 0; i < CONNECTIONS_SIZE; ++i){
        uint16_t CONNECTION_X_SIZE;
        GG_Read(Navmesh, CONNECTION_X_SIZE);
        Connections[i].reserve(CONNECTION_X_SIZE);
        for(uint16_t j = 0; j < CONNECTION_X_SIZE; ++j){
            float Value;
            GG_Read(Navmesh, Value);

            uint16_t From;
            GG_Read(Navmesh, From);

            uint16_t To;
            GG_Read(Navmesh, To);

            Connections[i].emplace_back(Value, From, To);
        }
    }

    uint16_t SQUARE_FACES_SIZE;
    GG_Read(Navmesh, SQUARE_FACES_SIZE);

    SQUARE_FACES.reserve(SQUARE_FACES_SIZE);
    for(uint16_t i = 0; i < SQUARE_FACES_SIZE; ++i){
        gg::Vector3f TL;
        GG_Read(Navmesh, TL);

        gg::Vector3f BR;
        GG_Read(Navmesh, BR);

        SQUARE_FACES.emplace_back(TL, BR);

        uint16_t PORTALS_SIZE;
        GG_Read(Navmesh, PORTALS_SIZE);
        for(uint16_t j = 0; j < PORTALS_SIZE; ++j){
            uint16_t PortalID;
            GG_Read(Navmesh, PortalID);
            SQUARE_FACES.back().Portals.push_back(PortalID);
        }

    }

}


void BinaryParser::test(){

    std::ifstream inStream("assets/BinaryFiles/City.data", std::ios::binary);
    uint8_t NUMBER_OF_OBJECTS = 0;
    GG_Read(inStream, NUMBER_OF_OBJECTS);

    std::cout << "NUM " << (uint16_t)NUMBER_OF_OBJECTS << '\n';

    for(uint16_t i = 0; i < NUMBER_OF_OBJECTS; ++i){
        uint8_t MODEL = 0;
        GG_Read(inStream, MODEL);
        std::string str = std::to_string(MODEL);
        str+=".obj";

        auto Manager = Singleton<ObjectManager>::Instance();
        uint16_t NewEntity = Manager->createEntity();

        std::cout << "Model->  " << str << '\n';
        float x,y,z;
        GG_Read(inStream, x);
        GG_Read(inStream, y);
        GG_Read(inStream, z);
        gg::Vector3f Position(x,y,z);
        std::cout << "   -Position: " << x << ", " << y << ", " << z << '\n';
        GG_Read(inStream, x);
        GG_Read(inStream, y);
        GG_Read(inStream, z);
        gg::Vector3f Rotation(x,y,z);
        std::cout << "   -Rotation: " << x << ", " << y << ", " << z << '\n';

        bool HasCollider;
        GG_Read(inStream, HasCollider);
        CTransform* Transform = new CTransform(Position, Rotation);
        Manager->addComponentToEntity(Transform, gg::TRANSFORM, NewEntity);
        Material yelo("assets/Textures/prueba1.png");
        CRenderable_3D* Renderable_3D = new CRenderable_3D("assets/Models/CALLE_PRINCIPAL/"+str, yelo);
        Manager->addComponentToEntity(Renderable_3D, gg::RENDERABLE_3D, NewEntity);

        std::cout << "Collider? = " << HasCollider << '\n';
        if(HasCollider){
            float sx,sz,sy;
            GG_Read(inStream, x);
            GG_Read(inStream, y);
            GG_Read(inStream, z);
            std::cout << "      -Center: " << x << ", " << y << ", " << z << '\n';
            GG_Read(inStream, sx);
            GG_Read(inStream, sy);
            GG_Read(inStream, sz);
            std::cout << "      -Size: " << x << ", " << y << ", " << z << '\n';
            CRigidBody* RIGID = new CRigidBody(false, false,"", x/2, y/2, z/2, sx/2, sy/2, sz/2, 0, 0,0,0, 0.2);
            Manager->addComponentToEntity(RIGID, gg::RIGID_BODY, NewEntity);
        }

        std::cout << '\n';

    }
}


bool BinaryParser::ImportMesh(
    const std::string& BinaryFile,
    std::vector<float> &PositionsNormals,
    std::vector<float> &uv,
    std::vector<float> &TangentsBitangents,
    std::vector<unsigned short> &index
){

    PositionsNormals.clear();
    uv.clear();
    TangentsBitangents.clear();
    index.clear();

    std::ifstream Model(BinaryFile, std::ios::binary);

    uint16_t i = 0;
    float FloatV;
    GG_Read(Model, i);  // POSITIONS_AND_NORMALS_SIZE
    PositionsNormals.reserve(i);
    while(i--){
        GG_Read(Model, FloatV);
        PositionsNormals.emplace_back(FloatV);
    }

    GG_Read(Model, i);  // UV_COORDS_SIZE
    uv.reserve(i);
    while(i--){
        GG_Read(Model, FloatV);
        uv.emplace_back(FloatV);
    }

    GG_Read(Model, i);  // TANGENTS_AND_BITANGENTS_SIZE
    TangentsBitangents.reserve(i);
    while(i--){
        GG_Read(Model, FloatV);
        TangentsBitangents.emplace_back(FloatV);
    }

    GG_Read(Model, i);  // INDEX_SIZE
    index.reserve(i);
    unsigned short UnsignedShortV = 0;
    while(i--){
        GG_Read(Model, UnsignedShortV);
        index.emplace_back(UnsignedShortV);
    }

    return true;
}
