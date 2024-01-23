#pragma once
#include "../Object.h"

enum class ScreenLocationY{

    TOP,
    CENTER,
    BOTTOM,
    
    FREE,
};
enum class ScreenLocationX{

    LEFT,
    CENTER,
    RIGHT,
    
    FREE,
};

// positioning component for ui elements, allows a screen location to be specified through SetAlign()
class UIRect : public Component { 

    public:
        UIRect(): width(0), height(0), screen_location_x(ScreenLocationX::FREE), screen_location_y(ScreenLocationY::FREE){}

        // snap the rect into different screen locations
        void SetAlign(ScreenLocationX align_x, ScreenLocationY align_y);
        void Align();
        virtual void OnAlign(){}

        // allows one time alignments for objects which dont have the attached component
        static void Align(Transform* transform, int width, int height, ScreenLocationX align_x, ScreenLocationY align_y); 
        static void AlignX(Transform* transform, int width, ScreenLocationX align_x); 
        static void AlignY(Transform* transform, int height, ScreenLocationY align_y); 
                        
        int width;
        int height;

        static int padding;

    protected:
        ScreenLocationX screen_location_x;
        ScreenLocationY screen_location_y;


};