#include "EditorScene.h"
#include "Game.h"
#include "Serilizer.h"
#include "World/Lighting/LightingManager.h"
#include "World/TileBehaviourManager.h"

void EditorScene::Start(){

    camera = AddObject<Object>()->AddComponent<Camera>();
    camera->background_colour = sf::Color(42, 40, 48);
    SetActiveCamera(camera);

    client = Game::GetGameClientFromScene(this);
    client->LinkScene(this);

    world = AddObject<World>();
    world->LinkClient(client);
    world->SetFocus(camera->GetThisObject()->GetTransform());


    

    client->LinkWorld(world);

    Serilizer::LoadStructureAsWorld(client->GetCurrentWorld().filepath, world);
    LightingManager::show_lighting = false;

    // editor setup ----------------------------------------------------------

    // draw a rectangle around around the drawable area
    canvas_bounds.setFillColor(sf::Color::Transparent);
    canvas_bounds.setOutlineColor(sf::Color::Red);
    canvas_bounds.setOutlineThickness(1);
    canvas_bounds.setSize(sf::Vector2f(
        world->GetWorldProfile()->width * world->GetWorldProfile()->tilemap_profile.width * world->GetWorldProfile()->tilemap_profile.tile_width,
        world->GetWorldProfile()->height * world->GetWorldProfile()->tilemap_profile.height * world->GetWorldProfile()->tilemap_profile.tile_height));

    selected_tile_shape.setFillColor(sf::Color::Transparent);
    selected_tile_shape.setOutlineColor(sf::Color::White);
    selected_tile_shape.setOutlineThickness(1);
    selected_tile_shape.setSize(sf::Vector2f(ItemDictionary::tile_size + 2, ItemDictionary::tile_size + 2));

    // remove camera bounds
    this->SetMinXBound(INT_MIN);
    this->SetMaxXBound(INT_MAX);
    this->SetMinYBound(INT_MIN);
    this->SetMaxYBound(INT_MAX);

    cursor_shape.setFillColor(sf::Color::Transparent);
    cursor_shape.setOutlineColor(sf::Color::White);
    cursor_shape.setOutlineThickness(1);
    cursor_shape.setOrigin(sf::Vector2f(-ItemDictionary::tile_size / 2.0f, -ItemDictionary::tile_size / 2.0f));

    tile_palette_sprite.setTexture(*AssetManager::GetTexture("tiles"));
    tile_palette_sprite.setPosition(sf::Vector2f(UIRect::padding, UIRect::padding));
    
    drawing_rectangle = false;
    subtracting_rectangle = false;
    
    selected_block = 0;
    set_location = SetLocation::MAIN;
    tiles_per_row = ItemDictionary::main_tiles_sprites_per_row;
    tool_mode = ToolMode::BRUSH;
    object_mode = ObjectMode::TILES;


    sf::Font* font = AssetManager::GetFont("m3x6");
    
    if(font == nullptr){
        std::cout << "ERROR : font is nullptr, EditorScene::Start()\n";
        return;
    }

    structures_text.setFont(*font);
    structures_text.setCharacterSize(16);
    structures_text.setPosition(sf::Vector2f(UIRect::padding, UIRect::padding));

    structure_sprite.setColor(sf::Color(255,255,255,200));
    structure_sprite.setTexture(*AssetManager::GetTexture("structures"));
}


