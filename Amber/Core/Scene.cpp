#include "Scene.h"
#include "../Utility/Time.h"
#include "../Object/Rendering/PointLight.h"
#include "../Object/Physics/BoxCollider.h"
#include "../Object/Rendering/Tilemap.h"
#include <iostream>
#include <climits>

Camera* Scene::active_camera = nullptr;

Scene::Scene() : bounds_min_x(INT_MIN), bounds_min_y(INT_MIN), bounds_max_x(INT_MAX), bounds_max_y(INT_MAX), event_focus(nullptr), object_holds_event_focus(false){}

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

    this->Update();

    // iterate over each render layer
    for (auto layer = objects.begin(); layer != objects.end(); layer++) {

        // iterate over each object in said layer
        for(auto obj : layer->second){

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

    // do the same for each ui element
    for (auto layer = ui.begin(); layer != ui.end(); layer++) {

        // iterate over each object in said layer
        for(auto obj : layer->second){

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
}

void Scene::InternalCatchEvent(sf::Event event){

    this->CatchEvent(event);
    
    // block all events if ^ we have a focus

    // iterate over each render layer
    for (auto layer = objects.begin(); layer != objects.end(); layer++) {

        // iterate over each object

        for(auto obj : layer->second){

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

    // do the same for each ui element
    for (auto layer = ui.begin(); layer != ui.end(); layer++) {

        // iterate over each object

        for(auto obj : layer->second){
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
}




void Scene::AddBoxCollider(BoxCollider* collider){
    this->box_colliders.push_back(collider);
}

void Scene::RemoveBoxCollider(BoxCollider* collider){
    for(int i = 0; i < box_colliders.size(); i++){

        if(collider == box_colliders[i]){
            box_colliders.erase(box_colliders.begin() + i);
            break;
        }
    }
}

void Scene::AddPointLight(PointLight* point_light){
    this->point_lights.push_back(point_light);
}

void Scene::RemovePointLight(PointLight* point_light){
    for(int i = 0; i < point_lights.size(); i++){

        if(point_light == point_lights[i]){
            point_lights.erase(point_lights.begin() + i);
            break;
        }
    }
}

void Scene::AddTilemap(Tilemap* tilemap){
    this->tilemaps.push_back(tilemap);
}

void Scene::RemoveTilemap(Tilemap* tilemap){
    for(int i = 0; i < tilemaps.size(); i++){

        if(tilemap == tilemaps[i]){
            tilemaps.erase(tilemaps.begin() + i);
            break;
        }
    }
}

void Scene::ClearAll(){
    
    active_camera = nullptr;

    point_lights.clear();
    tilemaps.clear();
    box_colliders.clear();

    for (auto layer = objects.begin(); layer != objects.end(); layer++) {

        for(auto obj : layer->second){

            obj->ClearComponents();
            Memory::Delete<Object>(obj, __FUNCTION__);
        }
    }
    objects.clear();

    for (auto layer = ui.begin(); layer != ui.end(); layer++) {

        for(auto obj : layer->second){

            obj->ClearComponents();
            Memory::Delete<Object>(obj, __FUNCTION__);
        }
    }
    ui.clear();
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
