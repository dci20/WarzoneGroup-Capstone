#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

#define PORT 8080

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    const char* http_request = "GET / HTTP/1.1\r\nHost: 192.168.1.65:8081\r\nConnection: close\r\n\r\n";

    // Create a socket for TCP protocol
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Bind the socket to a port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    // Accept incoming connections and continuously send HTTP responses
    while (true) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("Accept failed");
            exit(EXIT_FAILURE);
        }

        // Read HTTP request from client
        read(new_socket, buffer, 1024);
        std::cout << "Server 1 received: " << buffer << std::endl;

        // Send HTTP request to server 2  ****************
        send(new_socket, http_request, strlen(http_request), 0);
        std::cout << "Server 1 sent HTTP request" << std::endl;

        // Read HTTP response from server 2 ****************
        read(new_socket, buffer, 1024);
        std::cout << "Server 1 received: " << buffer << std::endl;

        // Send HTTP response to client
        const char* http_response = "HTTP/1.1 200 OK\r\nContent-Length: 12\r\n\r\nMessage from Server A";
        send(new_socket, http_response, strlen(http_response), 0);
        std::cout << "Server 1 sent HTTP response" << std::endl;

        close(new_socket);
    }

    return 0;
}
