
#include <teabot/helpers.h>

size_t teabot_curl_write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
	size_t realsize = size * nmemb;
	char *ptr;

	curl_data *mem = (curl_data *)userp;

	ptr = (char *)realloc(mem->val, mem->len + realsize + 1);
	if(ptr == NULL) {
		/* out of memory! */ 
		printf("not enough memory (realloc returned NULL)\n");
		return 0;
	}

	mem->val = ptr;
	memcpy(&(mem->val[mem->len]), contents, realsize);
	mem->len += realsize;
	mem->val[mem->len] = 0;

	return realsize;
}
