#include "BinaryParser.hpp"
#include <fstream>
// #include <experimental/filesystem>
#include <ComponentArch/Components/CRigidBody.hpp>
#include <ComponentArch/Components/Colliders/CBoxCollider.hpp>
#include <ComponentArch/Components/Colliders/CMeshCollider.hpp>
#include <ComponentArch/Components/CStaticModel.hpp>
#include <ComponentArch/ObjectManager.hpp>
#include <Omicron/CORE/BVH_ROOT_Node.hpp>
#include <Omicron/CORE/FrustrumLeaf.hpp>

bool aux_separator = false;

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

        glm::vec3 Position;
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
        glm::vec3 TL;
        GG_Read(Navmesh, TL);

        glm::vec3 BR;
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

void BinaryParser::ReadNavmeshDataZone(
    const std::string &BinaryFile,
    std::vector<SimpleFace> &SQUARE_FACES
)
{
    uint16_t SQUARE_FACES_SIZE;
    std::ifstream NavmeshZone(BinaryFile, std::ios::binary);

    if(!NavmeshZone.is_open()) std::cout << "Can't open " << BinaryFile << '\n';

    GG_Read(NavmeshZone, SQUARE_FACES_SIZE);
    SQUARE_FACES.reserve(SQUARE_FACES_SIZE);
    for(uint16_t i = 0; i < SQUARE_FACES_SIZE; ++i){
        glm::vec3 TL;
        GG_Read(NavmeshZone, TL);

        glm::vec3 BR;
        GG_Read(NavmeshZone, BR);

        SQUARE_FACES.emplace_back(TL, BR);
    }
}


