#include "Scene.h"
#include "../Utility/Time.h"
#include "../Object/PointLight.h"
#include "../Object/BoxCollider.h"
#include <iostream>


Core* Scene::GetCore(){
    return this->core;
}
void Scene::LinkCore(Core* core){
    this->core = core;
}


void Scene::Restart(){
    this->ClearObjects();
    this->Start();
}

Camera* Scene::GetActiveCamera(){
    if(active_camera == nullptr){
        std::cout << "ERROR : Something is trying to access the active camera before it has been assigned Scene::GetActiveCamera\n";
    }
    return active_camera;
}

void Scene::SetActiveCamera(Camera* camera){
    if(camera == nullptr){
        std::cout << "ERROR : Attempting to set active camera to nullptr in Scene::SetActiveCamera()\n";
    }
    this->active_camera = camera;
}

void Scene::InternalUpdate(){
    
    if(active_camera == nullptr){
        std::cout << "ERROR : No camera is selected, set using Scene::SetActiveCamera()\n";
        return;
    }

    this->Update();

    for(auto obj : objects){

        for(auto comp : *obj->GetComponents()){
            comp->Update();
            comp->UpdateSecondary();
        }
        obj->Update();
    }
}

void Scene::InternalCatchEvent(sf::Event event){

    this->CatchEvent(event);
    
    for(auto obj : objects){

        obj->CatchEvent(event);
    }
}

std::vector<Object*>* Scene::GetObjects(){
    return &objects;
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
std::vector<BoxCollider*>* Scene::GetBoxColliders(){
    return &box_colliders;
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
std::vector<PointLight*>* Scene::GetPointLights(){
    return &point_lights;
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
std::vector<Tilemap*>* Scene::GetTilemaps(){
    return &tilemaps;
}

void Scene::ClearObjects(){
    
    active_camera = nullptr;

    for(auto obj : objects){
        delete obj;
    }
    objects.clear();
}


Scene::~Scene(){
    this->ClearObjects();
}