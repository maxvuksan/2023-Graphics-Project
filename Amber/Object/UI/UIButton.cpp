#include "UIButton.h"
#include "../../Utility/Mouse.h"
#include "../../Core/AssetManager.h"


sf::Text UIButton::text;


UIButton::UIButton(): on_click_function([](){}){

    outline_shape.setFillColor(sf::Color::Transparent);
    previous_position = sf::Vector2f(-9999,-9999);
    text_position = sf::Vector2f(-9999,-9999);

}

void UIButton::CatchEventEventFocusBounded(sf::Event event){

    if(event.type == sf::Event::MouseButtonPressed){

        if(mouse_over){
            this->on_click_function();
            
        }
    }


}

void UIButton::SetString(std::string string){
    this->text_string = string;
    previous_position = sf::Vector2f(-9999,-9999);
    text_position = sf::Vector2f(-9999,-9999);

}

void UIButton::SetTexture(const char* label){
    sprite.setTexture(*AssetManager::GetTexture(label));
}
void UIButton::SetTextureRect(int left, int top, int width, int height){
    sprite.setTextureRect(sf::IntRect(left, top, width, height));
}


void UIButton::Update(){ 
    if(!IsActive()){
        return;
    }

    mouse_over = false;
    // is the mouse over the button
    if(Mouse::DisplayPosition().x > object->GetTransform()->position.x && Mouse::DisplayPosition().x < object->GetTransform()->position.x + width){
        if(Mouse::DisplayPosition().y > object->GetTransform()->position.y && Mouse::DisplayPosition().y < object->GetTransform()->position.y + height){

            mouse_over = true;
        
        }
    }
}

void UIButton::Draw(sf::RenderTarget& surface){

    if(!IsActive()){
        return;
    }

    if(sprite.getTexture() != nullptr){
        sprite.setPosition(object->GetTransform()->position);
        surface.draw(sprite);
    }

    if(mouse_over){
        outline_shape.setOutlineColor(sf::Color::White);
        outline_shape.setOutlineThickness(2);
    }
    else{
        outline_shape.setOutlineColor(sf::Color(200,200,200));
        outline_shape.setOutlineThickness(1);
    }
    outline_shape.setSize(sf::Vector2f(width, height));
    outline_shape.setPosition(object->GetTransform()->position);

    text.setString(text_string);

    UpdateTextPosition();

    surface.draw(outline_shape);
    surface.draw(text);
}

void UIButton::UpdateTextPosition(){

    if(object->GetTransform()->position != previous_position){
        sf::FloatRect textRect = text.getLocalBounds();
        
        text_origin = sf::Vector2f(textRect.left + textRect.width/2.0f, textRect.top  + textRect.height/2.0f);
        text_position = sf::Vector2f(object->GetTransform()->position.x + width/2.0f, object->GetTransform()->position.y + height/2.0f);
    }
    previous_position = object->GetTransform()->position;

    text.setPosition(text_position);
    text.setOrigin(text_origin);
}

void UIButton::SetOnClickCallback(const std::function<void()>& callback_function){
    this->on_click_function = callback_function;
}
