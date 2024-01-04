#include <string>
#include <SFML/Graphics.hpp>
/*
    a single layer in the post processing stack
*/
class Effect {

    public:

        /*
            @param fragment_shader_location what shader to construct this effect on
        */
        Effect(const std::string fragment_shader_location);


    private:

        sf::Shader fragment_shader;

};