#include "TimeManager.h"
#include "LightingManager.h"

sf::Color TimeManager::dawn_colour(255,207,186);
sf::Color TimeManager::midday_colour(255,255,255);
sf::Color TimeManager::dusk_colour(255,207,186);

sf::Color TimeManager::start_night(42,37,63);
sf::Color TimeManager::midnight_colour(0,0,0);
sf::Color TimeManager::end_night(20,23,38);

float TimeManager::cycle_passing_speed = 0.0015;
float TimeManager::time_of_day = 1200;
int TimeManager::total_time_of_day = 2400; 

/*

    - 0     
    - 100
    - 200   : midnight
    - 300   
    - 400   : end_night
    - 500   
    - 600   
    - 700   : dawn
    - 800   
    - 900  
    - 1000  : miday
    - 1100
    - 1200  
    - 1300
    - 1400  : miday
    - 1500
    - 1600  
    - 1700  : dusk
    - 1800  
    - 1900
    - 2000  : start_night
    - 2100
    - 2200  : midnight
    - 2300 
    - 2400 

*/


void TimeManager::SetTimeOfDay(int new_time){
    time_of_day = new_time;

    if(time_of_day < 0){
        time_of_day = total_time_of_day;
    }
}

void TimeManager::Tick(){

    time_of_day += Time::Dt() * cycle_passing_speed;

    if(time_of_day >= total_time_of_day){
        time_of_day = 0;
    }

    // the lerp t division is based of the the start and end values of each threshold
    /*
        e.g. 200 -> 400

        would be 200 because 400 (max) - 200 (min) == 200
    */

    // is midnight
    if(time_of_day >= 2200 || time_of_day < 200){
        LightingManager::sunlight_colour = midnight_colour;
    }
    // to end night
    else if(time_of_day >= 200 && time_of_day < 400){
        LightingManager::sunlight_colour = Calc::Lerp(midnight_colour, end_night, (time_of_day - 200) / 200.0f); 
    }
    // to dawn
    else if(time_of_day >= 400 && time_of_day < 700){
        LightingManager::sunlight_colour = Calc::Lerp(end_night, dawn_colour, (time_of_day - 400) / 300.0f); 
    }
    // to miday
    else if(time_of_day >= 700 && time_of_day < 1000){
        LightingManager::sunlight_colour = Calc::Lerp(dawn_colour, midday_colour, (time_of_day - 700) / 300.0f); 
    }
    // is miday
    else if(time_of_day >= 1000 && time_of_day < 1400){
        LightingManager::sunlight_colour = midday_colour;
    }
    // to dusk
    else if(time_of_day >= 1400 && time_of_day < 1700){
        LightingManager::sunlight_colour = Calc::Lerp(midday_colour, dusk_colour, (time_of_day - 1400) / 300.0f); 
    }
    // to start night
    else if(time_of_day >= 1700 && time_of_day < 2000){
        LightingManager::sunlight_colour = Calc::Lerp(dusk_colour, start_night, (time_of_day - 1700) / 300.0f); 
    }
    // to midnight
    else if(time_of_day >= 2000 && time_of_day < 2200){
        LightingManager::sunlight_colour = Calc::Lerp(start_night, midnight_colour, (time_of_day - 2000) / 200.0f); 
    }
}