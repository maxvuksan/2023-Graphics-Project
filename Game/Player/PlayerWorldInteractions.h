#include "../../Amber/Framework.h"
#include "../Items/ItemDictionary.h"
#include "CursorGraphic.h"

class World;
class Inventory;

class PlayerWorldInteractions : public Component {

    public:
        void Start();
        void Draw(sf::RenderTarget&) override;

        void ManageToolInHand(sf::RenderTarget&);
        void SwingToolInHand();

        void CalculateMouse(sf::RenderTarget&);

        void Mine(const sf::Vector2i& world_tile, ItemCode item_code);
        void PlaceUtility(const sf::Vector2i& rounded_world, const sf::Vector2i& coord_tile, ItemCode item_code);


        void CatchEvent(sf::Event) override;

        void LinkWorld(World* world);
        void LinkInventory(Inventory* inventory);



    private:


        sf::Sprite tool_in_hand_sprite;
        float swing_completion;
        float overswing_multiplier;
        float last_swing_value;
        float new_swing_rotation;
        float old_swing_rotation;
        bool swinging_tool;

        CursorGraphic* cursor_graphic;
        sf::Vector2i focused_block_position;


        // shows the player the footprint of a utility object
        sf::Sprite utility_hologram;
        bool utility_location_valid;


        float breaking_completeness;
        float sound_increment;
        int focused_block;
        ItemCode previous_item_code;
        

        bool auto_target_blocks;

        Inventory* inventory;
        World* world;
        sf::Vector2i mouse_world_pos; // where in world space is the mouse hovering over

        int selected_block;
};