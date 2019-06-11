
#include <stdio.h>
#include <teabot/network.h>
#include <teabot/config/config_parser.h>

int main(int argc, char *argv[], char *envp[]) {

	if (init_config(argc, argv, envp)) {
		int latest_update_id = 0;
		while (1) {
			latest_update_id = run_teabot(latest_update_id);
		}
	}
	
	return 0;
}
