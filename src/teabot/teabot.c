
#include <poll.h>
#include <unistd.h>
#include <signal.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/types.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>
#include <teabot/teabot.h>
#include <teabot/network.h>
#include <teabot/helpers.h>

typedef struct _update_cycle {
	cJSON **target;
	uint64_t cycle;
} update_cycle;

extern uint8_t threads_amount;
cJSON *thread_jobs[255];
bool busy_threads[255];
uint8_t cycle_states_ptr = 0;
update_cycle cycle_states[255];

static char *get_update() {
	CURL *curl;
	CURLcode res;
	char *url;
	curl_data data;

	data.val = (char *)malloc(1);
	data.len = 0;

	BOT_API_URL(url, "getUpdates");

	verbose_log("Getting updates...");

	curl = curl_easy_init();
	if(curl) {
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

static ssize_t wait_n_read(int fd, void *buf, size_t szBuf) {
    struct pollfd pfd = {
        .fd      = fd,
        .events  = POLLIN,
        .revents = 0
    };
    poll(&pfd, 1, -1);
    ssize_t ret = read(fd, buf, szBuf);
    if (ret == -1) {
        warning_log("Error: reading pipe");
    }
    return ret;
}

void run_daemon() {

	bool dispatched;
	cJSON *update_body, *updates, *update;
	char *update_string;
	uint64_t cycle = 0;
	pthread_t tid[threads_amount];
	int update_id, latest_update_id = 0;
	daemon_thread threads[threads_amount];	
	pthread_attr_t attr;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	for (int i = 0; i < 255; ++i) {
		busy_threads[i] = false;
	}

	for (uint8_t i = 0; i < threads_amount; i++) {
		threads[i].id = i;
		if (pipe(threads[i].fd) == -1) {
			warning_log("Error: Cannot create pipe in thread %d", i);
		} else {
			pthread_create(&(tid[i]), &attr, teabot_thread, &(threads[i]));
		}
	}

	_daemon_update:

	cycle++;

	if ((cycle % 500) == 0) {
		verbose_log("Sleeping for 30 seconds...");
		sleep(30);
	}

	update_string = get_update();
	update_body = cJSON_Parse(update_string);

	cycle_states[cycle_states_ptr].target = &update_body;
	cycle_states[cycle_states_ptr].cycle = cycle;

	updates = cJSON_GetObjectItemCaseSensitive(update_body, "result");

	cJSON_ArrayForEach(update, updates) {

		update_id = cJSON_GetObjectItemCaseSensitive(update, "update_id")->valueint;

		if (update_id > latest_update_id) {
			dispatched = false;
			_start_dispatch:

			for (uint8_t i = 0; i < threads_amount; ++i) {
				if (!busy_threads[i]) {
					thread_jobs[i] = update;
					latest_update_id = update_id;
					write(threads[i].fd[1], "\0\0\0\0", 4);
					dispatched = true;
					break;
				}
			}

			if (!dispatched) {
				verbose_log("All threads are busy, waiting for the next queue...");
				sleep(1);
				goto _start_dispatch;
			}
		}
	}

	cycle_states_ptr++;

	free(update_string);
	update_string = NULL;

	goto _daemon_update;
}

void *teabot_thread(void *fd) {

	char buff[5];
	#define thread ((daemon_thread *)fd)

	_start_thread:

	if (wait_n_read(thread->fd[0], buff, 4) != -1) {
		busy_threads[thread->id] = true;
		
		verbose_log("Thread %d is working...", thread->id);

		verbose_log(
			"Update ID: %d\n",
			cJSON_GetObjectItemCaseSensitive(thread_jobs[thread->id], "update_id")->valueint
		);

		sleep(10);
		#include "responses.h"

		busy_threads[thread->id] = false;
		verbose_log("Thread %d has finished its job.\n", thread->id);
	} else {
		warning_log("Thread %d wait_n_read error!", thread->id);
	}

	goto _start_thread;

	return NULL;

	#undef thread
}
