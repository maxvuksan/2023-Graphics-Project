#pragma once
#include <math.h>

class Calc{

    public:

        static float Clamp(float val, float min, float max){
            if(val > max){
                val = max;
            }
            else if(val < min){
                val = min;
            }
            return val;
        }

        static float Radians(float degrees){
            // 1Deg × π/180 = 0.01745Rad
            return degrees * 0.01745;
        }

        static float Degrees(float radians){
            // 1Rad × 180/π = 57.296Deg
            return radians * 57.296;
        }

};