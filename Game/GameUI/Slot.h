#pragma once
#include "../Items/ItemDictionary.h"
#include <SFML/Graphics.hpp>
#include <cmath>



enum class SlotType{
    OPEN,  // holds items
    CLOSED, // decorative
    RECIPE, 
};

enum class SlotSprite{
    DEFAULT,
    SELECTED,
    RECIPE,
};

class Slot {

    public:

        Slot(SlotType type = SlotType::OPEN, SlotSprite sprite = SlotSprite::DEFAULT) : sprite(sprite), count(0), type(type), item_code(item_NO_DROP){};
        static sf::Vector2f GetTextPosition(const sf::Vector2f& item_sprite_position, int count);

        static int spacing; 
        static int cellsize;

        static sf::Text item_count_text;
        static sf::Sprite item_sprite;
        static sf::RectangleShape slot_rectangle_shape;

        SlotType type;
        SlotSprite sprite;
        ItemCode item_code;
        unsigned short count;

        static void Construct(); // init all sf:: variables

        bool Hovered(sf::Vector2f position);
        void DrawSlot(sf::RenderTarget& surface, sf::Vector2f position);
        
        // @returns true if count is > 0, false otherwise
        bool Occupied();
        
        sf::Vector2f GetPosition();

        void SetPosition(sf::Vector2f position);

};