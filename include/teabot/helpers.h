
#ifndef teabot__helpers
#define teabot__helpers

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
#endif

#define verbose_log(...) \
	if (_verbose) __log(__VA_ARGS__)

#define warning_log(...) \
	if (_warning) __log(__VA_ARGS__)

#endif
