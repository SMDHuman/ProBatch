#include <stdio.h>
#include <raylib.h>

#define DISPLAY_W 240
#define DISPLAY_H 240

#define FAKE_W 600
#define FAKE_H 600

int main(){
    // Initialization
    InitWindow(FAKE_W, FAKE_H, "Fake ProBatch Emulator");

    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(BLACK);

        DrawText("Hello, ProBatch!", 10, 10, 20, LIGHTGRAY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}