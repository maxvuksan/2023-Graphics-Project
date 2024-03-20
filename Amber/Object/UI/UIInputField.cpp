#include "UIInputField.h"

UIInputField::UIInputField() : tracked_string(""){
    
}

void UIInputField::CatchEventEventFocusBounded(sf::Event event){

    if(event.type == sf::Event::MouseButtonPressed){

        if(mouse_over){
            selected = true;
        }
        else{
            selected = false;
        }
    }
    else if(event.type == sf::Event::TextEntered){

        if(selected){
            
            // backspace
            if(event.text.unicode == 8){
                if(tracked_string.size() > 0){
                    tracked_string.pop_back();
                }
            }
            else if(tracked_string.size() <= 50 ){

                tracked_string += event.text.unicode;
            }
        }
    }

    if(selected){
        SetString(tracked_string);
    }
    else if(tracked_string == ""){
        SetString(placeholder_string);
    }
    else{
        SetString(tracked_string);
    }
};

void UIInputField::SetPlaceholder(std::string string){
    this->placeholder_string = string;
    SetString(placeholder_string);
}
