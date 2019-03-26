#include "AssetManager.hpp"
#include <iostream>
#include <SOIL2/SOIL2.h>

AssetManager::AssetManager(){
    ShaderMap["Nature"].loadFiles("assets/Shaders/VertexShader.glsl", nullptr, "assets/Shaders/FragmentShader_NATURE.glsl");
    ShaderMap["Default"].loadFiles("assets/Shaders/VertexShader.glsl", nullptr, "assets/Shaders/FragmentShader.glsl");
    ShaderMap["Lines"].loadFiles("assets/Shaders/Line_VS.glsl", nullptr, "assets/Shaders/Line_FS.glsl");
    ShaderMap["2D"].loadFiles("assets/Shaders/VertexShader2D.glsl", nullptr, "assets/Shaders/FragmentShader2D.glsl");
    ShaderMap["Bill"].loadFiles("assets/Shaders/VertexShaderBillboard.glsl", nullptr, "assets/Shaders/FragmentShader2D.glsl");
    ShaderMap["Blend"].loadFiles("assets/Shaders/VertexShaderblend.glsl", nullptr, "assets/Shaders/FragmentShaderblend.glsl");
    ShaderMap["Plano"].loadFiles("assets/Shaders/VertexShader2DPlano.glsl", nullptr, "assets/Shaders/FragmentShader2DPlano.glsl");
    ShaderMap["skyboxShader"].loadFiles("assets/Shaders/SkyBox.vs", nullptr, "assets/Shaders/SkyBox.frag");
    ShaderMap["AnimationShader"].loadFiles("assets/Shaders/Animation.vs", nullptr, "assets/Shaders/FragmentShader.glsl");
}

