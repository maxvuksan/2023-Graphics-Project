#include "UIRect.h"
#include "../Object.h"
#include "../../Core/Core.h"

int UIRect::padding = 8;

void UIRect::Align(){

    float _width = object->GetTransform()->scale.x * width;
    float _height = object->GetTransform()->scale.y * height;

    Align(object->GetTransform(), _width, _height, screen_location_x, screen_location_y);

    this->OnAlign();
}

void UIRect::Align(Transform* transform, int _width, int _height, ScreenLocationX align_x, ScreenLocationY align_y){

    AlignX(transform, _width, align_x);
    AlignY(transform, _height, align_y);
}

void UIRect::AlignX(Transform* transform, int _width, ScreenLocationX align_x){

    int screen_width = Core::GetDisplayWidth();

    float positions[4] = {
        padding, 
        screen_width / 2.0f - _width / 2.0f,
        screen_width - _width - padding,

        transform->position.x, // FREE, does not realign
    };

    transform->position.x = positions[(int)align_x];
}
void UIRect::AlignY(Transform* transform, int _height, ScreenLocationY align_y){

    int screen_height = Core::GetDisplayHeight();

    float positions[4] = {
        padding, 
        screen_height / 2.0f - _height / 2.0f,
        screen_height - _height - padding,

        transform->position.y, // FREE, does not realign
    };

    transform->position.y = positions[(int)align_y];
}


void UIRect::SetAlign(ScreenLocationX align_x, ScreenLocationY align_y){

    screen_location_x = align_x;
    screen_location_y = align_y;
    Align();
}
