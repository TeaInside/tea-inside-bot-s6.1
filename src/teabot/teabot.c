
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>
#include <teabot/network.h>
#include <teabot/helpers.h>

extern uint8_t threads_amount;

int run_teabot(int latest_update_id) {
	CURL *curl;
	CURLcode res;
	char *url;
	curl_data data;
	pthread_t threads[threads_amount + 3];
	uint8_t thread_pos = 0;
	uint8_t busy_thread = 0;
	cJSON *json, *result, *event;

	data.val = (char *)malloc(1024);
	data.len = 0;

	BOT_API_URL(url, "getUpdates");

	printf("Getting updates...\n");
	curl = curl_easy_init();
	if(curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
		curl_write(curl, data);
		res = curl_easy_perform(curl);
		if(res != CURLE_OK) {
			warning_log("curl_easy_perform() failed: %s", curl_easy_strerror(res));
		}
		curl_easy_cleanup(curl);

		json = cJSON_Parse(data.val);
		free(data.val);
		data.val = NULL;
		result = cJSON_GetObjectItemCaseSensitive(json, "result");

		cJSON_ArrayForEach(event, result) {

			cJSON *update_id = cJSON_GetObjectItemCaseSensitive(event, "update_id");

			if (latest_update_id < update_id->valueint) {
				latest_update_id = update_id->valueint;
				pthread_create(&threads[thread_pos], NULL, teabot_thread, event);
				thread_pos++;
				busy_thread++;
				if (thread_pos >= threads_amount) {
					thread_pos = 0;
				}
			}

			while (busy_thread >= threads_amount) {
				verbose_log("Waiting for busy thread...");
				for (uint8_t i = 0; i < threads_amount; ++i) {
					if (pthread_kill(threads[i], 0) != 0) {
						pthread_detach(threads[i]);
						busy_thread--;
					}
				}
				sleep(1);
			}

	    };
	    cJSON_Delete(json);
	    json = NULL;
	    printf("Done!\n");
	}

	return latest_update_id;
}

void *teabot_thread(void *event) {

	cJSON *update_id = cJSON_GetObjectItemCaseSensitive((cJSON *)event, "update_id");
	printf("Update ID: %d\n", update_id->valueint);
	printf("Sleep: %d seconds\n", update_id->valueint - 598146290);
	fflush(stdout);

	sleep(update_id->valueint - 598146290);

	printf("Thread is finished!\n");
	fflush(stdout);

	return NULL;
}