void AssetManager::loadInit(){
    Shader* shader = getShader("Default");
    ZMaterial* 		MAT = getMaterial("Default");
    MAT->attachShader(shader);
    MAT->addTexture(GN::DIFFUSE_MAP, "assets/Textures/DefaultDiffuse.jpg",      GN::RGBA, GN::REPEAT_TEXTURE | GN::GEN_MIPMAPS);
    MAT->addTexture(GN::NORMAL_MAP, "assets/Textures/DefaultNormal.jpg",        GN::RGBA, GN::REPEAT_TEXTURE | GN::GEN_MIPMAPS);
    MAT->addTexture(GN::SPECULAR_MAP, "assets/Textures/DefaultSpecular.jpeg",   GN::RGBA, GN::REPEAT_TEXTURE | GN::GEN_MIPMAPS);

    MAT = getMaterial("Morado");
    MAT->attachShader(shader);
    MAT->addTexture(GN::DIFFUSE_MAP,      "assets/Textures/prueba1.png",       		GN::RGBA, GN::REPEAT_TEXTURE | GN::GEN_MIPMAPS);
    MAT->addTexture(GN::NORMAL_MAP,       "assets/Textures/COMOUNPUTOPRO3.png",        GN::RGBA, GN::REPEAT_TEXTURE | GN::GEN_MIPMAPS);
    MAT->addTexture(GN::SPECULAR_MAP,     "assets/Textures/DefaultSpecular.jpeg",      GN::RGBA, GN::REPEAT_TEXTURE | GN::GEN_MIPMAPS);

    ZMaterial* 		Blue = getMaterial("Blue");
    Blue->attachShader(shader);
    Blue->addTexture(GN::DIFFUSE_MAP,      "assets/Textures/Blue.png",       		     GN::RGBA, GN::REPEAT_TEXTURE | GN::GEN_MIPMAPS);
    Blue->addTexture(GN::NORMAL_MAP,       "assets/Textures/DefaultNormal.jpg",        GN::RGBA, GN::REPEAT_TEXTURE | GN::GEN_MIPMAPS);
    Blue->addTexture(GN::SPECULAR_MAP,     "assets/Textures/DefaultSpecular.jpeg",      GN::RGBA, GN::REPEAT_TEXTURE | GN::GEN_MIPMAPS);

    ZMaterial* 		Nav = getMaterial("Nav");
    Nav->attachShader(shader);
    Nav->addTexture(GN::DIFFUSE_MAP,      "assets/Textures/TEST.png",       		     GN::RGBA, GN::REPEAT_TEXTURE | GN::GEN_MIPMAPS);
    Nav->addTexture(GN::NORMAL_MAP,       "assets/Textures/DefaultNormal.jpg",         GN::RGBA, GN::REPEAT_TEXTURE | GN::GEN_MIPMAPS);
    Nav->addTexture(GN::SPECULAR_MAP,     "assets/Textures/DefaultSpecular.jpeg",      GN::RGBA, GN::REPEAT_TEXTURE | GN::GEN_MIPMAPS);

    ZMaterial* 		Hero = getMaterial("Hero");
    Hero->attachShader(shader);
    Hero->addTexture(GN::DIFFUSE_MAP,      "assets/Textures/DefaultDiffuse.jpg",        GN::RGBA, GN::REPEAT_TEXTURE | GN::GEN_MIPMAPS);
    Hero->addTexture(GN::NORMAL_MAP,       "assets/Textures/HERO_NORMALS2.png",         GN::RGBA, GN::INVERT_Y | GN::REPEAT_TEXTURE | GN::GEN_MIPMAPS);
    Hero->addTexture(GN::SPECULAR_MAP,     "assets/Textures/DefaultSpecular.jpeg",      GN::RGBA, GN::REPEAT_TEXTURE | GN::GEN_MIPMAPS);

    ZMaterial* 		Metal = getMaterial("Metal");
    Metal->attachShader(shader);
    Metal->addTexture(GN::DIFFUSE_MAP,      "assets/Textures/metal.jpg",        GN::RGBA, GN::REPEAT_TEXTURE | GN::GEN_MIPMAPS);
    Metal->addTexture(GN::NORMAL_MAP,       "assets/Textures/DefaultNormal.png",         GN::RGBA, GN::INVERT_Y | GN::REPEAT_TEXTURE | GN::GEN_MIPMAPS);
    Metal->addTexture(GN::SPECULAR_MAP,     "assets/Textures/DefaultSpecular.jpeg",      GN::RGBA, GN::REPEAT_TEXTURE | GN::GEN_MIPMAPS);

    ZMaterial* 		Tree = getMaterial("Tree");
    Tree->attachShader(shader);
    Tree->addTexture(GN::DIFFUSE_MAP,      "assets/Textures/Tree.jpg",        GN::RGBA, GN::REPEAT_TEXTURE | GN::GEN_MIPMAPS);
    Tree->addTexture(GN::NORMAL_MAP,       "assets/Textures/DefaultNormal.png",         GN::RGBA, GN::INVERT_Y | GN::REPEAT_TEXTURE | GN::GEN_MIPMAPS);
    Tree->addTexture(GN::SPECULAR_MAP,     "assets/Textures/DefaultSpecular.jpeg",      GN::RGBA, GN::REPEAT_TEXTURE | GN::GEN_MIPMAPS);

    ZMaterial* 		Grey = getMaterial("Grey");
    Grey->attachShader(shader);
    Grey->addTexture(GN::DIFFUSE_MAP,      "assets/Textures/grey.jpg",        GN::RGBA, GN::REPEAT_TEXTURE | GN::GEN_MIPMAPS);
    Grey->addTexture(GN::NORMAL_MAP,       "assets/Textures/DefaultNormal.png",         GN::RGBA, GN::INVERT_Y | GN::REPEAT_TEXTURE | GN::GEN_MIPMAPS);
    Grey->addTexture(GN::SPECULAR_MAP,     "assets/Textures/DefaultSpecular.jpeg",      GN::RGBA, GN::REPEAT_TEXTURE | GN::GEN_MIPMAPS);

    ZMaterial* 		Red = getMaterial("Red");
    Red->attachShader(shader);
    Red->addTexture(GN::DIFFUSE_MAP,      "assets/Textures/red.jpg",        GN::RGBA, GN::REPEAT_TEXTURE | GN::GEN_MIPMAPS);
    Red->addTexture(GN::NORMAL_MAP,       "assets/Textures/DefaultNormal.png",         GN::RGBA, GN::INVERT_Y | GN::REPEAT_TEXTURE | GN::GEN_MIPMAPS);
    Red->addTexture(GN::SPECULAR_MAP,     "assets/Textures/DefaultSpecular.jpeg",      GN::RGBA, GN::REPEAT_TEXTURE | GN::GEN_MIPMAPS);

    ZMaterial* 		White = getMaterial("White");
    White->attachShader(shader);
    White->addTexture(GN::DIFFUSE_MAP,      "assets/Textures/red.jpg",        GN::RGBA, GN::REPEAT_TEXTURE | GN::GEN_MIPMAPS);
    White->addTexture(GN::NORMAL_MAP,       "assets/Textures/DefaultNormal.png",         GN::RGBA, GN::INVERT_Y | GN::REPEAT_TEXTURE | GN::GEN_MIPMAPS);
    White->addTexture(GN::SPECULAR_MAP,     "assets/Textures/DefaultSpecular.jpeg",      GN::RGBA, GN::REPEAT_TEXTURE | GN::GEN_MIPMAPS);

    ZMaterial* 		Ground = getMaterial("Ground");
    Ground->attachShader(shader);
    Ground->addTexture(GN::DIFFUSE_MAP,      "assets/Textures/ground.jpg",        GN::RGBA, GN::REPEAT_TEXTURE | GN::GEN_MIPMAPS);
    Ground->addTexture(GN::NORMAL_MAP,       "assets/Textures/DefaultNormal.png",         GN::RGBA, GN::INVERT_Y | GN::REPEAT_TEXTURE | GN::GEN_MIPMAPS);
    Ground->addTexture(GN::SPECULAR_MAP,     "assets/Textures/DefaultSpecular.jpeg",      GN::RGBA, GN::REPEAT_TEXTURE | GN::GEN_MIPMAPS);

    ZMaterial* 		Farola = getMaterial("Farola");
    Farola->attachShader(shader);
    Farola->addTexture(GN::DIFFUSE_MAP,      "assets/Textures/farola1.png",        GN::RGBA, GN::REPEAT_TEXTURE | GN::GEN_MIPMAPS);
    Farola->addTexture(GN::NORMAL_MAP,       "assets/Textures/DefaultNormal.png",         GN::RGBA, GN::INVERT_Y | GN::REPEAT_TEXTURE | GN::GEN_MIPMAPS);
    Farola->addTexture(GN::SPECULAR_MAP,     "assets/Textures/DefaultSpecular.jpeg",      GN::RGBA, GN::REPEAT_TEXTURE | GN::GEN_MIPMAPS);

    ZMaterial* 		CarRed = getMaterial("CarRed");
    CarRed->attachShader(shader);
    CarRed->addTexture(GN::DIFFUSE_MAP,      "assets/Textures/carred.png",        GN::RGBA, GN::REPEAT_TEXTURE | GN::GEN_MIPMAPS);
    CarRed->addTexture(GN::NORMAL_MAP,       "assets/Textures/DefaultNormal.png",         GN::RGBA, GN::INVERT_Y | GN::REPEAT_TEXTURE | GN::GEN_MIPMAPS);
    CarRed->addTexture(GN::SPECULAR_MAP,     "assets/Textures/DefaultSpecular.jpeg",      GN::RGBA, GN::REPEAT_TEXTURE | GN::GEN_MIPMAPS);

    ZMaterial* 		CarGreen = getMaterial("CarGreen");
    CarGreen->attachShader(shader);
    CarGreen->addTexture(GN::DIFFUSE_MAP,      "assets/Textures/cargreen.png",        GN::RGBA, GN::REPEAT_TEXTURE | GN::GEN_MIPMAPS);
    CarGreen->addTexture(GN::NORMAL_MAP,       "assets/Textures/DefaultNormal.png",         GN::RGBA, GN::INVERT_Y | GN::REPEAT_TEXTURE | GN::GEN_MIPMAPS);
    CarGreen->addTexture(GN::SPECULAR_MAP,     "assets/Textures/DefaultSpecular.jpeg",      GN::RGBA, GN::REPEAT_TEXTURE | GN::GEN_MIPMAPS);

    ZMaterial* 		CarYellow = getMaterial("CarYellow");
    CarYellow->attachShader(shader);
    CarYellow->addTexture(GN::DIFFUSE_MAP,      "assets/Textures/caryellow.png",        GN::RGBA, GN::REPEAT_TEXTURE | GN::GEN_MIPMAPS);
    CarYellow->addTexture(GN::NORMAL_MAP,       "assets/Textures/DefaultNormal.png",         GN::RGBA, GN::INVERT_Y | GN::REPEAT_TEXTURE | GN::GEN_MIPMAPS);
    CarYellow->addTexture(GN::SPECULAR_MAP,     "assets/Textures/DefaultSpecular.jpeg",      GN::RGBA, GN::REPEAT_TEXTURE | GN::GEN_MIPMAPS);




    shader = getShader("Blend");

    ZMaterial* 		Build = getMaterial("building");
    Build->attachShader(shader);
    Build->addTexture(GN::BLEND1,      "assets/Textures/edif.png",       		     GN::RGBA, GN::REPEAT_TEXTURE | GN::GEN_MIPMAPS);
    Build->addTexture(GN::BLEND2,      "assets/Textures/edifluz.png",       		     GN::RGBA, GN::REPEAT_TEXTURE | GN::GEN_MIPMAPS);

    shader = getShader("AnimationShader");
    ZMaterial* Sold = getMaterial("Soldier");
    Sold->attachShader(shader);
    Sold->addTexture(GN::DIFFUSE_MAP,      "assets/Textures/DefaultDiffuse.jpg",        GN::RGBA, GN::REPEAT_TEXTURE | GN::GEN_MIPMAPS);
    Sold->addTexture(GN::NORMAL_MAP,       "assets/Textures/ALIEN.png",         GN::RGBA, GN::INVERT_Y | GN::REPEAT_TEXTURE | GN::GEN_MIPMAPS);
    Sold->addTexture(GN::SPECULAR_MAP,     "assets/Textures/DefaultSpecular.jpeg",      GN::RGBA, GN::REPEAT_TEXTURE | GN::GEN_MIPMAPS);


    ZAnimationData* Soldier = getAnimation("Soldier_Running");
    Soldier->addKeyframe("assets/BinaryFiles/BinaryModels/SoldierMov0.modelgg");
    Soldier->addKeyframe("assets/BinaryFiles/BinaryModels/SoldierMov1.modelgg");
    Soldier->addKeyframe("assets/BinaryFiles/BinaryModels/SoldierMov2.modelgg");
    Soldier->addKeyframe("assets/BinaryFiles/BinaryModels/SoldierMov3.modelgg");
    Soldier->addKeyframe("assets/BinaryFiles/BinaryModels/SoldierMov4.modelgg");
    Soldier->addKeyframe("assets/BinaryFiles/BinaryModels/SoldierMov5.modelgg");
}

