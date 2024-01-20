#include "NoodleCreature.h"
#include "PathfindingGraph.h"
#include "../World/Lighting/LightSource.h"
#include "InverseKinematicLimb.h"
#include "../World/World.h"

void NoodleCreature::LinkWorld(World* world){
    this->world = world;
}

void NoodleCreature::Start(){

    auto light_source = AddComponent<LightSource>();
    light_source->colour = sf::Color(200, 143, 255);
    light_source->decay = 0.04f;

    auto sr = AddComponent<SpriteRenderer>();
    sr->SetRenderLayer(4);
    sr->SetTexture("noodle_head", true);

    search_again_delay_tracked = search_again_delay;

    GetTransform()->position = sf::Vector2f(100,100);

    for(int i = 0; i < 4; i++){
        auto limb = AddComponent<InverseKinematicLimb>(3);
        limb->Init(1, 22, 5);
        limb->AddSubLimb(6, 12, 0, 6, 3);
        limb->AddSubLimb(10, 20, 0, 10, 5);
        limb->AddSubLimb(0, 8, 0, 14, 3);
        
        limbs.push_back(limb);
    }
    ideal_grab_angle.push_back(-35);
    ideal_grab_angle.push_back(35);
    ideal_grab_angle.push_back(-65);
    ideal_grab_angle.push_back(65);
}

sf::Vector2i NoodleCreature::RandomCoordinateNearTarget(sf::Vector2f& target_position){
    
    sf::Vector2i target = world->WorldToCoord(target_position.x, target_position.y);

    target.x += rand() % 10 - 5;
    target.y += rand() % 10 - 5;

    return target;
}


bool NoodleCreature::GrabCoordinateIsValid(sf::Vector2i& grab_coordinate){

    if(world->GetTile(grab_coordinate.x, grab_coordinate.y, SetLocation::MAIN) != -1){

        sf::Vector2i offsets[4] = {sf::Vector2i(-1, 0), sf::Vector2i(1, 0), sf::Vector2i(0, 1), sf::Vector2i(0, -1)};

        for(int i = 0; i < 4; i ++){
            if(world->GetTile(grab_coordinate.x + offsets[i].x, grab_coordinate.y + offsets[i].y, SetLocation::MAIN) == -1){
                return true;
            }
        }
    }
    return false;
}


void NoodleCreature::Draw(sf::RenderTarget& surface){

    if(Calc::Distance(GetTransform()->position, Camera::ScreenToWorldPosition(Mouse::DisplayPosition())) < 3.0f){
        return;
    }

    float direction_of_travel = Calc::Radians(Calc::AngleBetween(GetTransform()->position, Camera::ScreenToWorldPosition(Mouse::DisplayPosition())));


    float limbs_attached = 1;

    sf::VertexArray target_points;
    target_points.setPrimitiveType(sf::PrimitiveType::Points);

    for(int i = 0; i < 4; i++){

        sf::Vector2f target_position = Calc::RadiansToVector(Calc::Radians(ideal_grab_angle[i]) + direction_of_travel);
        target_position.x *= 50;
        target_position.y *= 50;
        target_position += GetTransform()->position;

        if(Core::DEBUG_MODE){
            sf::Vertex vertex;
            vertex.color = Globals::DEBUG_COLOUR_3;
            vertex.position = Camera::WorldToScreenPosition(target_position);
            target_points.append(vertex);
        }

        InverseKinematicLimb* limb = limbs[i];
        limb->SetOrigin(GetTransform()->position);


        if(Calc::Distance(limb->GetTarget(), GetTransform()->position) < 80){
            continue;
        }

        sf::Vector2i choosen_coord = RandomCoordinateNearTarget(target_position);

        if(!GrabCoordinateIsValid(choosen_coord)){
            continue;
        }

        sf::Vector2f choosen_world_position = world->CoordToWorld(choosen_coord.x, choosen_coord.y);

        if(Calc::Distance(choosen_world_position, target_position) <= Calc::Distance(limb->GetTarget(), target_position)){

            limb->SetTargetOverTime(choosen_world_position, 150);
        }

        if(limb->GetIsAttached()){
            limbs_attached++;
        }
    }

    GetTransform()->position += Calc::RadiansToVector(direction_of_travel) * Time::Dt() * 0.03f * limbs_attached;

    if(Core::DEBUG_MODE){
        surface.draw(target_points);
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