void BinaryParser::LoadLevelData(const std::string &DATA, int8_t map_zone){

    Factory *fac = Singleton<Factory>::Instance();
    std::ifstream inStream(DATA, std::ios::binary);
    uint8_t NUMBER_OF_OBJECTS = 0;
    GG_Read(inStream, NUMBER_OF_OBJECTS);

    ////std::cout << "NUM " << (uint16_t)NUMBER_OF_OBJECTS << '\n';
    AssetManager* _AssetManager = Singleton<AssetManager>::Instance();

    Shader* DEFAULT_SHADER = _AssetManager->getShader("Default");
    StandardNode* Node = Singleton<Omicron>::Instance()->ZONES[map_zone];


    for(uint16_t i = 0; i < NUMBER_OF_OBJECTS; ++i) {

        glm::vec2 CORNERS[4];
        GG_Read(inStream, CORNERS);

        uint8_t MODEL = 0;
        GG_Read(inStream, MODEL);
        std::string str = std::to_string(MODEL);
        std::string lod = str + "_LOD1.modelgg";
        auto Manager = Singleton<ObjectManager>::Instance();
        uint16_t NewEntity = Manager->createEntity();

        float x,y,z;
        GG_Read(inStream, x);
        GG_Read(inStream, y);
        GG_Read(inStream, z);
        glm::vec3 Position(x,y,z);

        float w;
        GG_Read(inStream, x);
        GG_Read(inStream, y);
        GG_Read(inStream, z);
        GG_Read(inStream, w);
        glm::quat Rotation(w,x,y,z);

        uint8_t HasCollider;
        GG_Read(inStream, HasCollider);

        if(HasCollider){
            // std::cout << str << " | HasCollider: " << static_cast<int>(HasCollider) << '\n';
            if(HasCollider == 2){
                CMeshCollider* RIGID = new CMeshCollider("assets/BulletBoundingBoxes/"+str+".bullet", Position.x,Position.y,Position.z);
                Manager->addComponentToEntity(RIGID, gg::MESHCOLLIDER, NewEntity);

                // continue;
                goto readMaterial;
            }
            GG_Read(inStream, x);
            GG_Read(inStream, y);
            GG_Read(inStream, z);

            float rx, ry, rz, rw;
            GG_Read(inStream, rx);
            GG_Read(inStream, ry);
            GG_Read(inStream, rz);
            GG_Read(inStream, rw);

            float sx,sz,sy;
            GG_Read(inStream, sx);
            GG_Read(inStream, sy);
            GG_Read(inStream, sz);

            CBoxCollider* RIGID = new CBoxCollider(x, y, z, rx,ry,rz,rw, sx/2, sy/2, sz/2);
            Manager->addComponentToEntity(RIGID, gg::BOXCOLLIDER, NewEntity);
        }

        readMaterial:

        auto checkExtension = [](std::string &PATH){
            std::array<std::string,2> EXT = {".png",".jpg"};
            for(uint8_t i = 0 ; i<2 ; i++){
                struct stat buffer;
                if(stat ((PATH+EXT[i]).c_str(), &buffer) == 0) {
                    PATH = PATH+EXT[i];
                    break;
                }
            }
        };

        // Materiales
        ZMaterial* Material;
        uint8_t hasDifuseT, difuseT;
        uint8_t hasNormalT, normalT;
        uint8_t hasSpecularT, specularT;
        difuseT = normalT = specularT = 0;

        std::string difusePath = "assets/Textures/DefaultDiffuse.jpg";
        std::string normalPath = "assets/Textures/DefaultNormal.jpg";
        std::string specularPath = "assets/Textures/DefaultSpecular.jpeg";
        //unsigned int NORMAL_FLAG = GN::INVERT_Y;

        GG_Read(inStream, hasDifuseT);
        if(hasDifuseT){
            GG_Read(inStream, difuseT);
            difusePath = "assets/Textures/UINT8_T_TEXTURES/"+std::to_string(difuseT)+"_t";
            checkExtension(difusePath);
        }

        GG_Read(inStream, hasNormalT);
        if(hasNormalT){
            GG_Read(inStream, normalT);
            normalPath = "assets/Textures/UINT8_T_NORMALS/"+std::to_string(normalT)+"_n";
            //NORMAL_FLAG = 0;
            checkExtension(normalPath);
        }

        GG_Read(inStream, hasSpecularT);
        if(hasSpecularT){
            GG_Read(inStream, specularT);
            // -- Como no se usa por ahora, pues comentado

            // if(specularT){
            //     specularPath = "assets/Textures/UINT8_T_SPECULAR/"+std::to_string(specularT);
            //     checkExtension(specularPath);
            // }
        }

        if(!hasDifuseT && !hasNormalT && !hasSpecularT){
            Material = _AssetManager->getMaterial("Default");
        }
        else{
            Material = _AssetManager->getMaterial("M_"+std::to_string(difuseT)+"_"+std::to_string(normalT)+"_"+std::to_string(specularT));
            Material->attachShader(DEFAULT_SHADER);
            Material->addTexture(GN::DIFFUSE_MAP,    _AssetManager->getTexture(difusePath, 3));
            Material->addTexture(GN::NORMAL_MAP,     _AssetManager->getTexture(normalPath, 3));
            Material->addTexture(GN::SPECULAR_MAP,   _AssetManager->getTexture(specularPath, 1));
        }


        TTransform T_Position;
        TTransform T_Rotation;
        T_Position.setPosition(Position);
        T_Rotation.setRotation(Rotation);

        glm::mat4 Model = T_Position.matrix * T_Rotation.matrix;

        ZStaticMesh* M = new ZStaticMesh(Model);
        M->load("assets/BinaryFiles/BinaryModels/"+str+".modelgg");

        FrustrumLeaf* Malla = new FrustrumLeaf(Node, M);
        Malla->CORNERS[0] = CORNERS[0];
        Malla->CORNERS[1] = CORNERS[1];
        Malla->CORNERS[2] = CORNERS[2];
        Malla->CORNERS[3] = CORNERS[3];

        CStaticModel* Transform = new CStaticModel(Malla, Material);
        Manager->addComponentToEntity(Transform, gg::STATICMODEL, NewEntity);
        Transform->addLOD("assets/BinaryFiles/BinaryModels/"+lod);
    }

}

