#ifndef NATURE_GENERATOR_HPP
#define NATURE_GENERATOR_HPP

#include <vector>
#include <array>
#include <GL/glew.h>
#include <cstdint>
#include <random>

#include "Factory.hpp"
#include "Singleton.hpp"
#include "BinaryParser.hpp"
#include "AssetManager.hpp"
#include "Shader.hpp"

class NatureGenerator {
    public:
        NatureGenerator();
        ~NatureGenerator();

        void init(const glm::vec3 &Pos);
        void update();
    private:
        AssetManager *Manager;
        Factory *fabrik;

        // float *tmp_positions;
        int GS_SIZE;
};

#endif