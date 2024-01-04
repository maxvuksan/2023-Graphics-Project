#pragma once
#include "../../Amber/Framework.h"

/*
    a pixel to tile based minimap, intended for the World class, the minimap has 3 grids of pixels,
    
    :background 
    :main
    :foreground

    representing the three tile layers within the world.
    and :explored (either black or transparent, has the player "explored" this part of the map)


    note : each pixel grid should have its Create() method called before use
*/
class World;
class Minimap : public Object{

    friend class CommandParser;

    public:
        
        void Start();
        void LinkWorld(World* world);
        void CatchEvent(sf::Event) override;
        void OnSetActive() override;

        void Update() override;
        void Draw(sf::RenderTarget& surface);

        UIPixelGrid* GetMainPixelGrid(){return main_pixel_grid;}
        UIPixelGrid* GetForegroundPixelGrid(){return foreground_pixel_grid;}
        UIPixelGrid* GetBackgroundPixelGrid(){return background_pixel_grid;}
        UIPixelGrid* GetExploredPixelGrid(){return explored_pixel_grid;}
        
        // how much over the map is revealed around the player
        static int exploring_radius;
        
    
    private:

        // assigned the map_outline size property, should be called when zooming occurs
        void RecalculateMapOutline();

        static float default_map_zoom;

        World* world;

        sf::Sprite map_icon;
        sf::RectangleShape map_outline;

        sf::Vector2f focus_coord;

        UIPixelGrid* main_pixel_grid;
        UIPixelGrid* foreground_pixel_grid;
        UIPixelGrid* background_pixel_grid;
        UIPixelGrid* explored_pixel_grid;

        sf::Vector2i mouse_pan_inital; // position of mouse when panning started
        sf::Vector2f pan_position_inital; // position of the object when panning started
        bool set_pan;

        bool reveal_all = false;

};