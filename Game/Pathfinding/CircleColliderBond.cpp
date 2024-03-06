#include "CircleColliderBond.h"
#include "PathfindingGraph.h"

void CircleColliderBond::Start(){


    for(int i = 0; i < 2; i++){

        int radius = 3;
        if(i >= 5){
            radius = 3;
        }
        if(i >= 7){
            radius = 2;
        }

        mass_points.push_back(
            {
                AddChild<Object>(),
                radius,
                0.5f,
            });
        mass_points[i].mass_object->GetTransform()->position;

        //mass_points[i].circle_collider = mass_points[i].mass_object->AddComponent<CircleCollider>();
        //mass_points[i].circle_collider->SetRadius(radius);


        mass_points[i].physics_body = mass_points[i].mass_object->AddComponent<PhysicsBody>();
        mass_points[i].physics_body->SetVelocityBlend(0.5);
        mass_points[i].physics_body->SetGravityState(false);
    }
    mass_points[0].physics_body->SetVelocityBlend(0);
}

void CircleColliderBond::SetPosition(sf::Vector2f position){
    GetTransform()->position = position;

    for(int i = 0; i < mass_points.size(); i++){
        mass_points[i].mass_object->GetTransform()->position = position;
    }
}

void CircleColliderBond::Draw(sf::RenderTarget& surface){

    // we need atleast 2 mass points to create a bond
    if(mass_points.size() < 2){
        return;
    }

    skin.clear();
    skin.setPrimitiveType(sf::PrimitiveType::TrianglesStrip);

    sf::Vertex vertex;
    vertex.color = sf::Color::White;//sf::Color(24, 23, 33);

    for(int i = 0; i < mass_points.size(); i++){

        float next_angle;

        // first
        if(i == 0){

            next_angle = Calc::RadiansBetween(mass_points[1].mass_object->GetTransform()->position, mass_points[0].mass_object->GetTransform()->position);

            vertex.position = this->PositionOnMassPointEdge(0, next_angle - Globals::QUARTER_PI);
            skin.append(vertex);
            vertex.position = this->PositionOnMassPointEdge(0, next_angle + Globals::QUARTER_PI);
            skin.append(vertex);

            vertex.position = this->PositionOnMassPointEdge(0, next_angle - Globals::HALF_PI);
            skin.append(vertex);

            vertex.position = this->PositionOnMassPointEdge(0, next_angle + Globals::HALF_PI);
            skin.append(vertex);

        }
        // last
        else if(i == mass_points.size() - 1){

            next_angle = Calc::RadiansBetween(mass_points[i - 1].mass_object->GetTransform()->position, mass_points[i].mass_object->GetTransform()->position);

            vertex.position = this->PositionOnMassPointEdge(i, next_angle + Globals::HALF_PI);
            skin.append(vertex);

            vertex.position = this->PositionOnMassPointEdge(i, next_angle - Globals::HALF_PI);
            skin.append(vertex);

            vertex.position = this->PositionOnMassPointEdge(i, next_angle + Globals::QUARTER_PI);
            skin.append(vertex);

            vertex.position = this->PositionOnMassPointEdge(i, next_angle - Globals::QUARTER_PI);
            skin.append(vertex);
        }
        // in between
        else{

            next_angle = Calc::RadiansBetween(mass_points[i - 1].mass_object->GetTransform()->position, mass_points[i].mass_object->GetTransform()->position);

            vertex.position = this->PositionOnMassPointEdge(i, next_angle + Globals::HALF_PI);
            skin.append(vertex);

            vertex.position = this->PositionOnMassPointEdge(i, next_angle - Globals::HALF_PI);
            skin.append(vertex);

        }
    }

    // convert to screen space
    sf::RenderStates render_states;
    render_states.transform.translate(Scene::GetActiveCamera()->WorldToScreenPosition(sf::Vector2f(0,0)));
    
    surface.draw(skin, render_states);

}

sf::Vector2f CircleColliderBond::PositionOnMassPointEdge(int mass_point, float radians){

    sf::Vector2f position = this->mass_points[mass_point].mass_object->GetTransform()->position;
    position += Calc::RadiansToVector(radians) * (float)this->mass_points[mass_point].radius;
    return position;
}

void CircleColliderBond::Update(){
    std::vector<sf::Vector2i> path = PathfindingGraph::RequestPathWorld(mass_points[0].mass_object->GetTransform()->position, Camera::ScreenToWorldPosition(Mouse::DisplayPosition()));

    // moving leading piece
    if(path.size() <= 1){
        mass_points[0].physics_body->velocity *= 0.7f;
    }   
    else{

        int point_index = 0;
        while(point_index < 3 && point_index < path.size() - 1){

            if(Calc::Distance(mass_points[0].mass_object->GetTransform()->position, sf::Vector2f(path[point_index].x, path[point_index].y)) > 8){
                break;
            }
            point_index++;
        }
        mass_points[0].physics_body->velocity = Calc::VectorBetween(mass_points[0].mass_object->GetTransform()->position, sf::Vector2f(path[point_index].x, path[point_index].y)) * 0.06f;
        //}
        //else{
        //    mass_points[0].physics_body->velocity *= 0.7f;
        //}
    }

    for(int i = 0; i < mass_points.size(); i++){

        sf::Vector2f next_mass_pos = mass_points[i].mass_object->GetTransform()->position;
        sf::Vector2f previous_mass_pos = mass_points[i].mass_object->GetTransform()->position;
        float next_radius = 0;
        float previous_radius = 0;

        if(i > 0){
            previous_mass_pos = mass_points[i - 1].mass_object->GetTransform()->position;
            previous_radius = mass_points[i - 1].radius;
        }
        if(i < mass_points.size() - 1){
            next_mass_pos = mass_points[i + 1].mass_object->GetTransform()->position;
            next_radius = mass_points[i + 1].radius;
        }



        float distance_to_next = Calc::Distance(mass_points[i].mass_object->GetTransform()->position, next_mass_pos);
        distance_to_next -= mass_points[i].radius + next_radius + gap_between_points;
        distance_to_next *= 0.23f;

        float distance_to_previous = Calc::Distance(mass_points[i].mass_object->GetTransform()->position, previous_mass_pos);
        distance_to_previous -= mass_points[i].radius + previous_radius + gap_between_points;

        if(abs(distance_to_next) < 0.02){
            distance_to_next = 0;
        }
        if(abs(distance_to_previous) < 0.02){
            distance_to_previous = 0;
        }
        // follow previous mass point
        mass_points[i].physics_body->secondary_velocity = 
            Calc::VectorBetween(mass_points[i].mass_object->GetTransform()->position, previous_mass_pos) * 0.15f * distance_to_previous;// * 0.7f +
            //Calc::VectorBetween(mass_points[i].mass_object->GetTransform()->position, previous_mass_pos) * 0.5f;

    }
}