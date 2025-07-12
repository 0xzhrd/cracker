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

int build_post_data_from_form(const char *html, CURL *curl, const char *username, const char *password, char **post_data, size_t *post_len)
{
    htmlDocPtr doc = htmlReadMemory(html, strlen(html), NULL, NULL, HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING | HTML_PARSE_RECOVER);
    if(!doc)
    {
        fprintf(stderr, "Failed to parse HTML.\n");
        return(0);
    }

    xmlXPathContextPtr ctx = xmlXPathNewContext(doc);
    if(!ctx)
    {
        xmlFreeDoc(doc);
        return(-1);
    }

    xmlXPathObjectPtr inputs = xmlXPathEvalExpression((const xmlChar *)"//form[1]//input[@name]", ctx);
    if(!inputs || xmlXPathNodeSetIsEmpty(inputs->nodesetval))
    {
        fprintf(stderr, "No form inputs found.\n");
        if(inputs) xmlXPathFreeObject(inputs);
        xmlXPathFreeContext(ctx);
        xmlFreeDoc(doc);
        return(-1);
    }

    for(int i = 0; i < inputs->nodesetval->nodeNr; i++)
    {
        xmlNodePtr node = inputs->nodesetval->nodeTab[i];
        xmlChar *name = xmlGetProp(node, (const xmlChar *)"name");
        xmlChar *type = xmlGetProp(node, (const xmlChar *)"type");
        xmlChar *value = xmlGetProp(node, (const xmlChar *)"value");

        const char *use_value = (value) ? (const char *)value : "";

        if(type)
        {
            if(strcasecmp((const char *)type, "password") == 0)
            {
                use_value = password;
            }
            else if(strcasecmp((const char *)type, "text") == 0 || strcasecmp((const char *)type, "email") == 0)
            {
                use_value = username;
            }

            append_post_field(post_data, post_len, curl, (const char *)name, use_value, *post_len == 0);
        
        }
        

            if(name) xmlFree(name);
            if(type) xmlFree(type);
            if(value) xmlFree(value);

    }
 
    xmlXPathFreeObject(inputs);
    xmlXPathFreeContext(ctx);
    xmlFreeDoc(doc);
    return(0);
}

