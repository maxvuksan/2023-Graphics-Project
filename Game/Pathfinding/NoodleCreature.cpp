#include "NoodleCreature.h"
#include "PathfindingGraph.h"

void NoodleCreature::Start(){

    AddComponent<SpriteRenderer>()->SetTexture("noodle_head");
    
    AddComponent<BoxCollider>();
    physics = AddComponent<PhysicsBody>();

    physics->SetGravityState(false);

    search_again_delay_tracked = search_again_delay;
}

void NoodleCreature::Update(){

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
}