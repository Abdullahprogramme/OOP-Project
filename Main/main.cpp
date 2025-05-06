#include "../raylib.h"
#include "../Headers/Screens/IntroScreen.hpp"
#include "../Headers/Screens/PlayerSelectionScreen.hpp"
#include "../Headers/Screens/GameScreen.hpp"

int main() {
    // Initialize the window
    const int screenWidth = 1280;
    const int screenHeight = 720;
    InitWindow(screenWidth, screenHeight, "Maze Escape");

    InitAudioDevice(); // Initialize audio device


    Screen* currentScreen = new IntroScreen();  // Start from the Intro

    while (currentScreen != nullptr) {
        Screen* nextScreen = currentScreen->show();
        delete currentScreen;  // Clean up the previous screen
        currentScreen = nextScreen;  // Move to the next screen
    }

    CloseWindow();
    CloseAudioDevice(); // Close audio device
    return 0;
}
