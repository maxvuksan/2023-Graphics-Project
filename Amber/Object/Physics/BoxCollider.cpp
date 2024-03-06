#include "BoxCollider.h"
#include "TilemapCollider.h"
#include "../../Core/Scene.h"
#include "PhysicsBody.h"
#include "CircleCollider.h"
#include "../../Utility/Calc.h"



void BoxCollider::Start(){
    AddRect({0,0,16,16});
}

void BoxCollider::Update(){

    if(!IsTrigger()){
        return;
    }

    // if we have more than 1 overlap, the trigger is 'triggered'...
    auto overlaps = DetermineBoxOverlaps(rects[0].Bottom());

    if(overlaps.size() > 0){
        SetTriggered(true);
    }
    else{
        SetTriggered(false);
    }
}

std::vector<BoxCollider::Rect*> BoxCollider::DetermineBoxOverlaps(float previous_frame_bottom){
    std::vector<BoxCollider*>* box_colliders = object->GetScene()->GetBoxColliders();

    std::vector<Rect*> overlaps;

    for(auto& collider : *box_colliders){

        // ignore itself and triggers
        if(collider == this || collider->GetThisObject() == object){
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
            (tilemap_collider->WithinBoundsX(rects[0].Left()) 
            || tilemap_collider->WithinBoundsX(rects[0].Right())) && 
            (tilemap_collider->WithinBoundsY(rects[0].Top())
            || tilemap_collider->WithinBoundsY(rects[0].Bottom()))
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
            if(rects[0].Top() < rect.Bottom() && rects[0].Bottom() > rect.Top()){
                if(rects[0].Right() > rect.Left() && rects[0].Left() < rect.Right()){

                    //overlapping
                    overlaps.push_back(&rect);
                }            
            }
        }
    }

    return overlaps;
}

bool BoxCollider::Overlaps(float other_left, float other_right, float other_top, float other_bottom){
    
    if(rects[0].Top() < other_bottom && rects[0].Bottom() > other_top){
        if(rects[0].Right() > other_left && rects[0].Left() < other_right){

            return true;
        }            
    }
    return false;
}

void BoxCollider::Move(sf::Vector2f movement, PhysicsBody* pb){
    Move(movement, pb, 0);
}

void BoxCollider::Move(sf::Vector2f movement, PhysicsBody* pb, int move_depth){

    if(move_depth > 3){
        return;
    }

    Transform* transform = object->GetTransform();

    // resolving collisions with rectangles ---------------------------------------

    float previous_frame_bottom = rects[0].Bottom();

    transform->position.y += movement.y;

    std::vector<Rect*> overlapping; 

    if(movement.y != 0){

        overlapping = DetermineBoxOverlaps(previous_frame_bottom);

        // vertical (Y) collisions
        for(auto& other_rect : overlapping){
            
            // collide bottom
            if(movement.y < 0){
                
                transform->position.y = other_rect->Bottom() - rects[0].y;
                pb->velocity.y = 0;
                pb->SetLastBottomCollision(0);
            }
            else{ // collide top
                
                transform->position.y = other_rect->Top() - rects[0].height - rects[0].y;
                pb->velocity.y = 0;
                pb->SetLastTopCollision(0);
            }
        }
        overlapping.clear();
    }

    transform->position.x += movement.x;

    if(movement.x != 0){
        overlapping = DetermineBoxOverlaps(previous_frame_bottom);

        // horizontal (X) collisions
        for(auto& other_rect : overlapping){
            
            // attempts to step up a collider
            if(!StepUpCollider(movement.x, other_rect, pb)){

                // otherwise
                if(movement.x < 0){ // collide left
                    
                    transform->position.x = other_rect->Right() - rects[0].x;
                    pb->velocity.x = 0;
                    pb->SetLastLeftCollision(0);
                }
                else{ // collide right
                    
                    transform->position.x = other_rect->Left() - rects[0].width - rects[0].x;
                    pb->velocity.x = 0;
                    pb->SetLastRightCollision(0);
                }
            }
        }
    }

    // resolving collisions with circles ------------------------------------------

    std::vector<CircleCollider*>* circle_colliders = GetThisObject()->GetScene()->GetCircleColliders();

    // iterate over each box overlap
    for(int i = 0; i < circle_colliders->size(); i++){

        Transform* circle_transform =  circle_colliders->at(i)->GetThisObject()->GetTransform();

        // circle is in bounds of rectangle
        if(!Overlaps(circle_transform->position.x - circle_colliders->at(i)->GetRadius(), circle_transform->position.y - circle_colliders->at(i)->GetRadius(), circle_colliders->at(i)->GetRadius() * 2, circle_colliders->at(i)->GetRadius() * 2)){
            continue;
        }

        sf::Vector2f clamped_pos;
        clamped_pos.x = Calc::Clamp(transform->position.x, rects[0].Left(), rects[0].Right());
        clamped_pos.y = Calc::Clamp(transform->position.y, rects[0].Top(), rects[0].Bottom());

        sf::Vector2f overlap_vec = clamped_pos - transform->position;

        float overlap = circle_colliders->at(i)->GetRadius() - Calc::Magnitude(overlap_vec);

        // in the case the circle is right on the edge, calculating the magnitude would divide by zero
        if(std::isnan(overlap)){
            overlap = 0; 
        }

        // we are colliding
        if(overlap > 0){
            
            pb->velocity *= 0.5f;
            Move(Calc::Normalize(overlap_vec) * overlap, pb, move_depth + 1);
        }
    }
    

    this->ContainToSceneBounds();
}

