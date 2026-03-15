#include <chrono>
#include <thread>
#include "networking.hpp"
#include "constants.hpp"

int main(){
    NET::serverSocket server(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    ball gameBall(screenWidth/2, screenHeight/2, -100, 0);
    
    auto lastTime = std::chrono::high_resolution_clock::now();

    // The server will keep track of the ball and pass that data to the players
    while (true){
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> elapsedTime = currentTime - lastTime;
        lastTime = currentTime;

        float deltaT = elapsedTime.count(); // Time in seconds since last loop

        gameBall.positionX += gameBall.velocityX * deltaT;
        gameBall.positionY += gameBall.velocityY * deltaT;

        if (gameBall.positionX > screenWidth || gameBall.positionX < 0){
            gameBall.velocityX = -gameBall.velocityX;
        }
        if (gameBall.positionY > screenHeight || gameBall.positionY < 0){
            gameBall.velocityY = -gameBall.velocityY;
        }

        server.pollNetwork();
        while (server.hasMessages()){
            NetworkMessage message = server.getMessage();

            // Ok so now the server knows who sent what

            int playerID = message.client_fd;
            std::string data = message.payload;

            server.sendMessageExcept(playerID, "P" + data); // Basically tell everyone the position of this player
        }

        std::string ballP1 = "";
        ballP1 += std::to_string(gameBall.positionX);
        ballP1 += '|';
        ballP1 += std::to_string(gameBall.positionY);
        ballP1 += '|';
        ballP1 += std::to_string(gameBall.velocityX);
        ballP1 += '|';
        ballP1 += std::to_string(gameBall.velocityY);

        std::string ballP2 = "";
        ballP2 += std::to_string(screenWidth - gameBall.positionX);
        ballP2 += '|';
        ballP2 += std::to_string(gameBall.positionY);
        ballP2 += '|';
        ballP2 += std::to_string(-gameBall.velocityX);
        ballP2 += '|';
        ballP2 += std::to_string(gameBall.velocityY);

        std::vector<int> IDs = server.getIDs();
        if (IDs.size() < 2){
            gameBall.positionX = screenWidth/2; 
            gameBall.positionY = screenHeight/2;
            gameBall.velocityX =  -100;
            gameBall.velocityY = 0;
        }
        else{
            server.sendMessage(IDs[0], ballP1);
            server.sendMessage(IDs[1], ballP2);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }
}