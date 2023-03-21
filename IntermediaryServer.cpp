#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

//#define PORT 9000

int main() {
    // create a socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        std::cerr << "Can't create a socket!" << std::endl;
        return -1;
    }

    // get the IP address of the web server
    std::string hostname = "192.168.1.65";
    struct hostent *he = gethostbyname(hostname.c_str());
    if (he == nullptr) {
        std::cerr << "Can't get IP address for " << hostname << std::endl;
        return -1;
    }
    struct in_addr **addr_list = (struct in_addr **)he->h_addr_list;
    struct in_addr ip_address = *addr_list[0];
    std::string ip_str = inet_ntoa(ip_address);

    while (true) {
        // connect to the web server
        int port = 8081; // HTTP port
        struct sockaddr_in server_addr;
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port);
        if (inet_pton(AF_INET, ip_str.c_str(), &server_addr.sin_addr) <= 0) {
            std::cerr << "Can't convert IP address to binary form!" << std::endl;
            return -1;
        }

        // connect to the web server
        if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            std::cerr << "Can't connect to the web server!..." << std::endl;
            //break; ////changed this
        }

        // send an HTTP request to the web server
        std::string request = "GET / HTTP/1.1\r\nHost: " + hostname + "\r\n\r\n";
        if (send(sock, request.c_str(), request.size(), 0) != (int)request.size()) {
            std::cerr << "Can't send request to the web server!" << std::endl;
            break;
        }

        // receive the response from the web server
        const int buffer_size = 1024;
        char buffer[buffer_size];
        std::string response;
        int bytes_received;
        while ((bytes_received = recv(sock, buffer, buffer_size - 1, 0)) > 0) {
            buffer[bytes_received] = '\0';
            response += buffer;
        }
        if (bytes_received < 0) {
            std::cerr << "Can't receive response from the web server!" << std::endl;
            //break;  //// changed this
        }

        // print the response from the web server
        std::cout << response << std::endl;
        //command = response; 

        
        // close the socket
        //close(sock);  //// changed this 
    }

    // close the socket
    close(sock);  //// changed this

    return 0;
}
