
#include <curl/curl.h>
#include <teabot/helpers.h>
#include <teabot/telegram_api.h>

char *api_curl_post(char *url, char *body) {
	CURL *curl;
	CURLcode res;
	curl_data data;

	data.val = (char *)malloc(1);
	data.len = 0;

	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
		curl_easy_setopt(curl, CURLOPT_POST, true);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body);
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
	return data.val;

nullret:
	free(data.val);
	return NULL;
}

char *send_message(char *body) {
	char *url, *ret;
	BOT_API_URL(url, "sendMessage");
	ret = api_curl_post(url, body);
	free(url);
	return ret; 
}
