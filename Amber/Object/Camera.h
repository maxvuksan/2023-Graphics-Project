#pragma once
#include "Component.h"
#include <iostream>
#include <SFML/Graphics.hpp>


/*
    determines where the scene should be viewed from, by having Camera as a component we can attach it to objects 
    and follow there movement (e.g. follow the player) aswell as have multiple cameras switching between which is active 
    with Scene::SetActiveCamera(Camera*);
*/
class Camera : public Component {

    public:
        Camera();

        sf::Color background_colour;

        void Update() override;

        // Converts a world space position to its position relative to the scenes active camera
        static sf::Vector2f WorldToScreenPosition(sf::Vector2f world);

        // Converts a screen space position to its position in the world 
        static sf::Vector2f ScreenToWorldPosition(sf::Vector2f screen);
        static sf::Vector2f ScreenToWorldPosition(sf::Vector2i screen);             
    private:
        static sf::Vector2f bounded_position;
};