#pragma once
#include <math.h>
#include <SFML/Graphics.hpp>

class Calc{

    public:
        
        static float EaseOutBack(float x) {

            float c1 = 1.70158;
            float c3 = c1 + 1;

            return 1 + c3 * pow(x - 1, 3) + c1 * pow(x - 1, 2);
        }

        static bool InRange(float val, float min, float max){
            if(val > max){
                return false;
            }
            else if(val < min){
                return false;
            }
            return true;        
        }

        static float Clamp(float val, float min, float max){
            if(val > max){
                val = max;
            }
            else if(val < min){
                val = min;
            }
            return val;
        }

        // @returns the distance between two points without traversing diagonally 
        static int ManhattenDistance(sf::Vector2i start, sf::Vector2i end){
            return abs(start.x - end.x) + abs(start.y - end.y);
        }
        static float Distance(sf::Vector2i start, sf::Vector2i end){
            return sqrt(pow(abs(start.x - end.x), 2) + pow(abs(start.y - end.y), 2));
        }
        static float Distance(sf::Vector2f start, sf::Vector2f end){
            return sqrt(pow(abs(start.x - end.x), 2) + pow(abs(start.y - end.y), 2));
        }
        // @returns the distance squared between 2 points. Though inaccurate to actual distance, is more efficent than Distance() by removing the final sqrt() step.
        static float DistanceSimple(sf::Vector2i start, sf::Vector2i end){
            return pow(abs(start.x - end.x), 2) + pow(abs(start.y - end.y), 2);
        }

        static float Radians(float degrees){
            // using the approximation 1Deg × π/180 = 0.01745Rad
            return degrees * 0.01745;
        }

        static float Degrees(float radians){
            // using the approximation 1Rad × 180/π = 57.296Deg
            return radians * 57.296;
        }

        // @returns the interporlated value between a and b at position t
        static float Lerp(float a, float b, float t){
            return a * (1.0 - t) + b * t;
        }

        // @returns the interporlated vector between a and b and position t
        static sf::Vector2f Lerp(const sf::Vector2f& a, const sf::Vector2f& b, float t){
            return sf::Vector2f(Lerp(a.x, b.x, t), Lerp(a.y, b.y, t));
        }

        // @returns a vector of magnitude 1 from the provided vector
        static sf::Vector2f Normalize(sf::Vector2f vector){

            float magnitude = sqrt(pow(vector.x, 2) + pow(vector.y, 2));
            return sf::Vector2f(vector.x / magnitude, vector.y / magnitude);
        
        }
        // @returns a vector of magnitude 1 from the provided vector
        static sf::Vector2f Normalize(sf::Vector2i vector){
            return Normalize(sf::Vector2f(vector.x, vector.y));
        }

        // @returns the angle between two points in degreess
        static float AngleBetween(sf::Vector2f a, sf::Vector2f b){
            sf::Vector2f dif = b - a;
            return Degrees(atan2(dif.y, dif.x));
        }

        static float VectorToRadians(sf::Vector2f vector){
            return atan2(vector.y, vector.x);
        }

        static sf::Vector2f RadiansToVector(float radians){
            return sf::Vector2f(cos(radians), sin(radians));
        }
};