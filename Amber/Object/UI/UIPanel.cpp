#include "UIPanel.h"
#include "../../Core/Core.h"


void UIPanel::Start(){
    OnResize();
    rectangle.setPosition(0, 0);
    rectangle.setFillColor(sf::Color(0,0,0,150));
}

void UIPanel::Draw(sf::RenderTarget& surface){
    surface.draw(rectangle);
}

void UIPanel::SetColour(const sf::Color& colour){
    rectangle.setFillColor(colour);
}

void UIPanel::OnResize(){

    rectangle.setSize(sf::Vector2f(Core::GetDisplayWidth() + 10, Core::GetDisplayHeight() + 10));
}
 
