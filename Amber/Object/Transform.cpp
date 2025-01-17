#include "Transform.h"
#include "Object.h"

sf::Vector2f Transform::GetGlobalPosition() const{

    Object* parent = object->GetParent();

    if(parent == nullptr){
        return position;
    }
    else{
        return position + parent->GetTransform()->GetGlobalPosition();
    }
}