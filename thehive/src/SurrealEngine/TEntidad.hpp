#ifndef TENTIDAD_H
#define TENTIDAD_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <SurrealEngine/Shader.hpp>
#include <stack>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <cstdint>

#include <iostream>

class TEntidad {
    public:
        virtual ~TEntidad()         = 0;
        virtual void beginDraw(const uint8_t&)    = 0;
        virtual void endDraw(const uint8_t&)      = 0;

    protected:
        static std::stack<glm::mat4> matrixStack;

        static glm::mat4 modelMatrix;
        static glm::mat4 projMatrix;
        static glm::mat4 viewMatrix;

};

#endif
