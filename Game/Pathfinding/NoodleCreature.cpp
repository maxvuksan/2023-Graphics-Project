#include "NoodleCreature.h"
#include "PathfindingGraph.h"
#include "../LightSource.h"

void NoodleCreature::Start(){

    auto light_source = AddComponent<LightSource>();
    light_source->colour = sf::Color(179, 103, 245);
    light_source->decay = 0.05f;

    AddComponent<SpriteRenderer>()->SetTexture("noodle_head");
    
    //AddComponent<BoxCollider>();
   // physics = AddComponent<PhysicsBody>();
   // physics->SetGravityState(false);

    search_again_delay_tracked = search_again_delay;

    GetTransform()->position = sf::Vector2f(100,100);
}

void NoodleCreature::Update(){

    if(Calc::Distance(GetTransform()->position, Camera::ScreenToWorldPosition(Mouse::DisplayPosition())) > 3.0f){
        GetTransform()->position += Calc::RadiansToVector(Calc::Radians(Calc::AngleBetween(GetTransform()->position, Camera::ScreenToWorldPosition(Mouse::DisplayPosition())))) * Time::Dt() * 0.04f;
    }
    /*
    return;

    if(current_node < path_to_target.size()){

        physics->velocity = Calc::Normalize(sf::Vector2f(path_to_target.at(current_node).x * 8, path_to_target.at(current_node).y * 8) - GetTransform()->position) * 0.07f;

        if(Calc::Distance(sf::Vector2f(path_to_target[current_node].x * 8, path_to_target[current_node].y * 8), GetTransform()->position) < 3){
            current_node++;
        }
    }
    
    if(search_again_delay_tracked >= search_again_delay){

        path_to_target = PathfindingGraph::RequestPathWorld(GetTransform()->position, Camera::ScreenToWorldPosition(Mouse::DisplayPosition()), 2000);
        search_again_delay_tracked = 0;
        current_node = 0;
    }
    
    search_again_delay_tracked += Time::Dt();
    */
}