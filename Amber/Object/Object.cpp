#include "Object.h"
#include "./Rendering/Tilemap.h"
#include "../Core/Scene.h"
#include <iostream>

Object::Object() : active(true), parent(nullptr), deleted_from_ui_map(false), render_at_window_size(false){
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

    DestroyCascadeToChildren();

    if(deleted_from_ui_map){
        GetScene()->DeleteUI(this);
    }
    else{
        GetScene()->DeleteObject(this);
    }
}

void Object::DestroyCascadeToChildren(){
    
    for(auto& child : children){
        child->Destroy();
    }
}

void Object::ClearComponents(){

    for(auto comp : components){
        Memory::Delete<Component>(comp, __FUNCTION__);
    }
    
    components.clear();
}

void Object::SetRenderAtWindowSize(bool render_at_window_size){
    this->render_at_window_size = render_at_window_size;
}

bool Object::GetRenderAtWindowSize(){
    return this->render_at_window_size;
}