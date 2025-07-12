#include "headers.h"

int extract_csrf_token(const char *html, char *name_buf, size_t name_buf_size, char *value_buf, size_t value_buf_size)
{
    if(!html || !name_buf || !value_buf || name_buf_size == 0 || value_buf_size == 0)
    {
        fprintf(stderr, "Invalid arguments.\n");
        return(-1);
    }

    htmlDocPtr doc = htmlReadMemory(html, strlen(html), NULL, NULL, HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING | HTML_PARSE_RECOVER);
    if(!doc)
    {
        fprintf(stderr, "Failed to parse HTML document.\n");
        return(-1);
    }

    xmlXPathContextPtr ctx = xmlXPathNewContext(doc);
    if(!ctx)
    {
        fprintf(stderr, "Failed to create XPath content.\n");
        xmlFreeDoc(doc);
        return(-1);
    } 

    xmlXPathObjectPtr obj = xmlXPathEvalExpression((const xmlChar *)"//input[@type='hidden'][@name][@value]", ctx);
    
    if(!obj)
    {
        fprintf(stderr, "XPath evaluation failed.\n");
        xmlXPathFreeContext(ctx);
        xmlFreeDoc(doc);
        return(-1);
    }

    if(xmlXPathNodeSetIsEmpty(obj->nodesetval))
    {
        fprintf(stderr, "No hidden input with both name and value found.\n");
        xmlXPathFreeObject(obj);
        xmlXPathFreeContext(ctx);
        xmlFreeDoc(doc);
        return(-1);
    }

    xmlNodePtr node = obj->nodesetval->nodeTab[0];
    xmlChar *name_prop = xmlGetProp(node, (const xmlChar *)"name");
    xmlChar *value_prop = xmlGetProp(node, (const xmlChar *)"value");
    
    if(!name_prop || !value_prop)
    {
        fprintf(stderr, "Failed to get name or value attribute.\n");
        if(name_prop) xmlFree(name_prop);
        if(value_prop) xmlFree(value_prop);
        xmlXPathFreeObject(obj);
        xmlXPathFreeContext(ctx);
        xmlFreeDoc(doc);
        return(-1);
    }

    if(snprintf(name_buf, name_buf_size, "%s", (const char *)name_prop) >= (int)name_buf_size)
    {
        fprintf(stderr, "CSRF name truncated; buffer too small.\n");
        xmlFree(name_prop);
        xmlFree(value_prop);
        xmlXPathFreeObject(obj);
        xmlXPathFreeContext(ctx);
        xmlFreeDoc(doc);
        return(-1);
    }

    if(snprintf(value_buf, value_buf_size, "%s", (const char *)value_prop) >= (int)value_buf_size)
    {
        fprintf(stderr, "CSRF value truncated; buffer too small.\n");
        xmlFree(name_prop);
        xmlFree(value_prop);
        xmlXPathFreeObject(obj);
        xmlXPathFreeContext(ctx);
        xmlFreeDoc(doc);
        return(-1);
    }

    xmlFree(name_prop);
    xmlFree(value_prop);
    xmlXPathFreeObject(obj);
    xmlXPathFreeContext(ctx);
    xmlFreeDoc(doc);

    return(0);
}