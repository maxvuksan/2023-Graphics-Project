#pragma once
#include "Component.h"


class Transform : public Component{

    /*
        Transform

        determines the transformations of an object (position, rotation, scale)

    */

    public: 
        Transform() : position(0,0), rotation(0), scale(1,1){}


        sf::Vector2f position;
        float rotation;
        sf::Vector2f scale;    
};