void BinaryParser::LoadBVHLevelData(const std::string &DATA, int8_t map_zone){
    Factory *fac = Singleton<Factory>::Instance();
    AssetManager* _AssetManager = Singleton<AssetManager>::Instance();
    Shader* DEFAULT_SHADER = _AssetManager->getShader("Default");


    std::ifstream inStream(DATA, std::ios::binary);

    uint8_t NUMBER_OF_NODES = 0;
    GG_Read(inStream, NUMBER_OF_NODES);

    StandardNode* Node = Singleton<Omicron>::Instance()->ZONES[map_zone];
    BVH_ROOT_Node* BVH_ROOT = new BVH_ROOT_Node(Node);
    BVH_ROOT->Hierarchy.reserve(NUMBER_OF_NODES);
    // std::cout << "NUMBER OF NODES " << (uint16_t)NUMBER_OF_NODES << '\n';
    for(uint8_t i = 0; i < NUMBER_OF_NODES; ++i) {
        BVH_Node* NODE = nullptr;

        BoundingBox B;
        GG_Read(inStream, B);

        uint8_t FATHER;
        GG_Read(inStream, FATHER);

        uint8_t FIRST_CHILD;
        GG_Read(inStream, FIRST_CHILD);
        BVH_ROOT->Hierarchy.emplace_back(FATHER, FIRST_CHILD, B, nullptr);
        NODE = &BVH_ROOT->Hierarchy.back();

        bool isLeaf = false;
        GG_Read(inStream, isLeaf);

        if(isLeaf) {
            NODE->Leaf = new StandardNode();
            CreateMesh(inStream, _AssetManager, DEFAULT_SHADER, NODE);
        }

    }
}

void BinaryParser::CreateMesh(std::ifstream &inStream, AssetManager* _AssetManager, Shader* DEFAULT_SHADER, BVH_Node* NODE){

    uint8_t MODEL = 0;
    GG_Read(inStream, MODEL);
    std::string str = std::to_string(MODEL);
    std::string lod = str + "_LOD1.modelgg";
    auto Manager = Singleton<ObjectManager>::Instance();
    uint16_t NewEntity = Manager->createEntity();

    float x,y,z;
    GG_Read(inStream, x);
    GG_Read(inStream, y);
    GG_Read(inStream, z);
    glm::vec3 Position(x,y,z);

    float w;
    GG_Read(inStream, x);
    GG_Read(inStream, y);
    GG_Read(inStream, z);
    GG_Read(inStream, w);
    glm::quat Rotation(w,x,y,z);

    uint8_t HasCollider;
    GG_Read(inStream, HasCollider);

    if(HasCollider){
        // std::cout << str << " | HasCollider: " << static_cast<int>(HasCollider) << '\n';
        if(HasCollider == 2){
            CMeshCollider* RIGID = new CMeshCollider("assets/BulletBoundingBoxes/"+str+".bullet", Position.x,Position.y,Position.z);
            Manager->addComponentToEntity(RIGID, gg::MESHCOLLIDER, NewEntity);

            // continue;
            goto readMaterial;
        }
        GG_Read(inStream, x);
        GG_Read(inStream, y);
        GG_Read(inStream, z);

        float rx, ry, rz, rw;
        GG_Read(inStream, rx);
        GG_Read(inStream, ry);
        GG_Read(inStream, rz);
        GG_Read(inStream, rw);

        float sx,sz,sy;
        GG_Read(inStream, sx);
        GG_Read(inStream, sy);
        GG_Read(inStream, sz);

        CBoxCollider* RIGID = new CBoxCollider(x, y, z, rx,ry,rz,rw, sx/2, sy/2, sz/2);
        Manager->addComponentToEntity(RIGID, gg::BOXCOLLIDER, NewEntity);
    }

    readMaterial:

    auto checkExtension = [](std::string &PATH){
        std::array<std::string,2> EXT = {".png",".jpg"};
        for(uint8_t i = 0 ; i<2 ; i++){
            struct stat buffer;
            if(stat ((PATH+EXT[i]).c_str(), &buffer) == 0) {
                PATH = PATH+EXT[i];
                break;
            }
        }
    };

    // Materiales
    ZMaterial* Material;
    uint8_t hasDifuseT, difuseT;
    uint8_t hasNormalT, normalT;
    uint8_t hasSpecularT, specularT;
    difuseT = normalT = specularT = 0;

    std::string difusePath = "assets/Textures/DefaultDiffuse.jpg";
    std::string normalPath = "assets/Textures/DefaultNormal.jpg";
    std::string specularPath = "assets/Textures/DefaultSpecular.jpeg";
    //unsigned int NORMAL_FLAG = GN::INVERT_Y;

    GG_Read(inStream, hasDifuseT);
    if(hasDifuseT){
        GG_Read(inStream, difuseT);
        difusePath = "assets/Textures/UINT8_T_TEXTURES/"+std::to_string(difuseT)+"_t";
        checkExtension(difusePath);
    }

    GG_Read(inStream, hasNormalT);
    if(hasNormalT){
        GG_Read(inStream, normalT);
        normalPath = "assets/Textures/UINT8_T_NORMALS/"+std::to_string(normalT)+"_n";
        //NORMAL_FLAG = 0;
        checkExtension(normalPath);
    }

    GG_Read(inStream, hasSpecularT);
    if(hasSpecularT){
        GG_Read(inStream, specularT);
        // if(specularT){
        //     specularPath = "assets/Textures/UINT8_T_SPECULAR/"+std::to_string(specularT);
        //     checkExtension(specularPath);
        // }
    }

    if(!hasDifuseT && !hasNormalT && !hasSpecularT){
        Material = _AssetManager->getMaterial("Default");
    }
    else{
        Material = _AssetManager->getMaterial("M_"+std::to_string(difuseT)+"_"+std::to_string(normalT)+"_"+std::to_string(specularT));
        Material->attachShader(DEFAULT_SHADER);
        Material->addTexture(GN::DIFFUSE_MAP,    _AssetManager->getTexture(difusePath, 3));
        Material->addTexture(GN::NORMAL_MAP,     _AssetManager->getTexture(normalPath, 3));
        Material->addTexture(GN::SPECULAR_MAP,   _AssetManager->getTexture(specularPath, 1));
    }

    CStaticModel* Transform = new CStaticModel(NODE->Leaf, "assets/BinaryFiles/BinaryModels/"+str+".modelgg", Material, Position, Rotation);
    Manager->addComponentToEntity(Transform, gg::STATICMODEL, NewEntity);
    //Transform->addLOD("assets/BinaryFiles/BinaryModels/"+lod);
}


