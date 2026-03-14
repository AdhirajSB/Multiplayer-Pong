#include "networking.hpp"

int main(){
    NET::serverSocket server(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    while (true){
        server.pollNetwork();

        while (server.hasMessages()){
            NetworkMessage message = server.getMessage();

            // Now we can parse the message
            std::cout << "Client" << message.cliend_fd << " says: " << message.payload << std::endl;
        }
    }
}