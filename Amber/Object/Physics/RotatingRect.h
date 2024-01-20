#pragma once
#include "../Component.h"
/*
    a utility rectangle which responds to the objects transform
*/
class RotatingRect : public Component{

    public:

        RotatingRect();



        void SetSize(int width, int height);

        void Update() override;
        void DrawDebug(sf::RenderTarget& surface) override;


        const sf::Vector2f& GetTopLeft(){return top_left;}
        const sf::Vector2f& GetTopRight(){return top_right;}
        const sf::Vector2f& GetBottomLeft(){return bottom_left;}
        const sf::Vector2f& GetBottomRight(){return bottom_right;}


        /*
            calculates each corner of the rectangle using the objects transform rotation
        */
        void CalculatePoints();

        /*
            @returns true if the provided rectangle points overlap this RotatingRect 
        */
        bool Overlapping(const sf::Vector2f& top_left, const sf::Vector2f& top_right, const sf::Vector2f& bottom_left, const sf::Vector2f& bottom_right);

        /*
            projects each point on the provided axis

            @returns the min and max values of the points post projection, represented as (x, y), (min, max)
        */
        static sf::Vector2f GetMinMaxProjectedToAxis(const sf::Vector2f& top_left, const sf::Vector2f& top_right, const sf::Vector2f& bottom_left, const sf::Vector2f& bottom_right, const sf::Vector2f& axis);

        static sf::Vector2f CalculateProjectionAxis(const sf::Vector2f& point_start, const sf::Vector2f& point_end);

    private:

        bool overlapping = false;

        int width;
        int height;

        float half_width;
        float half_height;

        std::vector<sf::Vector2f> project_axis;

        sf::Vector2f top_left;
        sf::Vector2f top_right;
        sf::Vector2f bottom_left;
        sf::Vector2f bottom_right;
};