void BinaryParser::LoadLevelDataEvents(const std::string &DATA, int8_t map_zone){

    // ------------------------------------------------------------------------------------ //
    Factory *fac = Singleton<Factory>::Instance();
    AssetManager* _AssetManager = Singleton<AssetManager>::Instance();
    StandardNode* Node = Singleton<Omicron>::Instance()->ZONES.back();

    std::ifstream inStream(DATA, std::ios::binary);

    uint8_t TOTAL = 0;
    uint8_t EVENT = 0;
    GG_Read(inStream,TOTAL);

    // //std::cout << "EVENT TOTAL : " << static_cast<int>(TOTAL) << " --- " << '\n';

    for (size_t i=0 ; i<TOTAL; ++i) {
        GG_Read(inStream,EVENT);

        if(EVENT == 0){
            // Armas
            uint8_t arma;
            GG_Read(inStream,arma);

            float x,y,z;
            GG_Read(inStream,x);
            GG_Read(inStream,y);
            GG_Read(inStream,z);
            glm::vec3 position(x,y,z);

            // //std::cout << " - Arma     : " << static_cast<int>(arma) << '\n';
            // //std::cout << "   |  - POS : " << "(" << x << "," << y << "," << z << ")" << '\n';
            // //std::cout << '\n';

            fac->createCollectableWeapon(position, arma);
        }
        else{
            // Touchables y pickables
            // NOTA INFORMATIVA: NUNCA ningun pickable va solo

            uint8_t obj;
            GG_Read(inStream,obj);

            // Touchable
            float x,y,z;
            float rx,ry,rz,rw;

            // Collider del touchable
            float cx,cy,cz;
            float crx,cry,crz,crw;
            float csx,csy,csz;

            // Toggle del touchable
            float tx,ty,tz;
            float trx,tory,trz,trw;

            // Collider del toggle del touchable
            float tcx,tcy,tcz;
            float tcrx,tcry,tcrz,tcrw;
            float tcsx,tcsy,tcsz;

            // Pickable
            float px,py,pz;

            GG_Read(inStream,x);
            GG_Read(inStream,y);
            GG_Read(inStream,z);

            GG_Read(inStream,rx);
            GG_Read(inStream,ry);
            GG_Read(inStream,rz);
            GG_Read(inStream,rw);

            // //std::cout << " - Touchable: " << static_cast<int>(obj) << '\n';
            // //std::cout << "   |  - POS : " << "(" << x << "," << y << "," << z << ")" << '\n';
            // //std::cout << "   |  - ROT : " << "(" << rx << "," << ry << "," << rz << ")" << '\n';

            uint8_t hasCollider;
            GG_Read(inStream,hasCollider);
            if(hasCollider){
                GG_Read(inStream,cx);
                GG_Read(inStream,cy);
                GG_Read(inStream,cz);

                GG_Read(inStream,crx);
                GG_Read(inStream,cry);
                GG_Read(inStream,crz);
                GG_Read(inStream,crw);

                GG_Read(inStream,csx);
                GG_Read(inStream,csy);
                GG_Read(inStream,csz);

                // //std::cout << "   |  Has collider    : " << '\n';
                // //std::cout << "      |  - POS        : " << "(" << cx << "," << cy << "," << cz << ")" << '\n';
                // //std::cout << "      |  - ROT        : " << "(" << crx << "," << cry << "," << crz << "[" << crw << "])" << '\n';
                // //std::cout << "      |  - SIZE       : " << "(" << csx << "," << csy << "," << csz << ")" << '\n';
            }
            else{
                // //std::cout << "   |  No collider" << '\n';
            }

            // Toggle
            uint8_t toggleObj;
            GG_Read(inStream,toggleObj);

            GG_Read(inStream,tx);
            GG_Read(inStream,ty);
            GG_Read(inStream,tz);

            // Pongo tory en vez de try porque sino al compilador le peta la cabeza
            GG_Read(inStream,trx);
            GG_Read(inStream,tory);
            GG_Read(inStream,trz);
            GG_Read(inStream,trw);

            // //std::cout << "   |  Has toggle      : " << '\n';
            // //std::cout << "      |  - OBJ        : " << static_cast<int>(toggleObj) << '\n';
            // //std::cout << "      |  - POS        : " << "(" << tx << "," << ty << "," << tz << ")" << '\n';
            // //std::cout << "      |  - ROT        : " << "(" << trx << "," << tory << "," << trz << ")" << '\n';

            uint8_t toggleHasCollider;
            GG_Read(inStream,toggleHasCollider);
            if(toggleHasCollider){
                GG_Read(inStream,tcx);
                GG_Read(inStream,tcy);
                GG_Read(inStream,tcz);

                GG_Read(inStream,tcrx);
                GG_Read(inStream,tcry);
                GG_Read(inStream,tcrz);
                GG_Read(inStream,tcrw);

                GG_Read(inStream,tcsx);
                GG_Read(inStream,tcsy);
                GG_Read(inStream,tcsz);

                // //std::cout << "      |  Toggle has collider  : " << '\n';
                // //std::cout << "         |  - POS             : " << "(" << tcx << "," << tcy << "," << tcz << ")" << '\n';
                // //std::cout << "         |  - ROT             : " << "(" << tcrx << "," << tcry << "," << tcrz << "[" << tcrw << "])" << '\n';
                // //std::cout << "         |  - SIZE            : " << "(" << tcsx << "," << tcsy << "," << tcsz << ")" << '\n';
            }
            else{
                // //std::cout << "      |  Toggle has no collider" << '\n';
            }

            // Pickable?
            uint8_t hasPickable;
            GG_Read(inStream,hasPickable);
            if(hasPickable){

                GG_Read(inStream,px);
                GG_Read(inStream,py);
                GG_Read(inStream,pz);

                // //std::cout << "   |  Has pickable    : " << '\n';
                // //std::cout << "      |  - POS        : " << "(" << px << "," << py << "," << pz << ")" << '\n';
            }
            else{
                // //std::cout << "   |  No Pickable" << '\n';
            }

            auto Manager = Singleton<ObjectManager>::Instance();
            // ZMaterial* Dark = Singleton<AssetManager>::Instance()->getMaterial("Door");
            ZMaterial* Dark = Singleton<AssetManager>::Instance()->getMaterial("Default");
            uint16_t NewEntity = Manager->createEntity();

            std::string str = "assets/BinaryFiles/BinaryModels/"+std::to_string(obj)+".modelgg";
            glm::vec3 Position(x,y,z);
            glm::quat Rotation(rw,rx,ry,rz);

            // PUERTA
            CTransform* T = new CTransform(Position, Rotation);
            Manager->addComponentToEntity(T, gg::TRANSFORM, NewEntity);

            CRenderable_3D* Transform = new CRenderable_3D(Node, str, Dark);
            Manager->addComponentToEntity(Transform, gg::RENDERABLE_3D, NewEntity);

            if(hasCollider){
                CRigidBody* RIGID = new CRigidBody(cx, cy, cz, crx,cry,crz,crw, csx/2, csy/2, csz/2);
                Manager->addComponentToEntity(RIGID, gg::RIGID_BODY, NewEntity);
                RIGID->setVirtualRotation(Rotation);
            }

            // LLAVE
            uint16_t key = 0;
            if(hasPickable){
                Position = glm::vec3(px,py,pz);
                key = fac->createPickableItem(Node, Position);
            }

            str = "assets/BinaryFiles/BinaryModels/"+std::to_string(toggleObj)+".modelgg";
            Position = glm::vec3(tx,ty,tz);
            Rotation = glm::quat(trw,trx,tory,trz);
            glm::vec3 Vel(0.0,0.0542,0.0);

            uint16_t NewToggle;
            // INTERRUPTOR
            if(hasPickable){
                NewToggle = fac->createTouchableObject(Node, str,Position,Rotation,NewEntity,Vel,6000,1,key);
            }
            else{
                NewToggle = fac->createTouchableObject(Node, str,Position,Rotation,NewEntity,Vel,6000,2,key);
            }

            if(toggleHasCollider){
                CBoxCollider* RIGID = new CBoxCollider(tcx, tcy, tcz, tcrx,tcry,tcrz,tcrw, tcsx/2, tcsy/2, tcsz/2);
                Manager->addComponentToEntity(RIGID, gg::BOXCOLLIDER, NewToggle);
            }

            //std::cout << '\n';
        }
    }
}

