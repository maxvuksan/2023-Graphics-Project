#include "../BlockTypes.h"
#include <SFML/Graphics.hpp>
#include <cmath>
class InventorySlot {

    public:

        void SetPosition(sf::Vector2f position);
        void RenderSlot(sf::RenderTarget& surface);

        static int spacing; 
        static int items_per_row; // in regards to the texture sheet 
        static int cellsize;
        static sf::Text item_count_text;
        static sf::Sprite item_sprite;
        static sf::RectangleShape slot_rectangle_shape;

        bool occupied = false;
        ItemCode item_code;
        unsigned short count;

    private:
        sf::Vector2f position;

};