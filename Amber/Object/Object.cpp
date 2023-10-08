#include "Object.h"
#include <iostream>

Object::Object() : active(true){
    this->transform = AddComponent<Transform>();
}



void Object::LinkCore(Core* core){
    if(core == nullptr){
        std::cout << "ERROR : Attempting to set objects core property to nullptr in Object::LinkCore()\n";
    }
    this->core = core;
}
void Object::LinkScene(Scene* scene){
    if(scene == nullptr){
        std::cout << "ERROR : Attempting to link objects scene property to a nullptr Object::LinkScene()\n";
    }
    this->scene = scene;
}


Object::~Object(){

    this->OnDestroy();

    for(auto& comp : components){
        delete comp;
    }
}