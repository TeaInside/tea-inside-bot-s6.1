 
const char *msg;
char *body, *res;

if (!strcmp(chat_type->valuestring, "private")) {
	msg = "%3Cb%3EGlobal+Commands%3A%3C%2Fb%3E%0A%2Fsh+%3Ccode%3E%5Bcommand+arg...%5D+Execute+linux+shell+command.%3C%2Fcode%3E%0A%2Ftr+%3Ccode%3E%26lt%3Bfrom%26gt%3B+%26lt%3Bto%26gt%3B+%26lt%3Bstring%26gt%3B%3C%2Fcode%3E";

} else {
	msg = "%2Fhelp+command+can+only+be+used+in+private%21";
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

// printf("%s\n", body);
res = send_message(body);
// printf("res: %s\n", res);
// fflush(stdout);

free(body);
free(res);
res = body = NULL;

return;
