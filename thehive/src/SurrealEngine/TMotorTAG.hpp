#ifndef TMOTORTAG_H
#define TMOTORTAG_H

#include <map>
#include <cstdint>
#include <string>
#include <Util.hpp>

#include "TNodo.hpp"
#include "TEntidad.hpp"
#include "TLuz.hpp"
#include "TTransform.hpp"
#include "TCamara.hpp"
#include "Shader.hpp"
#include "ZStaticMesh.hpp"
#include "ZMaterial.hpp"
#include "AssetManager.hpp"
#include <SurrealEngine/KEYCODES.hpp>
#include "TCamara.hpp"

template <typename T>
class Singleton;

class TMotorTAG {
    friend class Singleton<TMotorTAG>;
    public:
        ~TMotorTAG();

        TNodo* crearCamara(const float&, const float&, const float&, const glm::vec3&, const glm::vec3&, const float&);
        TNodo* crearLuz(gg::Color&, const glm::vec3&, const glm::vec3&, Shader* sh);
        TNodo* crearMalla(const char*, const glm::vec3& = glm::vec3(), const glm::vec3& = glm::vec3());

        int8_t addShaderToMap(const char*,bool);
        bool bindMaterialToMesh(TNodo*,ZMaterial*);

        void move(TNodo*,const glm::vec3&);
        void rotate(TNodo*,const float&,const glm::vec3&);
        void setPosition(TNodo* _node, const glm::vec3& _offpos);
        void setRotation(TNodo* _node,const glm::vec3& _offrot);

        void BeginDraw();
        void draw();
        void EndDraw();

        void PollEvents();

        void PointAt(TNodo *_node, const gg::Vector3f& _offpos);
        inline bool isLClickPressed(){ return LCLICK; };
        inline int getWheelState(){return wheel;};
        inline bool isWindowOpen(){ return !glfwWindowShouldClose(window);};
        inline int getScreenWidth(){ return Half_Window_Width; };
        inline int getScreenHeight(){ return Half_Window_Height; };
        inline TNodo* getCamera(){return main_camera;};

        void getCursorPosition(double &posX, double &posY);
        void clean();

        void Draw3DLine(const glm::vec3 &From, const glm::vec3 &To, const gg::Color &c);

        inline bool key(gg::KEYCODES keyCode){ return KEYS[keyCode];};
        void print();
        bool Initialize();
        GLFWwindow* window;
    private:
        TMotorTAG();

        TNodo* ESCENA;
        AssetManager* gestorRecursos;

        TNodo* bindTransform(const glm::vec3& pos, const glm::vec3& rot);

        static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
        static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

        static bool* KEYS;
        static bool LCLICK;
        static int wheel;
        static int IdButon;

        static int Half_Window_Width;
        static int Half_Window_Height;

        TNodo* main_camera;
        TCamara* cam_;
};

#endif
