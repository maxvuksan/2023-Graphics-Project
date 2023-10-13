#include "AssetManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <SFML/OpenGL.hpp>

std::unordered_map<const char*, sf::Texture> AssetManager::textures; 
std::unordered_map<const char*, Scene*> AssetManager::scenes; /*scenes are stored as pointers to allow for inherited scenes*/
std::unordered_map<const char*, sf::Shader*> AssetManager::shaders;
std::unordered_map<const char*, AnimationSet*> AssetManager::animation_sets;
std::unordered_map<const char*, sf::SoundBuffer> AssetManager::sounds;

void AssetManager::Construct()
{
    CreateShader("Amber_Blur", "Amber/Shaders/Blur.frag");
    CreateShader("Amber_LightSource", "Amber/Shaders/LightSource.frag");

    // creating a shadow texture, to swap to when rendering shadows off a textured vertex buffer 
    sf::RenderTexture shadow_texture;
    shadow_texture.create(20,20);
    shadow_texture.clear(Globals::BASE_SHADOW_COLOUR);
    CreateTexture("Amber_Shadow", shadow_texture, true);

    // + white texture to override shadows when needed
    shadow_texture.clear(sf::Color(255,255,255));
    CreateTexture("Amber_White", shadow_texture, true);

    shadow_texture.clear(sf::Color(0,0,0));
    CreateTexture("Amber_Black", shadow_texture, true);
}

sf::Shader* AssetManager::CreateShader(const char* label, const std::string fragment_file_location){
        
    std::string shader_code;
    std::ifstream shader_file;

    try{
        shader_file.open(fragment_file_location);
        std::stringstream shader_stream;

        shader_stream << shader_file.rdbuf();
        shader_file.close();

        shader_code = shader_stream.str();
    }
    catch(std::ifstream::failure e){
        std::cout << "ERROR : Failed to read '" << label << "' shader source\n";
        return nullptr;
    }

    sf::Shader* new_shader = new sf::Shader;
    
    if(!new_shader->loadFromMemory(shader_code, sf::Shader::Fragment)){
        std::cout << "ERROR : Could not load shader '" << label << "' from '" << fragment_file_location <<"'\n";
        delete new_shader;
        return nullptr; 
    }
    
    shaders.insert(std::make_pair(label, new_shader));

    return shaders.find(label)->second;
}
sf::Shader* AssetManager::GetShader(const char* label){

    for(auto& result : shaders){

        if(strcmp(result.first, label) == 0){
            return result.second;
        }
    }

    std::cout << "ERROR : A shader with the label '" << label << "' could not be found\n";
    return nullptr;

}

AnimationSet* AssetManager::CreateAnimationSet(const char* label, std::vector<Animation> animations, std::vector<const char*> state_labels){
    animation_sets.insert(std::make_pair(label, new AnimationSet(animations, state_labels))); 
    return animation_sets[label];
}
AnimationSet* AssetManager::GetAnimationSet(const char* label){

    for(auto& result : animation_sets){

        if(strcmp(result.first, label) == 0){
            return result.second;
        }
    }

    std::cout << "ERROR : An AnimationSet with the label '" << label << "' could not be found\n";
    return nullptr;
}

sf::Texture* AssetManager::CreateTexture(const char* label, const char* file_location, bool repeat){
    sf::Texture texture;
    if (!texture.loadFromFile(file_location))
    {
        std::cout << "ERROR : Could not create texture " << label << " from location " << file_location << "\n";
        return nullptr;
    }
    texture.setRepeated(repeat);
    textures.insert(std::make_pair(label, texture));

    return &textures.find(label)->second;
}
sf::Texture* AssetManager::CreateTexture(const char* label, const sf::RenderTexture& render_texture, bool repeat){
    sf::Texture texture = render_texture.getTexture();
    texture.setRepeated(repeat);
    textures.insert(std::make_pair(label, texture));

    return &textures.find(label)->second;
}
sf::Texture* AssetManager::GetTexture(const char* label){
    for(auto& result : textures){

        if(strcmp(result.first, label) == 0){
            return &result.second;
        }
    }

    std::cout << "ERROR : A texture with the label '" << label << "' could not be found\n";
    return nullptr;
}

sf::SoundBuffer* AssetManager::CreateSound(const char* label, const char* file_location){
    sf::SoundBuffer sound_buf;
    if (!sound_buf.loadFromFile(file_location))
    {
        std::cout << "ERROR : Could not create sound buffer " << label << " from location " << file_location << "\n";
        return nullptr;
    }
    sounds.insert(std::make_pair(label, sound_buf));
    return &sounds.find(label)->second;
}
sf::SoundBuffer* AssetManager::GetSound(const char* label){
    
    for(auto& result : sounds){

        if(strcmp(result.first, label) == 0){
            return &result.second;
        }
    }

    std::cout << "ERROR : A sound buffer with the label '" << label << "' could not be found\n";
    return nullptr;
}

Scene* AssetManager::GetScene(const char* label){
   
    for(auto& result : scenes){

        if(strcmp(result.first, label) == 0){
            return result.second;
        }
    }
    
    std::cout << "ERROR : A scene with the label '" << label << "' could not be found\n";
    return nullptr;
}



void AssetManager::Destruct(){
    
    for (auto& scene : scenes) {
        delete scene.second;
    }

    for (auto& animation_s : animation_sets) {
        delete animation_s.second;
    }

    for (auto& shader : shaders) {
        delete shader.second;
    }
}