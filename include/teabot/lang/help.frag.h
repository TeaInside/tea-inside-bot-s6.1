
#ifndef teabot__lang__help_h
#define teabot__lang__help_h

<?php

$lang = [
	"en" => [
		"comment" => "English",
		"data" => []
	],
	"id" => [
		"comment" => "Indonesia",
		"data" => []
	]
];

// English
$lang["en"]["data"]["help_group"] = "<code>/help</code> command can only be used in private!";
$lang["en"]["data"]["help_private"] = "
<b>Global Commands:</b>
/sh <code>".htmlspecialchars("[command arg...]")."</code> Execute linux shell command.
/tr <code>".htmlspecialchars("<from> <to> <string>")."</code> Translate a string.";



// Indonesia
$lang["id"]["data"]["help_group"] = "Perintah <code>/help</code> hanya bisa digunakan di private message!";
$lang["id"]["data"]["help_private"] = "
<b>Perintah global:</b>
/sh <code>".htmlspecialchars("[command arg...]")."</code> Execute linux shell command.
/tr <code>".htmlspecialchars("<from> <to> <string>")."</code> Terjemahkan string";


foreach ($lang as $key => $v) {
	echo "// {$v['comment']}\n";
	foreach ($v["data"] as $kprop => $prop) {
		echo "const char *{$kprop}_{$key} = ".rd(urlencode(trim($prop))).";\n";
	}
	echo "\n\n";
}

unset($lang);
?>
#endif
