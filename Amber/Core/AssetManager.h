#pragma once
#include <SFML/Audio.hpp>
#include <unordered_map>
#include <iostream>
#include "../Utility/Memory.h"

#include "../Object/Rendering/AnimationSet.h"
#include "Scene.h"

/*
    responsible for storing resusable assets, preventing duplication 

    this could include

    Animations
    Scenes
    Textures
    etc...

*/
class Core;
class AssetManager {

    public:
        static void Construct();
        static void Destruct();

        static sf::Font* CreateFontObj(const char* label, const std::string file_location);
        static sf::Font* GetFont(const char* label);

        // assuming all shaders are fragment
        static sf::Shader* CreateShader(const char* label, const std::string fragment_file_location);
        static sf::Shader* GetShader(const char* label);

        static AnimationSet* CreateAnimationSet(const char* label, std::vector<Animation> animations, std::vector<const char*> state_labels);
        static AnimationSet* GetAnimationSet(const char* label);

        static sf::Texture* CreateTexture(const char* label, const sf::RenderTexture& render_texture, bool repeat = false);
        static sf::Texture* CreateTexture(const char* label, const char* file_location, bool repeat = false);
        static sf::Texture* GetTexture(const char* label);
        
        static sf::SoundBuffer* CreateSound(const char* label, const char* file_location);
        static sf::SoundBuffer* GetSound(const char* label);
        
        /*
            Creates a scene of the specified type T (allowing only classes which inherit Scene)
        */

        template <typename T>
        static Scene* CreateScene(const char* label) {

            Scene* new_scene = Memory::New<T>(__FUNCTION__);

            scenes.insert(std::make_pair(label, new_scene));

            return new_scene;
        }

        /*
            @returns A pointer to a desired scene
            @param label The name the scene is saved as  
        */
        static Scene* GetScene(const char* label); 

    protected:
        static Core* core;

        static std::unordered_map<const char*, sf::Font> fonts;
        static std::unordered_map<const char*, sf::Texture> textures; 
        static std::unordered_map<const char*, Scene*> scenes; /*scenes are stored as pointers to allow for inherited scenes*/
        static std::unordered_map<const char*, sf::Shader*> shaders;
        static std::unordered_map<const char*, AnimationSet*> animation_sets;
        static std::unordered_map<const char*, sf::SoundBuffer> sounds;
};