#include "UIRectArray.h"
#include "../../Utility/Calc.h"

void UIRectArray::AddUIRect(UIRect* rect){

    rects.push_back(rect);
    rect->SetAlign(ScreenLocationX::FREE, ScreenLocationY::FREE);

    CalculateTotalSize();
}

void UIRectArray::SetGap(int gap){
    this->gap = gap;

    CalculateTotalSize();
}

void UIRectArray::SetElementSize(int element_width, int element_height){
    this->element_width = element_width;
    this->element_height = element_height;

    CalculateTotalSize();
}

void UIRectArray::CalculateTotalSize(){
    width = this->element_width;
    height = this->element_height * rects.size() + Calc::Clamp(this->gap * rects.size() - 1, 0, INT_MAX);

    Align();
}

void UIRectArray::OnAlign(){
    
    // align all rects
    for(int i = 0; i < rects.size(); i++){
        rects[i]->width = this->element_width;
        rects[i]->height = this->element_height;

        rects[i]->GetThisObject()->GetTransform()->position.x = GetThisObject()->GetTransform()->position.x;
        rects[i]->GetThisObject()->GetTransform()->position.y = GetThisObject()->GetTransform()->position.y + (this->element_height + this->gap) * i ;
    }
}

void UIRectArray::OnSetActive(){
    for(int i = 0; i < rects.size(); i++){
        rects[i]->SetActive(true);
    }
}
void UIRectArray::OnDisable(){
    for(int i = 0; i < rects.size(); i++){
        rects[i]->SetActive(false);
    }
}
