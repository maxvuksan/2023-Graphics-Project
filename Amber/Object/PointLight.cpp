#include "PointLight.h"
#include "Object.h"
#include "../Core/Scene.h"
#include "../Core/Core.h"

PointLight::PointLight():
    colour(1,1,1),
    radius(2.2),
    strength(0) 
{}

void PointLight::Start()
{
    point_shader = object->GetCore()->GetAssetManager()->GetShader("Amber_LightSource");
    
    object->GetScene()->AddPointLight(this);

    auto display_size = object->GetCore()->GetDisplaySize();
    texture.create(display_size.x, display_size.y);

    DrawLightRenderTexture();
  
}

void PointLight::SetColour(sf::Color colour){
    this->colour = sf::Color(colour.r / 255, colour.g / 255, colour.b / 255, colour.a);  
    DrawLightRenderTexture();
}
void PointLight::SetRadius(float radius){
    this->radius = 10.0f / radius;
    DrawLightRenderTexture();
}
void PointLight::SetStrength(float strength){
    this->strength = strength / 10.0f;
    DrawLightRenderTexture();
}



void PointLight::DrawLight(sf::RenderTarget& surface){    
    sf::Sprite sprite(texture.getTexture());

    surface.draw(sprite, sf::BlendAdd);
}

void PointLight::DrawLightRenderTexture(){
    sf::Sprite sprite(texture.getTexture());

    sf::Vector2f light_position = object->GetScene()->GetActiveCamera()->WorldToScreenPosition(
        object->GetTransform()->position
    );

    light_position.x /= object->GetCore()->GetDisplaySize().x;
    light_position.y /= object->GetCore()->GetDisplaySize().y;

    point_shader->setUniform("u_light_position", light_position);
    point_shader->setUniform("u_colour", sf::Glsl::Vec4(colour.r, colour.g, colour.b, colour.a));
    point_shader->setUniform("u_strength", strength);
    point_shader->setUniform("u_radius", radius);
    
    texture.draw(sprite, point_shader);

}


void PointLight::OnDestroy(){
    object->GetScene()->RemovePointLight(this);
}
