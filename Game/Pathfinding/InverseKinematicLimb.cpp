#include "InverseKinematicLimb.h"





void InverseKinematicLimb::Segment::CalculateStartPosition(){
    float dx = this->length * cos(this->angle);
    float dy = this->length * sin(this->angle);

    this->start = this->end + sf::Vector2f(dx, dy);
}

void InverseKinematicLimb::Segment::Follow(sf::Vector2f target){

    float dx = target.x - end.x;
    float dy = target.y - end.y;

    this->angle = atan2(dy, dx);

    CalculateStartPosition();

    this->end = target;
    float move_x = cos(this->angle);
    float move_y = sin(this->angle);

    this->end += sf::Vector2f(move_x * -this->length, move_y * -this->length);

    CalculateStartPosition();
}

void InverseKinematicLimb::Segment::Translate(sf::Vector2f translation){
    this->start += translation;
    this->end += translation;
}

void InverseKinematicLimb::Segment::Update(){
    if(this->parent != nullptr){
        this->Follow(parent->end);
    }
}




void InverseKinematicLimb::Init(int thickness, int count, int length_per_segment){
    this->thickness = thickness;
    this->count = count;
    this->length = length_per_segment;

    segments.resize(count);

    for(int i = 0; i < segments.size(); i++){
        segments[i].length = length;
        
        if(i != 0){
            segments[i].parent = &segments[i - 1];
        }
    }
}

void InverseKinematicLimb::Start(){
    Init(1,1,5);
}

void InverseKinematicLimb::SetTarget(sf::Vector2f target){
    this->target = target;
}

void InverseKinematicLimb::SetTargetOverTime(sf::Vector2f next_target, float time){
    this->next_target = next_target;
    this->original_target = this->target;
    this->time_tracked = time;
    this->time_total = time;
}

void InverseKinematicLimb::SetOrigin(sf::Vector2f origin){
    this->origin = origin;
}

void InverseKinematicLimb::AddSubLimb(int start, int end, int thickness, int count, int length_per_segment){

    if(start >= this->count || end >= this->count){
        std::cout << "ERROR : Could not create sublimb at positions out of main limb bounds, InverseKinematicLimb::AddSubLimb\n";
        return;
    }

    sublimbs.push_back(object->AddComponent<InverseKinematicLimb>());
    sublimbs.back()->Init(thickness, count, length_per_segment);
    sublimb_contact_points.push_back(std::make_pair(start, end));
}


void InverseKinematicLimb::Draw(sf::RenderTarget& surface){

    if(time_tracked > 0){

        time_tracked -= Time::Dt();
        if(time_tracked < 0){
            time_tracked = 0;
        }

        target = Calc::Lerp(next_target, original_target, time_tracked / time_total);
    }


    segments.at(0).Follow(target);

    for(int i = 0; i < segments.size(); i++){
        segments[i].Update();
    }

    // locking the start of the limb to the objects position -------------------------

    Segment* last = &segments[count - 1];
    float distance = Calc::Distance(last->end, origin);

    float dx = origin.x - last->end.x;
    float dy = origin.y - last->end.y;

    float angle = atan2(dy, dx);

    float move_x = cos(angle);
    float move_y = sin(angle);

    for(int i = 0; i < segments.size(); i++){
        segments[i].Translate(sf::Vector2f(move_x * distance, move_y * distance));
    }

    for(int i = 0; i < sublimbs.size(); i++){

        sublimbs[i]->SetOrigin(segments.at(sublimb_contact_points[i].first).start);
        sublimbs[i]->SetTarget(segments.at(sublimb_contact_points[i].second).start);       
    }
    

    sf::VertexArray vertex_array;
    sf::Vertex vertex;
    vertex.color = sf::Color(24, 23, 33);

    if(thickness >= 1){
        vertex_array.setPrimitiveType(sf::PrimitiveType::TriangleStrip);
    }
    else{
        vertex_array.setPrimitiveType(sf::PrimitiveType::LineStrip);
    }
    
    vertex.position = Camera::WorldToScreenPosition(segments[0].start);
    vertex_array.append(vertex);

    for(int i = 0; i < segments.size(); i++){

        if(i != segments.size() - 2){

            if(thickness >= 1){
                vertex.position = Camera::WorldToScreenPosition(
                    segments[i].end - sf::Vector2f(cos(segments[i].angle - Globals::HALF_PI) * thickness, sin(segments[i].angle - Globals::HALF_PI) * thickness)   
                );
                vertex_array.append(vertex);

                vertex.position = Camera::WorldToScreenPosition(
                    segments[i].end - sf::Vector2f(cos(segments[i].angle + Globals::HALF_PI) * thickness, sin(segments[i].angle + Globals::HALF_PI) * thickness)
                );
                vertex_array.append(vertex);
            }
            else{
                vertex.position = Camera::WorldToScreenPosition(segments[i].end);
                vertex_array.append(vertex);
            }
            //vertex.position = Camera::WorldToScreenPosition(segments[i].end);
            //vertex_array.append(vertex);
        }
    }

    vertex.position = Camera::WorldToScreenPosition(origin);
    vertex_array.append(vertex);
        

    surface.draw(vertex_array);

}
