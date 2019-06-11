
#define IN_CONFIG_PARSER

#include <stdio.h>
#include <teabot/helpers.h>
#include <teabot/config/config_parser.h>

#define CMP(A,B,N) (N == sizeof(B)) && strncmp(A, B, N)

char *bot_token;
char **sudoers;
uint8_t sudoers_count = 0;
bool _verbose = true;
bool _warning = true;

bool init_config(int argc, char *argv[], char *envp[]) {

	char buff[1025];
	size_t i, llen;
	uint32_t line = 1;

	FILE *handle = fopen("teabot.conf", "r");
	while (!feof(handle)) {

		#pragma GCC diagnostic push
		#pragma GCC diagnostic ignored "-Wunused-result"
		fgets(buff, 1024, handle);
		#pragma GCC diagnostic pop

		llen = strlen(buff);
		if (buff[llen - 1] == '\n') {
			buff[llen - 1] = '\0';
			llen--;
		} else {
			buff[llen] = '\0';
		}

		for (i = 0; i < llen; ++i) {
			if (buff[i] == '#') {
				break;
			}

			if (buff[i] == '=') {
				if (CMP(buff, "token", i)) {
					bot_token = (char *)malloc(llen - i + 1);
					memcpy(bot_token, &(buff[i + 1]), llen - i - 1);
					bot_token[llen - i] = '\0';
					TRIMN(bot_token, llen - i - 1)
					printf("test\n");
					verbose_log("Bot token has been loaded: %s", bot_token);
				}
			}
		}

		memset(buff, '\0', llen);
		line++;
	}
	fclose(handle);
	return true;
}

#undef CMP
