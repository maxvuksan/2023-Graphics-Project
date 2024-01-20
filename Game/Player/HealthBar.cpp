#include "HealthBar.h"

int HealthBar::hunger_decay_delay = 400;

HealthBar::HealthBar(){
    health_frame.setPosition(sf::Vector2f(UIRect::padding, UIRect::padding));
    health_remaining.setPosition(sf::Vector2f(UIRect::padding, UIRect::padding));

    hunger_frame.setPosition(sf::Vector2f(UIRect::padding, UIRect::padding + 8 + UIRect::padding / 2.0f));
    hunger_remaining.setPosition(sf::Vector2f(UIRect::padding, UIRect::padding + 8 + UIRect::padding / 2.0f));

    cycle_frame.setPosition(sf::Vector2f(UIRect::padding, hunger_frame.getPosition().y + UIRect::padding * 2));
    cycle_remaining.setPosition(sf::Vector2f(UIRect::padding, hunger_frame.getPosition().y + UIRect::padding * 2));


    health_frame.setSize(sf::Vector2f(100, 8));
    health_remaining.setSize(sf::Vector2f(100, 8));
    hunger_frame.setSize(sf::Vector2f(100, 4));
    hunger_remaining.setSize(sf::Vector2f(100, 4));

    cycle_frame.setSize(sf::Vector2f(100, 4));
    cycle_remaining.setSize(sf::Vector2f(100, 4));
   
    health_frame.setFillColor(sf::Color(40,37,48));
    health_remaining.setFillColor(sf::Color(252, 81, 85));
    hunger_frame.setFillColor(sf::Color(40,37,48));
    hunger_remaining.setFillColor(sf::Color(242, 181, 68));

    cycle_frame.setFillColor(sf::Color(50, 55, 66));
    cycle_remaining.setFillColor(sf::Color(221, 226, 237));


    SetHealth(max_health);
    SetHunger(max_hunger);

    hunger_decay_delay_tracked = 0;
}


void HealthBar::Draw(sf::RenderTarget& surface){

    hunger_decay_delay_tracked += Time::Dt();
    if(hunger_decay_delay_tracked > hunger_decay_delay){
        SetHunger(GetHunger() - 1);
        hunger_decay_delay_tracked = 0;
    }

    surface.draw(health_frame);
    surface.draw(health_remaining);
    surface.draw(hunger_frame);
    surface.draw(hunger_remaining);
    surface.draw(cycle_frame);
    surface.draw(cycle_remaining);
}

void HealthBar::SetHealth(int _health){
    
    if(_health > max_health){
        _health = max_health;
    }
    else if(_health < 0){
        _health = 0;
    }

    this->health = _health;
    health_remaining.setScale(sf::Vector2f((health / (float)max_health), 1));
}

void HealthBar::SetMaxHealth(int _max_health){
    this->max_health = _max_health;
    health_remaining.setScale(sf::Vector2f((health / (float)max_health), 1));
}

void HealthBar::SetHunger(int _hunger){
    
    if(_hunger > max_health){
        _hunger = max_health;
    }
    else if(_hunger < 0){
        _hunger = 0;
    }

    this->hunger = _hunger;
    hunger_remaining.setScale(sf::Vector2f((hunger / (float)max_hunger), 1));
}

void HealthBar::SetMaxHunger(int _max_hunger){
    this->max_hunger = _max_hunger;
    hunger_remaining.setScale(sf::Vector2f((hunger / (float)max_hunger), 1));
}


void HealthBar::SetCycle(int _cycle){
    
    if(_cycle > max_cycle){
        _cycle = max_cycle;
    }

    this->cycle = _cycle;
    cycle_remaining.setScale(sf::Vector2f(1.0f - (cycle / (float)max_cycle), 1));
}

void HealthBar::SetMaxCycle(int _max_cycle){
    this->max_cycle = _max_cycle;
    cycle_remaining.setScale(sf::Vector2f(1.0f - (cycle / (float)max_cycle), 1));
}
