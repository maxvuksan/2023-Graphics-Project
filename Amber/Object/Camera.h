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

        sf::Vector2f GetBoundedPosition(){ return bounded_position;}

        void Update() override;
        // sets the intensity of screenshake to apply each frame
        void SetConstantScreenShake(int shake_factor);

        /*
            changes the screenwipe colour, used to transitions

            @param new_colour the colour to fill the screen
            @param milliseconds how long it takes to transition from the current screen wipe colour to new_colour
        */
        void SetScreenWipeColourOvertime(sf::Color new_colour, int milliseconds);
        void SetScreenWipeColour(sf::Color new_colour);
        const sf::Color& GetFinalScreenWipeColour(){return final_screen_wipe_colour;}

        // Converts a world space position to its position relative to the scenes active camera
        static sf::Vector2f WorldToScreenPosition(sf::Vector2f world);
        // same as WorldToScreenPosition but does not floor end value (leaves floating points)
        static sf::Vector2f WorldToScreenPositionNoFloor(sf::Vector2f world);



        // Converts a screen space position to its position in the world 
        static sf::Vector2f ScreenToWorldPosition(sf::Vector2f screen);
        static sf::Vector2f ScreenToWorldPosition(sf::Vector2i screen);        

    
    private:    

        sf::Sprite background_sprite;

        int constant_screen_shake_factor;
        static sf::Vector2f bounded_position;
        static sf::Vector2f ceil_bounded_position;

        float milliseconds_to_screen_wipe;
        float milliseconds_to_screen_wipe_tracked;
        sf::Color previous_screen_wipe_colour;
        sf::Color target_screen_wipe_colour;
        sf::Color final_screen_wipe_colour;
};