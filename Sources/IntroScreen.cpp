#include "../Headers/Screens/IntroScreen.hpp"
#include "../Headers/Screens/PlayerSelectionScreen.hpp"
#include "../raylib.h"

IntroScreen::IntroScreen() {
    bgMusic = LoadMusicStream("Resources\\background.wav"); // Load the background music
    SetMusicVolume(bgMusic, 0.5f); // Set volume for the background music
    PlayMusicStream(bgMusic); // Start playing the music
}

IntroScreen::~IntroScreen() {
    StopMusicStream(bgMusic); // Stop the music
    UnloadMusicStream(bgMusic); // Unload the music stream
}

Screen* IntroScreen::show() {
    while (!WindowShouldClose()) { // Keep displaying until user presses a key
        BeginDrawing();
        ClearBackground(BLACK);

        UpdateMusicStream(bgMusic); // Update the music stream

        // Display the game rules
        DrawText("Welcome to Maze Escape!", 100, 50, 30, YELLOW); // Title aligned to the left
        DrawText("Rules:", 100, 150, 25, WHITE); // Rules header aligned to the left
        DrawText("1. Collect coins to reach the goal.", 100, 200, 20, WHITE); // Rule 1
        DrawText("2. Avoid the enemies. If close, you can attack them, but you have limited Attacks!", 100, 230, 20, WHITE); // Rule 2
        DrawText("3. Look out for bonus coin that give extra points.", 100, 260, 20, WHITE); // Rule 3
        DrawText("4. Beware of the Big Enemy, it's tougher and more dangerous!", 100, 290, 20, WHITE); // Rule 4
        DrawText("5. The bonus coin and big enemy will disappear after 10 seconds.", 100, 320, 20, WHITE); // Rule 5
        DrawText("6. You have to collect the key to escape the room.", 100, 350, 20, WHITE); // Rule 6
        DrawText("7. You can only move in the four directions: up, down, left, right.", 100, 380, 20, WHITE); // Rule 7

        // Draw the box for the "Press ENTER to continue..." message
        const int BOX_WIDTH = 400;
        const int BOX_HEIGHT = 50;
        const int BOX_X = (GetScreenWidth() - BOX_WIDTH) / 2; // Center horizontally
        const int BOX_Y = GetScreenHeight() - 100; // Position near the bottom
        DrawRectangle(BOX_X, BOX_Y, BOX_WIDTH, BOX_HEIGHT, DARKGRAY); // Draw the box
        DrawText("Press ENTER to continue...", BOX_X + 50, BOX_Y + 15, 20, WHITE); // Centered text inside the box
        EndDrawing();

        // If ENTER is pressed, break the loop
        if (IsKeyPressed(KEY_ENTER)) {
            return new PlayerSelectionScreen(); // Move to the next screen
        }
    }
    return nullptr; // No next screen
}
