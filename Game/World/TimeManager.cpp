#include "TimeManager.h"
#include "Lighting/LightingManager.h"

sf::Color TimeManager::dawn_colour(186,186,207);
sf::Color TimeManager::midday_colour(255,255,255);
sf::Color TimeManager::dusk_colour(186,186,207);

sf::Color TimeManager::start_night(42,37,63);
sf::Color TimeManager::midnight_colour(42,37,63);
sf::Color TimeManager::end_night(20,23,38);

sf::Color TimeManager::nightime_sunlight_colour(210,220,235);

float TimeManager::cycle_passing_speed = 0.003;
float TimeManager::time_of_day = 0;
int TimeManager::total_time_of_day = 2200; 
bool TimeManager::triggered_screen_wipe = false;
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
    LightingManager::sunlight_colour = nightime_sunlight_colour;
    // the lerp t division is based of the the start and end values of each threshold
    /*
        e.g. 200 -> 400

        would be 200 because 400 (max) - 200 (min) == 200
    */

    // is midnight
    /*
    if(time_of_day < 200){
        LightingManager::sunlight_colour = end_night;
        Scene::GetActiveCamera()->SetConstantScreenShake(0);
        Scene::GetActiveCamera()->SetScreenWipeColour(sf::Color::Transparent);
        triggered_screen_wipe = false;
    }
    // to end night
    else if(time_of_day >= 200 && time_of_day < 400){
        LightingManager::sunlight_colour = Calc::Lerp(end_night, end_night, (time_of_day - 200) / 200.0f); 
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

        // begin screen shake


        int shake_factor = round((time_of_day - 2000) / (2200 - 2000) * 2.0f);
        
        if(time_of_day > 2100){
            shake_factor + 1;
        }

        if(time_of_day > 2190){
            shake_factor + 1;
            Scene::GetActiveCamera()->SetScreenWipeColourOvertime(sf::Color(16, 16, 26), 5000);
        }
        else if(time_of_day > 2150){
            Scene::GetActiveCamera()->SetScreenWipeColourOvertime(sf::Color::Transparent, 8000);
        }
        else if(time_of_day > 2125){
            Scene::GetActiveCamera()->SetScreenWipeColourOvertime(sf::Color(16, 16, 26, 50), 8000);
        }

        Scene::GetActiveCamera()->SetConstantScreenShake(shake_factor);
    
        LightingManager::sunlight_colour = Calc::Lerp(start_night, midnight_colour, (time_of_day - 2000) / 200.0f); 

    }
    */
}