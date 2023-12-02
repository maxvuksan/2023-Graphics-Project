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
        sf::Color ui_overlay_colour;
        
        // will display an image on the background, if no background image is provided, the background colour should be used
        void SetBackgroundTexture(const char* label);
        // @returns a reference to the background sprite, returns nullptr if no background is set, set with Camera::SetBackgroundTexture()
        sf::Sprite* GetBackgroundSprite();

        void Update() override;

        // Converts a world space position to its position relative to the scenes active camera
        static sf::Vector2f WorldToScreenPosition(sf::Vector2f world);

        // Converts a screen space position to its position in the world 
        static sf::Vector2f ScreenToWorldPosition(sf::Vector2f screen);
        static sf::Vector2f ScreenToWorldPosition(sf::Vector2i screen);             
    
    private:

        sf::Sprite background_sprite;

        static sf::Vector2f bounded_position;
};