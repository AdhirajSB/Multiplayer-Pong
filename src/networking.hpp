#ifndef NETWORKING_HPP
#define NETWORKING_HPP

#include <poll.h>
#include <queue>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

struct NetworkMessage{
    int cliend_fd;
    std::string payload;
};

// Only works for IPv4 (TCP)
namespace NET{

    class serverSocket{
        private:
        int m_socket;
        int m_port;
        std::vector<pollfd> m_fds;
        std::queue<NetworkMessage> m_messageQueue;

        public:
        serverSocket(int family, int type, int protocol); 
        ~serverSocket();

        void acceptConnection();
        void pollNetwork();
        NetworkMessage getMessage();
        const int getPort() const;
        const bool hasMessages() const;
    };

    class clientSocket{
        private:
        int m_socket;
        int m_port;

        public:
        clientSocket(int family, int type, int protocol);
        ~clientSocket();

        bool sendData(const std::string& message);
    };
}

#endif