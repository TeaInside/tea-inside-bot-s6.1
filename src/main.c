
#include <stdio.h>
#include <teabot/teabot.h>
#include <teabot/config/config_parser.h>

int main(int argc, char *argv[], char *envp[]) {

	if (init_config(argc, argv, envp)) {
		run_daemon();
	}
	
	return 0;
}
