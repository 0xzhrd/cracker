int extract_content_type(const char *html, char *name_buf)


char *prepare_http_request(char *type, char *path, char *params, char *headers) {
  uint32_t reqlen = 0;
  char *http_request = NULL;

  if (type && path && headers) {
    reqlen = strlen(path) + strlen(headers) + 20;
    if (params)
      reqlen += strlen(params);

    http_request = (char *)malloc(reqlen);
    if (http_request) {
      memset(http_request, 0, reqlen);

      // append the request verb (GET or POST)
      if (strcmp(type, "GET") == 0)
        strcat(http_request, "GET ");
      else
        strcat(http_request, "POST ");

      // append the request path
      strcat(http_request, path);

      // if GET, append the params now
      if (params && strcmp(type, "GET") == 0) {
        strcat(http_request, "?");
        strcat(http_request, params);
      }
      // append the headers
      strcat(http_request, " HTTP/1.0\r\n");
      strcat(http_request, headers);
      strcat(http_request, "\r\n");

      // if POST, append the params now
      if (params && strcmp(type, "POST") == 0)
        strcat(http_request, params);
    }
  }

  return http_request;
}
