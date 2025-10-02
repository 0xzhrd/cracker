#include "headers.h"

int multipart_post(char *csrf_name, char *csrf_value, char *pwd, char *username, CURL *curl, const char *login_url)
{
    struct MemoryStruct chunk;
    chunk.memory = malloc(1);
    chunk.size = 0;

    long response_code;
    CURLcode res;
    curl_mime *mime;
    curl_mimepart *part; 
    mime = curl_mime_init(curl);
    if(!mime)
    {
        fprintf(stderr, "MIME init failed. post.c\n");
        free(chunk.memory);
        return(1);
    }

    part = curl_mime_addpart(mime);
    curl_mime_name(part, csrf_name);
    curl_mime_data(part, csrf_value, CURL_ZERO_TERMINATED);
    
    part = curl_mime_addpart(mime);
    curl_mime_name(part, "EmailAddress");
    curl_mime_data(part, username, CURL_ZERO_TERMINATED);

    part = curl_mime_addpart(mime);
    curl_mime_name(part, "Password");
    curl_mime_data(part, pwd, CURL_ZERO_TERMINATED);

    curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);
    curl_easy_setopt(curl, CURLOPT_URL, login_url);
    curl_easy_setopt(curl, CURLOPT_COOKIESESSION, 1L);
    curl_easy_setopt(curl, CURLOPT_COOKIEJAR, COOKIE_FILE);
    curl_easy_setopt(curl, CURLOPT_COOKIEFILE, COOKIE_FILE);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

    res = curl_easy_perform(curl);
    if(res != CURLE_OK)
    {
        fprintf(stderr, "POST request failed: %s\n", curl_easy_strerror(res));
        curl_mime_free(mime);
        free(chunk.memory);
        return(1);
    }
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
    printf("\n\nHTTP Response code: %ld\n", response_code);
    
    printf("username: %s\tpassword: %s\n", username, pwd);

    if(strstr(chunk.memory, "Forgot"))//could try using regex, or adding more checks
    {
        printf(ANSI_COLOR_RED"Login failed.\n"ANSI_COLOR_RESET);
    }
    else
    {
        printf(ANSI_COLOR_GREEN"Login successful.\n"ANSI_COLOR_RESET);
        /*curl_mime_free(mime);
        free(chunk.memory);
        return(0);*/
    }
    curl_mime_free(mime);
    free(chunk.memory);
    return(0);
}

/*int url_encoded_post(char *csrf_name, char *csrf_value, char *pwd, char *username, CURL *curl, const char *login_url)
{

}*/

