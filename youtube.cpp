#include <iostream>
#include <string>
#include <curl/curl.h>
using namespace std;

string YouTube(string ID); //Search with video ID (single video) 
string getVideoInfo(string channelId); //Search with channel ID (all videos on channel) 

// Define a callback function to write the response data to a string
size_t writeCallback(char* ptr, size_t size, size_t nmemb, void* userdata) {
    size_t numBytes = size * nmemb;
    string* responseData = static_cast<string*>(userdata);
    responseData->append(ptr, numBytes);
    return numBytes;
}

// YouTube Function - Search with video ID
string YouTube(string ID)
{
    cout << "Retrieving YouTube Video Information for Video ID: " + ID;
    CURL* curl = curl_easy_init();
    if (curl)
    {
        std::string url = "https://www.googleapis.com/youtube/v3/videos?part=snippet&id=" + ID + "&key=AIzaSyCrbZ26eqevD7BPigznR8-eENXHvi0CJfk";

        string responseData;

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseData);

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK)
        {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
            curl_easy_cleanup(curl);
            return "";
        }

        long response_code;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
        if (response_code != 200)
        {
            std::cerr << "Request failed with HTTP status code " << response_code << std::endl;
            curl_easy_cleanup(curl);
            return "";
        }

        curl_easy_cleanup(curl);
        return responseData;
    }
    else
    {
        std::cerr << "Failed to initialize libcurl" << std::endl;
        return "";
    }
}

// YouTube Function - Search with channel ID
string getVideoInfo(string channelId)
{
    cout << "Retrieving YouTube Video Information for channel ID: " + channelId;
    CURL* curl = curl_easy_init();
    if (curl)
    {
        string url = "https://www.googleapis.com/youtube/v3/search?part=snippet&channelId=" + channelId + "&maxResults=5&order=date&type=video&key=AIzaSyCrbZ26eqevD7BPigznR8-eENXHvi0CJfk"; 

        string responseData;

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseData);

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK)
        {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
            curl_easy_cleanup(curl);
            return "";
        }

        long response_code;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
        if (response_code != 200)
        {
            std::cerr << "Request failed with HTTP status code " << response_code << std::endl;
            curl_easy_cleanup(curl);
            return "";
        }

        curl_easy_cleanup(curl);
        return responseData;
    }
    else
    {
        std::cerr << "Failed to initialize libcurl" << std::endl;
        return "";
    }
}