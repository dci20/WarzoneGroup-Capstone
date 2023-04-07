#include <iostream>
#include <string>
#include <curl/curl.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
using namespace std;

string YouTube(string ID); //Search with video ID (single video) 
string getVideoInfo(string channelId); //Search with channel ID (all videos on channel) 
bool download_youtube_video(const std::string& url, const std::string& file_path); //Download video based on video ID 
std::string getPlaylistId(std::string playlistName); //Retrieve playlist ID based on playlist name (playlist name same as username)
std::string getVideoPlaylist(string playlistId); //Retrieve playlist videos based on playlist ID 

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
        string url = "https://www.googleapis.com/youtube/v3/search?part=snippet&channelId=" + channelId + "&maxResults=10&order=date&type=video&key=AIzaSyCrbZ26eqevD7BPigznR8-eENXHvi0CJfk"; 

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

// YouTube Function - Download video by video ID 
bool download_youtube_video(const std::string& url, const std::string& file_path) {
    std::string command = "youtube-dl -f mp4 -o '" + file_path + "' '" + url + "'";
    int result = system(command.c_str());
    return (result == 0);
}

// YouTube Function - Get playlist ID based on playlist name 
std::string getPlaylistId(std::string playlistName)
{
    std::cout << "Retrieving YouTube playlist ID for playlist: " << playlistName << std::endl;

    CURL* curl = curl_easy_init();
    if (curl)
    {
        std::string channelId = "UCB0CpnqE_P4LPHZ0XXZz-Hg";
        std::string url = "https://www.googleapis.com/youtube/v3/playlists?part=snippet,id&maxResults=10&q=" + playlistName + "&channelId=" + channelId + "&key=AIzaSyCrbZ26eqevD7BPigznR8-eENXHvi0CJfk";

        std::string responseData;

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

        // Extract total number of results from response data
        size_t start = responseData.find("\"totalResults\": ") + 16;
        if (start == std::string::npos)
        {
            std::cerr << "Invalid response format" << std::endl;
            return "";
        }

        size_t end = responseData.find(',', start);
        if (end == std::string::npos)
        {
            std::cerr << "Invalid response format" << std::endl;
            return "";
        }

        int totalResults = std::stoi(responseData.substr(start, end - start));

        std::cout << "Total number of playlists on channel: " << totalResults << std::endl;

        // Find the playlist ID in the response data
        start = responseData.find("\"id\": \"");
        for (int i = 0; i < totalResults; i++)
        {
            start = responseData.find("\"id\": \"", start) + 7;
            if (start == std::string::npos)
            {
                std::cerr << "No playlist found with name " << playlistName << std::endl;
                return "";
            }

            end = responseData.find('\"', start);
            if (end == std::string::npos)
            {
                std::cerr << "Invalid response format" << std::endl;
                return "";
            }

            std::string playlistId = responseData.substr(start, end - start);

            start = responseData.find("\"title\": \"", end);
            if (start == std::string::npos)
            {
                std::cerr << "Invalid response format" << std::endl;
                return "";
            }

            start += 10;
            end = responseData.find('\"', start);
            if (end == std::string::npos)
            {
                std::cerr << "Invalid response format" << std::endl;
                return "";
            }

            std::string title = responseData.substr(start, end - start);

            if (title == playlistName)
            {
                std::cout << "Found playlist ID for: " << playlistName << endl; 
                //std::cout << "API response data: " << responseData << std::endl;
                return playlistId;
            }
        }
    }
    else
    {
        std::cerr << "Failed to initialize libcurl" << std::endl;
        return "";
    }
    return ""; 
}

// YouTube Function - Get playlist based on playlist ID 
string getVideoPlaylist(string playlistId)
{
    cout << "Retrieving YouTube Video Information for playlist: " + playlistId << endl;
    CURL* curl = curl_easy_init();
    if (curl)
    {
        string url = "https://www.googleapis.com/youtube/v3/playlistItems?part=snippet&maxResults=5&playlistId=" + playlistId + "&key=AIzaSyCrbZ26eqevD7BPigznR8-eENXHvi0CJfk";

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
