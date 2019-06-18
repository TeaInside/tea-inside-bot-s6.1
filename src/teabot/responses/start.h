
char *msg, *body, *res;

if (!strcmp(chat_type->valuestring, "private")) {
	#define _text "Send /help to see the command list."
	msg = (char *)malloc(sizeof(_text) * 3);
	msg = (char *)urlencode(_text);
	#undef _text
} else {
	#define _text "/start command can only be used in private!"
	msg = (char *)malloc(sizeof(_text) * 3);
	msg = (char *)urlencode(_text);
	#undef _text
}

#define format \
	"chat_id=%.0lf&" \
	"reply_to_message_id=%d&" \
	"text=%s"

body = (char *)malloc(sizeof(format) + strlen(msg) + 72);
sprintf(
	body,
	format,
	chat_id->valuedouble,
	data->message_id,
	msg
);
free(msg);
msg = NULL;

#undef format

// printf("%s\n", body);
res = send_message(body);
// printf("res: %s\n", res);
// fflush(stdout);

free(body);
free(res);
res = body = NULL;

start_ret:
return;
