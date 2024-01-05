#include "../../Amber/Framework.h"
#include "../Items/ItemDictionary.h"
#include "CursorGraphic.h"
#include "../World/Chunk.h"

class World;
class Inventory;
class UtilityStation;

class PlayerWorldInteractions : public Component {

    public:
        void Start();
        void Draw(sf::RenderTarget&) override;

        void ManageToolInHand(sf::RenderTarget&);
        void SwingToolInHand();

        /*
            responds to the current state of the mouse

            @param surface the surface we are drawing to (provided by Draw()) is used as a canvas to draw utility hologram visual
        */
        void CalculateMouse(sf::RenderTarget& surface);

        /*
            incrementally mines whatever is at the mouse position 

            @param coord_tile the position we are placing at (as a coordinate)
            @param rounded_world the position we are placing at (as a world position, rounded to the coordinate grid)
            @param item_code the item currently in the players hand, determines what layer to be mined (if any) 
        */
        void Mine(const sf::Vector2i& coord_tile, const sf::Vector2i& rounded_world, ItemCode item_code);

        /*
            creates a utility object at the provided position, 

            @param coord_tile the position we are placing at (as a coordinate)
            @param rounded_world the position we are placing at (as a world position, rounded to the coordinate grid)
            @param item_code associated with the new utility object
        */
        void PlaceUtility(const sf::Vector2i& coord_tile, const sf::Vector2i& rounded_world, ItemCode item_code);
        
        /*
            determines if a new utility object will overlap any existing ones

            @returns pointer to station which data overlaps, nullptr otherwise
            @param coord_tile the position we are placing at (as a coordinate)
            @param rounded_world the position we are placing at (as a world position, rounded to the coordinate grid)
            @param footprint the dimensions of the new utility object
        */
        UtilityStation* NewUtilityOverlaps(const sf::Vector2i& coord_tile, const sf::Vector2i& rounded_world, sf::Vector2i footprint);


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
        Chunk* chunk_last_utility_overlap_was_in;

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
        sf::Vector2f mouse_world_pos; // where in world space is the mouse hovering over

        int selected_block;
};