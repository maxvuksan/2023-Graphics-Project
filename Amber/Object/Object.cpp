#include "Object.h"
#include "../Core/Scene.h"
#include <iostream>

Object::Object() : active(true), parent(nullptr), deleted_from_ui_map(false){
    this->transform = AddComponent<Transform>();
}

void Object::LinkScene(Scene* scene, int render_layer){
    if(scene == nullptr){
        std::cout << "ERROR : Attempting to link objects scene property to a nullptr Object::LinkScene()\n";
    }
    this->render_layer = render_layer;
    this->scene = scene;
}

bool Object::IsActive(){
    if(!active){
        return false;
    }
    if(parent != nullptr && !parent->IsActive()){
        return false;
    }
    return true;
}

Object* Object::AddObjectToScene(Object* object, int render_layer){
    scene->AddExisitingObject(object, render_layer);
    return object;
}

Object* Object::GetParent(){
    return this->parent;
}

void Object::Destroy(){

    for(auto& child : children){
        child->Destroy();
    }

    OnDestroy();

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