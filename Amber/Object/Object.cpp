#include "Object.h"
#include "../Core/Scene.h"
#include <iostream>

Object::Object() : active(true), deleted_from_ui_map(false){
    this->transform = AddComponent<Transform>();
}

void Object::LinkScene(Scene* scene, int render_layer){
    if(scene == nullptr){
        std::cout << "ERROR : Attempting to link objects scene property to a nullptr Object::LinkScene()\n";
    }
    this->render_layer = render_layer;
    this->scene = scene;
}


void Object::Destroy(){
    OnDestroy();

    for(auto& child : children){
        child->Destroy();
    }

    if(deleted_from_ui_map){
        GetScene()->DeleteUI(this);
    }
    else{
        GetScene()->DeleteObject(this);
    }
}

Object::~Object(){

    for(auto& comp : components){
        delete comp;
    }
}