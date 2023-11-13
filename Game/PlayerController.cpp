#include "PlayerController.h"
#include "World.h"
#include "BlockTypes.h"

void PlayerController::Start(){
    
    object->GetScene()->SetActiveCamera(object->AddComponent<Camera>());


    body_collider = object->AddComponent<BoxCollider>();
    body_collider->SetSize(sf::Vector2f(6, 16));
    body_collider->SetOffset(sf::Vector2f(-3,-8));

    ground = object->AddComponent<BoxCollider>();
    ground->SetIsTrigger(true);
    ground->SetSize(sf::Vector2f(8, 4));
    ground->SetOffset(sf::Vector2f(-3,8));

    left = object->AddComponent<BoxCollider>();
    left->SetIsTrigger(true);
    left->SetSize(sf::Vector2f(2, 8));
    left->SetOffset(sf::Vector2f(-8,-4));

    right = object->AddComponent<BoxCollider>();
    right->SetIsTrigger(true);
    right->SetSize(sf::Vector2f(2, 6));
    right->SetOffset(sf::Vector2f(6,-3));
    
    pb = object->AddComponent<PhysicsBody>();

    cursor_graphic = object->GetScene()->AddUI<CursorGraphic>();
    selected_block = 0;
    in_fly_mode = false;

}

void PlayerController::Update(){

    // Movement...
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        pb->velocity.x = -0.08;
        object->GetComponent<SpriteRenderer>()->SetFlip(true);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        pb->velocity.x = 0.08;
        object->GetComponent<SpriteRenderer>()->SetFlip(false);
    }
    else{
        pb->velocity.x = 0;       
    }

    if(in_fly_mode){

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            pb->velocity.y = -0.08;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            pb->velocity.y = 0.08;
        }
        else{
            pb->velocity.y = 0;       
        }
    }



    CalculateMouse();
}


void PlayerController::CalculateMouse(){

    sf::Vector2i _mouse_pos = Mouse::DisplayPosition();
    sf::Vector2f player_center = object->GetScene()->GetActiveCamera()->WorldToScreenPosition(object->GetTransform()->position);

    _mouse_pos.x -= round(player_center.x);
    _mouse_pos.y -= round(player_center.y);

    mouse_world_pos = sf::Vector2i(round(object->GetTransform()->position.x) + _mouse_pos.x, 
                             round(object->GetTransform()->position.y) + _mouse_pos.y );



    sf::Vector2i cursor = world->RoundWorld(mouse_world_pos.x, mouse_world_pos.y);
    cursor_graphic->GetTransform()->position = sf::Vector2f(cursor.x, cursor.y);

    focused_block = world->GetTileWorld(cursor.x, cursor.y);

    if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
        
        if(cursor != focused_block_position){
            focused_block_position = cursor;

            breaking_completeness = 0;
            sound_increment = 0;
        }

        if(focused_block != -1){
            float increment = 0.02f * Time::Dt() / (float)Items[focused_block].durability;
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
            world->BreakTileWorld(cursor.x, cursor.y, SetLocation::FOREGROUND, true);
            focused_block_position = sf::Vector2i(-1,-1); // clearing the focused block (an impossible position)
        }
    }
    else{
        cursor_graphic->SetComplete(0);
    }

    if(sf::Mouse::isButtonPressed(sf::Mouse::Right)){
        
        if(focused_block != selected_block && 
        !body_collider->Overlaps(mouse_world_pos.x, mouse_world_pos.x + 8, 
                                mouse_world_pos.y, mouse_world_pos.y + 8)){

            world->SetTileWorld(selected_block, mouse_world_pos.x, mouse_world_pos.y, SetLocation::FOREGROUND, true);
        }
    }
}


void PlayerController::CatchEvent(sf::Event event){

    if (event.type == sf::Event::KeyPressed)
    {
        switch(event.key.scancode){

            case sf::Keyboard::Scan::W: {
        
                if(in_fly_mode){
                    break;
                }

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
                selected_block %= ItemCode::c_NUMBER_OF_ITEMS;
                if(selected_block < 0){
                    selected_block = ItemCode::c_NUMBER_OF_ITEMS - 1;
                }

                while(!Items[selected_block].placable){
                    selected_block--;
                    selected_block %= ItemCode::c_NUMBER_OF_ITEMS;
                    if(selected_block < 0){
                        selected_block = ItemCode::c_NUMBER_OF_ITEMS - 1;
                    }
                }

                break;
            }
            case sf::Keyboard::Scan::Num2: {
                selected_block++;
                selected_block %= ItemCode::c_NUMBER_OF_ITEMS;

                while(!Items[selected_block].placable){
                    selected_block--;
                    selected_block %= ItemCode::c_NUMBER_OF_ITEMS;
                    if(selected_block < 0){
                        selected_block = ItemCode::c_NUMBER_OF_ITEMS - 1;
                    }
                }
            }        
        }
        
    }       
}

void PlayerController::Respawn(){
    object->GetTransform()->position = sf::Vector2f(50, -500);
}


void PlayerController::SetFlyMode(bool state){
    in_fly_mode = state;
    pb->SetGravityState(!state);
}
bool PlayerController::GetFlyMode(){
    return in_fly_mode;
}