ZMaterial* AssetManager::getMaterial(const std::string &Name) {

    auto it = MaterialMap.find(Name);
    if(it != MaterialMap.end()){
        return &it->second;
    }
    else {
        return &MaterialMap[Name];
    }
}

ZMeshData* AssetManager::getMeshData(const std::string &Name){

    auto it = MeshDataMap.find(Name);
    if(it != MeshDataMap.end())
        return &it->second;
    else{
        ZMeshData* ret = &MeshDataMap[Name];
        ret->load(Name);
        return ret;
    }

}

Shader* AssetManager::getShader(const std::string &Name) {

    auto it = ShaderMap.find(Name);
    if(it != ShaderMap.end())
        return &it->second;
    else{
        //std::cout << "  --No existe ningún Shader llamado '" << Name <<"', devolviendo 'Default'..." << '\n';
        return &ShaderMap["Default"];
    }
}

ZAnimationData* AssetManager::getAnimation(const std::string &Name){

    auto it = AnimationMap.find(Name);
    if(it != AnimationMap.end()){
        return &it->second;
    }
    else {
        return &AnimationMap[Name];
    }
}

Shader* AssetManager::createShader(std::string Name) {
    if(ShaderMap.find(Name) == ShaderMap.end())
        ShaderMap[Name];

    return &ShaderMap[Name];
}

unsigned int AssetManager::getTexture(const std::string &Name, unsigned int mode, unsigned int flags){
    if(TextureMap.find(Name) != TextureMap.end()){
        return TextureMap[Name];
    }
    else {
        unsigned int NewID = SOIL_load_OGL_texture(Name.c_str(), mode, SOIL_CREATE_NEW_ID, flags );
        TextureMap.insert(std::make_pair(Name, NewID));
        return NewID;
    }
}

AssetManager::~AssetManager(){
    //std::cout << "Deleting textures..." << '\n';
    auto iterator = TextureMap.begin();
    while(iterator != TextureMap.end()){
        glDeleteTextures(1, &iterator->second);
        ++iterator;
    }
}
