#include <iostream>
#include <curl/curl.h>
using namespace std;


int main(int argc, char** argv)
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

        // Open a file for writing the video content
        FILE* fp = fopen("videoxxx.mp4", "wb");
        if (!fp)
        {
            cerr << "Failed to open file for writing" << endl;
            return 1;
        }

        // Set the curl write function to write to the file
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, fwrite);

        // Download the video content and write it to the file
        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
        {
            cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
            curl_easy_cleanup(curl);
            fclose(fp);
            return 1;
        }

        // Close the file and clean up curl
        fclose(fp);
        curl_easy_cleanup(curl);
        
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
