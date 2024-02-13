#include "Minimap.h"
#include "World.h"

int Minimap::exploring_radius = 45;
float Minimap::default_map_zoom = 1.0f;
int Minimap::map_icon_size = 16;

void Minimap::Start(){

    // create all UIPixelGrids (map layers) ---------------------------------------

    ui_panel = AddComponent<UIPanel>();
    ui_panel->SetColour(sf::Color::Black);    
    ui_panel->SetActive(false);
    ui_panel->SetRenderLayer(3);

    background_pixel_grid = AddComponent<UIPixelGrid>();
    background_pixel_grid->SetAlign(ScreenLocationX::CENTER, ScreenLocationY::CENTER);
    background_pixel_grid->SetActive(false);
    background_pixel_grid->SetRenderLayer(3);

    main_pixel_grid = AddComponent<UIPixelGrid>();
    main_pixel_grid->SetAlign(ScreenLocationX::CENTER, ScreenLocationY::CENTER);
    main_pixel_grid->SetActive(false);
    main_pixel_grid->SetRenderLayer(3);

    foreground_pixel_grid = AddComponent<UIPixelGrid>();
    foreground_pixel_grid->SetAlign(ScreenLocationX::CENTER, ScreenLocationY::CENTER);
    foreground_pixel_grid->SetActive(false);
    foreground_pixel_grid->SetRenderLayer(3);

    explored_pixel_grid = AddComponent<UIPixelGrid>();
    explored_pixel_grid->SetAlign(ScreenLocationX::CENTER, ScreenLocationY::CENTER);
    explored_pixel_grid->SetActive(false);
    explored_pixel_grid->SetRenderLayer(3);


    SetRenderLayer(4);

    // load player icon texture 

    map_icon.setTexture(*AssetManager::GetTexture("map_icon"));
    map_icon.setOrigin(map_icon.getTexture()->getSize().x / 2.0f, map_icon.getTexture()->getSize().x / 2.0f);
 
    set_pan = false;



    // create buttons

    marker1_button = GetScene()->AddUI<Object>()->AddComponent<UIButton>(5);
    marker1_button->SetTexture("map_icon");
    marker1_button->SetTextureRect(map_icon_size * 2, 0, map_icon_size, map_icon_size);
    
    marker1_button->SetOnClickCallback([this](){        
        placing_marker = true;
        marker_type_to_place = 2;
    });

    
    marker2_button = GetScene()->AddUI<Object>()->AddComponent<UIButton>(5);
    marker2_button->SetTexture("map_icon");
    marker2_button->SetTextureRect(map_icon_size * 3, 0, map_icon_size, map_icon_size);
    
    marker2_button->SetOnClickCallback([this](){        
        placing_marker = true;
        marker_type_to_place = 3;
    });

    
    marker3_button = GetScene()->AddUI<Object>()->AddComponent<UIButton>(5);
    marker3_button->SetTexture("map_icon");
    marker3_button->SetTextureRect(map_icon_size * 4, 0, map_icon_size, map_icon_size);
    
    marker3_button->SetOnClickCallback([this](){        
        placing_marker = true;
        marker_type_to_place = 4;
    });


    remove_markers = GetScene()->AddUI<Object>()->AddComponent<UIButton>(5);
    remove_markers->SetTexture("map_icon");
    remove_markers->SetTextureRect(map_icon_size, 0, map_icon_size, map_icon_size);
    
    remove_markers->SetOnClickCallback([this](){        
        map_markers.clear();
    });    

    button_rect_array = GetScene()->AddUI<Object>()->AddComponent<UIRectArray>(5);
    button_rect_array->SetElementSize(map_icon_size, map_icon_size);
    button_rect_array->SetGap(UIRect::padding);
    button_rect_array->SetAlign(ScreenLocationX::LEFT, ScreenLocationY::TOP);
    button_rect_array->AddUIRect(marker1_button);
    button_rect_array->AddUIRect(marker2_button);
    button_rect_array->AddUIRect(marker3_button);
    button_rect_array->AddUIRect(remove_markers);
    button_rect_array->SetActive(false);

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

void Minimap::ToggleMap(bool state){
    placing_marker = false;

    background_pixel_grid->SetActive(state);
    foreground_pixel_grid->SetActive(state);
    main_pixel_grid->SetActive(state);
    ui_panel->SetActive(state);

    // default zoom when opening map
    if(state){
        Scene::GetActiveCamera()->ui_overlay_colour = sf::Color::Black;
        GetTransform()->scale.x = default_map_zoom;
        GetTransform()->scale.y = default_map_zoom;
        RecalculateMapOutline();
        button_rect_array->SetActive(true);
        GetScene()->SetEventFocus(this);
    }
    else{ // closing map
        Scene::GetActiveCamera()->ui_overlay_colour = sf::Color::Transparent;     
        set_pan = false;
        button_rect_array->SetActive(false);
        GetScene()->ClearEventFocus();

    }

    // translates the map to the focus positionn
    sf::Vector2i focus_coord = world->WorldToCoord(world->GetFocus()->position.x, world->GetFocus()->position.y);
    GetTransform()->position = sf::Vector2f(-focus_coord.x * GetTransform()->scale.x + Core::GetDisplayWidth() / 2.0f, -focus_coord.y * GetTransform()->scale.y + Core::GetDisplayHeight() / 2.0f);


    if(reveal_all){           
        explored_pixel_grid->SetActive(false);
    }
    else{
        explored_pixel_grid->SetActive(state);
    }
}

void Minimap::CatchEvent(sf::Event event){

    if (event.type == sf::Event::KeyPressed)
    {

        switch(event.key.scancode){

            case sf::Keyboard::Scan::M: {
                ToggleMap(!main_pixel_grid->IsActive());
                break;
            }
            case sf::Keyboard::Scan::E: {
                ToggleMap(false);
                break;
            }
        }
    }

}

void Minimap::CatchEventEventFocusBounded(sf::Event event){

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

        if(event.mouseButton.button == sf::Mouse::Button::Left){
            if(placing_marker){
            
                map_markers.push_back({marker_type_to_place, ScreenToMinimapPosition(Mouse::DisplayPosition())});    
                placing_marker = false;
            
            }
        }

        if(event.mouseButton.button == sf::Mouse::Button::Right){
            mouse_pan_inital = Mouse::DisplayPosition();
            pan_position_inital = GetTransform()->position;
            set_pan = true;
        }
        // ping
        if(event.mouseButton.button == sf::Mouse::Button::Middle){


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

sf::Vector2f Minimap::ScreenToMinimapPosition(sf::Vector2i screen_position){

    float x = screen_position.x - GetTransform()->position.x;
    float y = screen_position.y - GetTransform()->position.y;
    
    return sf::Vector2f(ceil(x / GetTransform()->scale.x), ceil(y / GetTransform()->scale.y));

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

        // draw each placed marker
        for(auto& marker : map_markers){
                                        // move marker to center of tile
            DrawMarker(marker.type, marker.position  + sf::Vector2f(-0.5f, -0.5f), surface);
        }

        // draw players position
        sf::Vector2i player_pos = world->WorldToCoord(world->GetFocus()->position.x, world->GetFocus()->position.y);
        DrawMarker(0, sf::Vector2f(player_pos.x, player_pos.y), surface);

    }
}

void Minimap::DrawMarker(int type, sf::Vector2f position, sf::RenderTarget& surface){

    map_icon.setTextureRect(sf::IntRect(map_icon_size * type, 0, map_icon_size, map_icon_size));
    map_icon.setPosition(GetTransform()->position +  sf::Vector2f(position.x * GetTransform()->scale.x, position.y * GetTransform()->scale.y));
    surface.draw(map_icon);
}
