#include "Omicron.hpp"
#include <glm/gtx/matrix_decompose.hpp>
#include <iostream>
#include <Omicron/FX/ParticleSystem.hpp>
#include <Singleton.hpp>


bool* Omicron::KEYS = new bool[349];
bool Omicron::LCLICK = false;
int Omicron::wheel;
int Omicron::IdButon;

Omicron::Omicron()
:main_camera(nullptr), FPS(0), _DeferredShading(), WINDOW_WIDTH(0), WINDOW_HEIGHT(0)
{
    ESCENA = new TNodo();
    Initialize();
    gestorRecursos = Singleton<AssetManager>::Instance();

    OKAMERAS_LAYER  = new TNodo(ESCENA, nullptr);
      LIGHTS_LAYER  = new TNodo(ESCENA, nullptr);
     BUFFERS_LAYER  = new TNodo(ESCENA, nullptr);

    for(uint16_t i = 0; i < 349; ++i)
        Omicron::KEYS[i] = false;
}

Omicron::~Omicron(){}

void Omicron::createZones(uint8_t NumberOfZones){
    NumberOfZones += 1;
    ZONES.reserve(NumberOfZones);
    ZONES.resize(NumberOfZones);
    for(uint8_t i = 0; i < NumberOfZones; ++i)
        ZONES[i] = new TNodo(BUFFERS_LAYER, nullptr);
}

void Omicron::resetSceneGraph() {
    ZONES.clear();
    delete ESCENA;

    ESCENA = new TNodo();
    OKAMERAS_LAYER  = new TNodo(ESCENA, nullptr);
      LIGHTS_LAYER  = new TNodo(ESCENA, nullptr);
     BUFFERS_LAYER  = new TNodo(ESCENA, nullptr);
}


void Omicron::clean(){
    //std::cout << "DeleTAG..." << '\n';
    ZONES.clear();
    delete Omicron::KEYS;
    delete ESCENA;
    glfwTerminate();
}

void Omicron::DisplayFPS(){
    if(FPS_Clock.ElapsedTime().Seconds() > 1){
        std::string TEXT = "The Hive - ALPHA FPS: " + std::to_string(FPS);
        glfwSetWindowTitle(window, TEXT.c_str());
        //std::cout << "FPS: " << FPS << '\n';
        FPS = 0;
        FPS_Clock.Restart();
    }
}

void Omicron::Draw3DLine(const glm::vec3 &From, const glm::vec3 &To, const gg::Color &c){
    // glm::mat4 MVP = ESCENA->getEntidad()->projMatrix * ESCENA->getEntidad()->viewMatrix;
    // Singleton<Debug>::Instance()->DroLine(From, To, c, MVP);
}


void Omicron::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if(action == GLFW_PRESS)
        KEYS[key] = true;
    else if(action == GLFW_RELEASE)
        KEYS[key] = false;
}

// void Omicron::resetClicked(){
//     clicked=false;
// }
// bool Omicron::isLClicked(){
//     return clicked;
// }

void Omicron::resetClickVariable(){
    LCLICK = false;
}

void Omicron::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
        LCLICK = true;
    }
    else if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE){
        LCLICK = false;
    }
}

void Omicron::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    wheel = yoffset;
}

void Omicron::PollEvents()     {   glfwPollEvents();  }

void Omicron::getCursorPosition(double &posX, double &posY) {  glfwGetCursorPos(window, &posX, &posY); }

TNodo* Omicron::crearCamara(const float& _fov, const float& _near, const float& _far, const glm::vec3& pos, const glm::vec3& rot, const float& _ppv){
    TCamara* C = new TCamara(_fov,_near,_far);
    C->setPerspectiva(_ppv);

    TNodo* Cam = new TNodo(bindTransform(pos,rot, OKAMERAS_LAYER),C);

    main_camera = Cam;
    cam_ = C;
    return Cam;
}

TNodo* Omicron::crearLuz(gg::Color &_color, const glm::vec3& pos, const glm::vec3& rot, Shader* sh){
    TLuz* L = new TLuz(_color,sh);
    TNodo* Luz = new TNodo(bindTransform(pos,rot, LIGHTS_LAYER),L);

    return Luz;
}

