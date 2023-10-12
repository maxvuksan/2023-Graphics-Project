#include "PointLight.h"
#include "../Object.h"
#include "../../Core/Scene.h"
#include "../../Core/Core.h"

PointLight::PointLight():
    colour(1,1,1),
    radius(1),
    strength(0) 
{}

void PointLight::Start()
{
    point_shader = AssetManager::GetShader("Amber_LightSource");
    
    object->GetScene()->AddPointLight(this);

    auto display_size = Core::GetDisplaySize();
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
    DrawLightRenderTexture();
    surface.draw(light_sprite, sf::BlendMax);
}

void PointLight::DrawLightRenderTexture(){
    sf::Sprite sprite(texture.getTexture());

    sf::Vector2f light_position = object->GetScene()->GetActiveCamera()->WorldToScreenPosition(
        object->GetTransform()->position
    );

    light_position.x /= Core::GetDisplaySize().x;
    light_position.y /= -Core::GetDisplaySize().y;
    light_position.y += 1.0f; // flipping y axis, then shifting it back to location (+1.0f)

    point_shader->setUniform("u_light_position", light_position);
    point_shader->setUniform("u_colour", sf::Glsl::Vec4(colour.r, colour.g, colour.b, colour.a));
    point_shader->setUniform("u_strength", strength);
    point_shader->setUniform("u_radius", radius);
    
    texture.draw(sprite, point_shader);
    texture.display();

    light_sprite.setTexture(texture.getTexture());

}


void PointLight::OnDestroy(){
    object->GetScene()->RemovePointLight(this);
}
