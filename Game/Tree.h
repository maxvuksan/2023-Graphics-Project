#include "../Amber/Framework.h"

class Tree : public Object {

    public:

        void Start() override{
            auto sr = AddComponent<SpriteRenderer>();
            sr->SetTexture("tree");    
            sr->SetOffset(sf::Vector2f(-(float)AssetManager::GetTexture("tree")->getSize().x / 2.0f, -(float)AssetManager::GetTexture("tree")->getSize().y));
        }

        void Update() override{

        }
};