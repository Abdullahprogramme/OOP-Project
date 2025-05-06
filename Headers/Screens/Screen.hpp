#ifndef Screen_hpp
#define Screen_hpp

using namespace std;

// Abstract class for the screens
class Screen {
    public:

        // Destructor
        virtual ~Screen() = default;

        // Methods
        virtual Screen* show() = 0; // displays all the stuff on the screen
};

#endif /* Screen_hpp */