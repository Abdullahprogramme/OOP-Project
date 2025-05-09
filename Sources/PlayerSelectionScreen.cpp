#include "../Headers/Screens/PlayerSelectionScreen.hpp"
#include "../Headers/Screens/IntroScreen.hpp"
#include "../Headers/Screens/GameScreen.hpp"
#include "../Headers/Players/SpeedPlayer.hpp"
#include "../Headers/Players/HealthPlayer.hpp"
#include "../Headers/Players/LivesPlayer.hpp"
#include "../raylib.h"

PlayerSelectionScreen::PlayerSelectionScreen() {
    bgMusic = LoadMusicStream("Resources\\background.wav"); // Load the background music
    SetMusicVolume(bgMusic, 0.5f); // Set volume for the background music
    PlayMusicStream(bgMusic); // Start playing the music    
}

PlayerSelectionScreen::~PlayerSelectionScreen() {
    StopMusicStream(bgMusic); // Stop the music
    UnloadMusicStream(bgMusic); // Unload the music stream
}

Screen* PlayerSelectionScreen::show() {
    int selectedPlayer = 0;
    char playerName[20] = ""; // Store player's name
    int letterCount = 0;

    // Load player images
    Texture2D playerImages[3];
    playerImages[0] = LoadTexture("Resources\\SpeedPlayer.png");
    playerImages[1] = LoadTexture("Resources\\HealthPlayer.png");
    playerImages[2] = LoadTexture("Resources\\LivesPlayer.png");

    // Load enemy images
    Texture2D enemyImages[3];
    enemyImages[0] = LoadTexture("Resources\\Enemy1.png");
    enemyImages[1] = LoadTexture("Resources\\Enemy2.png");
    enemyImages[2] = LoadTexture("Resources\\BigEnemy.png");

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        UpdateMusicStream(bgMusic); // Update the music stream

        // Centered Title
        DrawText("Select Your Player", (GetScreenWidth() - MeasureText("Select Your Player", 30)) / 2, 50, 30, YELLOW);

        // Player Name Input
        DrawText("Enter your name:", (GetScreenWidth() - MeasureText("Enter your name:", 20)) / 2, 100, 20, WHITE);
        DrawText(playerName, (GetScreenWidth() - MeasureText(playerName, 20)) / 2, 130, 20, WHITE);


        // Button to go back to the IntroScreen
        const int BUTTON_WIDTH = 100;
        const int BUTTON_HEIGHT = 40;
        const int BUTTON_X = 10;
        const int BUTTON_Y = 10;

        DrawRectangle(BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT, RED); // Button background
        DrawText("Back", BUTTON_X + 20, BUTTON_Y + 10, 20, WHITE); // Button text

        // Check if the button is clicked
        Vector2 mousePosition = GetMousePosition();
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (mousePosition.x >= BUTTON_X && mousePosition.x <= BUTTON_X + BUTTON_WIDTH &&
                mousePosition.y >= BUTTON_Y && mousePosition.y <= BUTTON_Y + BUTTON_HEIGHT) {
                // Return to the IntroScreen
                for (int i = 0; i < 3; i++) {
                    UnloadTexture(playerImages[i]);
                    UnloadTexture(enemyImages[i]);
                }
                return new IntroScreen();
            }
        }

        // Grid layout for player selection
        const int GRID_X = 100; // Starting X position for the grid
        const int GRID_Y = 200; // Starting Y position for the grid
        const int GRID_WIDTH = 300; // Width of each grid cell
        const int GRID_HEIGHT = 300; // Height of each grid cell
        const int GRID_SPACING = 50; // Spacing between grid cells

        for (int i = 0; i < 3; i++) {
            int cellX = GRID_X + i * (GRID_WIDTH + GRID_SPACING);
            int cellY = GRID_Y;

            // Highlight the selected player
            if (selectedPlayer == i) {
                DrawRectangle(cellX - 5, cellY - 5, GRID_WIDTH + 10, GRID_HEIGHT + 10, GREEN);
            }

            // Draw the grid cell
            DrawRectangle(cellX, cellY, GRID_WIDTH, GRID_HEIGHT, DARKGRAY);

            // Draw the player image
            DrawTextureEx(playerImages[i], {static_cast<float>(cellX + 50), static_cast<float>(cellY + 20)}, 0.0f, 0.2f, WHITE);

            // Draw the player details
            if (i == 0) {
                DrawText("SpeedPlayer", cellX + 20, cellY + 150, 20, WHITE);
                DrawText("Health: 100", cellX + 20, cellY + 180, 20, WHITE);
                DrawText("Attack: 50", cellX + 20, cellY + 210, 20, WHITE);
                DrawText("Speed: 2", cellX + 20, cellY + 240, 20, WHITE);
                DrawText("Lives: 4", cellX + 20, cellY + 270, 20, WHITE);
            } else if (i == 1) {
                DrawText("HealthPlayer", cellX + 20, cellY + 150, 20, WHITE);
                DrawText("Health: 150", cellX + 20, cellY + 180, 20, WHITE);
                DrawText("Attack: 50", cellX + 20, cellY + 210, 20, WHITE);
                DrawText("Speed: 1", cellX + 20, cellY + 240, 20, WHITE);
                DrawText("Lives: 4", cellX + 20, cellY + 270, 20, WHITE);
            } else if (i == 2) {
                DrawText("LivesPlayer", cellX + 20, cellY + 150, 20, WHITE);
                DrawText("Health: 100", cellX + 20, cellY + 180, 20, WHITE);
                DrawText("Attack: 50", cellX + 20, cellY + 210, 20, WHITE);
                DrawText("Speed: 1", cellX + 20, cellY + 240, 20, WHITE);
                DrawText("Lives: 7", cellX + 20, cellY + 270, 20, WHITE);
            }
        }

        // Grid layout for enemies
        const int ENEMY_GRID_WIDTH = 150; // Width of each enemy grid cell
        const int ENEMY_GRID_HEIGHT = 150; // Height of each enemy grid cell
        const int ENEMY_GRID_SPACING = 50; // Spacing between enemy grid cells
        const int TOTAL_ENEMY_WIDTH = 3 * ENEMY_GRID_WIDTH + 2 * ENEMY_GRID_SPACING; // Total width of the enemy grid
        const int ENEMY_GRID_X = (GetScreenWidth() - TOTAL_ENEMY_WIDTH) / 2; // Centered X position for the enemy grid
        const int ENEMY_GRID_Y = GRID_Y + GRID_HEIGHT + 50; // Adjusted Y position for the enemy grid

        for (int i = 0; i < 3; i++) {
            int cellX = ENEMY_GRID_X + i * (ENEMY_GRID_WIDTH + ENEMY_GRID_SPACING);
            int cellY = ENEMY_GRID_Y;

            // Draw the enemy image (smaller size)
            if (i == 0) {
                DrawTextureEx(enemyImages[i], {static_cast<float>(cellX + 25), static_cast<float>(cellY + 25)}, 0.0f, 0.60f, WHITE);
            } else {
                DrawTextureEx(enemyImages[i], {static_cast<float>(cellX + 25), static_cast<float>(cellY + 25)}, 0.0f, 0.15f, WHITE);
            }

            // Draw the enemy name
            if (i == 0) {
                DrawText("Hornox", cellX + 25, cellY + ENEMY_GRID_HEIGHT - 40, 20, WHITE);
                DrawText("Type 1", cellX + 25, cellY + ENEMY_GRID_HEIGHT - 10, 20, WHITE);
            } else if (i == 1) {
                DrawText("Cryobot", cellX + 25, cellY + ENEMY_GRID_HEIGHT - 40, 20, WHITE);
                DrawText("Type 2", cellX + 25, cellY + ENEMY_GRID_HEIGHT - 10, 20, WHITE);
            } else if (i == 2) {
                DrawText("Puffrage", cellX + 25, cellY + ENEMY_GRID_HEIGHT - 40, 20, WHITE);
                DrawText("Type 3", cellX + 25, cellY + ENEMY_GRID_HEIGHT - 10, 20, WHITE);
            }
        }

        EndDrawing();

        // Handle input
        if (IsKeyPressed(KEY_LEFT)) {
            selectedPlayer = (selectedPlayer - 1 + 3) % 3;
        }
        if (IsKeyPressed(KEY_RIGHT)) {
            selectedPlayer = (selectedPlayer + 1) % 3;
        }

        // Handle name entry
        int key = GetCharPressed();
        if (key > 0 && letterCount < 19) {
            // Allow only alphanumeric characters
            if ((key >= 'A' && key <= 'Z') || (key >= 'a' && key <= 'z') || (key >= '0' && key <= '9')) {
                playerName[letterCount] = (char)key;
                letterCount++;
                playerName[letterCount] = '\0';
            }
        }

        if (IsKeyPressed(KEY_BACKSPACE) && letterCount > 0) {
            letterCount--;
            playerName[letterCount] = '\0';
        }

        if (IsKeyPressed(KEY_ENTER)) {
            break; // Proceed to next screen (gameplay)
        }
    }

    // Free textures
    for (int i = 0; i < 3; i++) {
        UnloadTexture(playerImages[i]);
        UnloadTexture(enemyImages[i]);
    }

    // Dynamically allocate selected player
    Player* selected;
    if (selectedPlayer == 0)
        selected = new SpeedPlayer(playerName, 100, 50, 4, 0);
    else if (selectedPlayer == 1)
        selected = new HealthPlayer(playerName, 150, 50, 4, 0);
    else
        selected = new LivesPlayer(playerName, 100, 50, 7, 0);

    return new GameScreen(selected);  // Return the next screen
}
