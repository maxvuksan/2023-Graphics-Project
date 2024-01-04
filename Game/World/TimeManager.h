#pragma once
#include <SFML/Graphics.hpp>

// manages the time of day 
class TimeManager {

    public:

        static int GetTotalTimeInDay(){return total_time_of_day;}
        static void SetTimeOfDay(int new_time);
        static void Tick();
        

    private:

        // how fast days go by
        static float cycle_passing_speed;
        // current time of cycle
        static float time_of_day;
        // total time in cycle
        static int total_time_of_day; 

        // lerps between these colours to determine the current sunlight colour

        static sf::Color dawn_colour;
        static sf::Color midday_colour;
        static sf::Color dusk_colour;

        static sf::Color start_night;
        static sf::Color midnight_colour;
        static sf::Color end_night;
};