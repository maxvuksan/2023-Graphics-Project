#include "Component.h"
#include <iostream>

Component::Component() : active(true){}

void Component::LinkObject(Object* object){
    if(object == nullptr){
        std::cout << "ERROR : Attempting to link component object property to a nullptr Component::LinkObject()\n";
    }
    this->object = object;
}

Object* Component::GetObject(){
    return object;
}

void Component::SetActive(bool state){
    this->active = state;
}