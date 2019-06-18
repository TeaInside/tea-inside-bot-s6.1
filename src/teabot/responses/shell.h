
cJSON *user_id = NULL;
FILE *fp;
bool is_sudo = false;
char path[2048], *output, *msg, *body, *res;
const char *stop_msg;
register size_t curloc = 2048;
register size_t cursize = 0;

DETSE("id", data->from, user_id)

if (user_id == NULL) {
	stop_msg = LANG_CHOICE(shell_error, lang);
	goto shell_noexec;
}

for (uint8_t i = 0; i < sudoers_count; i++) {
	if (user_id->valueint == sudoers[i]) {
		is_sudo = true;
		break;
	}
}

if (!is_sudo) {
	stop_msg = LANG_CHOICE(shell_sudo_reject, lang);
	goto shell_noexec;
}

fp = popen(cmd, "r");
free(cmd);
output = (char *)malloc(2048 + 1);
if (fp == NULL) {
	warning_log("Failed to execute command!");
	return;
}
while (fgets(path, sizeof(path) - 1, fp) != NULL) {
	len = strlen(path);
	memcpy(output + cursize, path, len);
	cursize += len;
	if ((cursize + 1024) > curloc) {
		curloc += 2048;
		output = (char *)realloc(output, curloc);
	}
}
pclose(fp);
msg = (char *)urlencode(output);
free(output);
output = NULL;


// Normal execution.
#define format \
	"chat_id=%.0lf&" \
	"reply_to_message_id=%d&" \
	"text=%s"

body = (char *)malloc(sizeof(format) + strlen(msg) + 72);
sprintf(body, format, 
	chat_id->valuedouble,
	data->message_id,
	msg
);
free(msg);
msg = NULL;
res = send_message(body);
free(body);
free(res);
res = body = NULL;
return;


// Shell error or sudo reject.
shell_noexec:

body = (char *)malloc(sizeof(format) + strlen(stop_msg) + 72);
sprintf(body, format, 
	chat_id->valuedouble,
	data->message_id,
	stop_msg
);
#undef format
res = send_message(body);
free(body);
free(res);
res = body = NULL;
return;
