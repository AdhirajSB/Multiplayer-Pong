#include "raylib.h"
#include "networking.hpp"
#include "constants.hpp"

struct ball{
    float positionX;
    float positionY;

    float velocityX;
    float velocityY;

    ball(float x, float y, float vX, float vY){
        positionX = x;
        positionY = y;

        velocityX = vX;
        velocityY = vY;
    }
};

struct paddle{
    float positionX;
    float positionY;

    paddle(float x, float y){
        positionX = x;
        positionY = y;
    }
};

int main(){
    NET::clientSocket client(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (client.connectServer() == false){
        return 1;
    }

    InitWindow(screenWidth, screenHeight, "Pong");
    InitAudioDevice();
    SetTargetFPS(60);

    paddle myPaddle(0, screenHeight/2);
    paddle enemyPaddle(screenWidth, screenHeight/2);
    ball gameBall(screenWidth/2, screenHeight/2, -100, 0);

    while (!WindowShouldClose()){
        if (IsKeyDown(KEY_UP)){
            myPaddle.positionY -= 10;
            if (myPaddle.positionY < 0.0f){
                myPaddle.positionY = 0.0f;
            } 
        }
        if (IsKeyDown(KEY_DOWN)){
            myPaddle.positionY += 10;
            if (myPaddle.positionY > screenHeight){
                myPaddle.positionY = screenHeight;
            }
        }
        
        // The server will handle the ball positions
        // float deltaT = GetFrameTime();
        // gameBall.positionX += gameBall.velocityX * deltaT;
        // gameBall.positionY += gameBall.velocityY * deltaT;

        // Send the paddle and ball data over to the server
        std::string myData = std::to_string(myPaddle.positionY);
        if (client.sendData(myData) == false){
            std::cerr << "Error sending data to server:" << strerror(errno) << std::endl;
        }

        // Recieve the enemy paddle data from the server
        while (true){
            std::string recievedData = client.getData().payload;
            if (recievedData.empty()) break;

            if (recievedData[0] == 'P'){
                // Opponent Data
                std::string pos = recievedData.substr(1);
                enemyPaddle.positionY = std::stof(pos);
            }
            else{
                // Ball Data
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawRectangle(myPaddle.positionX, myPaddle.positionY, paddleWidth, paddleLength, BLACK);
        DrawRectangle(enemyPaddle.positionX - paddleWidth, enemyPaddle.positionY, paddleWidth, paddleLength, BLACK);
        DrawCircleV({gameBall.positionX, gameBall.positionY}, ballRadius, BLACK);

        EndDrawing();
    }

    CloseAudioDevice();
    CloseWindow();
}