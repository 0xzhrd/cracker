#include "headers.h"

int multipart_post(char *csrf_name, char *csrf_value, char *line, char *username, CURL *curl, const char *login_url)
{//content type multipart

    //Sets flags for redirects (3xx) or authentication failures (401).
    //Extracts redirect URLs from Location: headers.

    curl_mime *mime;
    curl_mimepart *part; 
    mime = curl_mime_init(curl);
    part = curl_mime_addpart(mime);
    curl_mime_data(part, csrf_value, CURL_ZERO_TERMINATED);
    curl_mime_name(part, csrf_name);

    
    part = curl_mime_addpart(mime);
    curl_mime_name(part, "EmailAddress");
    curl_mime_data(part, username, CURL_ZERO_TERMINATED);

    part = curl_mime_addpart(mime);
    curl_mime_name(part, "Password");
    curl_mime_data(part, line, CURL_ZERO_TERMINATED);

    curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);
    curl_easy_setopt(curl, CURLOPT_URL, login_url);

    curl_easy_setopt(curl, CURLOPT_COOKIESESSION, 1L);
    curl_easy_setopt(curl, CURLOPT_COOKIEJAR, COOKIE_FILE);
    curl_easy_setopt(curl, CURLOPT_COOKIEFILE, COOKIE_FILE);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    chunk.memory = malloc(1);
    chunk.size = 0;
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
    res = curl_easy_perform(curl);

    if(res != CURLE_OK)
    {
        fprintf(stderr, "POST request failed: %s\n", curl_easy_strerror(res));
        return(1);
    }
    else
    {
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
        printf("\n\nHTTP Response code: %ld\n", response_code);
        printf("username: %s\tpassword: %s\n", username, line);
        if(strstr(chunk.memory, "Forgot"))
        {
            printf(ANSI_COLOR_RED   "\nLogin failed.\n"     ANSI_COLOR_RESET"\n");
        }
        else
        {
            printf(ANSI_COLOR_GREEN     "Login successful.\n"   ANSI_COLOR_RESET"\n");
        }

    }
    free(chunk.memory);
    return(0);
}



