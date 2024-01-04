#include "HealthBar.h"


HealthBar::HealthBar(){
    health_frame.setPosition(sf::Vector2f(UIRect::padding, UIRect::padding));
    health_remaining.setPosition(sf::Vector2f(UIRect::padding, UIRect::padding));

    hunger_frame.setPosition(sf::Vector2f(UIRect::padding, UIRect::padding + 8 + UIRect::padding / 2.0f));
    hunger_remaining.setPosition(sf::Vector2f(UIRect::padding, UIRect::padding + 8 + UIRect::padding / 2.0f));

    health_frame.setSize(sf::Vector2f(100, 8));
    health_remaining.setSize(sf::Vector2f(100, 8));
    hunger_frame.setSize(sf::Vector2f(100, 4));
    hunger_remaining.setSize(sf::Vector2f(100, 4));
   
    health_frame.setFillColor(sf::Color(40,37,48));
    health_remaining.setFillColor(sf::Color(252, 81, 85));
    hunger_frame.setFillColor(sf::Color(40,37,48));
    hunger_remaining.setFillColor(sf::Color(242, 181, 68));
     
    SetHealth(max_health);
}


void HealthBar::Draw(sf::RenderTarget& surface){

    surface.draw(health_frame);
    surface.draw(health_remaining);
    surface.draw(hunger_frame);
    surface.draw(hunger_remaining);
}

void HealthBar::SetHealth(int _health){
    
    if(_health > max_health){
        _health = max_health;
    }

    this->health = _health;
    health_remaining.setScale(sf::Vector2f((health / max_health), 1));
}

void HealthBar::SetMaxHealth(int _max_health){
    this->max_health = _max_health;
    health_remaining.setScale(sf::Vector2f((health / max_health), 1));
}

void HealthBar::SetHunger(int _hunger){
    
    if(_hunger > max_health){
        _hunger = max_health;
    }

    this->hunger = _hunger;
    hunger_remaining.setScale(sf::Vector2f((hunger / max_hunger), 1));
}

void HealthBar::SetMaxHunger(int _max_hunger){
    this->max_hunger = _max_hunger;
    hunger_remaining.setScale(sf::Vector2f((hunger / max_hunger), 1));
}

