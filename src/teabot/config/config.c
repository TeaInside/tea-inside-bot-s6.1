
#define IN_CONFIG_PARSER

#include <stdio.h>
#include <teabot/helpers.h>
#include <teabot/config/config_parser.h>

#undef IN_CONFIG_PARSER

#define QCMP(A, B, NA) ((NA == (sizeof(B) - 1)) && (!strncmp(A, B, NA)))

char *bot_token = NULL;
size_t bot_token_size = 0;
char **sudoers;
uint8_t sudoers_count = 0;
bool _verbose = true;
bool _warning = true;
uint8_t threads_amount = 3;

bool init_config(int argc, char *argv[], char *envp[]) {
	char buf[2048], *buf2;
	size_t len, blen, i;

	FILE *handle = fopen("teabot.conf", "r");
	while (fgets(buf, 2047, handle)) {
		len = strlen(buf);
		if (buf[len - 1] == '\n') {
			buf[len - 1] = '\0';
			len--;
		} else {
			buf[len] = '\0';
		}

		for (i = 0; i < len; i++) {
			if (buf[i] == '#') {
				break;
			} else if (buf[i] == '=') {
				buf2 = (char *)malloc(len + 1);
				strcpy(buf2, buf);
				buf2[i] = '\0';
				buf2 = trim(buf2);
				blen = strlen(buf2);

				if (QCMP(buf2, "token", blen)) {
					bot_token = (char *)malloc(len - i);
					strcpy(bot_token, &(buf[i + 1]));
					bot_token = trim(bot_token);
					bot_token_size = strlen(bot_token);
					verbose_log("Bot token has been loaded: \"%s\"", bot_token);
				}

				free(buf2);
			}
		}
	}

	fclose(handle);

	return (bot_token != NULL);
}

#undef CMP
