#include "Object.h"
#include <iostream>

Object::Object() : active(true){
    this->transform = AddComponent<Transform>();
}

void Object::LinkScene(Scene* scene, int render_layer){
    if(scene == nullptr){
        std::cout << "ERROR : Attempting to link objects scene property to a nullptr Object::LinkScene()\n";
    }
    this->render_layer = render_layer;
    this->scene = scene;
}


Object::~Object(){

    this->OnDestroy();

    for(auto& comp : components){
        delete comp;
    }
}