void EditorScene::CatchEvent(sf::Event event){

    if(event.type == sf::Event::KeyPressed){
        
        switch(event.key.scancode){

            case sf::Keyboard::Scancode::Tab: {

                int object_mode_int = (int)object_mode;
                object_mode_int++;

                if(object_mode_int >= (int)ObjectMode::NUMBER_OF_OBJECT_MODES){
                    object_mode_int = 0;
                }

                object_mode = (ObjectMode)object_mode_int;

                break;
            }

            case sf::Keyboard::Scancode::S: {


                if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
                {
                    Serilizer::SaveStructure(client->GetCurrentWorld(), world);
                }

                break;
            }
            case sf::Keyboard::Scancode::L: {
                LightingManager::show_lighting = !LightingManager::show_lighting;

                break;
            }
        }
        if(object_mode == ObjectMode::TILES){

            switch(event.key.scancode){

                case sf::Keyboard::Scancode::B: {
                    tool_mode = ToolMode::BRUSH;
                    drawing_rectangle = false;
                    break;
                }
                case sf::Keyboard::Scancode::R: {
                    tool_mode = ToolMode::RECTANGLE;
                    break;
                }

                case sf::Keyboard::Scancode::Num1: {
                    tile_palette_sprite.setTexture(*AssetManager::GetTexture("tiles"), true);
                    set_location = SetLocation::MAIN;
                    selected_block = 0;
                    tiles_per_row = ItemDictionary::main_tiles_sprites_per_row;
                    break;
                }
                case sf::Keyboard::Scancode::Num2: {
                    tile_palette_sprite.setTexture(*AssetManager::GetTexture("background_tiles"), true);
                    set_location = SetLocation::BACKGROUND;
                    selected_block = 0;
                    tiles_per_row = ItemDictionary::background_tiles_sprites_per_row;
                    break;
                }
                case sf::Keyboard::Scancode::Num3: {
                    tile_palette_sprite.setTexture(*AssetManager::GetTexture("foreground_tiles"), true);
                    set_location = SetLocation::FOREGROUND;
                    selected_block = 0;
                    tiles_per_row = ItemDictionary::foreground_tiles_sprites_per_row;
                    break;
                }
            }
        }
    }

    // scrolling through tiles
    if (event.type == sf::Event::MouseWheelScrolled)
    {

        if(object_mode == ObjectMode::TILES){
            int tile_count = main_NUMBER_OF_BLOCKS;

            if(set_location == SetLocation::FOREGROUND){
                tile_count = foreground_NUMBER_OF_BLOCKS;
            }
            else if(set_location == SetLocation::BACKGROUND){
                tile_count = background_NUMBER_OF_BLOCKS;
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)){

                if (event.mouseWheelScroll.delta > 0) // moving up
                {
                    selected_block -= tiles_per_row;
                }
                else if (event.mouseWheelScroll.delta < 0) // moving down
                {
                    selected_block += tiles_per_row;
                }

                if(selected_block >= tile_count){
                    selected_block = selected_block - tile_count + 1;
                }
                else if(selected_block < 0){
                    selected_block = selected_block + tile_count - 1;
                }
            }
            else{
                if (event.mouseWheelScroll.delta > 0) // moving up
                {
                    selected_block--;
                }
                else if (event.mouseWheelScroll.delta < 0) // moving down
                {
                    selected_block++;
                }
            
                if(selected_block >= tile_count){
                    selected_block = 0;
                }
                else if(selected_block < 0){
                    selected_block = tile_count - 1;
                }
            }
        }
        else if(object_mode == ObjectMode::STRUCTURES){

            int structure_count = structure_NUMBER_OF_STRUCTURES;

            if (event.mouseWheelScroll.delta > 0) // moving up
            {
                selected_structure--;
            }
            else if (event.mouseWheelScroll.delta < 0) // moving down
            {
                selected_structure++;
            }

            if(selected_structure < 0){
                selected_structure = structure_NUMBER_OF_STRUCTURES;
            }
            else if(selected_structure >= structure_NUMBER_OF_STRUCTURES){
                selected_structure = 0;
            }


        }

    }

    if(event.type == sf::Event::MouseButtonPressed){

        if(object_mode == ObjectMode::TILES){

            if(event.mouseButton.button == sf::Mouse::Button::Left){

                // start drawing rectangle
                if(tool_mode == ToolMode::RECTANGLE){
                    drawing_rectangle = true;
                    subtracting_rectangle = false;
                    rectangle_inital_mouse_position = Mouse::DisplayPosition();
                    rectangle_inital_coordinate = coord_mouse_position;
                }
            }
            if(event.mouseButton.button == sf::Mouse::Button::Right){

                // start subtracting rectangle
                if(tool_mode == ToolMode::RECTANGLE){
                    drawing_rectangle = false;
                    subtracting_rectangle = true;
                    rectangle_inital_mouse_position = Mouse::DisplayPosition();
                    rectangle_inital_coordinate = coord_mouse_position;
                }
            }
        }
        else if(object_mode == ObjectMode::STRUCTURES){

            // place structure
            if(event.mouseButton.button == sf::Mouse::Button::Left){

                if(coord_mouse_position.x >= 0 && coord_mouse_position.y >= 0){

                    sf::Vector2i chunk = world->ChunkFromCoord(coord_mouse_position.x, coord_mouse_position.y);

                    if(world->ChunkInBounds(chunk.x, chunk.y)){

                        sf::Vector2i pos = world->OffsetFromCoord(coord_mouse_position.x, coord_mouse_position.y, chunk.x, chunk.y);

                        world->GetChunks()->at(chunk.x)[chunk.y]->AddStructure((BackgroundStructure)selected_structure, pos.x, pos.y);

                    }
                }

            }


        }
    }

    if(event.type == sf::Event::MouseButtonReleased){

        if(event.mouseButton.button == sf::Mouse::Button::Left){

            if(tool_mode == ToolMode::RECTANGLE && drawing_rectangle){

                drawing_rectangle = false;

                CreateRectangle(selected_block);
            }
        }
        else if(event.mouseButton.button == sf::Mouse::Button::Right){

            if(tool_mode == ToolMode::RECTANGLE && subtracting_rectangle){

                subtracting_rectangle = false;

                CreateRectangle(-1);
            }
        }
    }
}

