
#include <vector>

/*
    a post processing stack, effects are applied to the main render texture after all 
    scene drawing is complete (but before an ui drawing)
*/
class Effect;
class PostProcessing {

    public:

        /*
            pushes a post processing effect to the top of the stack
            @param label the effect to fetch (from AssetManager)
        */
        static void Push(const char* label);

        /*
            clears all effects in the stack
        */
        static void Clear();
        
    private:

        static std::vector<Effect*> effect_stack;

};