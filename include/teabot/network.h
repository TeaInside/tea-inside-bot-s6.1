
#ifndef teabot__network
#define teabot__network

char *send_message();
int run_teabot(int latest_update_id);
void *teabot_thread(void *);

#endif
