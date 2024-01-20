#pragma once
#include "Component.h"


class Transform : public Component{

    /*
        determines the transformations of an object (position, rotation, scale)
    */

    public: 
        Transform() : position(0,0), rotation(0), scale(1,1){}

        /*
            the position of the object in world space, applying its parents position
        */
        sf::Vector2f GetGlobalPosition() const;

        sf::Vector2f position;
        float rotation;
        sf::Vector2f scale;    
};