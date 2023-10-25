#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include "../Object/Object.h"
#include "../Object/Camera.h"
#include <iostream>

class PointLight;
class Tilemap;
class Core;
class BoxCollider;

class Scene {

    /*
        A way to divide sections of content, 
        
        some scenes could include...

        Main Menu Page
        Level Select
        Level 1
        Level 2
        etc...
    */

    public:

        Core* GetCore();
        void LinkCore(Core* core);

        virtual void Start(){}
        virtual void Update(){}
        virtual void CatchEvent(sf::Event){}

        // Responsible for updating objects and object components
        void InternalUpdate();
        void InternalCatchEvent(sf::Event);
        void InternalOnResize();

        // Removes all objects from the scene and recalls the Start() method
        void Restart();

        static Camera* GetActiveCamera();
        static void SetActiveCamera(Camera* camera);

        // Adds an object of type T to the scene (T must have a base class of Object)
        template <typename T>
        T* AddObject(int render_layer = 0){

            // not in map yet
            if(objects.count(render_layer) < 1){
                objects[render_layer] = {};
            }

            T* obj = new T;
            objects[render_layer].push_back(obj);

            obj->LinkScene(this, render_layer);
            obj->Start();

            return obj;
        }

        // Adds an object to the scene which has already had its memory allocated
        Object* AddExisitingObject(Object* obj, int render_layer){
            
            // not in map yet
            if(objects.count(render_layer) < 1){
                objects[render_layer] = {};
            }

            objects[render_layer].push_back(obj);

            obj->LinkScene(this, render_layer);
            obj->Start();

            return obj;
        }

        /*
            Removes an object from the scene
            @param target The object to remove
        */
        void DeleteObject(Object* target){

            for(int i = 0; i < objects[target->GetRenderLayer()].size(); i++){
                if(objects[target->GetRenderLayer()][i] == target){

                    objects[target->GetRenderLayer()].erase(objects[target->GetRenderLayer()].begin() + i);
                    delete target;

                }
            }
        }

        template <typename T>
        T* AddUI(int render_layer = 0){

            // not in map yet
            if(ui.count(render_layer) < 1){
                ui[render_layer] = {};
            }

            T* obj = new T;
            ui[render_layer].push_back(obj);

            obj->SetUI(true);
            obj->LinkScene(this, render_layer);
            obj->Start();

            return obj;
        }

        void DeleteUI(Object* target){

            for(int i = 0; i < ui[target->GetRenderLayer()].size(); i++){
                if(ui[target->GetRenderLayer()][i] == target){

                    ui[target->GetRenderLayer()].erase(ui[target->GetRenderLayer()].begin() + i);
                    delete target;

                }
            }
        }

        std::map<int, std::vector<Object*>>* GetObjects(){ return &objects;}
        std::map<int, std::vector<Object*>>* GetUI(){ return &ui;}

        void AddBoxCollider(BoxCollider* collider);
        void RemoveBoxCollider(BoxCollider* collider);
        std::vector<BoxCollider*>* GetBoxColliders(){return &box_colliders;}

        void AddPointLight(PointLight* point_light);
        void RemovePointLight(PointLight* point_light);
        std::vector<PointLight*>* GetPointLights(){return &point_lights;}

        void AddTilemap(Tilemap* point_light);
        void RemoveTilemap(Tilemap* point_light);
        std::vector<Tilemap*>* GetTilemaps(){return &tilemaps;}

        ~Scene();
        // deletes all objects from the scene
        void ClearAll();

    private:

        std::vector<BoxCollider*> box_colliders;

        // render layer, vector<Object*>
        std::map<int, std::vector<Object*>> objects;
        std::map<int, std::vector<Object*>> ui;

        std::vector<PointLight*> point_lights;
        std::vector<Tilemap*> tilemaps;

        Core* core;
        static Camera* active_camera;

};