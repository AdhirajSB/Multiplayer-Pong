#include "raylib.h"
#include "networking.hpp"

const int screenWidth = 960;
const int screenHeight = 720;

int main(){
    // Lets first connect to the server
    NET::clientSocket client(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (client.connectServer() == false){
        return 1;
    }

    std::string message = "Hey There";
    client.sendData(message);

    InitWindow(screenWidth, screenHeight, "Pong");
    InitAudioDevice();
    SetTargetFPS(60);

    while (!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawRectangle(0, 0, screenWidth, screenHeight, RAYWHITE);

        EndDrawing();
    }

    CloseAudioDevice();
    CloseWindow();
}