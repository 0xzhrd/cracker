#ifndef HEADERS_H
#define HEADERS_H

#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <string.h>
#include <libxml/xpath.h>
#include <libxml/HTMLparser.h>

#define COOKIE_FILE "/tmp/cookies.txt"
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_RESET "\x1b[0m"

typedef struct MemoryStruct
{
   char *memory;
   size_t size;
};


int verify_input(int argc, char **argv);

size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);

int multipart_post(char *csrf_name, char *csrf_value, char *line, char *username, CURL *curl, const char *login_url);

int extract_csrf_token(const char *html, char *name_buf, size_t name_buf_size, char *value_buf, size_t value_buf_size);


#endif
