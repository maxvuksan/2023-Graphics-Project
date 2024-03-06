#include "CircleCollider.h"
#include "../Camera.h"
#include "../Object.h"
#include "../../Core/Scene.h"
#include "BoxCollider.h"
#include "TilemapCollider.h"
#include "../../Utility/Calc.h"
#include "PhysicsBody.h"

void CircleCollider::Start(){
    radius = 8;
}


std::vector<BoxCollider::Rect*> CircleCollider::DetermineBoxOverlaps(){
    
    std::vector<BoxCollider*>* box_colliders = object->GetScene()->GetBoxColliders();
    std::vector<BoxCollider::Rect*> overlaps;

    // bounding box of circle
    float left = GetThisObject()->GetTransform()->position.x - radius;
    float right = GetThisObject()->GetTransform()->position.x + radius;
    float top = GetThisObject()->GetTransform()->position.y - radius;
    float bottom = GetThisObject()->GetTransform()->position.y + radius;
              

    for(auto& collider : *box_colliders){

        // ignore itself and triggers
        if(collider->GetThisObject() == object){
            continue; 
        }
        if(collider->IsTrigger()){
            continue;
        }

        // if we are dealing with a tilemap (lots of collision rects) perform a bounds check first...
        TilemapCollider* tilemap_collider = dynamic_cast<TilemapCollider*>(collider);
        bool in_bounds = true;

        if(tilemap_collider != nullptr){

            in_bounds = false;

            if(
            (tilemap_collider->WithinBoundsX(left) 
            || tilemap_collider->WithinBoundsX(right)) && 
            (tilemap_collider->WithinBoundsY(top)
            || tilemap_collider->WithinBoundsY(bottom))
            ){
                in_bounds = true;
            }
        }

        // out of bounds
        if(!in_bounds){
            continue;
        }


        for(auto& rect : collider->GetRects()){
            
            // compare overlaps between two BoxColliders

            // only allow platforms to work if the collider is above 
            if(rect.collision_mode == ColliderType::PLATFORM && (previous_frame_bottom > rect.Top() || collision_interaction_mode == CollisionInteractionMode::IGNORE_PLATFORM)){
                continue;
            }
            if(top < rect.Bottom() && bottom > rect.Top()){
                if(right > rect.Left() && left < rect.Right()){

                    //overlapping
                    overlaps.push_back(&rect);
                }            
            }
        }
    }

    return overlaps;

    previous_frame_bottom = bottom;
}


void CircleCollider::Move(sf::Vector2f movement, PhysicsBody* pb){
    Move(movement, pb, 0);
}

void CircleCollider::Move(sf::Vector2f movement, PhysicsBody* pb, int move_depth){

    // prevent infinite loops
    if(move_depth > 4){
        return;
    }

    Transform* transform = GetThisObject()->GetTransform();
    sf::Vector2f inital_position = transform->position;

    transform->position += movement;

    std::vector<BoxCollider::Rect*> box_overlaps = DetermineBoxOverlaps();

    // iterate over each box overlap -------------------------------------------------------------------

    for(int i = 0; i < box_overlaps.size(); i++){

        sf::Vector2f clamped_pos;
        clamped_pos.x = Calc::Clamp(transform->position.x, box_overlaps[i]->Left(), box_overlaps[i]->Right());
        clamped_pos.y = Calc::Clamp(transform->position.y, box_overlaps[i]->Top(), box_overlaps[i]->Bottom());

    
        // circle center is in the rectangle
        if(transform->position.y < box_overlaps[i]->Bottom() && transform->position.y > box_overlaps[i]->Top()){
            if(transform->position.x > box_overlaps[i]->Left() && transform->position.x < box_overlaps[i]->Right()){
                
                ResolveWhenCircleCenterInRect(inital_position, *box_overlaps[i], pb, move_depth);

            }            
        }
        // circle edge crosses over rectangle but center is outside
        else{
            sf::Vector2f overlap_vec = clamped_pos - transform->position;
            float overlap = radius - Calc::Magnitude(overlap_vec);

            // in the case the circle is right on the edge, calculating the magnitude would divide by zero
            if(std::isnan(overlap)){
                overlap = 0; 
            }

            // we are colliding
            if(overlap > 0){
                
                pb->velocity *= 0.98f;
                pb->secondary_velocity *= 0.98f;
                Move(-Calc::Normalize(overlap_vec) * overlap, pb, move_depth + 1);
            }
        }

    }





    // resolve circle collisions --------------------------------------------------------------------------

    std::vector<CircleCollider*>* all_circle_colliders = GetThisObject()->GetScene()->GetCircleColliders();
    float distance;
    sf::Vector2f other_position;

    for(int i = 0; i < all_circle_colliders->size(); i++){

        // the collider is not itself
        if(all_circle_colliders->at(i) == this){
            continue;
        }

        other_position = all_circle_colliders->at(i)->GetThisObject()->GetTransform()->position;
        distance = Calc::Distance(transform->position, other_position);

        // the circles are too far apart to be colliding 
        if(distance > this->radius + all_circle_colliders->at(i)->GetRadius()){
            continue;
        }
        
        // how far in the circle the is 
        float overlap = this->radius + all_circle_colliders->at(i)->GetRadius() - distance;
        float half_overlap = overlap / 2.0f;
        sf::Vector2f vector_between = Calc::VectorBetween(transform->position, other_position);

        // termnimate because NaN error
        if(std::isnan(vector_between.x) || std::isnan(vector_between.y)){
            continue;
        }

        pb->velocity *= 0.98f;
        pb->secondary_velocity *= 0.98f;
        Move(vector_between * -half_overlap, pb, move_depth + 1);
        all_circle_colliders->at(i)->Move(vector_between * half_overlap, pb, move_depth + 1);
    }   





}

