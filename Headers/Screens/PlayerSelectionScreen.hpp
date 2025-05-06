#ifndef PlayerSelectionScreen_hpp
#define PlayerSelectionScreen_hpp

#include "Screen.hpp"
#include "raylib.h"

class PlayerSelectionScreen : public Screen {
    private:
        Music bgMusic;
        
    public:
        PlayerSelectionScreen();

        ~PlayerSelectionScreen();

        Screen* show() override;
};

#endif /* PlayerSelectionScreen_hpp */