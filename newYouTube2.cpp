#include <iostream>
#include <curl/curl.h>
#include <jsoncpp/json/json.h>

using namespace std;

// This function will be called by libcurl for each received chunk of data
size_t write_data(void* buffer, size_t size, size_t nmemb, void* userp)
{
    FILE* fp = (FILE*)userp;
    size_t written = fwrite(buffer, size, nmemb, fp);
    return written;
}

int main(int argc, char** argv)
{
    cout << "Retrieving YouTube Video Information...\n";
    CURL* curl = curl_easy_init();
    if (curl)
    {
        std::string url = "https://www.googleapis.com/youtube/v3/videos?part=snippet&id=Fn_6M1rz_TE&key=AIzaSyCrbZ26eqevD7BPigznR8-eENXHvi0CJfk";

        // Set up curl to retrieve video metadata
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

        // Parse the JSON response
        Json::Value root;
        Json::Reader reader;
        bool parsing_successful = reader.parse(curl_easy_strerror(res), root);
        if (!parsing_successful)
        {
            std::cerr << "Failed to parse JSON response: " << reader.getFormattedErrorMessages() << std::endl;
            curl_easy_cleanup(curl);
            return 1;
        }

        // Extract the video URL from the JSON response
        std::string video_url = root["items"][0]["snippet"]["thumbnails"]["medium"]["url"].asString();

        // Open a file for writing the video content
        FILE* fp = fopen("video.mp4", "wb");
        if (!fp)
        {
            cerr << "Failed to open file for writing" << endl;
            return 1;
        }

        // Set up a new curl session for downloading the video content
        CURL* curl2 = curl_easy_init();
        if (curl2)
        {
            // Set up curl to download the video content
            curl_easy_setopt(curl2, CURLOPT_URL, video_url.c_str());
            curl_easy_setopt(curl2, CURLOPT_WRITEDATA, fp);
            curl_easy_setopt(curl2, CURLOPT_WRITEFUNCTION, write_data);

            // Download the video content and write it to the file
            res = curl_easy_perform(curl2);
            if (res != CURLE_OK)
            {
                cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
                curl_easy_cleanup(curl2);
                fclose(fp);
                return 1;
            }

            cout << "Video downloaded successfully!" << endl;

            // Clean up curl2
            curl_easy_cleanup(curl2);
        }
        else
        {
            cerr << "Failed to initialize libcurl for downloading video" << endl;
            fclose(fp);
            return 1;
        }

        // Close the file and clean up curl
        fclose(fp);
        curl_easy_cleanup(curl);
    }
    else
    {
        std::cerr << "Failed to initialize libcurl" << std::endl;
        return 1;
    }

    return 0;
}
