#include "Scene.h"
#include "../Utility/Time.h"
#include "../Object/Physics/BoxCollider.h"
#include "../Object/Rendering/Tilemap.h"
#include <iostream>
#include <algorithm>
#include <climits>

Camera* Scene::active_camera = nullptr;

Scene::Scene() : bounds_min_x(INT_MIN), bounds_min_y(INT_MIN), bounds_max_x(INT_MAX), bounds_max_y(INT_MAX), event_focus(nullptr), object_holds_event_focus(false){
    render_layers_in_use.push_back(0);
    ui_render_layers_in_use.push_back(0);
}

Core* Scene::GetCore(){
    return this->core;
}
void Scene::LinkCore(Core* core){
    this->core = core;
}


void Scene::Restart(){
    this->ClearAll();
    this->Start();
}

Camera* Scene::GetActiveCamera(){
    if(active_camera == nullptr){
        std::cout << "ERROR : Something is trying to access the active camera before it has been assigned Scene::GetActiveCamera\n";
    }
    return active_camera;
}

void Scene::SetActiveCamera(Camera* camera){
    active_camera = camera;
}

void Scene::InternalUpdate(){
    
    if(active_camera == nullptr){
        std::cout << "ERROR : No camera is selected, set using Scene::SetActiveCamera(), Scene::InternalUpdate()\n";
        return;
    }


    box_colliders.clear();
    tilemaps.clear();

    CollectBoxCollider(objects);
    CollectBoxCollider(objects_additional);
    
    CollectTilemap(objects);
    CollectTilemap(objects_additional);


    this->Update();

    UpdateObjectArray(objects);
    UpdateObjectArray(objects_additional);
    UpdateObjectArray(ui);
    UpdateObjectArray(ui_additional);
}

void Scene::InternalCatchEvent(sf::Event event){

    this->CatchEvent(event);
    
    CatchEventObjectArray(objects, event);
    CatchEventObjectArray(objects_additional, event);
    CatchEventObjectArray(ui, event);
    CatchEventObjectArray(ui_additional, event);
}

void Scene::UpdateObjectArray(std::vector<Object*>& array){
    
    // iterate over each object in said layer
    for(auto obj : array){

        // skip disabled objects
        if(!obj->IsActive()){
            continue;
        }

        obj->UpdateEventFocusBounded();

        for(auto comp : *obj->GetComponents()){
            // ignore disabled comps
            if(!comp->IsActive()){
                continue;
            }

            comp->UpdateEventFocusBounded();
            comp->Update();
            comp->UpdateSecondary();
        }
        obj->Update();
    }
}

void Scene::CatchEventObjectArray(std::vector<Object*>& array, sf::Event event){

    for(auto obj : array){

        // skip disabled objects
        if(!obj->IsActive()){
            continue;
        }

        obj->CatchEvent(event);
        
        for(auto comp : *obj->GetComponents()){
            // ignore disabled comps
            if(!comp->IsActive()){
                continue;
            }
            comp->CatchEvent(event);
        }
    }
}


void Scene::CollectBoxCollider(std::vector<Object*>& array){

    for(auto obj : array){
        std::vector<BoxCollider*> bx_colliders = obj->GetAllComponentsOf<BoxCollider>();

        for(int i = 0; i < bx_colliders.size(); i++){

            if(!bx_colliders[i]->IsActive()){
                continue;
            }

            box_colliders.push_back(bx_colliders[i]);
        }
    }
}
void Scene::CollectTilemap(std::vector<Object*>& array){
    
    for(auto obj : array){
        std::vector<Tilemap*> t_maps = obj->GetAllComponentsOf<Tilemap>();

        for(int i = 0; i < t_maps.size(); i++){
            tilemaps.push_back(t_maps[i]);
        }
    }
}

void Scene::ClearObjectArray(std::vector<Object*>& array){
    
    for(auto obj : array){

        obj->ClearComponents();
        Memory::Delete<Object>(obj, __FUNCTION__);
    }
    array.clear();
}

void Scene::ClearAll(){
    
    active_camera = nullptr;

    tilemaps.clear();
    box_colliders.clear();

    // objects_additonal should be managed and deleted! by any inherited scene that uses it
    ClearObjectArray(objects);
    ClearObjectArray(ui);
}

void Scene::DefineNewRenderLayer(int render_layer, bool is_ui){
    
    std::vector<int>* render_vector = &render_layers_in_use;
    if(is_ui){
        render_vector = &ui_render_layers_in_use;
    }

    for(int i = 0; i < render_vector->size(); i++){
        if(render_vector->at(i) == render_layer){
            return;
            // nothing to process, render_layer already exists
        }
    }

    // sort to maintain correct rendering order
    render_vector->push_back(render_layer);
    std::sort(render_vector->begin(), render_vector->end());
}


void Scene::SetMinXBound(int x){
    bounds_min_x = x;
}
void Scene::SetMinYBound(int y){
    bounds_min_y = y;
}
void Scene::SetMaxXBound(int x){
    bounds_max_x = x;
}
void Scene::SetMaxYBound(int y){
    bounds_max_y = y;
}