void EditorScene::CreateRectangle(signed_byte block){

     
    int dif_x = coord_mouse_position.x - rectangle_inital_coordinate.x;
    int dif_y = coord_mouse_position.y - rectangle_inital_coordinate.y;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::LShift)){

        // only draw outline
        for(int x = 0; x < dif_x; x++){
            for(int y = 0; y < dif_y; y++){

                if(x == 0 || x == dif_x - 1 || y == 0 || y == dif_y - 1){
                    SetTile(block, rectangle_inital_coordinate.x + x, rectangle_inital_coordinate.y + y, set_location);
                }
            }
        }
    }
    else{
        // draw filled in rect
        for(int x = 0; x < dif_x; x++){
            for(int y = 0; y < dif_y; y++){

                SetTile(block, rectangle_inital_coordinate.x + x, rectangle_inital_coordinate.y + y, set_location);
            }
        }
    }


}

void EditorScene::SetTile(signed_byte tile_index, int x, int y, SetLocation set_location){

    sf::Vector2i chunk = world->ChunkFromCoord(x, y);

    if(!world->ChunkInBounds(chunk.x, chunk.y)){
        return;
    }

    sf::Vector2i pos = world->OffsetFromCoord(x, y, chunk.x, chunk.y);

    signed_byte old_tile = world->GetChunks()->at(chunk.x).at(chunk.y)->GetTile(pos.x, pos.y, set_location);

    world->GetChunks()->at(chunk.x).at(chunk.y)->SetTile(tile_index, pos.x, pos.y, set_location);

}

void EditorScene::Update(){

    // force nightime
    TimeManager::SetTimeOfDay(0);

    // camera should not move when drawing rect
    if(drawing_rectangle || subtracting_rectangle){
        return;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::S)){
        camera->GetThisObject()->GetTransform()->position.y += ItemDictionary::tile_size;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W)){
        camera->GetThisObject()->GetTransform()->position.y -= ItemDictionary::tile_size;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A)){
        camera->GetThisObject()->GetTransform()->position.x -= ItemDictionary::tile_size;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D)){
        camera->GetThisObject()->GetTransform()->position.x += ItemDictionary::tile_size;
    }
}