TNodo* Omicron::createStaticMesh(const char* _path, const glm::vec3& pos, const glm::quat &Rotation, int8_t map_zone, const std::string& BoundingBoxPath){

    TTransform T_Position;
    TTransform T_Rotation;
    T_Position.setPosition(pos);
    T_Rotation.setRotation(Rotation);

    glm::mat4 Model = T_Position.matrix * T_Rotation.matrix;

    ZStaticMesh* M = new ZStaticMesh(Model);
    M->load(_path);
    M->loadBoundingBox(BoundingBoxPath);

    TNodo* PADRE = ZONES[map_zone];
    TNodo* Malla = new TNodo(PADRE, M);

    return Malla;
}

TNodo* Omicron::createMovableMesh(const char* _path, const glm::vec3& pos, const glm::quat &Rotation, int8_t map_zone, const std::string& BoundingBoxPath){
    ZMovableMesh* M = new ZMovableMesh();
    M->load(_path);
    M->loadBoundingBox(BoundingBoxPath);

    TNodo* PADRE = ZONES[map_zone];
    TNodo* Malla = new TNodo(bindTransform(pos,Rotation, PADRE),M);

    return Malla;
}

TNodo* Omicron::CreateDynamicMesh(const glm::vec3& Position, const glm::quat& Rotation, int8_t map_zone, const std::string& BoundingBoxPath){
    ZDynamicMesh* M = new ZDynamicMesh();

    TNodo* PADRE = ZONES[map_zone];
    TNodo* Malla = new TNodo(bindTransform(Position, Rotation, PADRE), M);

    return Malla;
}

TNodo* Omicron::CreateParticleSystem(const ParticleSystem_Data &Data, int8_t map_zone){
    ParticleSystem* P = new ParticleSystem();

    P->Init(Data.MaxParticles);
    P->setGenerationTime(Data.SpawnTime);
    P->setTexture(Data.Texture);

    TNodo* PADRE = ZONES[map_zone];
    TNodo* ParticleNode = new TNodo(PADRE, P);

    return ParticleNode;
}



TNodo* Omicron::bindTransform(const glm::vec3& pos, const glm::quat& rot, TNodo* FATHER){
    TTransform* Rotate = new TTransform();
    TTransform* Translate = new TTransform();

    Rotate->setRotation(rot);
    Translate->setPosition(pos);

    TNodo* NodoRot = new TNodo(FATHER,Rotate);
    TNodo* NodoTrans = new TNodo(NodoRot,Translate);

    return NodoTrans;
}


bool Omicron::bindMaterialToMesh(TNodo *_mesh, ZMaterial* Material){
    ZStaticMesh*    O   = static_cast<ZStaticMesh*>(_mesh->getEntidad());
	O->assignMaterial(Material);

    return true;
}

bool Omicron::bindMaterialToDynamicMesh(TNodo *_mesh, ZMaterial* Material){
    ZDynamicMesh*    O   = static_cast<ZDynamicMesh*>(_mesh->getEntidad());
	O->assignMaterial(Material);

    return true;
}

