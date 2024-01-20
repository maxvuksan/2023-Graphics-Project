#include "RotatingRect.h"
#include "../Object.h"
#include "../../Core/Scene.h"
#include "../../Utility/Calc.h"
#include <cmath>
#include <vector>
#include <algorithm>

RotatingRect::RotatingRect(): width(8), height(8){}

void RotatingRect::SetSize(int width, int height){
    this->width = width;
    this->height = height;

    half_width = width * 0.5f;
    half_height = height * 0.5f;
}

void RotatingRect::Update(){

    // rotation has changed, recalculate
    CalculatePoints();
}

void RotatingRect::DrawDebug(sf::RenderTarget& surface){

    sf::VertexArray vertex_array;
    vertex_array.setPrimitiveType(sf::PrimitiveType::LineStrip);

    sf::Vertex vertex;
    vertex.color = Globals::DEBUG_COLOUR_SECONDARY;

    if(overlapping){
        vertex.color = Globals::DEBUG_COLOUR_3;
    }

    vertex.position = Scene::GetActiveCamera()->WorldToScreenPosition(top_left);
    vertex_array.append(vertex);

    vertex.position = Scene::GetActiveCamera()->WorldToScreenPosition(top_right);
    vertex_array.append(vertex);

    vertex.position = Scene::GetActiveCamera()->WorldToScreenPosition(bottom_right);
    vertex_array.append(vertex);

    vertex.position = Scene::GetActiveCamera()->WorldToScreenPosition(bottom_left);
    vertex_array.append(vertex);

    vertex.position = Scene::GetActiveCamera()->WorldToScreenPosition(top_left);
    vertex_array.append(vertex);

    surface.draw(vertex_array);



    sf::VertexArray axis_array;

    axis_array.setPrimitiveType(sf::PrimitiveType::Lines);

    vertex.color = sf::Color::Blue;

    for(int i = 0; i < project_axis.size(); i++){
        
        vertex.position = Scene::GetActiveCamera()->WorldToScreenPosition(project_axis[i] + object->GetTransform()->position);
        axis_array.append(vertex);

        vertex.position = Scene::GetActiveCamera()->WorldToScreenPosition(project_axis[i] * 100.0f + object->GetTransform()->position);
        axis_array.append(vertex);
    }

    surface.draw(axis_array);


}

void RotatingRect::CalculatePoints(){

    sf::Vector2f origin = object->GetTransform()->position;

    // translating points by rotation
    // formula sourced from : 
    // https://math.stackexchange.com/questions/4240275/calculating-xy-coordinates-of-a-rectangle-that-is-rotated-with-a-given-rotation#:~:text=The%20easiest%20way%20to%20do,1)%20to%20each%20rotated%20point.


    float radians = Calc::Radians(object->GetTransform()->rotation);

    sf::Vector2f point(half_width, -half_height);
    top_left.x = origin.x + cos(radians) * (point.x + object->GetTransform()->position.x - origin.x) - sin(radians) * (point.y + object->GetTransform()->position.y - origin.y);
    top_left.y = origin.y + sin(radians) * (point.x + object->GetTransform()->position.x - origin.x) + cos(radians) * (point.y + object->GetTransform()->position.y - origin.y);

    point = sf::Vector2f(-half_width, -half_height);
    top_right.x = origin.x + cos(radians) * (point.x + object->GetTransform()->position.x - origin.x) - sin(radians) * (point.y + object->GetTransform()->position.y - origin.y);
    top_right.y = origin.y + sin(radians) * (point.x + object->GetTransform()->position.x - origin.x) + cos(radians) * (point.y + object->GetTransform()->position.y - origin.y);

    point = sf::Vector2f(half_width, half_height);
    bottom_left.x = origin.x + cos(radians) * (point.x + object->GetTransform()->position.x - origin.x) - sin(radians) * (point.y + object->GetTransform()->position.y - origin.y);
    bottom_left.y = origin.y + sin(radians) * (point.x + object->GetTransform()->position.x - origin.x) + cos(radians) * (point.y + object->GetTransform()->position.y - origin.y);

    point = sf::Vector2f(-half_width, half_height);
    bottom_right.x = origin.x + cos(radians) * (point.x + object->GetTransform()->position.x - origin.x) - sin(radians) * (point.y + object->GetTransform()->position.y - origin.y);
    bottom_right.y = origin.y + sin(radians) * (point.x + object->GetTransform()->position.x - origin.x) + cos(radians) * (point.y + object->GetTransform()->position.y - origin.y);

}

bool RotatingRect::Overlapping(const sf::Vector2f& other_top_left, const sf::Vector2f& other_top_right, const sf::Vector2f& other_bottom_left, const sf::Vector2f& other_bottom_right){



    // using sepearting axis theorom 

    sf::Vector2f axis_to_check[4] = {CalculateProjectionAxis(top_left, top_right), 
                                     CalculateProjectionAxis(top_left, bottom_left) , 
                                     CalculateProjectionAxis(other_top_left, other_top_right), 
                                     CalculateProjectionAxis(other_top_left, other_bottom_left)};

    project_axis.clear();


    // for each axis
    for(int i = 0; i < 4; i++){

        sf::Vector2f interval1 = GetMinMaxProjectedToAxis(top_left, top_right, bottom_left, bottom_right, axis_to_check[i]);
        sf::Vector2f interval2 = GetMinMaxProjectedToAxis(other_top_left, other_top_right, other_bottom_left, other_bottom_right, axis_to_check[i]);

        // overlapping on that axis
        if((interval2.x <= interval1.y) && (interval1.x <= interval2.y)){
            project_axis.push_back(axis_to_check[i]);
            continue;
        }
        overlapping = false;
        return false;
    }

    overlapping = true;
    return true;


}

sf::Vector2f RotatingRect::GetMinMaxProjectedToAxis(const sf::Vector2f& top_left, const sf::Vector2f& top_right, const sf::Vector2f& bottom_left, const sf::Vector2f& bottom_right, const sf::Vector2f& axis){

    // https://youtu.be/Nm1Cgmbg5SQ?t=494

    sf::Vector2f points[4] = {top_left, top_right, bottom_left, bottom_right};

    float min = __FLT_MAX__;
    float max = __FLT_MIN__;

    for(int i = 0; i < 4; i++){
        
        //float theta = Calc::VectorToRadians(points[i]) - Calc::VectorToRadians(points[i]);
        float projection = Calc::DotProduct(axis, points[i]);

        if(projection > max){
            max = projection;
        }
        if(projection < min){
            min = projection;
        }
    }

    return sf::Vector2f(min, max);

}

sf::Vector2f RotatingRect::CalculateProjectionAxis(const sf::Vector2f& point_start, const sf::Vector2f& point_end){
    
    const double axis_x = abs(point_end.y - point_start.y);
    const double axis_y =   abs(point_end.x - point_start.x);
    const double magnitude = hypot(axis_x, axis_y);

    sf::Vector2f axis_normalised;
    axis_normalised.x = axis_x / magnitude;
    axis_normalised.y = axis_y / magnitude;

    return axis_normalised;
}