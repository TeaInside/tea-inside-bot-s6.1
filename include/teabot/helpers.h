
#ifndef teabot__helpers_h
#define teabot__helpers_h

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <teabot/logger/logger.h>

#ifndef IN_CONFIG_PARSER
	extern bool _verbose;
	extern bool _warning;
	extern char *bot_token;
	extern size_t bot_token_size;
#endif

char *trim(char *str, const char *seps = NULL);
char *rtrim(char *str, const char *seps = NULL);
char *ltrim(char *str, const char *seps = NULL);

#define verbose_log(...) \
	if (_verbose) __log(__VA_ARGS__)

#define warning_log(...) \
	if (_warning) __log(__VA_ARGS__)

#define BOT_API_URL(VAR, TARGET) \
	VAR = (char *)malloc(sizeof("https://api.telegram.org/bot") + strlen(TARGET) + bot_token_size + 2); \
	sprintf(VAR, "https://api.telegram.org/bot%s/%s", bot_token, TARGET);

typedef struct _curl_data {
	char *val;
	size_t len;
} curl_data;

size_t teabot_curl_write_callback(void *contents, size_t size, size_t nmemb, void *userp);

#define curl_write(CURL_RES, VAR) \
	curl_easy_setopt(CURL_RES, CURLOPT_WRITEFUNCTION, teabot_curl_write_callback); \
	curl_easy_setopt(CURL_RES, CURLOPT_WRITEDATA, (void *)&VAR);





#endif
