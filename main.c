#include "headers.h"


int main(int argc, char **argv)
{
    if(argc < 6)
    {
        printf("Usage %s --url <LOGINURL> -u <USERFILE> -P <PASSFILE>\n", argv[0]);
        return(1);
    }

    const char *login_url = argv[2];
    char *username = argv[4];

    CURL *curl;
    CURLcode res;
    long response_code;
    struct MemoryStruct chunk;
    chunk.memory = malloc(1);
    chunk.size = 0;
    curl_global_init(CURL_GLOBAL_DEFAULT);
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
            return(1);
        }

        //parse_and_print_sanitized_html(chunk.memory);

        char csrf_name[128];
        char csrf_value[256];

        if(extract_csrf_token(chunk.memory, csrf_name, sizeof(csrf_name), csrf_value, sizeof(csrf_value)) == 0)
        {
            printf("Found CSRF field: %s = %s\n", csrf_name, csrf_value);
        }
        else
        {
            fprintf(stderr, "Failed to extract CSRF token.\n");
        }
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
        printf("HTTP Response Code: %ld\n", response_code);
        free(chunk.memory);

        //start of POST REQUESTING
        
        FILE *fp = fopen(argv[6], "r");

        if (!fp) 
        {
            printf("The file did not open\n");
            return 1;
        }

        char line[50];

        while(fgets(line, sizeof(line), fp))
        {
            size_t len = strlen(line);
            if(len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r'))
            {
                line[len - 1] = '\0';
            }

            multipart_post(csrf_name, csrf_value, line, username, curl, login_url);

        }
        fclose(fp);
        return(0);

    }
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    return 0;
    
}


