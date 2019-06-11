
#ifndef teabot__helpers
#define teabot__helpers

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <teabot/logger/logger.h>

#define TRIMN(A, N) \
	if (A[0] == ' ') { \
		for (size_t _i = 1; _i < N; _i++) { \
			if (A[_i] != ' ') { \
				memcpy(A, &(A[_i]), N - _i); \
				A[N - _i] = '\0'; \
				break; \
			} \
		} \
	} \
	if (A[N - 1] == ' ') { \
		for (size_t _i = N - 2; _i >= 0; _i--) { \
			if (A[_i] != ' ') { \
				A[_i + 1] = '\0'; \
				break; \
			} \
		} \
	}

#ifndef IN_CONFIG_PARSER
	extern bool _verbose;
	extern bool _warning;
	extern char *bot_token;
	extern size_t bot_token_size;
#endif

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
