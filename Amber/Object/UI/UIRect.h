#pragma once
#include "../Object.h"

enum ScreenLocation{

    TOP_LEFT,
    TOP_RIGHT,
    TOP_CENTER,

    LEFT,
    RIGHT,
    CENTER,

    BOTTOM_LEFT,
    BOTTOM_RIGHT,
    BOTTOM_CENTER,
    
    FREE,
};

/*
    base class for all UI elements
*/
class UIRect : public Component { 


    public:
        UIRect(): width(0), height(0), screen_location(FREE){}

        // snap the rect into different screen locations
        void SetAlign(ScreenLocation alignment);
        void Align();
        
        int width;
        int height;

        static int padding;

    protected:
        ScreenLocation screen_location;


};