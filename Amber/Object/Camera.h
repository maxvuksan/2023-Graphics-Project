#pragma once
#include "Component.h"
#include <SFML/Graphics.hpp>

class Camera : public Component {

    /*
        Camera:

        determines where the scene should be viewed from, by having Camera as a component we can attach it to objects 
        and follow there movement (e.g. follow the player) aswell as have multiple cameras switching between which is active 
        with Scene::SetActiveCamera(Camera*);
    */

    public:
        Camera();

        sf::Vector2f offset;
        sf::Color background_colour;

        // Converts a position to its position relative to this camera
        sf::Vector2f WorldToScreenPosition(sf::Vector2f world);
};