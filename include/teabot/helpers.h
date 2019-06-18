
#ifndef teabot__helpers_h
#define teabot__helpers_h

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <cjson/cJSON.h>
#include <teabot/logger/logger.h>

#ifndef IN_CONFIG_PARSER
	extern bool _verbose;
	extern bool _warning;
	extern char *bot_token;
	extern char *storage_dir;
	extern size_t bot_token_size;
	extern uint32_t *sudoers;
#endif

char *__trim(char *str, const char *seps);
char *__rtrim(char *str, const char *seps);
char *__ltrim(char *str, const char *seps);

#define trim(STR) __trim(STR, NULL)
#define rtrim(STR) __rtrim(STR, NULL)
#define ltrim(STR) __ltrim(STR, NULL)

#define verbose_log(...) \
	if (_verbose) __log(__VA_ARGS__)

#define warning_log(...) \
	if (_warning) __log(__VA_ARGS__)

#define error_log(...) \
	__log(__VA_ARGS__)

#define BOT_API_URL(VAR, TARGET) \
	VAR = (char *)malloc(sizeof("https://api.telegram.org/bot") + strlen(TARGET) + bot_token_size + 2); \
	sprintf(VAR, "https://api.telegram.org/bot%s/%s", bot_token, TARGET);

typedef struct _curl_data {
	char *val;
	size_t len;
} curl_data;

size_t teabot_curl_write_callback(void *contents, size_t size, size_t nmemb, void *userp);
unsigned char *urlencode(const char *s, size_t len = 0);

#define curl_write(CURL_RES, VAR) \
	curl_easy_setopt(CURL_RES, CURLOPT_WRITEFUNCTION, teabot_curl_write_callback); \
	curl_easy_setopt(CURL_RES, CURLOPT_WRITEDATA, (void *)&VAR);

#endif
