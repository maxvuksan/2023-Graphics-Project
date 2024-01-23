#pragma once
#include "UIButton.h"

class UIInputField : public UIButton {

    public:

        UIInputField(); 

        void CatchEvent(sf::Event event) override;
        void SetPlaceholder(std::string string);

        // @returns the users character input
        std::string GetInput(){return tracked_string;}


    private:
        bool selected = true;
        std::string tracked_string;
        std::string placeholder_string;
};