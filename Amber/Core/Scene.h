#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "../Object/Object.h"
#include "../Object/Camera.h"
#include <iostream>

class PointLight;
class Tilemap;
class Core;
class BoxCollider;

class Scene {

    /*
        Scene:

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

        // Removes all objects from the scene and recalls the Start() method
        void Restart();

        Camera* GetActiveCamera();
        void SetActiveCamera(Camera* camera);

        // Adds an object of type T to the scene (T must have a base class of Object)
        template <typename T>
        T* AddObject(){
            T* obj = new T;
            objects.push_back(obj);

            obj->LinkCore(core);
            obj->LinkScene(this);
            
            obj->Start();

            return obj;
        }

        /*
            Removes an object from the scene
            @param target The object to remove
        */
        template <typename T>
        void DeleteObject(Object* target){

            for(int i = 0; i < objects.size(); i++){
                if(objects[i] == target){

                    delete target;
                    objects.erase(objects.begin() + i);
                }
            }
        }
        std::vector<Object*>* GetObjects();

        void AddBoxCollider(BoxCollider* collider);
        void RemoveBoxCollider(BoxCollider* collider);
        std::vector<BoxCollider*>* GetBoxColliders();

        void AddPointLight(PointLight* point_light);
        void RemovePointLight(PointLight* point_light);
        std::vector<PointLight*>* GetPointLights();

        void AddTilemap(Tilemap* point_light);
        void RemoveTilemap(Tilemap* point_light);
        std::vector<Tilemap*>* GetTilemaps();

        ~Scene();
        // deletes all objects from the scene
        void ClearObjects();

    private:

        std::vector<BoxCollider*> box_colliders;
        std::vector<Object*> objects;
        std::vector<PointLight*> point_lights;
        std::vector<Tilemap*> tilemaps;

        Core* core;
        Camera* active_camera;

};