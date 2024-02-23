#pragma once
#include "UIRect.h"

/*
    a container to hold other rects for easier alignment purposes (e.g. a list of buttons)
*/
class UIRectArray : public UIRect {

    public: 

        // pushes a new rect to the end of the container
        void AddUIRect(UIRect* rect);
        std::vector<UIRect*>& GetUIRects(){return rects;}
        void ClearUIRects(){rects.clear();}

        void CatchEventEventFocusBounded(sf::Event event) override;

        void OnSetActive() override;
        void OnDisable() override;
        
        void OnAlign() override;

        // the space between each UIRect
        void SetGap(int gap);
        int GetGap(){return gap;}

        // decides the size of each element,
        void SetElementSize(int element_width, int element_height);

    private:

        void CalculateTotalSize();

        int element_width;
        int element_height;
        int gap;

        std::vector<UIRect*> rects;
};