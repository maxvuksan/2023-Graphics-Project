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


    pb = AddComponent<PhysicsBody>();
    AddComponent<PointLight>();

    cursor_shape.setOutlineColor(sf::Color::White);
    cursor_shape.setSize(sf::Vector2f(8,8));
    cursor_shape.setOutlineThickness(1);
    cursor_shape.setFillColor(sf::Color::Transparent);

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

    mouse_pos = sf::Vector2i(round(GetTransform()->position.x) + _mouse_pos.x, 
                             round(GetTransform()->position.y) + _mouse_pos.y );


    if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
        
        world->SetTile_FromWorld(-1, mouse_pos.x, mouse_pos.y);
    }
    if(sf::Mouse::isButtonPressed(sf::Mouse::Right)){
        
        world->SetTile_FromWorld(selected_block, mouse_pos.x, mouse_pos.y);
    }
}

void Player::Draw_Debug(sf::RenderTarget& surface){

    sf::Vector2i cursor = world->RoundWorld(mouse_pos.x, mouse_pos.y);
    cursor_shape.setPosition(GetScene()->GetActiveCamera()->WorldToScreenPosition(sf::Vector2f(cursor.x, cursor.y)));

    surface.draw(cursor_shape);
}


void Player::CatchEvent(sf::Event event){

    if (event.type == sf::Event::KeyPressed)
    {
        switch(event.key.scancode){

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
    if(event.type == sf::Event::MouseButtonPressed){
        return;
        if(event.mouseButton.button == 0){
            
            world->SetTile_FromWorld(-1, mouse_pos.x, mouse_pos.y);
        }
        if(event.mouseButton.button == 1){
            
            world->SetTile_FromWorld(BlockCode::c_Stone_Bricks, mouse_pos.x, mouse_pos.y);
        }
    }
}

