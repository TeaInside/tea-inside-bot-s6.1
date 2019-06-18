
const char *msg;
char *body, *res;

if (!strcmp(chat_type->valuestring, "private")) {
	msg = <?php echo rd(urlencode("Send /help to see the command list.")); ?>;
} else {
	msg = <?php echo rd(urlencode("/start command can only be used in private!")); ?>;
}

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
