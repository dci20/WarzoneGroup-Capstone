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
