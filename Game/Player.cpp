#include "Player.h"
#include "World.h"
#include "BlockTypes.h"
#include <cmath>

void Player::Start(){

    GetScene()->SetActiveCamera(AddComponent<Camera>());

    auto col = AddComponent<BoxCollider>();
    col->SetSize(sf::Vector2f(8, 10));
    col->SetOffset(sf::Vector2f(-4,-5));

    ground = AddComponent<BoxCollider>();
    ground->SetIsTrigger(true);
    ground->SetSize(sf::Vector2f(8, 3));
    ground->SetOffset(sf::Vector2f(-4,6));

    left = AddComponent<BoxCollider>();
    left->SetIsTrigger(true);
    left->SetSize(sf::Vector2f(2, 8));
    left->SetOffset(sf::Vector2f(-8,-4));

    right = AddComponent<BoxCollider>();
    right->SetIsTrigger(true);
    right->SetSize(sf::Vector2f(2, 8));
    right->SetOffset(sf::Vector2f(6,-4));
    
    GetTransform()->position = sf::Vector2f(50, -500);
    


    pb = AddComponent<PhysicsBody>();
    AddComponent<PointLight>();

    selected_block = 0;
    cursor_graphic = GetScene()->AddUI<CursorGraphic>();
}


void Player::Update(){

    // Movement...

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        pb->velocity.x = -0.08;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        pb->velocity.x = 0.08;
    }
    else{
        pb->velocity.x = 0;       
    }

    CalculateMouse();
    
}


void Player::CalculateMouse(){

    sf::Vector2i _mouse_pos = Mouse::DisplayPosition();
    sf::Vector2f player_center = GetScene()->GetActiveCamera()->WorldToScreenPosition(GetTransform()->position);

    _mouse_pos.x -= round(player_center.x);
    _mouse_pos.y -= round(player_center.y);

    mouse_world_pos = sf::Vector2i(round(GetTransform()->position.x) + _mouse_pos.x, 
                             round(GetTransform()->position.y) + _mouse_pos.y );



    sf::Vector2i cursor = world->RoundWorld(mouse_world_pos.x, mouse_world_pos.y);
    cursor_graphic->GetTransform()->position = sf::Vector2f(cursor.x, cursor.y);


    if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
        
        if(cursor != focused_block_position){
            focused_block = world->GetTileWorld(cursor.x, cursor.y);
            focused_block_position = cursor;

            breaking_completeness = 0;
            sound_increment = 0;
        }

        if(focused_block != -1){
            float increment = 0.02f * Time::Dt() / (float)Blocks[focused_block].durability;
            breaking_completeness += increment;
            sound_increment += increment;

            if(sound_increment > 0.3){
                sound_increment = 0;
                Sound::Play("hit", 30);
            }
        }

        cursor_graphic->SetComplete(breaking_completeness);

        if(breaking_completeness >= 1){
            // block breaks...
            Sound::Play("break", 10);
            world->SetTileWorld(-1, cursor.x, cursor.y);
            focused_block_position = sf::Vector2i(-1,-1); // clearing the focused block (an impossible position)
        }
    }
    else{
        cursor_graphic->SetComplete(0);
    }

    if(sf::Mouse::isButtonPressed(sf::Mouse::Right)){
        
        world->SetTileWorld(selected_block, mouse_world_pos.x, mouse_world_pos.y);
    }
}


void Player::CatchEvent(sf::Event event){

    if (event.type == sf::Event::KeyPressed)
    {
        switch(event.key.scancode){

            case sf::Keyboard::Scan::R:{
                GetTransform()->position = sf::Vector2f(50, -500);
                break;
            }

            case sf::Keyboard::Scan::W: {
        
                // only allow jumping when we have ground below
                if(ground->Triggered()){
                    pb->velocity.y = -jump_height;
                }
                else if(left->Triggered()){
                    pb->velocity.y = -jump_height;
                    pb->velocity.x = 0.17f;
                }
                else if(right->Triggered()){
                    pb->velocity.y = -jump_height;
                    pb->velocity.x = -0.17f;
                }
                break;
            }   

            case sf::Keyboard::Scan::Num1: {
                selected_block--;
                selected_block %= BlockCode::c_NUMBER_OF_BLOCKS;
                if(selected_block < 0){
                    selected_block = BlockCode::c_NUMBER_OF_BLOCKS - 1;
                }
                break;
            }
            case sf::Keyboard::Scan::Num2: {
                selected_block++;
                selected_block %= BlockCode::c_NUMBER_OF_BLOCKS;
                break;
            }        
        }
        
    }       
}

