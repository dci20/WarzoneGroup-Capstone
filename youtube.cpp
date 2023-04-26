#include <iostream>
#include <string>
#include <curl/curl.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <vector> 
using namespace std;

string YouTube(string ID); //Search with video ID (single video) 
string getVideoInfo(string channelId); //Search with channel ID (all videos on channel) 
bool download_youtube_video(const std::string& url, const std::string& file_path); //Download video based on video ID 
std::string getPlaylistId(std::string playlistName); //Retrieve playlist ID based on playlist name (playlist name same as username)
std::string getVideoPlaylist(string playlistId); //Retrieve playlist videos based on playlist ID 
std::string getAccessToken(); //Get OAuth access token for authenticating with YouTube API 
string createPlaylist(string access_token, string playlist_title); //Create new YouTube playlist 
std::string uploadVideo(std::string access_token, std::string playlist_id, std::string video_file_path); //Upload video to YouTube (this function uploads video and then calls addVideoToPlaylist())
std::string addVideoToPlaylist(std::string access_token, std::string video_id, std::string playlist_id); //Add uploaded video to a playlist 

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

// Function to obtain an access token using the client ID, client secret, and refresh token
std::string getAccessToken() {

    // Google API client ID and secret
    std::string CLIENT_ID = "474285732191-pi07fg9hpml8i4tmr1nh5u5dl4qne2k4.apps.googleusercontent.com";
    std::string CLIENT_SECRET = "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";

    // Google OAuth 2.0 API endpoints
    std::string TOKEN_ENDPOINT = "https://oauth2.googleapis.com/token";

    // Refresh token obtained previously
    std::string REFRESH_TOKEN = "1//04e2HyGrhBOo0CgYIARAAGAQSNwF-L9IrY-toPZkKcnaA7SN1Uk-Xb4IdB2ooLGW0prndwFlUcpB2UBd18TufUZdz0Nb6nKzAyB0";
    //"1//04WO6okF6bEhhCgYIARAAGAQSNwF-L9Ir4i0fnxqsnxceXE_hTrLvIj3iaf4xdBIqljqh_VN4J-Bq6LoAk6zpYftPSivuXLxklVM";

    std::string access_token = "";

    // Build the POST data for the token endpoint
    std::string post_data = "client_id=" + CLIENT_ID + "&client_secret=" + CLIENT_SECRET + "&refresh_token=" + REFRESH_TOKEN + "&grant_type=refresh_token";

    std::string response;
    curl_global_init(CURL_GLOBAL_ALL);

    CURL *curl = curl_easy_init();

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data.c_str());
        curl_easy_setopt(curl, CURLOPT_URL, TOKEN_ENDPOINT.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "Error getting access token: " << curl_easy_strerror(res) << std::endl;
        }
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();

    // Extract the access token from the response
    std::string token_key = "\"access_token\": \"";
    std::size_t token_pos = response.find(token_key);
    if (token_pos != std::string::npos) {
        token_pos += token_key.length();
        std::size_t token_end = response.find('"', token_pos);
        access_token = response.substr(token_pos, token_end - token_pos);
    } else {
        std::cerr << "Error extracting access token from response" << std::endl;
    }

    return access_token;
}

string createPlaylist(string access_token, string playlist_title) {
    CURL *curl;
    CURLcode res;

    std::string url = "https://youtube.googleapis.com/youtube/v3/playlists?part=snippet,status&snippet.channelId=UCB0CpnqE_P4LPHZ0XXZz-Hg&snippet.title=MyNewPlaylist&status.privacyStatus=public";
    //std::string data = "{\"snippet\":{\"channelId\":\"UCB0CpnqE_P4LPHZ0XXZz-Hg\",\"title\":\"newPlaylistZZZZ\"},\"status\":{\"privacyStatus\":\"public\"}}";
    std::string data = "{\"snippet\":{\"channelId\":\"UCB0CpnqE_P4LPHZ0XXZz-Hg\",\"title\":\"" + playlist_title + "\"},\"status\":{\"privacyStatus\":\"public\"}}";
    std::string newToken = "Authorization: Bearer " + access_token;

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, newToken.c_str()); 

    string response;
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback); //added 
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);  //added
        res = curl_easy_perform(curl);
        if (res != CURLE_OK){
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
            curl_easy_cleanup(curl);
            curl_slist_free_all(headers);
            string errorResponse = "Failed to create playlist";
            return errorResponse;
        }
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
        string success = "Playlist created successfully";
        return success;
        
    }
    //cout << "Response: " << response << endl;
    curl_slist_free_all(headers);
    string final = "Failed to create playlist";
    return final;

}

