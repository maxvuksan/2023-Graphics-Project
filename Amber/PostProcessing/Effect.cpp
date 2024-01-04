#include "Effect.h"
#include <fstream>
#include <sstream>
#include <iostream>

Effect::Effect(const std::string fragment_shader_location){
    
    // loading shader

    std::string shader_code;
    std::ifstream shader_file;

    try{
        shader_file.open(fragment_shader_location);
        std::stringstream shader_stream;

        shader_stream << shader_file.rdbuf();
        shader_file.close();

        shader_code = shader_stream.str();
    }
    catch(std::ifstream::failure e){
        std::cout << "ERROR : Failed to load effect with shader location :  '" + fragment_shader_location + "' shader source\n";
        return;
    }

    if(!this->fragment_shader.loadFromMemory(shader_code, sf::Shader::Fragment)){

        std::cout << "ERROR : Failed to load effects shader from '" << fragment_shader_location << "'\n";
        return; 
    }
    
    

}