#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <curl/curl.h>
using namespace std;

#define PORT 8081

int main() {

    int YouTube(string ID);

    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    const char* http_request = "GET / HTTP/1.1\r\nHost: 192.168.1.65:8080\r\nConnection: close\r\n\r\n";

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

    // Accept incoming connections and continuously send HTTP requests
    while (true) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("Accept failed");
            exit(EXIT_FAILURE);
        }

        // Send HTTP request to server 1
        //send(new_socket, http_request, strlen(http_request), 0);
        //std::cout << "Server 2 sent HTTP request" << std::endl;

        // Read HTTP response from server 1 
        //read(new_socket, buffer, 1024);
        //std::cout << "Server 2 received: " << buffer << std::endl;

        // Read HTTP request from server 1
        read(new_socket, buffer, 1024);
        std::cout << "Server 2 received: " << buffer << std::endl;

        // Send HTTP response to server 1
        const char* http_response = "HTTP/1.1 200 OK\r\nContent-Length: 12\r\n\r\n*** Message from Server B";
        send(new_socket, http_response, strlen(http_response), 0);
        std::cout << "Server 2 sent HTTP response" << std::endl;

        string videoID = ""; 
        cout << "What is the video ID: ";
        cin >> videoID;
        YouTube(videoID);

        close(new_socket);
    }

    return 0;
}

// YouTube Function 
int YouTube(string ID)
{
    cout << "Retrieving YouTube Video Information...";
    CURL* curl = curl_easy_init();
    if (curl)
    {
        std::string url = "https://www.googleapis.com/youtube/v3/videos?part=snippet&id=Fn_6M1rz_TE&key=AIzaSyCrbZ26eqevD7BPigznR8-eENXHvi0CJfk";
        
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK)
        {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
            curl_easy_cleanup(curl);
            return 1;
        }
        
        long response_code;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
        if (response_code != 200)
        {
            std::cerr << "Request failed with HTTP status code " << response_code << std::endl;
            curl_easy_cleanup(curl);
            return 1;
        }
        
        // TODO: Parse and print the response
        
        curl_easy_cleanup(curl);
    }
    else
    {
        std::cerr << "Failed to initialize libcurl" << std::endl;
        return 1;
    }
    
    return 0;
}
