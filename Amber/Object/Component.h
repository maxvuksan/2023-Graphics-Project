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

        virtual void OnSetActive(){}
        virtual void OnDisable(){}

        // is called when the window size changes
        virtual void OnResize(){}

        virtual void Draw(sf::RenderTarget& surface){}
        // is drawn as the topmost render layer no matter the components set render layer, should be used for debug graphics
        virtual void DrawDebug(sf::RenderTarget& surface){}

        virtual ~Component(){}

        // Providing a reference to the object this is attached to
        void LinkObject(Object* object);
        Object* GetThisObject();

        bool IsActive(){return active;}
        void SetActive(bool state);

        int GetRenderLayer(){return render_layer;}
        void SetRenderLayer(int render_layer);
    protected:

        int render_layer;
        bool active;
        Object* object;

};