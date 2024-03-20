#pragma once

#include "../Amber/Framework.h"
#include "Player/Player.h"
#include "Player/PlayerController.h"
#include "World/World.h"
#include <math.h>
#include "Networking/GameClient.h"
#include "Serilizer.h"
#include "Items/ItemDictionary.h"

/*
    The editor scene (tohand make structures)
*/
class EditorScene : public Scene {

    enum class ToolMode{

        BRUSH, // PRESS B
        RECTANGLE, // PRESS R
        RECTANGLE_OUTLINE, // PRESS O
    };

    // PRESS TAB to switch
    enum class ObjectMode{
        TILES,
        STRUCTURES,

        NUMBER_OF_OBJECT_MODES,
    };

    public: 

        void Start() override;
        void CatchEvent(sf::Event event) override;

        void CreateRectangle(signed_byte block);
        void SetTile(signed_byte tile_index, int x, int y, SetLocation set_location);

        void Update();
        void Draw(sf::RenderTarget& surface) override;


    private:

        Camera* camera;
        World* world;
        GameClient* client;

        sf::Text structures_text;
        int selected_structure;
        sf::Sprite structure_sprite;
        
        ToolMode tool_mode;
        ObjectMode object_mode;

        sf::RectangleShape selected_tile_shape; // outlines the selected tile to easily see what is currently selected
        sf::RectangleShape cursor_shape;
        sf::RectangleShape canvas_bounds; // the bounds of the world;

        sf::Vector2i focused_block_position;
        sf::Vector2i coord_mouse_position;

        sf::Vector2i rectangle_inital_coordinate;
        sf::Vector2i rectangle_inital_mouse_position;
        bool drawing_rectangle;
        bool subtracting_rectangle;

        sf::Sprite tile_palette_sprite;
        SetLocation set_location;
        signed_byte selected_block;
        int tiles_per_row; // on spritesheet
};