std::string uploadVideo(std::string access_token, std::string playlist_id, std::string video_file_path) {
    // YouTube Data API endpoint for uploading a video
    std::string ENDPOINT = "https://www.googleapis.com/upload/youtube/v3/videos?part=snippet";

    std::string response;
    curl_global_init(CURL_GLOBAL_ALL);

    CURL *curl = curl_easy_init();

    if (curl) {

        // Build the POST data for the videos endpoint
        std::string post_data = "--BOUNDARY\r\n";
        post_data += "Content-Type: application/json; charset=UTF-8\r\n\r\n";
        post_data += "{ \"snippet\": { \"title\": \"My Video\", \"description\": \"My video description\", \"tags\": [\"my\", \"video\", \"tags\"] } }\r\n";
        post_data += "--BOUNDARY\r\n";
        post_data += "Content-Type: video/mp4\r\n\r\n";

        // Read the video file into memory
        std::ifstream file(video_file_path, std::ios::binary);
        std::vector<char> file_data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

        // Add the video file data to the POST data
        post_data += std::string(file_data.begin(), file_data.end()) + "\r\n";
        post_data += "--BOUNDARY--\r\n";
        

        std::string newToken = "Authorization: Bearer " + access_token;

        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: multipart/related; boundary=BOUNDARY");
        headers = curl_slist_append(headers, newToken.c_str());

        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data.c_str());
        curl_easy_setopt(curl, CURLOPT_URL, ENDPOINT.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, post_data.length()); //added this

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "Error uploading video to YouTube: " << curl_easy_strerror(res) << std::endl;
            curl_easy_cleanup(curl);
            curl_global_cleanup();
            std::string errorResponse = "Failed to upload video to YouTube";
            return errorResponse;
        }
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    
    cout << response << endl; //testing

    
    // Find the index of the video ID in the response string
    std::string video_id_tag = "\"id\":";
    size_t video_id_index = response.find(video_id_tag);
    if (video_id_index == std::string::npos) {
        std::cerr << "Error parsing video upload response: video ID not found" << std::endl;
        std::string errorResponse = "Failed to upload video to YouTube";
        return errorResponse;
    }

    // Extract the video ID from the response string
    size_t start_index = video_id_index + (video_id_tag.length() + 2);
    size_t end_index = (start_index + 11);
    if (end_index == std::string::npos) {
        std::cerr << "Error parsing video upload response: end of video ID not found" << std::endl;
        std::string errorResponse = "Failed to upload video to YouTube";
        return errorResponse;
    }
    std::string video_id = response.substr(start_index, end_index - start_index);

    // Add the video to the specified playlist
    addVideoToPlaylist(access_token, video_id, playlist_id);

    cout << "VideoId: " << video_id << endl; 

    return "Uploading Video";

}

// Function to upload a video to a playlist using the YouTube Data API
std::string addVideoToPlaylist(std::string access_token, std::string video_id, std::string playlist_id) {

    // YouTube Data API endpoint for adding a video to a playlist
    std::string ENDPOINT = "https://www.googleapis.com/youtube/v3/playlistItems?part=snippet";

    std::string response;
    curl_global_init(CURL_GLOBAL_ALL);

    CURL *curl = curl_easy_init();

    if (curl) {
        // Build the POST data for the playlistItems endpoint
        std::string post_data = "{ \"snippet\": { \"playlistId\": \"" + playlist_id + "\", \"resourceId\": { \"kind\": \"youtube#video\", \"videoId\": \"" + video_id + "\" } } }";
        
        std::string newToken = "Authorization: Bearer " + access_token;

        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, newToken.c_str()); 

        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers); //added 
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data.c_str());
        curl_easy_setopt(curl, CURLOPT_URL, ENDPOINT.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        //curl_easy_setopt(curl, CURLOPT_HTTPHEADER, createHeaders(access_token).get()); // Use the createHeaders function to add the Authorization header
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "Error uploading video to playlist: " << curl_easy_strerror(res) << std::endl;
            curl_easy_cleanup(curl);
            curl_global_cleanup();
            std::string errorResponse = "Failed to upload video to playlist";
            return errorResponse;
        }
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();

    return "Video uploaded to playlist successfully";
}
