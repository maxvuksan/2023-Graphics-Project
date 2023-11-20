#include "../../Amber/Framework.h"
#include "../BlockTypes.h"
#include "CursorGraphic.h"

class World;
class Inventory;

class PlayerWorldInteractions : public Component {

    public:
        void Start();
        void CalculateMouse();
        void Update();

        void LinkWorld(World* world);
        void LinkInventory(Inventory* inventory);

    private:
        CursorGraphic* cursor_graphic;
        sf::Vector2i focused_block_position;

        float breaking_completeness;
        float sound_increment;
        int focused_block;

        Inventory* inventory;
        World* world;
        sf::Vector2i mouse_world_pos; // where in world space is the mouse hovering over

        int selected_block;
};