
#ifndef teabot__teabot_h
#define teabot__teabot_h

#include <stdint.h>
#include <stdbool.h>
#include <cjson/cJSON.h>

void run_daemon();
void *thread_worker(void *__update_string);

typedef struct _daemon_thread {
	uint8_t id;
	int fd[2];
} daemon_thread;

typedef struct _in {
	cJSON *main;
	int *update_id;
	cJSON *message;
	int *message_id;
	cJSON *from;
	cJSON *chat;
	cJSON *reply_to_message;
	int *date;
	char *text;
} update_data;

#endif
