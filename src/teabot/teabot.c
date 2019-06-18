
#include <unistd.h>
#include <signal.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/types.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>
#include <thpool/thpool.h>
#include <teabot/teabot.h>
#include <teabot/network.h>
#include <teabot/helpers.h>
#include <teabot/msg_type.h>
#include <teabot/telegram_api.h>

#define SETSE(TMP_VAR, KEY, MAIN, TARGET, PROPERTY) \
	TMP_VAR = cJSON_GetObjectItemCaseSensitive(MAIN, KEY); \
	if (TMP_VAR != NULL) { \
		TARGET = PROPERTY; \
	}
#define DETSE(KEY, MAIN, TARGET) \
	if (MAIN != NULL) { \
		TARGET = cJSON_GetObjectItemCaseSensitive(MAIN, KEY); \
	}

#include "responses.h"

static char *get_update(bool noff = false);

void run_daemon() {
	char *job;
	threadpool thpool;
	uint64_t cycle = 0;
	char *update_string;
	int latest_update_id = 0;
	bool noff = false;
	cJSON *update_json = NULL, *result = NULL, *update = NULL, *update_id = NULL;
	thpool = thpool_init(threads_amount);

	_start_daemon:
	update_string = get_update(noff);

	// strdup("{\"ok\":true,\"result\":[{    \"update_id\": 345237741,    \"message\": {        \"message_id\": 22283,        \"from\": {\"id\": 243692601,\"is_bot\": false,\"first_name\": \"Ammar\",\"last_name\": \"Faizi\",\"username\": \"ammarfaizi2\",\"language_code\": \"en\"        },        \"chat\": {\"id\": -1001128970273,\"title\": \"Private Cloud\",\"type\": \"private\"        },        \"date\": 1555600051,        \"text\": \""
	// 	"/sh ls"
	// 	"\",        \"entities\": [{    \"offset\": 0,    \"length\": 6,    \"type\": \"bot_command\"}        ]    }}]}");

	update_json = cJSON_Parse(update_string);
	free(update_string);
	result = cJSON_GetObjectItemCaseSensitive(update_json, "result");
	if (result != NULL) {
		cJSON_ArrayForEach(update, result) {
			if (update != NULL) {
				update_id = cJSON_GetObjectItemCaseSensitive(update, "update_id");
				if (update_id != NULL && (latest_update_id < (update_id->valueint))) {
					latest_update_id = update_id->valueint;
					job = cJSON_Print(update);
					thpool_add_work(thpool, thread_worker, job);
				}
			}
		}
	}
	cycle++;
	cJSON_Delete(update_json);
	update_json = NULL;
	printf("Cycle: %ld\n", cycle);
	noff = !noff;
	goto _start_daemon;

	free(storage_dir);
	free(bot_token);
	free(sudoers);
}

void *thread_worker(void *__update_string) {
	#define update_string ((char *)__update_string)
	cJSON *tmp;
	// Initialize data with NULL.
	update_data data = {
		.main = NULL,
		.update_id = 0,
		.message = NULL,
		.message_id = 0,
		.from = NULL,
		.chat = NULL,
		.reply_to_message = NULL,
		.date = 0,
		.text = NULL
	};

	data.main = cJSON_Parse(update_string);
	free(update_string);
	__update_string = NULL;

	if (data.main != NULL) {
		DETSE("message", data.main, data.message)
		DETSE("from", data.message, data.from)
		DETSE("chat", data.message, data.chat)
		DETSE("reply_to_message", data.message, data.reply_to_message)
		SETSE(tmp, "update_id", data.main, data.update_id, tmp->valueint)
		SETSE(tmp, "text", data.message, data.text, tmp->valuestring)
		SETSE(tmp, "date", data.message, data.date, tmp->valueint)
		SETSE(tmp, "message_id", data.message, data.message_id, tmp->valueint)
		if (data.text != NULL) {
			internal_thread_worker(&data);
		}
	}
	cJSON_Delete(data.main);

	return NULL;

	#undef DETSE
	#undef SETSE
	#undef update_string
}

static char *get_update(bool noff) {
	CURL *curl;
	CURLcode res;
	char *url;
	curl_data data;

	data.val = (char *)malloc(1);
	data.len = 0;

	if (noff) {
		BOT_API_URL(url, "getUpdates?offset=-1");
		verbose_log("Getting updates with noff...");
	} else {
		BOT_API_URL(url, "getUpdates");
		verbose_log("Getting updates...");
	}

	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
		curl_write(curl, data);
		res = curl_easy_perform(curl);
		if(res != CURLE_OK) {
			warning_log("curl_easy_perform() failed: %s", curl_easy_strerror(res));
			curl_easy_cleanup(curl);
			curl_global_cleanup();
			goto nullret;
		}
		curl_easy_cleanup(curl);
		curl_global_cleanup();
	} else {
		warning_log("curl_easy_init() error");
		goto nullret;
	}

	free(url);
	url = NULL;
	return data.val;

nullret:
	free(url);
	free(data.val);
	url = NULL;
	data.val = NULL;
	return NULL;
}
