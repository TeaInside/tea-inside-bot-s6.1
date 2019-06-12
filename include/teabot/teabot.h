
#ifndef teabot__teabot_h
#define teabot__teabot_h

#include <stdint.h>

void run_daemon();
void *teabot_thread(void *fd);

typedef struct _daemon_thread {
	uint8_t id;
	int fd[2];
} daemon_thread;

#endif
