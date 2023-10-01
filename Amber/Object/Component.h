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

        virtual void Start(){}
        virtual void Update(){}

        // drawing out what on screen is in shadow, black is fully in shadow, white is fully lit
        virtual void Draw_ShadowPass(sf::RenderTarget& surface){}
        // after the shadow texture has been blurred we can change it before it is composed with the scene
        virtual void Draw_ShadowPass_PostBlur(sf::RenderTarget& surface){}
        virtual void Draw(sf::RenderTarget& surface){}
        virtual void Draw_Debug(sf::RenderTarget& surface){}

        virtual void OnDestroy(){}

        ~Component();

        // Providing a reference to the object this is attached to
        void LinkObject(Object* object);
        Object* GetObject();

    protected:

        Object* object;

};