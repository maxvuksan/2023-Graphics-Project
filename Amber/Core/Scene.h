#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include "../Object/Object.h"
#include "../Object/Camera.h"
#include <iostream>
#include "../Utility/Memory.h"

class Tilemap;
class Core;
class BoxCollider;

class Client;
class Server;

/*
    A way to divide sections of content, 
    
    an example of this division could be...

    Main Menu Page
    Settings Page
    Level Select
    Level World
    etc...
*/
class Scene {


    public:

        Scene();

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

            T* obj = Memory::New<T>(__FUNCTION__);

            objects.push_back(obj);
            
            obj->LinkScene(this);
            obj->SetRenderLayer(render_layer);
            obj->Start();

            return obj;
        }

        // Adds an object to the scene which has already had its memory allocated
        Object* AddExisitingObject(Object* obj, int render_layer){
            
            objects.push_back(obj);

            obj->LinkScene(this);
            obj->SetRenderLayer(render_layer);
            obj->Start();


            return obj;
        }

        /*
            Removes an object from the scene
            @param target The object to remove
        */
        void DeleteObject(Object* target){

            for(int i = 0; i < objects.size(); i++){
                if(objects[i] == target){

                    objects.erase(objects.begin() + i);
                    
                    target->ClearComponents();
                    target->DestroyCascadeToChildren();
                    Memory::Delete<Object>(target, __FUNCTION__);
                }
            }
        }

        template <typename T>
        T* AddUI(int render_layer = 0){

            T* obj = Memory::New<T>(__FUNCTION__);

            ui.push_back(obj);

            obj->SetUI(true);
            obj->LinkScene(this);
            obj->SetRenderLayer(render_layer);
            obj->Start();

            return obj;
        }

        void DeleteUI(Object* target){

            for(int i = 0; i < ui.size(); i++){
                if(ui[i] == target){

                    ui.erase(ui.begin() + i);

                    target->ClearComponents();
                    target->DestroyCascadeToChildren();
                    Memory::Delete<Object>(target, __FUNCTION__);
                }
            }
        }

        // @returns all the active objects in the scene
        std::vector<Object*>* GetThisObjects(){ return &objects;}
        // providing inheritied scenes control over there own object container 
        std::vector<Object*>* GetThisObjectsAdditional(){ return &objects_additional; };
        // @returns all the active UI objects in the scene
        std::vector<Object*>* GetUI(){ return &ui;}
        // providing inheritied scenes control over there own object container (drawn in terms of ui)
        std::vector<Object*>* GetUIAdditional(){ return &ui_additional; };

        // checks if a box collider exists in each object
        void CollectBoxCollider(std::vector<Object*>& array);
        std::vector<BoxCollider*>* GetBoxColliders(){return &box_colliders;}

        // checks if a tilemap exists in each object
        void CollectTilemap(std::vector<Object*>& array);
        std::vector<Tilemap*>* GetTilemaps(){return &tilemaps;}

        sf::Vector2i GetMinBounds(){return sf::Vector2i(bounds_min_x, bounds_min_y);}
        sf::Vector2i GetMaxBounds(){return sf::Vector2i(bounds_max_x, bounds_max_y);}
        void SetMinXBound(int x);
        void SetMinYBound(int y);
        void SetMaxXBound(int x);
        void SetMaxYBound(int y);
              
        virtual ~Scene(){}
        
        // adds a new render layer to the drawing process (only if it does not already exist)
        void DefineNewRenderLayer(int render_layer, bool is_ui);
        std::vector<int>* GetRenderLayers(){return &render_layers_in_use;}
        std::vector<int>* GetUIRenderLayers(){ return &ui_render_layers_in_use;}
        // deletes all objects from the scene
        void ClearAll();
        // SetEventFocus allows objects to only update or catch events while in said focus
        void SetEventFocus(Object* object);
        Object* GetEventFocus(){return event_focus_object;}
        void ClearEventFocus();

        /*
            allows references of client server objects to be accessed within a scene
        */
        void SetClient(Client* client){this->set_client = client;}
        void SetServer(Server* server){this->set_server = server;}
        Client* GetClient(){return this->set_client;}
        Server* GetServer(){return this->set_server;}

    


    private:

        Client* set_client;
        Server* set_server;

        void UpdateObjectArray(std::vector<Object*>& array);
        void CatchEventObjectArray(std::vector<Object*>& array, sf::Event event);
        void ClearObjectArray(std::vector<Object*>& array);
        void OnResizeObjectArray(std::vector<Object*>& array);

        std::vector<Object*> objects;
        std::vector<Object*> ui;

        // reserved for scenes for have there own object container to manipulate (e.g. lots of chunks we dont want to keep in simulation at all times)
        std::vector<Object*> objects_additional;
        std::vector<Object*> ui_additional;
           
        std::vector<Tilemap*> tilemaps;
        std::vector<BoxCollider*> box_colliders;

        // a sorted list of what render layers are in use
        std::vector<int> render_layers_in_use;
        std::vector<int> ui_render_layers_in_use;


        Core* core;
        static Camera* active_camera;

        // if no object is focused, this is nullptr
        Object* event_focus_object;

        int bounds_min_x;
        int bounds_min_y;
        int bounds_max_x;
        int bounds_max_y;

};