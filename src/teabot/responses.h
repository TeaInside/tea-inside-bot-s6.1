
#include <teabot/lang.h>
#include <teabot/lang/help.h>
#include <teabot/lang/start.h>
#include <teabot/lang/shell.h>

static void internal_thread_worker(update_data *data) {
	cJSON *chat_id = NULL;
	cJSON *chat_type = NULL;

	DETSE("id", data->chat, chat_id)
	DETSE("type", data->chat, chat_type)

	if ((chat_id != NULL) && (chat_type != NULL)) {
		goto responses;
	}

	return;

responses:
	// Start command.
	if (!strcmp(data->text, "/start")) {
		#include "responses/start.h"
	}

	// Show help.
	if (!strcmp(data->text, "/help")) {
		#include "responses/help.h"
	}

	// Execute linux shell.
	if (!strncmp((data->text + 1), "sh", 2)) {
		if (
			(
				((*(data->text)) == '/') ||
				((*(data->text)) == '!') ||
				((*(data->text)) == '~') ||
				((*(data->text)) == '.')
			) && (
				((*(data->text + 3)) == ' ') ||
				((*(data->text + 3)) == '\n') ||
				((*(data->text + 3)) == '\v') ||
				((*(data->text + 3)) == '\t') ||
				((*(data->text + 3)) == '\r')
			)
		) {
			register size_t len;
			char *input, *cmd;

			#define base_cmd "/bin/sh -c "
			input = escapeshellarg(data->text + 4);
			len = strlen(input);
			cmd = (char *)malloc(sizeof(base_cmd) + len + 1);
			sprintf(cmd, base_cmd "%s", input);
			free(input);
			input = NULL;

			verbose_log("cmd: %s\n", cmd);

			#include "responses/shell.h"
		}
	}
}
