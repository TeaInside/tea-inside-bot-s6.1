
#include <stdio.h>
#include <teabot/network.h>
#include <teabot/config/config_parser.h>

int main(int argc, char *argv[], char *envp[]) {

	if (init_config(argc, argv, envp)) {
		run_teabot();
	}
	
	return 0;
}
