#include "NavmeshParser.hpp"
#include <iostream>

int main(int argc, char const *argv[]) {
    NavmeshParser::generateBinaryGG_Navmesh("assets/NavMeshes/PROTOTIPO_CIUDAD.obj", "NavmeshCITY.gg");
    return 0;
}
