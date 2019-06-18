 
const char *msg;
char *body, *res;

if (!strcmp(chat_type->valuestring, "private")) {
	msg = LANG_CHOICE(help_private, lang);
} else {
	msg = LANG_CHOICE(help_private, lang);
}

#define format \
	"chat_id=%.0lf&" \
	"reply_to_message_id=%d&" \
	"text=%s&" \
	"parse_mode=html"

body = (char *)malloc(sizeof(format) + strlen(msg) + 72);
sprintf(body, format, 
	chat_id->valuedouble,
	data->message_id,
	msg
);
#undef format
res = send_message(body);
free(body);
free(res);
res = body = NULL;
return;