void CircleCollider::ResolveWhenCircleCenterInRect(sf::Vector2f inital_position, const BoxCollider::Rect& rect, PhysicsBody* pb, int move_depth){

    // each edge on the rectangle is represented with start_edge (point the edge starts), and end_edge, where the edge ends

    Transform* transform = GetThisObject()->GetTransform();

    sf::Vector2f start_edge[4] = {
        sf::Vector2f( rect.Left(), rect.Top()),
        sf::Vector2f( rect.Right(), rect.Top()),
        sf::Vector2f( rect.Right(), rect.Bottom()),
        sf::Vector2f( rect.Left(), rect.Top()),
    };

    sf::Vector2f end_edge[4] = {
        sf::Vector2f( rect.Right(), rect.Top()),
        sf::Vector2f( rect.Right(), rect.Bottom()),
        sf::Vector2f( rect.Left(), rect.Bottom()),
        sf::Vector2f( rect.Left(), rect.Bottom()),
    };

    float delta_x = transform->position.x - inital_position.x;
    float delta_y = transform->position.y - inital_position.y;

    struct Intersection{
        sf::Vector2f intersection = sf::Vector2f(0,0);
        int intersection_edge_index = -1;
    };

    std::vector<Intersection> intersections;

    // all 4 edges
    for(int e = 0; e < 4; e++){

        // avoid divison by zero, trajectory line is vertical
        if(delta_x == 0){

            // check if the edge is vertical
            if(start_edge[e].x == end_edge[e].x && start_edge[e].x == inital_position.x){

                float min_y = std::min(start_edge[e].y, end_edge[e].y);
                float max_y = std::max(start_edge[e].y, end_edge[e].y);

                if (inital_position.y <= max_y && transform->position.y >= min_y) {
                    // Intersection occurs at the start or end point of the trajectory line
                    if (inital_position.y <= min_y){
                        intersections.push_back({inital_position, e});
                    }
                    if (transform->position.y >= max_y){
                        intersections.push_back({transform->position, e});
                    }
                }
            }
            
            // no intersection
            continue;
        }

        // edge is vertical
        if(start_edge[e].x == end_edge[e].x){

            float edge_x = start_edge[e].x;

            // Check if the trajectory line intersects the vertical edge
            if ((start_edge[e].x <= edge_x && edge_x <= end_edge[e].x) ||
                (transform->position.x <= edge_x && edge_x <= inital_position.x)) {
                // Calculate the y-coordinate of the intersection point
                float intersection_y = (edge_x - inital_position.x) * delta_y / delta_x + inital_position.y;

                // Check if the intersection point is within the y-range of the edge
                float min_y = std::min(start_edge[e].y, end_edge[e].y);
                float max_y = std::max(start_edge[e].y, end_edge[e].y);

                if (min_y <= intersection_y && intersection_y <= max_y) {
                    intersections.push_back({sf::Vector2f(edge_x, intersection_y), e});
                }
            }
            
            continue;
        }

        // Calculate slope and y-intercept of the trajectory line
        float trajectory_slope = delta_y / delta_x;
        float trajectory_intercept = inital_position.y - trajectory_slope * inital_position.x;

        // Check if the edge is horizontal
        if (start_edge[e].y == end_edge[e].y) {
            float edge_y = start_edge[e].y;

            // Check if the trajectory line intersects the horizontal edge
            if ((inital_position.y <= edge_y && edge_y <= transform->position.y) ||
                (transform->position.y <= edge_y && edge_y <= inital_position.y)) {
                // Calculate the x-coordinate of the intersection point
                float intersection_x = (edge_y - trajectory_intercept) / trajectory_slope;

                // Check if the intersection point is within the x-range of the edge
                float min_x = std::min(start_edge[e].x, end_edge[e].x);
                float max_x = std::max(start_edge[e].x, end_edge[e].x);
                if (min_x <= intersection_x && intersection_x <= max_x) {

                    intersections.push_back({sf::Vector2f(intersection_x, edge_y), e});
                }
            }
        }


    }

    // no intersections?
    if(intersections.size() == 0){
        return;
    }


    int smallest_intersection;
    float smallest_dis = INT_MAX;

    for(int i = 0; i < intersections.size(); i++){

        float distance = Calc::Distance(intersections[i].intersection, inital_position);

        if(distance < smallest_dis){
            smallest_dis = distance;
            smallest_intersection = i;
        }
    }

    sf::Vector2f intersection_point = intersections[smallest_intersection].intersection;

    switch(smallest_intersection){

        // moving to top edge
        case 0:
            transform->position = intersection_point + sf::Vector2f(0, -radius);
            break;
        // right edge
        case 1:
            transform->position = intersection_point + sf::Vector2f(radius, 0);
            break;
        // bottom edge
        case 2:
            transform->position = intersection_point + sf::Vector2f(0, radius);
            break;
        // left edge
        case 3:
            transform->position = intersection_point + sf::Vector2f(-radius, 0);
            break;      
    }

}

void CircleCollider::DrawDebug(sf::RenderTarget& surface){

    sf::CircleShape circle;
    circle.setRadius(radius);
    circle.setOrigin(sf::Vector2f(radius, radius));
    circle.setFillColor(sf::Color::Transparent);
    circle.setOutlineColor(Globals::DEBUG_COLOUR);
    circle.setOutlineThickness(1);

    circle.setPosition(Camera::WorldToScreenPosition(GetThisObject()->GetTransform()->position));

    surface.draw(circle);

}