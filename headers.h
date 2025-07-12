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

struct MemoryStruct
{
   char *memory;
   size_t size;
};

CURL *curl;
CURLcode res;
long response_code;
struct MemoryStruct chunk;


size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);
void print_sanitized_text(xmlNode *node);
int multipart_post(char *csrf_name, char *csrf_value, char *line, char *username, CURL *curl, const char *login_url);
int append_post_field(char **post_data, size_t *length, CURL *curl, const char *name, const char *value, int is_first);
void parse_and_print_sanitized_html(const char *html);
int extract_csrf_token(const char *html, char *name_buf, size_t name_buf_size, char *value_buf, size_t value_buf_size);
int build_post_data_from_form(const char *html, CURL *curl, const char *username, const char *password, char **post_data, size_t *post_len);
char *condition(char *argv);


