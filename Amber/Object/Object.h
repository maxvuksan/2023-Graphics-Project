#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "../Utility/Time.h"
#include "Component.h"
#include "Transform.h"
#include "../Core/Globals.h"
#include "../Utility/Memory.h"

class Scene;
class Core;
    
/*
    Acts as a building block for all entities in the program, taking a module approach,
    an object can be extended through generic reusable components.

    common methods to override...

    void Start(){}
    void Update(){}
    void Draw(sf::RenderTarget&){}
    void CatchEvent(sf::Event){}
*/
class Object{

    public:

        Object();

        virtual void Start(){}
        virtual void Update(){}
        // updates only when the scene has no event focus or this object is the focus
        virtual void UpdateEventFocusBounded(){}
        virtual void DrawDebug(sf::RenderTarget&){}
        virtual void Draw(sf::RenderTarget&){} // drawing the scene
        virtual void CatchEvent(sf::Event){}
        virtual void OnSetActive(){} // is called when SetActive(true) occurs
        virtual void OnDisable(){}
        
        bool IsActive();
        // @param state the state the object is being set to
        void SetActive(bool state);
        // should only be set by Scene::AddUI()
        void SetUI(bool state){ deleted_from_ui_map = state;}
        bool IsUI(){ return deleted_from_ui_map; }

        // An objects render layer is determined when it is initally added to the scene, Scene::AddObject<>( render_Layer )
        int GetRenderLayer(){return render_layer;}
        void SetRenderLayer(int render_layer);
        void LinkScene(Scene* scene);
        Scene* GetScene(){ return scene; }

        Transform* GetTransform() { return transform; }

        std::vector<Component*>* GetComponents(){ return &components; }

        template <typename T>
        T* AddComponent(int render_layer = 0){
            
            T* comp = Memory::New<T>(__FUNCTION__);
            Component* casted_comp = static_cast<Component*>(comp);

            components.push_back(casted_comp);

            casted_comp->LinkObject(this);          
            casted_comp->Start();
            casted_comp->SetRenderLayer(render_layer);

            return comp;
        }

        // @returns Pointer to the first component of type T
        template <typename T>
        T* GetComponent(){
            
            for(auto& comp : components){
                T* casted_comp = dynamic_cast<T*>(comp);

                if(casted_comp != NULL){
                    return casted_comp;
                }
            }       
            return nullptr;
        }
        
        // @returns Vector of all components of type T
        template <typename T>
        std::vector<T*> GetAllComponentsOf(){
            
            std::vector<T*> comp_vec;

            for(auto& comp : components){
                T* casted_comp = dynamic_cast<T*>(comp);

                if(casted_comp != NULL){
                    comp_vec.push_back(casted_comp);
                }
            }       
            return comp_vec;
        }

        // removes the first component of type T or the component specified by 'comp_reference'
        template <typename T>
        void RemoveComponent(T* comp_reference = nullptr){

            // remove first occurance
            if(comp_reference == nullptr){

                for(int i = 0; i < components.size(); i++){
                    T* casted_comp = dynamic_cast<T*>(components[i]);

                    if(casted_comp != NULL){
                        Memory::Delete<Component>(casted_comp, __FUNCTION__);
                        components.erase(components.begin() + i);
                        return;
                    }
                    i++;
                }
                return;
            }
            // remove through parameter
            for(int i = 0; i < components.size(); i++){
                if(components[i] == comp_reference){
                    Memory::Delete<Component>(comp_reference, __FUNCTION__);
                    components.erase(components.begin() + i);
                    return;
                }
            }
            
        }
        

        /*
            creates a new object, attaching to this object as a child. 
            When the parent is set active or destroyed, the child does the same

            @returns A pointer to the newly created object
        */
        template <typename T>
        T* AddChild(int render_Layer = 0){
            
            T* t = Memory::New<T>(__FUNCTION__);

            Object* obj = AddObjectToScene(t, render_Layer);
            obj->parent = this;
            children.push_back(obj);

            return t;
        }

        Object* GetParent();

        
        virtual ~Object(){}
        // clears all the components on the object
        void ClearComponents();
        // calls Destroy() on all child objects
        void DestroyCascadeToChildren();

        
        // Should this object be renderered on top of everything else (regardless of display size) at the windows resolution (specifically for UI Objects)
        void SetRenderAtWindowSize(bool render_at_window_size);
        // @returns true if the object will be rendered at the window size rather (regardless of display size) (specifcally for UI Objects)
        bool GetRenderAtWindowSize();

    protected:
        
        //removes the object from the scene, should be called by the object itself
        void Destroy();

    private:

        // any children are funneled to this function to be added to the scene object vector
        Object* AddObjectToScene(Object* object, int render_layer);

        int render_layer;

        // if either is false, treat as disabled
        Object* parent; 
        bool active;

        // should this object be deleted from the ui map or object map, is it an ui object
        bool deleted_from_ui_map;
        // specifically for UI objects, should this be drawn onto the window 
        bool render_at_window_size;

        Scene* scene;
        Transform* transform;
        
        std::vector<Object*> children;
        std::vector<Component*> components; 
};