void Omicron::BeginDraw(){
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

void Omicron::draw(){
    ++FPS;

    glDisable( GL_BLEND );

    // Bindeamos el shader que recibe la info de luces y cámara
    _DeferredShading.Bind_D_Shader();
    ESCENA->ROOT_OkameraUpdate();   // Le enviamos la info
    ESCENA->ROOT_LightsUpdate();

    // Ahora bindeamos nuestro G-Búffer y renderizamos a las texturas
    _DeferredShading.Bind_G_Buffer();
    glViewport(0,0,1280, 720);
    ESCENA->drawRoot_M();
    _DeferredShading.DrawQuad();
    glUniform1f(7, FPS/60.f);
    glViewport(0,0,WINDOW_WIDTH, WINDOW_HEIGHT);
    _DeferredShading.DrawPostProcessing();
}

void Omicron::EndDraw(){
    glfwSwapBuffers(window);
}

void Omicron::setPosition(TNodo* _node, const glm::vec3& _offpos){
    static_cast<TTransform*>(_node->getPadre()->getEntidad())->setPosition(_offpos);
}

void Omicron::setRotation(TNodo* _node,const glm::quat& _offrot){
    static_cast<TTransform*>(_node->getPadre()->getPadre()->getEntidad())->setRotation(_offrot);
}
glm::vec3 Omicron::vectorUp(){
    auto v=ESCENA->getEntidad()->viewMatrix;
    return glm::vec3(v[0][1], v[1][1], v[2][1]);
}

glm::vec3 Omicron::vectorRigth(){
    auto v=ESCENA->getEntidad()->viewMatrix;
    return glm::vec3(v[0][0], v[1][0], v[2][0]);
}

glm::mat4  Omicron::getMVP(){
    return  ESCENA->getEntidad()->projMatrix * ESCENA->getEntidad()->viewMatrix * ESCENA->getEntidad()->modelMatrix;
}
glm::mat4  Omicron::getVP(){
    return  ESCENA->getEntidad()->projMatrix * ESCENA->getEntidad()->viewMatrix;
}
glm::mat4  Omicron::getV(){
    return  ESCENA->getEntidad()->viewMatrix;
}
glm::mat4  Omicron::getM(){
    return ESCENA->getEntidad()->modelMatrix;
}
TCamara* Omicron::getCam(){
    return cam_;
}

// void Omicron::PointAt(TNodo *_node, const glm::vec3& _offpos){
//     auto trans =static_cast<TTransform*>(_node->getPadre()->getEntidad());
//     auto dir=glm::normalize(glm::vec3(_offpos.x,_offpos.y,_offpos.z)-trans->getDatos());
//
//     auto x=glm::degrees(glm::acos(dir.x));
//     auto y=glm::degrees(glm::acos(dir.y));
//     auto z=glm::degrees(glm::acos(dir.z));
//     //la condicion puede invertirse
//     if(dir.x<0){
//         x=360-x;
//     }
//     if(dir.y<0){
//         y=360-y;
//     }
//     if(dir.z<0){
//         z=360-z;
//     }
//     setRotation(_node, glm::vec3(x,y,z));
// }


void Omicron::HideCursor(bool t){
    if(t){
        glfwSetInputMode(window,  GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }else{
        glfwSetInputMode(window,  GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

void Omicron::close(){
    glfwSetWindowShouldClose(window, GL_TRUE);
}

bool Omicron::Initialize(){
	//INICIALIZAMOS GLFW
	if( !glfwInit() ){
	    //fprintf( stderr, "Error al inicializar GLFW\n" );
	    return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Queremos OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Para hacer feliz a MacOS ; Aunque no debería ser necesaria
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //No queremos el viejo OpenGL

    auto mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    WINDOW_WIDTH = mode->width;
    WINDOW_HEIGHT = mode->height;

	window = glfwCreateWindow(static_cast<int>(WINDOW_WIDTH), static_cast<int>(WINDOW_HEIGHT), "The Hive - ALPHA", NULL, NULL);
	if( window == NULL ){
	    glfwTerminate();
	    return false;
	}


    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);

	glfwMakeContextCurrent(window);
	glewExperimental=true;
	if (glewInit() != GLEW_OK) {
	    //fprintf(stderr, "Falló al inicializar GLEW\n");
	    return false;
	}

    _DeferredShading.init(1280, 720);

    glDepthRange(0.f,1.f);

	glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);
    glCullFace (GL_BACK);

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetInputMode(window,      GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glClearColor(0, 0, 0, 1.0f);

	return true;
}

void Omicron::deleteLeafNode(TNodo *node){
    TNodo *tmp = node->getPadre()->getPadre();
    TNodo *FATHER = tmp->getPadre();
    FATHER->remHijo(tmp);
}

void Omicron::SetMapZoneVisibility(const int8_t &zone,const bool &flag){
    ZONES[zone]->setVisibility(flag);
}
