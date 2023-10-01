#include "Component.h"
#include <iostream>

void Component::LinkObject(Object* object){
    if(object == nullptr){
        std::cout << "ERROR : Attempting to link component object property to a nullptr Component::LinkObject()\n";
    }
    this->object = object;
}

Object* Component::GetObject(){
    return object;
}

Component::~Component(){
    this->OnDestroy();
}