#pragma once
#include "UIButton.h"

class UIInputField : public UIButton {

    public:

        UIInputField(); 

        void CatchEventEventFocusBounded(sf::Event event) override;
        void SetPlaceholder(std::string string);

        // @returns the users character input
        std::string GetInput(){return tracked_string;}
        void SetInput(std::string string){tracked_string=string;}

    private:
        bool selected = true;
        std::string tracked_string;
        std::string placeholder_string;
};