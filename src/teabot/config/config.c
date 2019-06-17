
#define IN_CONFIG_PARSER

#include <stdio.h>
#include <teabot/helpers.h>
#include <teabot/config/config_parser.h>

#define QCMP(A, B, NA) ((NA == (sizeof(B) - 1)) && (!strncmp(A, B, NA)))

char *bot_token = NULL;
size_t bot_token_size = 0;
char *storage_dir = NULL;
size_t storage_dir_size = 0;
uint32_t *sudoers;
uint8_t sudoers_count = 0;
bool _verbose = false;
bool _warning = true;
uint8_t threads_amount = 4;

#define CONFIG_TAKE(VAR, VAR_SIZE, BUF, TARGET, BLEN, LOAD_LOG) \
	if (QCMP(BUF, TARGET, BLEN)) { \
		if (VAR == NULL) { \
			VAR = (char *)malloc(len - i); \
			strcpy(VAR, &(buf[i + 1])); \
			VAR = trim(VAR); \
			VAR_SIZE = strlen(VAR); \
			LOAD_LOG \
		} else { \
			warning_log(duplicate_msg, TARGET, line); \
		} \
	}


bool init_config(int argc, char *argv[], char *envp[]) {
	char buf[2048], *buf2;
	size_t len, blen, i;
	uint32_t line = 1;
	const char duplicate_msg[] = "Duplicate %s config is ignored in teabot.conf on line %d";

	for (int i = 0; i < argc; i++) {
		if (!strcmp(argv[i], "--verbose")) {
			_verbose = true;
		}
	}


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

				CONFIG_TAKE(bot_token, bot_token_size, buf2, "token", blen,
					verbose_log("Bot token has been loaded: \"%s\"", bot_token);
				) else
				CONFIG_TAKE(storage_dir, storage_dir_size, buf2, "storage_dir", blen,
					verbose_log("Loaded storage_dir: \"%s\"", storage_dir);
				) else

				if (QCMP(buf2, "sudoers", blen)) {
					if (sudoers == NULL) {
						size_t ii, lp;
						sudoers = (uint32_t *)malloc(sizeof(uint32_t *) * 32);
						char *rbuf = (char *)calloc(len, 1);
						for (lp = ii = i + 1; ii < len; ii++) {
							if (buf[ii] == ',' || (len == (ii + 1))) {
								strncpy(rbuf, &(buf[lp]), ii - i - 1);
								rbuf = trim(rbuf);
								sudoers[sudoers_count] = atoi(rbuf);
								verbose_log("Loaded sudoer user_id: %d", sudoers[sudoers_count++]);
								lp = ii + 1;
							}
						}
						free(rbuf);
					} else {
						warning_log(duplicate_msg, "sudoers", line);
					}
				}

				free(buf2);
			}
		}

		line++;
	}

	fclose(handle);

	return (bot_token != NULL);
}

#undef CMP
#undef CONFIG_TAKE
#undef IN_CONFIG_PARSER