void BinaryParser::LoadLevelLights(const std::string &DARTA, int8_t map_zone){
    std::ifstream inStream(DARTA, std::ios::binary);

    if(!inStream.is_open()){
        //std::cout << "No se pudo abrir " << DARTA << '\n';
        return;
    }

    Omicron* Engine = Singleton<Omicron>::Instance();
    uint8_t POINT_LIGHTS = 0;
    GG_Read(inStream, POINT_LIGHTS);
    for(uint8_t i = 0; i < POINT_LIGHTS; ++i){

        glm::vec3 Position = glm::vec3();
        GG_Read(inStream, Position);

        float Intensity;
        GG_Read(inStream, Intensity);

        glm::vec3 Color;
        GG_Read(inStream, Color);

        Engine->createStaticPointLight(Color, Position, Intensity, map_zone);
    }

    uint8_t SPOT_LIGHTS = 0;

    GG_Read(inStream, SPOT_LIGHTS);
    for(uint8_t i = 0; i < SPOT_LIGHTS; ++i){

        glm::vec3 Position = glm::vec3();
        GG_Read(inStream, Position);

        glm::vec3 Direction = glm::vec3();
        GG_Read(inStream, Direction);

        float Intensity;
        GG_Read(inStream, Intensity);

        glm::vec3 Color;
        GG_Read(inStream, Color);

        Engine->createStaticSpotLight(Color, Position, Direction, Intensity, map_zone);
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
    if (!Model.is_open()) return false;

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

bool BinaryParser::FillBuffers(
    const std::string& BinaryFile,
    unsigned int THE_BUFFER,
    unsigned int IndexBuffer,
    unsigned int &IndexSize
){
    std::ifstream Model(BinaryFile, std::ios::binary);
    if (!Model.is_open()) return false;

    uint16_t i = 0;
    GG_Read(Model, i);  // POSITIONS / NORMALS / UV / TANGENTS
                        //     X Y Z | X Y Z | U V | X Y Z
    glNamedBufferStorage(THE_BUFFER, i*sizeof(float), nullptr, GL_MAP_WRITE_BIT);
    char* ptr = (char*)glMapNamedBuffer(THE_BUFFER, GL_WRITE_ONLY);
    Model.read(ptr, i*sizeof(float));
    glUnmapNamedBuffer(THE_BUFFER);

    GG_Read(Model, i);  // INDEX_SIZE
    IndexSize = i;
    glNamedBufferStorage(IndexBuffer, i*sizeof(unsigned short), nullptr, GL_MAP_WRITE_BIT);
    ptr = (char*)glMapNamedBuffer(IndexBuffer, GL_WRITE_ONLY);
    Model.read(ptr, i*sizeof(unsigned short));
    glUnmapNamedBuffer(IndexBuffer);

    return true;
}

void BinaryParser::ReadLoadZonesData(const std::string &BinaryFile){
    std::ifstream inStream(BinaryFile, std::ios::binary);
    CTriggerSystem* TS = Singleton<CTriggerSystem>::Instance();

    uint8_t TOTAL;
    GG_Read(inStream,TOTAL);
    //std::cout << "TOTAL LOADERS: " << static_cast<int>(TOTAL) << '\n';

    for(int i=0 ; i<TOTAL ; ++i){
        uint8_t ZONE;
        GG_Read(inStream,ZONE);

        float x,y,z;
        GG_Read(inStream,x);
        GG_Read(inStream,y);
        GG_Read(inStream,z);
        glm::vec3 Position(x,y,z);
        
        TData mes;
        mes.add(kDat_LoadThatZone,ZONE);
        TS->RegisterTriger(kTrig_LoadZone,1,0,Position, 15, 0, false, mes);
        //std::cout << " - LOAD ZONE " << static_cast<int>(ZONE) << " ON " << glm::to_string(Position) << '\n';
    }
}

void BinaryParser::ReadUnLoadZonesData(const std::string &BinaryFile){
    std::ifstream inStream(BinaryFile, std::ios::binary);
    CTriggerSystem* TS = Singleton<CTriggerSystem>::Instance();

    uint8_t TOTAL;
    GG_Read(inStream,TOTAL);
    //std::cout << "TOTAL UNLOADERS: " << static_cast<int>(TOTAL) << '\n';

    for(int i=0 ; i<TOTAL ; ++i){
        uint8_t ZONE;
        GG_Read(inStream,ZONE);

        float x,y,z;
        GG_Read(inStream,x);
        GG_Read(inStream,y);
        GG_Read(inStream,z);
        glm::vec3 Position(x,y,z);

        TData mes;
        mes.add(kDat_LoadThatZone,ZONE);
        TS->RegisterTriger(kTrig_UnLoadZone,1,0,Position, 8, 0, false, mes);
        //std::cout << " - UNLOAD ZONE " << static_cast<int>(ZONE) << " ON " << glm::to_string(Position) << '\n';
    }
}

uint16_t BinaryParser::ReadRespawnNodesData(const std::string &BinaryFile){
    std::ifstream inStream(BinaryFile, std::ios::binary);
    Factory *fac = Singleton<Factory>::Instance();
    AIDirector *Director = Singleton<AIDirector>::Instance();

    uint8_t TOTAL;
    GG_Read(inStream,TOTAL);

    // std::cout << "TOTAL NODES: " << static_cast<int>(TOTAL) << '\n';

    float x,y,z;
    GG_Read(inStream,x);
    GG_Read(inStream,y);
    GG_Read(inStream,z);

    //posicion jugador
    glm::vec3 Position(x,y,z);
    // glm::vec3 Position(331.092,-43.1192,70.0263);
    uint16_t HERO = fac->createHero(Position);
/*
    std::vector<AINode*> nodes;
    nodes.reserve(TOTAL);

    for(int i=0 ; i<TOTAL ; ++i){
        GG_Read(inStream,x);
        GG_Read(inStream,y);
        GG_Read(inStream,z);
        Position = glm::vec3(x,y,z);

        nodes.emplace_back(Director->createNode(Position,5));
    }

    auto it = nodes.begin();
    while(it != nodes.end()){
        auto it2 = nodes.begin();

        while(it2 != nodes.end()){
            if(it != it2)   (*it2)->addNode(*it);
            ++it2;
        }
        ++it;
    }

    Director->init();
    nodes.clear();
    */

    return HERO;
}

void BinaryParser::ReadNatureData(const std::string &BinaryFile){
    std::ifstream inStream(BinaryFile, std::ios::binary);
    // Factory *fac = Singleton<Factory>::Instance();
    NatureGenerator nat;

    uint8_t TOTAL,TOTAL_ZONES,ZONE_NUMBER;
    GG_Read(inStream,TOTAL_ZONES);
    // std::cout << "TOTAL_ZONES: " << static_cast<int>(TOTAL_ZONES) << '\n';

    float x,y,z;
    glm::vec3 Position;
    for(int i=0 ; i<TOTAL_ZONES ; ++i){
        GG_Read(inStream,ZONE_NUMBER);
        GG_Read(inStream,TOTAL);
        // std::cout << " | ZONE_NUMBER: " << static_cast<int>(ZONE_NUMBER) << '\n';
        // std::cout << "   | TOTAL_SPAWNS: " << static_cast<int>(TOTAL) << '\n';

        for(int j=0 ; j<TOTAL ; ++j){

            GG_Read(inStream,x);
            GG_Read(inStream,y);
            GG_Read(inStream,z);

            Position = glm::vec3(x,y,z);
            nat.init(ZONE_NUMBER,Position);
            // std::cout << "     | POSITION: " << glm::to_string(Position) << '\n';

        }
    }
}


void BinaryParser::LoadParticleSystem(ParticleSystem_Data &PS, const std::string BinaryFile){
    std::ifstream inStream(BinaryFile, std::ios::binary);

    glm::vec3 Position;
    GG_Read(inStream, Position);
    glm::vec3 Size;
    GG_Read(inStream, Size);

    PS.Position = Position;
    PS.Size = Size;
}

void BinaryParser::MENU_getCameraPositionRotation(const std::string &File, glm::vec3 &Position, glm::vec3 &Target) {
    std::ifstream inStream(File, std::ios::binary);

    GG_Read(inStream, Position);
    GG_Read(inStream, Target);
}

// void BinaryParser::loadMaterials(std::string DATA){
//     std::ifstream inStream(DATA, std::ios::binary);
//
//     AssetManager* Manager = Singleton<AssetManager>::Instance();
//
//     uint8_t SIZE = 0;
//     GG_Read(inStream, SIZE);
//     Manager->reserveMaterials(SIZE);
//
//     for(uint16_t i = 0; i < SIZE; ++i){
//         uint8_t HD, ND, ED, HN, NN, EN, HS, NS, ES;
//         HD = ND = ED = HN = NN = EN = HS = NS = ES = 0;
//         uint8_t ID = 0;
//         GG_Read(inStream, ID);
//
//         GG_Read(inStream, HD);
//         if(HD){
//             GG_Read(inStream, ND);
//             GG_Read(inStream, ED);
//         }
//         GG_Read(inStream, HN);
//         if(HN){
//             GG_Read(inStream, NN);
//             GG_Read(inStream, EN);
//         }
//         GG_Read(inStream, HS);
//         if(HS){
//             GG_Read(inStream, NS);
//             if(NS){
//                 GG_Read(inStream, ES);
//             }
//         }
//
//         Manager->addMaterial(ID, HD, ND, ED, HN, NN, EN, HS, NS, ES);
//     }
//
// }
