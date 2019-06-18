
#ifndef teabot__telegram_api_h
#define teabot__telegram_api_h

char *api_curl_post(char *url, char *body);
char *send_message(char *body);

#endif