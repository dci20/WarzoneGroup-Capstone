#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

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

    // connect to the web server
    int port = 8080; // HTTP port
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip_str.c_str(), &server_addr.sin_addr) <= 0) {
        std::cerr << "Can't convert IP address to binary form!" << std::endl;
        return -1;
    }

    while (true) {
        // connect to the web server
        if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            std::cerr << "Can't connect to the web server!..." << std::endl;
            break;
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
            break;
        }

        // print the response from the web server
        std::cout << response << std::endl;

        // close the socket
        close(sock);
    }

    return 0;
}

#include <iostream>
#include <curl/curl.h>

int main()
{
    CURL* curl = curl_easy_init();
    if (curl)
    {
        std::string url = "https://www.googleapis.com/youtube/v3/videos?part=snippet&id=<video_id>&key=<api_key>";
        
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK)
        {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }
        
        curl_easy_cleanup(curl);
    }
    
    return 0;
}


//#####################################



/*
#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

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

    // connect to the web server
    int port = 8080; // HTTP port
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip_str.c_str(), &server_addr.sin_addr) <= 0) {
        std::cerr << "Can't convert IP address to binary form!" << std::endl;
        return -1;
    }
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Can't connect to the web server!..." << std::endl;
        return -1;
    }

    // send an HTTP request to the web server
    std::string request = "GET / HTTP/1.1\r\nHost: " + hostname + "\r\n\r\n";
    if (send(sock, request.c_str(), request.size(), 0) != (int)request.size()) {
        std::cerr << "Can't send request to the web server!" << std::endl;
        return -1;
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
    if (bytes_received == 0) { ///(bytes_received < 0)
        std::cerr << "Can't receive response from the web server!" << std::endl;
        return -1;
    }

    // print the response from the web server
    std::cout << response << std::endl;

    // close the socket
    close(sock);

    return 0;
}


/*
// Talking that is needed between UI and intermediary server

#include <iostream>
#include <vector>

using namespace std;

class intermediaryFunction{
    public:
        void setUsername(string uName);
        void setURL(string url);
        void setEmbedInfo(string eInfo);
        string getUsername();
        string getUrl();
        string getEmbedInfo();
    private:
        string userName;
        string vidUrl;
        string embedInfo;
    
};

//getters
string intermediaryFunction::getUsername(){
    return userName;
}
string intermediaryFunction::getUrl(){
    return vidUrl;
}
string intermediaryFunction::getEmbedInfo(){
    return embedInfo;
}
//setters
void intermediaryFunction::setUsername(string uName){
    userName = uName;
}
void intermediaryFunction::setURL(string url){
    vidUrl = url;
}
void intermediaryFunction::setEmbedInfo(string eInfo){
    embedInfo = eInfo;
}

int main() {
    // object for calling getters and setters
    intermediaryFunction intFunc;
    
    
    return 0;
}

*/



/*
#include <iostream>
#include "googleapis/auth/client_credentials.h"
#include "googleapis/client/auth_file_storage.h"
#include "googleapis/client/transport/curl_http_transport.h"
#include "googleapis/client/util/status.h"
#include "googleapis/services/youtube/youtube_service.h"

using googleapis::client::ClientCredentialsFromFile;
using googleapis::client::FileCredentialStore;
using googleapis::client::HttpTransportLayerConfig;
using googleapis::client::OAuth2Credential;
using googleapis::client::StatusCanceled;
using googleapis::client::StatusInvalidArgument;
using googleapis::client::StatusOk;
using googleapis::client::TransportCurl;
using googleapis::client::util::Status;
using googleapis::client::util::StatusOr;
using googleapis::services::youtube::YoutubeService;
using googleapis::services::youtube::VideoListResponse;

int main(int argc, char* argv[]) {
  const char* kClientSecretsFile = "/path/to/client_secrets.json";
  const char* kCredentialStorePath = "/path/to/credential_store";
  const char* kApplicationName = "My YouTube API Application";
  const char* kAccessToken = "YOUR_ACCESS_TOKEN";

  HttpTransportLayerConfig config;
  config.ResetDefaultOptions();
  config.set_curl_options(TransportCurl::kOptionDebugData, true);

  std::unique_ptr<ClientCredentials> credentials(
      ClientCredentialsFromFile(kClientSecretsFile));
  if (!credentials) {
    std::cerr << "Failed to read client secrets file" << std::endl;
    return 1;
  }

  FileCredentialStore credential_store(kCredentialStorePath);
  std::unique_ptr<OAuth2Credential> credential(dynamic_cast<OAuth2Credential*>(
      credential_store.Load()));
  if (!credential) {
    credential.reset(new OAuth2Credential(
        credentials->client_id(), credentials->client_secret(),
        OAuth2Credential::kRefreshToken));
  }

  if (kAccessToken) {
    credential->set_access_token(kAccessToken);
  }

  if (!credential->is_valid()) {
    std::cerr << "Credential is not valid, need to re-authorize" << std::endl;
    return 1;
  }

  YoutubeService service(&config);
  service.set_application_name(kApplicationName);
  service.set_auth_credentials(std::move(credential));

  std::string channel_id = "YOUR_CHANNEL_ID";
  int max_results = 10;
  StatusOr<VideoListResponse> response =
      service.videos().list("snippet", "id")
          .set_channel_id(channel_id)
          .set_max_results(max_results)
          .set_video_type("unlisted")
          .execute();
  if (!response.ok()) {
    std::cerr << "Failed to retrieve videos: "
              << response.status().error_message() << std::endl;
    return 1;
  }

  for (const auto& video : response.ValueOrDie().get_items()) {
    std::cout << "Title: " << video.get_snippet().get_title() << std::endl;
    std::cout << "ID: " << video.get_id() << std::endl;
  }

  return 0;
}




#include <iostream>
#include <string>
#include <vector>



// Function to retrieve unlisted videos from the YouTube API
std::vector<std::string> getUnlistedVideosFromAPI()
{
    // Code to connect to the YouTube API and retrieve a list of unlisted videos
    // ...

    // Assume that the API returns a vector of strings representing the URLs of the unlisted videos
    std::vector<std::string> unlistedVideoLinks = { "https://www.youtube.com/watch?v=ABC123", "https://www.youtube.com/watch?v=DEF456" };

    // Return the vector of unlisted video links
    return unlistedVideoLinks;
}


// Function to check the YouTube API for new unlisted videos and save the links to a local variable
void checkForNewUnlistedVideos()
{
    // Code to retrieve the last checked date from a database or file
    // ...

    // Get the current date and time
    // ...

    // Check the YouTube API for unlisted videos that were uploaded since the last checked date
    std::vector<std::string> newUnlistedVideoLinks;
    std::vector<std::string> unlistedVideos = getUnlistedVideosFromAPI();
    for (std::string link : unlistedVideos)
    {
        // Check the upload date of the video against the last checked date
        // ...

        // If the video was uploaded after the last checked date, add its link to the vector of new unlisted video links
        if (/ video was uploaded after last checked date )
        {
            newUnlistedVideoLinks.push_back(link);
        }
    }

    // Code to save the new unlisted video links to a local variable or database
    // ...
}

*/

