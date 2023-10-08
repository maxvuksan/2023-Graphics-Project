#pragma once
#include <math.h>

class Calc{

    public:

        static float clamp(float val, float min, float max){
            if(val > max){
                val = max;
            }
            else if(val < min){
                val = min;
            }
            return val;
        }

};