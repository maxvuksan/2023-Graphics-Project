#pragma once
#include <SFML/Graphics.hpp>
#include "../Utility/Time.h"
#include "../Core/Globals.h"

class Object;
class Component{

    /*
        Component

        Acts as a module to attach to objects.
    */

    public:

        Component();

        virtual void Start(){}
        virtual void Update(){}
        virtual void UpdateEventFocusBounded(){}
        virtual void CatchEvent(sf::Event event){}
        virtual void UpdateSecondary(){} // in case of inheritance using up functions

        virtual void Draw(sf::RenderTarget& surface){}
        virtual void DrawDebug(sf::RenderTarget& surface){}

        virtual ~Component(){}

        // Providing a reference to the object this is attached to
        void LinkObject(Object* object);
        Object* GetObject();

        bool IsActive(){return active;}
        void SetActive(bool state);

    protected:

        bool active;
        Object* object;

};