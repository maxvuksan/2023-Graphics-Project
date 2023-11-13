#pragma once
#include <math.h>
#include <SFML/Graphics.hpp>

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

        static float Lerp(float a, float b, float t){
            return a * (1.0 - t) + b * t;
        }

        static sf::Vector2f Lerp(const sf::Vector2f& a, const sf::Vector2f& b, float t){
            return sf::Vector2f(Lerp(a.x, b.x, t), Lerp(a.y, b.y, t));
        }

};