void EditorScene::Draw(sf::RenderTarget& surface){


    canvas_bounds.setPosition(Camera::WorldToScreenPosition(world->GetTransform()->position));
    surface.draw(canvas_bounds);

    sf::Vector2f camera_clamped(round(camera->GetThisObject()->GetTransform()->position.x / ItemDictionary::tile_size) * ItemDictionary::tile_size, round(camera->GetThisObject()->GetTransform()->position.y / ItemDictionary::tile_size) * ItemDictionary::tile_size);
    sf::Vector2f camera_difference = camera->GetThisObject()->GetTransform()->position - camera_clamped;

    // convert the mouse display position
    sf::Vector2f mouse_world_pos = Camera::ScreenToWorldPosition(sf::Vector2f(Mouse::DisplayPosition().x, Mouse::DisplayPosition().y));
    coord_mouse_position = sf::Vector2i(round(mouse_world_pos.x / ItemDictionary::tile_size), round(mouse_world_pos.y / ItemDictionary::tile_size));

    //sf::Vector2f visual_mouse_position(round((Mouse::DisplayPosition().x) / ItemDictionary::tile_size) * ItemDictionary::tile_size, round((Mouse::DisplayPosition().y) / ItemDictionary::tile_size) * ItemDictionary::tile_size );
    sf::Vector2f visual_mouse_position(Camera::WorldToScreenPosition(sf::Vector2f(coord_mouse_position.x * ItemDictionary::tile_size, coord_mouse_position.y * ItemDictionary::tile_size)));

    if(object_mode == ObjectMode::TILES){

        if(tool_mode == ToolMode::BRUSH){
            
            cursor_shape.setPosition(visual_mouse_position);
            cursor_shape.setSize(sf::Vector2f(ItemDictionary::tile_size, ItemDictionary::tile_size));

            if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)){
                if(world->CoordInBounds(coord_mouse_position.x, coord_mouse_position.y)){
                    SetTile(selected_block, coord_mouse_position.x, coord_mouse_position.y, set_location);
                }
            }
            else if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)){
                if(world->CoordInBounds(coord_mouse_position.x, coord_mouse_position.y)){
                    SetTile(-1, coord_mouse_position.x, coord_mouse_position.y, set_location);
                }
            }
        }
        else if(tool_mode == ToolMode::RECTANGLE){

            if(!drawing_rectangle && !subtracting_rectangle){
                cursor_shape.setPosition(visual_mouse_position);
                cursor_shape.setSize(sf::Vector2f(ItemDictionary::tile_size, ItemDictionary::tile_size));
            }
            else{
                cursor_shape.setPosition(round((rectangle_inital_mouse_position.x) / ItemDictionary::tile_size) * ItemDictionary::tile_size, round((rectangle_inital_mouse_position.y) / ItemDictionary::tile_size) * ItemDictionary::tile_size );
                cursor_shape.setSize(sf::Vector2f((coord_mouse_position.x - rectangle_inital_coordinate.x) * ItemDictionary::tile_size, (coord_mouse_position.y - rectangle_inital_coordinate.y) * ItemDictionary::tile_size));
            }
        }



        surface.draw(tile_palette_sprite);


        int x = selected_block % tiles_per_row; 
        int y = floor(selected_block / (float)tiles_per_row);

        selected_tile_shape.setPosition(sf::Vector2f(
            UIRect::padding + x * ItemDictionary::tile_size - 1, 
            UIRect::padding + y * ItemDictionary::tile_size - 1));

        surface.draw(cursor_shape);
        surface.draw(selected_tile_shape);


    }
    else if(object_mode == ObjectMode::STRUCTURES){

        structures_text.setString("");

        for(int i = 0; i < structure_NUMBER_OF_STRUCTURES; i++){
            std::string name = ItemDictionary::BACKGROUND_STRUCTURES[i].name;
            
            // highlight the selected structure
            if(selected_structure == i){
                structure_sprite.setTextureRect(sf::IntRect(ItemDictionary::BACKGROUND_STRUCTURES[i].pixel_position_on_sprite_sheet.x ,
                                                            ItemDictionary::BACKGROUND_STRUCTURES[i].pixel_position_on_sprite_sheet.y, 
                                                            ItemDictionary::BACKGROUND_STRUCTURES[i].pixel_dimensions.x, 
                                                            ItemDictionary::BACKGROUND_STRUCTURES[i].pixel_dimensions.y));
                name = "* " + name;
            }
            structures_text.setString(structures_text.getString() + name + "\n");
        }

        structure_sprite.setPosition(visual_mouse_position);

        surface.draw(structures_text);
        surface.draw(structure_sprite);
    }


}
