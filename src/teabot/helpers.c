
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

char *ltrim(char *str, const char *seps)
{
    size_t totrim;
    if (seps == NULL) {
        seps = "\t\n\v\f\r ";
    }
    totrim = strspn(str, seps);
    if (totrim > 0) {
        size_t len = strlen(str);
        if (totrim == len) {
            str[0] = '\0';
        }
        else {
            memmove(str, str + totrim, len + 1 - totrim);
        }
    }
    return str;
}

char *rtrim(char *str, const char *seps)
{
    size_t i = strlen(str) - 1;

    if (seps == NULL) {
        seps = "\t\n\v\f\r ";
    }

    while (i >= 0 && strchr(seps, str[i]) != NULL) {
        str[i] = '\0';
        i--;
    }
    return str;
}

char *trim(char *str, const char *seps)
{
    return ltrim(rtrim(str, seps), seps);
}
