#include "headers.h"

int append_post_field(char **post_data, size_t *length, CURL *curl, const char *name, const char *value, int is_first)
{
    char *encoded = curl_easy_escape(curl, value, 0);
    if(!encoded)
    {
        fprintf(stderr, "URL encoding failed for: %s\n", name);
        return(-1);
    }
    size_t extra_len = strlen(name) + strlen(encoded) + 2;
    char *new_buf = realloc(*post_data, *length + extra_len);
    if(!new_buf)
    {
        fprintf(stderr, "Out of memory building POST data.\n");
        curl_free(encoded);
        return(-1);
    }
    *post_data = new_buf;
    snprintf(*post_data + *length, extra_len, "%s%s=%s", is_first ? "" : "&", name, encoded);
    *length += strlen(*post_data + *length);
    curl_free(encoded);
    return(0);
}
