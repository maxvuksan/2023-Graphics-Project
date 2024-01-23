#include "Component.h"
#include "Object.h"
#include "../Core/Scene.h"
#include <iostream>

Component::Component() : active(true), render_layer(0){}

void Component::LinkObject(Object* object){
    if(object == nullptr){
        std::cout << "ERROR : Attempting to link component object property to a nullptr Component::LinkObject()\n";
    }
    this->object = object;
}

Object* Component::GetThisObject(){
    return object;
}

void Component::SetActive(bool state){

    if(state != this->active){
        if(state){
            OnSetActive();
        }
        else{
            OnDisable();
        }
    }

    this->active = state;


}

void Component::SetRenderLayer(int _render_layer){
    if(this->render_layer != _render_layer){
        this->render_layer = _render_layer;
        GetThisObject()->GetScene()->DefineNewRenderLayer(_render_layer, GetThisObject()->IsUI());
    }
}