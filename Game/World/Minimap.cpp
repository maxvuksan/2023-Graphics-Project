#include "Minimap.h"
#include "World.h"

int Minimap::exploring_radius = 45;
float Minimap::default_map_zoom = 3.0f;

void Minimap::Start(){

    // create all UIPixelGrids (map layers) ---------------------------------------

    background_pixel_grid = AddComponent<UIPixelGrid>();
    background_pixel_grid->SetAlign(ScreenLocationX::CENTER, ScreenLocationY::CENTER);
    background_pixel_grid->SetActive(false);

    main_pixel_grid = AddComponent<UIPixelGrid>();
    main_pixel_grid->SetAlign(ScreenLocationX::CENTER, ScreenLocationY::CENTER);
    main_pixel_grid->SetActive(false);

    foreground_pixel_grid = AddComponent<UIPixelGrid>();
    foreground_pixel_grid->SetAlign(ScreenLocationX::CENTER, ScreenLocationY::CENTER);
    foreground_pixel_grid->SetActive(false);

    explored_pixel_grid = AddComponent<UIPixelGrid>();
    explored_pixel_grid->SetAlign(ScreenLocationX::CENTER, ScreenLocationY::CENTER);
    explored_pixel_grid->SetActive(false);

    // load player icon texture 

    map_icon.setTexture(*AssetManager::GetTexture("map_icon"));
    map_icon.setOrigin(map_icon.getTexture()->getSize().x / 2.0f, map_icon.getTexture()->getSize().x / 2.0f);

    set_pan = false;
}

void Minimap::LinkWorld(World* world){
    this->world = world;

    // also init map outline, because we need the world reference to determine the map size
    map_outline.setFillColor(sf::Color::Transparent);
    map_outline.setOutlineColor(sf::Color(255,255,255,100));
    map_outline.setOutlineThickness(1);
    RecalculateMapOutline();
}

void Minimap::RecalculateMapOutline(){
    map_outline.setSize(sf::Vector2f(world->GetWorldProfile()->width  * world->GetWorldProfile()->tilemap_profile.width * GetTransform()->scale.x, 
                                     world->GetWorldProfile()->height * world->GetWorldProfile()->tilemap_profile.height * GetTransform()->scale.y));
}

void Minimap::OnSetActive(){
    
    GetTransform()->scale = sf::Vector2f(1,1);
}

void Minimap::CatchEvent(sf::Event event){

    if (event.type == sf::Event::KeyPressed)
    {
        if(event.key.scancode == sf::Keyboard::Scan::M){
            
            background_pixel_grid->SetActive(!background_pixel_grid->IsActive());
            foreground_pixel_grid->SetActive(!foreground_pixel_grid->IsActive());
            main_pixel_grid->SetActive(!main_pixel_grid->IsActive());

            // default zoom when opening map
            if(main_pixel_grid->IsActive()){
                Scene::GetActiveCamera()->ui_overlay_colour = sf::Color::Black;
                GetTransform()->scale.x = default_map_zoom;
                GetTransform()->scale.y = default_map_zoom;
                RecalculateMapOutline();
            }
            else{
                Scene::GetActiveCamera()->ui_overlay_colour = sf::Color::Transparent;     
            }

            // translates the map to the focus positionn
            sf::Vector2i focus_coord = world->WorldToCoord(world->GetFocus()->position.x, world->GetFocus()->position.y);
            GetTransform()->position = sf::Vector2f(-focus_coord.x * GetTransform()->scale.x + Core::GetDisplayWidth() / 2.0f, -focus_coord.y * GetTransform()->scale.y + Core::GetDisplayHeight() / 2.0f);


            if(reveal_all){           
                explored_pixel_grid->SetActive(false);
            }
            else{
                explored_pixel_grid->SetActive(main_pixel_grid->IsActive());
            }

        }
    }

    if (event.type == sf::Event::MouseWheelScrolled)
    {
        // when scaling, the minimap is translated to ensure we are scaling around the mouse position

        float scale_factor = 0.1;

        // between mouse and minimap pos
        sf::Vector2f difference = sf::Vector2f(Mouse::DisplayPosition().x - GetTransform()->position.x, Mouse::DisplayPosition().y - GetTransform()->position.y);

        if (event.mouseWheelScroll.delta > 0) // moving up
        {
            GetTransform()->scale.x *= 1.0f + scale_factor;
            GetTransform()->scale.y *= 1.0f + scale_factor;
            difference *= 1.0f + scale_factor;
        }
        else if (event.mouseWheelScroll.delta < 0) // moving down
        {
            GetTransform()->scale.x *= 1.0f - scale_factor;
            GetTransform()->scale.y *= 1.0f - scale_factor;
            difference *= 1.0f - scale_factor;
        }

        GetTransform()->position = sf::Vector2f(Mouse::DisplayPosition().x, Mouse::DisplayPosition().y) - difference;
        RecalculateMapOutline();
    }

    // we begin panning
    if(event.type == sf::Event::MouseButtonPressed){
        
        if(event.mouseButton.button == sf::Mouse::Button::Right){
            mouse_pan_inital = Mouse::DisplayPosition();
            pan_position_inital = GetTransform()->position;
            set_pan = true;
        }
    }

    // we have finished panning
    if(event.type == sf::Event::MouseButtonReleased){
        
        if(event.mouseButton.button == sf::Mouse::Button::Right){
            set_pan = false;
            sf::Vector2i mouse_now = Mouse::DisplayPosition();
            GetTransform()->position = pan_position_inital + sf::Vector2f(mouse_now.x - mouse_pan_inital.x, mouse_now.y - mouse_pan_inital.y);
        }
    }
}

void Minimap::Update(){

    if(set_pan && sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)){
    
        sf::Vector2i mouse_now = Mouse::DisplayPosition();
        GetTransform()->position = pan_position_inital + sf::Vector2f(mouse_now.x - mouse_pan_inital.x, mouse_now.y - mouse_pan_inital.y);
    }
}

void Minimap::Draw(sf::RenderTarget& surface){
    
    // only draw map icon if map is open
    
    if(main_pixel_grid->IsActive()){

        map_outline.setPosition(GetTransform()->position);
        surface.draw(map_outline);

        sf::Vector2i focus_coord = world->WorldToCoord(world->GetFocus()->position.x, world->GetFocus()->position.y);
        map_icon.setPosition(GetTransform()->position +  sf::Vector2f(focus_coord.x * GetTransform()->scale.x, focus_coord.y * GetTransform()->scale.y));
        surface.draw(map_icon);

        
    }
}

        
