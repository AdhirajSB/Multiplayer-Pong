#include "networking.hpp"
#include "constants.hpp"

int main(){
    NET::serverSocket server(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    struct ball{

    };

    // The server will keep track of the ball and pass that data to the players
    while (true){
        server.pollNetwork();

        while (server.hasMessages()){
            NetworkMessage message = server.getMessage();

            // Ok so now the server knows who sent what

            int playerID = message.client_fd;
            std::string data = message.payload;

            server.sendMessageExcept(playerID, "P" + data); // Basically tell everyone the position of this player
            server.sendMessageEveryone("Ball position");
        }
    }
}