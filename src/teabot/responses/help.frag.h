 
const char *msg;
char *body, *res;

if (!strcmp(chat_type->valuestring, "private")) {
	msg = <?php 
		$a  = "<b>Global Commands:</b>\n";
		$a .= "/sh <code>".htmlspecialchars("[command arg...] Execute linux shell command.")."</code>\n";
		$a .= "/tr <code>".htmlspecialchars("<from> <to> <string>")."</code>";
		echo rd(urlencode($a));
		unset($a);
	?>;

} else {
	msg = <?php echo rd(urlencode("/help command can only be used in private!")); ?>;
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
