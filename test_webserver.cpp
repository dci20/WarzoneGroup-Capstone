#include <iostream>
#include <string>
#include <sstream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

//#define PORT 8080 

using namespace std;

int main() {

    ///
    const char* hello = "Hello World!";
    ///

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        cout << "Error: Could not create socket" << endl;
        return 1;
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(8080);

    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        cout << "Error: Could not bind socket" << endl;
        return 1;
    }

    if (listen(server_socket, 3) == -1) {
        cout << "Error: Could not listen on socket" << endl;
        return 1;
    }

    cout << "Server started on port 8080" << endl;

    while (true) {
        int client_socket = accept(server_socket, NULL, NULL);
        if (client_socket == -1) {
            cout << "Error: Could not accept client connection" << endl;
            continue;
        }

        ///
        send(client_socket, hello, strlen(hello), 0);
        std::cout << "Hello World message sent." << std::endl;
        //close(client_socket);
        ///

        stringstream response;
        response << "HTTP/1.1 200 OK\r\n";
        response << "Content-Type: text/html\r\n";
        response << "\r\n";
        response << "<html><body><h1>Hello, World!</h1></body></html>";

        string response_string = response.str();
        const char *response_chars = response_string.c_str();
        int response_length = strlen(response_chars);

        if (send(client_socket, response_chars, response_length, 0) != response_length) {
            cout << "Error: Could not send response to client" << endl;
        }

        close(client_socket);
    }

    close(server_socket);
    return 0;
}
