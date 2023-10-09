#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "../Utility/Time.h"
#include "Component.h"
#include "Transform.h"
#include "../Core/Globals.h"

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
        virtual void Draw_Debug(sf::RenderTarget&){}
        virtual void Draw_Window(sf::RenderTarget&){}
        virtual void Draw(sf::RenderTarget&){} // drawing the scene
        virtual void CatchEvent(sf::Event){}

        bool IsActive(){ return active; }
        void SetActive(bool state){ active = state; }

        // provides a reference to the Core object, allowing communication to said parent
        void LinkCore(Core* core);
        Core* GetCore() { return core; }
        void LinkScene(Scene* scene);
        Scene* GetScene(){ return scene; }

        Transform* GetTransform() { return transform; }

        std::vector<Component*>* GetComponents(){ return &components; }

        template <typename T>
        T* AddComponent(){
            
            T* comp = new T;
            Component* casted_comp = static_cast<Component*>(comp);

            components.push_back(casted_comp);

            casted_comp->LinkObject(this);          
            casted_comp->Start();

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
                    components.erase(components.begin() + i);
                    return;
                }
            }
            
        }
        
        ~Object(); 

        // Allows children classes to expand upon ~Object(), due to inheritied nature, children should not have destructors 
        virtual void OnDestroy(){}

    private:

        bool active;

        Scene* scene;
        Core* core; 
        Transform* transform;
    
        std::vector<Component*> components; 
};