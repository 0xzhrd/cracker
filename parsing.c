#include "headers.h"

void print_sanitized_text(xmlNode *node)
{
    for( ; node; node = node ->next)
    {
        if(node->type == XML_TEXT_NODE)
        {
            xmlChar *content = xmlNodeGetContent(node);
            if(content)
            {
                if(xmlStrlen(content) > 0)
                {
                    printf("%s", (char *)content);
                }
                xmlFree(content);
            }
        }
        print_sanitized_text(node->children);
    }
}


void parse_and_print_sanitized_html(const char *html)
{
    htmlDocPtr doc = htmlReadMemory(html, strlen(html), NULL, NULL, HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING);
    if(doc == NULL)
    {
        fprintf(stderr, "Failed to parse HTML.\n");
        return;
    }

    xmlNode *root = xmlDocGetRootElement(doc);
    print_sanitized_text(root);
    printf("\n");

    xmlFreeDoc(doc);
}