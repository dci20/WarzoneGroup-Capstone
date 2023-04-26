#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <vector>
#include <unistd.h>
#include <curl/curl.h>
#include "youtube.cpp"
#include "accounts.cpp" 
using namespace std;

#define PORT 8081

int main() {

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


    // Accept incoming connections and continuously send HTTP responses 
    while (true) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("Accept failed");
            exit(EXIT_FAILURE);
        }

        // Read HTTP request from client
        read(new_socket, buffer, 1024);
        std::cout << "Server B received: " << buffer << std::endl;

        std::string http_request(buffer);

        //************************* Account Creation / Authentication / Login *************************
        // Check for new user header 
        if (http_request.find("newUser") != std::string::npos) {
            std::cout << "http_request contains newUser" << std::endl;
            string newUserName = "";
            //newUserName = addNewUser();

            //Extract username 
            size_t pos1 = http_request.find("/user=");
            if (pos1 == std::string::npos) {
                std::cerr << "Invalid HTTP request" << std::endl;
                close(new_socket);
                continue;
            }
            size_t pos2 = http_request.find_first_of("/?&' ", pos1 + 6); // find next "/?&'" after "/youtube?user="
            if (pos2 == std::string::npos) {
                pos2 = http_request.length() - 1; // if no "/?&'" found, username extends to end of string
            }
            std::string username = http_request.substr(pos1 + 6, pos2 - pos1 - 6);
            std::cout << "Username: " << username << std::endl;

            //Extract password 
            size_t posA = http_request.find("/password=");
            if (posA == std::string::npos) {
                std::cerr << "Invalid HTTP request" << std::endl;
                close(new_socket);
                continue;
            }
            size_t posB = http_request.find_first_of("/?&' ", posA + 10); // find next "/?&'" after "/youtube?user="
            if (posB == std::string::npos) {
                posB = http_request.length() - 1; // if no "/?&'" found, username extends to end of string
            }
            std::string password = http_request.substr(posA + 10, posB - posA - 10);
            std::cout << "Password: " << password << std::endl;

            newUserName = addNewUser(username, password);

            string responseData = "New user added: " + newUserName;
            // Send HTTP response to client 
            std::string http_response = "HTTP/1.1 200 OK\r\nContent-Length: " + std::to_string(responseData.size()) + "\r\n\r\n" + responseData;
            send(new_socket, http_response.c_str(), http_response.size(), 0);
            std::cout << "Server sent HTTP response" << std::endl;
            //Create YouTube playlist for new user
            string newToken = getAccessToken();
            string createPlaylistResponse = createPlaylist(newToken, newUserName);
            cout << createPlaylistResponse << " for " << newUserName << endl;

        }
        
        // Check for authorization header
        bool validCredentials = false; 
        if (http_request.find("Authorization") != std::string::npos) {
            std::cout << "http_request contains Authorization" << std::endl;

            //Extract username 
            size_t pos1 = http_request.find("/user=");
            if (pos1 == std::string::npos) {
                std::cerr << "Invalid HTTP request" << std::endl;
                close(new_socket);
                continue;
            }
            size_t pos2 = http_request.find_first_of("/?&' ", pos1 + 6); // find next "/?&'" after "/youtube?user="
            if (pos2 == std::string::npos) {
                pos2 = http_request.length() - 1; // if no "/?&'" found, username extends to end of string
            }
            std::string username = http_request.substr(pos1 + 6, pos2 - pos1 - 6);
            std::cout << "Username: " << username << std::endl;

            //Extract password 
            size_t posA = http_request.find("/password=");
            if (posA == std::string::npos) {
                std::cerr << "Invalid HTTP request" << std::endl;
                close(new_socket);
                continue;
            }
            size_t posB = http_request.find_first_of("/?&' ", posA + 10); // find next "/?&'" after "/youtube?user="
            if (posB == std::string::npos) {
                posB = http_request.length() - 1; // if no "/?&'" found, username extends to end of string
            }
            std::string password = http_request.substr(posA + 10, posB - posA - 10);
            std::cout << "Password: " << password << std::endl;

            validCredentials = checkLogin(username, password); 
            string responseData = ""; 
            if (validCredentials == true){
                responseData = "User authenticated";
            }
            else {
                responseData = "Invalid credentials";
            }
            // Send HTTP response to client 
            std::string http_response = "HTTP/1.1 200 OK\r\nContent-Length: " + std::to_string(responseData.size()) + "\r\n\r\n" + responseData;
            send(new_socket, http_response.c_str(), http_response.size(), 0);
            std::cout << "Server sent HTTP response" << std::endl;
        }

        
        // Wrap the conditional statement code below if(authorization successful)
        // Conditional statement based on HTTP request 
        //std::string http_request(buffer);
        if (http_request.find("/youtube?id=") != std::string::npos) {
            std::cout << "http_request contains /youtube?id=" << std::endl;

            // Extract video ID from URL
            size_t pos1 = http_request.find("/youtube?id=");
            if (pos1 == std::string::npos || pos1 + 12 + 11 > http_request.length()) {
                std::cerr << "Invalid HTTP request" << std::endl;
                close(new_socket);
                continue;
            }
            std::string videoID = http_request.substr(pos1 + 12, 11);
            std::cout << "Video ID: " << videoID << std::endl;

            // Call YouTube function with video ID
            string responseData = YouTube(videoID);
            if(responseData.empty()){
                cout << "Failed to retrieve video information." << endl;
                return 1;
            }
            cout << responseData << endl;
            
            // Send HTTP response to client 
            std::string http_response = "HTTP/1.1 200 OK\r\nContent-Length: " + std::to_string(responseData.size()) + "\r\n\r\n" + responseData;
            send(new_socket, http_response.c_str(), http_response.size(), 0);
            std::cout << "Server sent HTTP response" << std::endl;

            close(new_socket);
        }
        else if (http_request.find("channelId=") != std::string::npos) {
            std::cout << "http_request contains channelId=" << std::endl;

            // Extract channel ID from URL
            size_t pos1 = http_request.find("channelId=");
            if (pos1 == std::string::npos || pos1 + 10 + 24 > http_request.length()) {
                std::cerr << "Invalid HTTP request" << std::endl;
                close(new_socket);
                continue;
            }
            std::string channelId = http_request.substr(pos1 + 10, 24);
            std::cout << "Channel ID: " << channelId << std::endl;

            // Call YouTube function with channel ID
            string responseData = getVideoInfo(channelId);
            if(responseData.empty()){
                cout << "Failed to retrieve video information." << endl;
                return 1;
            }
            cout << responseData << endl;

            // Send HTTP response to client 
            std::string http_response = "HTTP/1.1 200 OK\r\nContent-Length: " + std::to_string(responseData.size()) + "\r\n\r\n" + responseData;
            send(new_socket, http_response.c_str(), http_response.size(), 0);
            std::cout << "Server sent HTTP response" << std::endl;

            close(new_socket);
        }
        else if (http_request.find("watch?v=") != std::string::npos) {
            std::cout << "http_request contains watch?v=" << std::endl;

            // Extract video ID from URL
            size_t pos1 = http_request.find("watch?v=");
            if (pos1 == std::string::npos || pos1 + 8 + 11 > http_request.length()) {
                std::cerr << "Invalid HTTP request" << std::endl;
                close(new_socket);
                continue;
            }
            std::string videoID = http_request.substr(pos1 + 8, 11);
            std::cout << "Video ID: " << videoID << std::endl;

            // Call YouTube video download function with video ID 
            string responseData = ""; 
            string video_url = "https://www.youtube.com/watch?v=" + videoID;
            string file_path = videoID + ".mp4"; 
            if (download_youtube_video(video_url, file_path)) {
                std::cout << "Video downloaded successfully!" << std::endl;
                responseData = "Video downloaded successfully!";
            } else {
                std::cerr << "Failed to download video" << std::endl;
                responseData = "Failed to download video";
            }

            // Send HTTP response to client 
            std::string http_response = "HTTP/1.1 200 OK\r\nContent-Length: " + std::to_string(responseData.size()) + "\r\n\r\n" + responseData;
            send(new_socket, http_response.c_str(), http_response.size(), 0);
            std::cout << "Server sent HTTP response" << std::endl;

            close(new_socket);

        }
        else if (http_request.find("/youtube?user=") != std::string::npos) {
            std::cout << "http_request contains /youtube?user=" << std::endl;

            // Extract username from URL
            size_t pos1 = http_request.find("/youtube?user=");
            if (pos1 == std::string::npos) {
                std::cerr << "Invalid HTTP request" << std::endl;
                close(new_socket);
                continue;
            }
            size_t pos2 = http_request.find_first_of("/?&' ", pos1 + 14); // find next "/?&'" after "/youtube?user="
            if (pos2 == std::string::npos) {
                pos2 = http_request.length() - 1; // if no "/?&'" found, username extends to end of string
            }
            std::string username = http_request.substr(pos1 + 14, pos2 - pos1 - 14);
            std::cout << "Username: " << username << std::endl;

            // Call YouTube function with username to retrieve playlist ID 
            string playlistId = getPlaylistId(username); 
            if(playlistId.empty()){
                cout << "Failed to retrieve video information." << endl;
                return 1;
            }
            cout << "Playlist ID: " << playlistId << endl;

            // Call YouTube function with playlist ID to retrieve playlist videos 
            string responseData = getVideoPlaylist(playlistId);
            if(responseData.empty()){
                cout << "Failed to retrieve playlist videos." << endl;
                return 1;
            }
            cout << "Playlist videos for user: " << username << endl;
            cout << responseData << endl; 

            // Send HTTP response to client 
            std::string http_response = "HTTP/1.1 200 OK\r\nContent-Length: " + std::to_string(responseData.size()) + "\r\n\r\n" + responseData;
            send(new_socket, http_response.c_str(), http_response.size(), 0);
            std::cout << "Server sent HTTP response" << std::endl;

            close(new_socket);
        }
        else if (http_request.find("/videos?part=") != std::string::npos) {
            std::cout << "http_request contains /videos?part=" << std::endl;

            // Extract username from URL
            size_t pos1 = http_request.find("/user=");
            if (pos1 == std::string::npos) {
                std::cerr << "Invalid HTTP request" << std::endl;
                close(new_socket);
                continue;
            }
            size_t pos2 = http_request.find_first_of("/?&' ", pos1 + 6); // find next "/?&'" after "/youtube?user="
            if (pos2 == std::string::npos) {
                pos2 = http_request.length() - 1; // if no "/?&'" found, username extends to end of string
            }
            std::string username = http_request.substr(pos1 + 6, pos2 - pos1 - 6);
            std::cout << "Username: " << username << std::endl;

            // Extract video_file_path from URL
            size_t posA = http_request.find("/path=");
            if (posA == std::string::npos) {
                std::cerr << "Invalid HTTP request" << std::endl;
                close(new_socket);
                continue;
            }
            size_t posB = http_request.find_first_of("?&' ", posA + 6); // find next "/?&'" after "/youtube?user="
            if (posB == std::string::npos) {
                posB = http_request.length() - 1; // if no "/?&'" found, username extends to end of string
            }
            std::string file_path = http_request.substr(posA + 6, posB - posA - 6);
            std::cout << "Video File Path: " << file_path << std::endl;

            // Call getAccessToken()
            string newToken = "";
            newToken = getAccessToken();

            // Call getPlaylistId(std::string playlistName) - "playlistName same as username" 
            string playlistId = getPlaylistId(username); 
            if(playlistId.empty()){
                cout << "Failed to retrieve video information." << endl;
                return 1;
            }
            cout << "Playlist ID: " << playlistId << endl;

            // Call uploadVideo(std::string access_token, std::string playlist_id, std::string video_file_path)
            string responseData = uploadVideo(newToken, playlistId, file_path);

            cout << "Response Data: " << responseData << endl;
            
            // Send HTTP response to client 
            std::string http_response = "HTTP/1.1 200 OK\r\nContent-Length: " + std::to_string(responseData.size()) + "\r\n\r\n" + responseData;
            send(new_socket, http_response.c_str(), http_response.size(), 0);
            std::cout << "Server sent HTTP response" << std::endl;

            close(new_socket);

        }

    }

    return 0;
}
