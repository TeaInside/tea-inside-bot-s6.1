
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
	if (!strcmp(data->text, "/start")) {
		#include "responses/start.h"
	}

}
