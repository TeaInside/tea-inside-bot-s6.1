
#ifndef teabot__teabot_h
#define teabot__teabot_h

#include <stdint.h>
#include <stdbool.h>
#include <cjson/cJSON.h>

void run_daemon();
void *thread_worker(void *);

typedef struct _daemon_thread {
	uint8_t id;
	int fd[2];
} daemon_thread;


#endif
