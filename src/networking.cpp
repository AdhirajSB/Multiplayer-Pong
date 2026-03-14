#include "networking.hpp"

NET::serverSocket::serverSocket(int family, int type, int protocol){
    int port = 5555;
    if (const char* env_p = std::getenv("PORT")){
        port = std::stoi(env_p);
    }
    m_port = port;

    m_socket = socket(family, type, protocol);

    if (m_socket == -1){
        throw std::runtime_error("Unable to create server socket\n");
    }

    sockaddr_in service;
    service.sin_family = family;
    service.sin_addr.s_addr = INADDR_ANY;
    service.sin_port = htons(m_port);

    if (bind(m_socket, (sockaddr*)&service, sizeof(service)) == -1){
        throw std::runtime_error("Unable to bind server socket\n");
    }

    if (listen(m_socket, SOMAXCONN) == -1){
        throw std::runtime_error("Server unable to listen for clients\n");
    }
    std::cout << "Server socket bound and looking for clients at port: " << m_port << std::endl;


    pollfd server_fd;
    server_fd.fd = m_socket;
    server_fd.events = POLLIN;

    m_fds.push_back(server_fd);
}

NET::serverSocket::~serverSocket(){
    for (auto& pfd: m_fds){
        if (pfd.fd != -1){
            close(pfd.fd);
        }
    }
}

void NET::serverSocket::acceptConnection(){
    int acceptSocket = accept(m_socket, NULL, NULL);

    if (acceptSocket == -1){
        std::cerr << "Unable to connect to client" << std::endl;
    }
    else{
        std::cout << "Connected to client successfully" << std::endl;

        pollfd accept_fd;
        accept_fd.fd = acceptSocket;
        accept_fd.events = POLLIN;

        m_fds.push_back(accept_fd);
    }
}

void NET::serverSocket::pollNetwork(){
    int activity = poll(m_fds.data(), m_fds.size(), 0);
    if (activity == -1){
        std::cerr << "Polling failed" << std::endl;
        return;
    }

    for (int i = 0; i < (int)m_fds.size(); i++){
        if (m_fds[i].revents == 0){
            continue;
        }

        if (m_fds[i].fd == m_socket){
            // New connection
            acceptConnection();
        }
        else{
            // Existing player sent some data
            char buffer[1024];
            memset(buffer, 0, sizeof(buffer));
            
            int bytesRecieved = recv(m_fds[i].fd, buffer, sizeof(buffer) - 1, 0);

            if (bytesRecieved > 0){
                NetworkMessage message;
                message.cliend_fd = m_fds[i].fd;
                message.payload = std::string(buffer, bytesRecieved);

                m_messageQueue.push(message);
            }
            else if (bytesRecieved == 0){
                close(m_fds[i].fd);
                m_fds.erase(m_fds.begin() + i);
                i--;
            }
            else{
                std::cerr << "Error recieving data" << std::endl;
            }
        }
    }
}

NetworkMessage NET::serverSocket::getMessage(){
    NetworkMessage message = m_messageQueue.front();
    m_messageQueue.pop();

    return message;
}

const int NET::serverSocket::getPort() const{
    return m_port;
}

const bool NET::serverSocket::hasMessages() const{
    return ((int)m_messageQueue.size() > 0);
}


NET::clientSocket::clientSocket(int family, int type, int protocol){
    int port = 5555; // Will have to look into this later (deployment time)
    
    m_socket = socket(family, type, protocol);
    if (m_socket == -1){
        throw std::runtime_error("Unable to create client socket\n");
    }

    sockaddr_in service;
    service.sin_family = family;
    inet_pton(family, "127.0.0.1", &service.sin_addr); // Might have to change IP as well
    service.sin_port = htons(port);

    if (connect(m_socket, (sockaddr*)&service, sizeof(service)) == -1){
        throw std::runtime_error("Unable to bind client socket\n");
    }

    std::cout << "Connected to server successfully" << std::endl;

    // This makes the recv() non-blocking
    int flags = fcntl(m_socket, F_GETFL, 0);
    if (fcntl(m_socket, F_SETFL, flags | O_NONBLOCK) == -1) {
        throw std::runtime_error("Failed to set socket to non-blocking\n");
    }
}

NET::clientSocket::~clientSocket(){
    close(m_socket);
}

bool NET::clientSocket::sendData(const std::string &message){
    std::string formattedMessage = message + "$"; // Separate messages by '$'
    
    int bytesSent = send(m_socket, formattedMessage.c_str(), formattedMessage.length(), 0);

    if (bytesSent == -1) {
        std::cerr << "Failed to send data to server." << std::endl;
        return false;
    }
    
    return true;
}