bool BoxCollider::StepUpCollider(float movement_x, Rect* other_rect, PhysicsBody* pb){

    Transform* transform = GetThisObject()->GetTransform();
    sf::Vector2f starting_position = transform->position;

    float height = other_rect->Top() - rects[0].Bottom();

    if(abs(height) < step_height){

        // translate upwards, 
        transform->position.y = other_rect->Bottom() - rects[0].height - 0.5f;
        // check if above space is free
        std::vector<Rect*> found_collisions = DetermineBoxOverlaps(rects[0].Bottom());

        // hit something
        if(found_collisions.size() > 0){
            transform->position = starting_position;
            return false;
        }

        else{
            // pushing collider over edge
            if(movement_x > 0){
                transform->position.x++;
            }
            else{
                transform->position.x--;
            }

            // move by translation
            Move(sf::Vector2f(0, 0), pb);
        }
        
    }
}



void BoxCollider::ContainToSceneBounds(){

    Transform* transform = object->GetTransform();

    // keep collider within bounds

    sf::Vector2i min_bounds = object->GetScene()->GetMinBounds();
    sf::Vector2i max_bounds = object->GetScene()->GetMaxBounds();

    if(rects[0].Left() < min_bounds.x){
        transform->position.x = min_bounds.x - rects[0].x;
    }
    else if(rects[0].Top() < min_bounds.y){
        transform->position.y = min_bounds.y - rects[0].y;
    }
    if(rects[0].Right() > max_bounds.x){
        transform->position.x = max_bounds.x - rects[0].x - rects[0].width;
    }
    else if(rects[0].Bottom() > max_bounds.y){
        transform->position.y = max_bounds.y - rects[0].y - rects[0].height;
    }
}


void BoxCollider::DrawDebug(sf::RenderTarget& surface){

    /*
        draws an outline of the collider rect
    */

    Camera* cam = object->GetScene()->GetActiveCamera();
    Transform* transform = object->GetTransform();

    for(auto& rect : rects){
        
        // corners of rectangle
        sf::Vector2f tl = transform->GetGlobalPosition() + sf::Vector2f(rect.x, rect.y);
        sf::Vector2f tr = transform->GetGlobalPosition() + sf::Vector2f(rect.x, rect.y) + sf::Vector2f(rect.width * transform->scale.x, 0);
        sf::Vector2f bl = transform->GetGlobalPosition() + sf::Vector2f(rect.x, rect.y) + sf::Vector2f(0, rect.height * transform->scale.y);
        sf::Vector2f br = transform->GetGlobalPosition() + sf::Vector2f(rect.x, rect.y) + sf::Vector2f(rect.width * transform->scale.x, rect.height * transform->scale.y);

        sf::VertexArray vertex_array;
        vertex_array.setPrimitiveType(sf::LineStrip);

        sf::Vertex vertex;
        vertex.color = Globals::DEBUG_COLOUR;

        if(rect.collision_mode == ColliderType::PLATFORM){
            vertex.color = Globals::DEBUG_COLOUR_3;
        }

        // change colour for triggers...
        if(IsTrigger()){
            vertex.color = Globals::DEBUG_COLOUR_SECONDARY;

            if(Triggered()){
                vertex.color = Globals::DEBUG_COLOUR_3;
            }
        }

        vertex.position = cam->WorldToScreenPosition(tl);   
        vertex_array.append(vertex);
        vertex.position = cam->WorldToScreenPosition(tr);   
        vertex_array.append(vertex);  
        vertex.position = cam->WorldToScreenPosition(br);   
        vertex_array.append(vertex);
        vertex.position = cam->WorldToScreenPosition(bl);   
        vertex_array.append(vertex);  
        vertex.position = cam->WorldToScreenPosition(tl);   
        vertex_array.append(vertex);

        surface.draw(vertex_array);
    }
}


void BoxCollider::AddRect(int x, int y, unsigned int width, unsigned int height, ColliderType collision_mode){
    Rect rect({x, y, width, height, object->GetTransform(), collision_mode});
    rects.push_back(rect);
}
void BoxCollider::AddRect(Rect rect){
    rect.LinkTransform(object->GetTransform());
    rects.push_back(rect);
}

std::vector<BoxCollider::Rect>& BoxCollider::GetRects(){
    return rects;
}
void BoxCollider::ClearRects(){
    rects.clear();
}

void BoxCollider::SetOffset(sf::Vector2f offset, int index){
    rects.at(index).x = offset.x; 
    rects.at(index).y = offset.y;
}
void BoxCollider::SetSize(sf::Vector2f size, int index){
    rects.at(index).width = size.x; 
    rects.at(index).height = size.y;
}

BoxCollider::~BoxCollider(){
}