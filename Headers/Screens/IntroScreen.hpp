#ifndef IntroScreen_hpp
#define IntroScreen_hpp

#include "Screen.hpp"
#include "raylib.h"

class IntroScreen : public Screen {
    private:
        Music bgMusic;
        
    public:
        IntroScreen();
        
        ~IntroScreen();

        Screen* show() override;
};

#endif /* IntroScreen_hpp */