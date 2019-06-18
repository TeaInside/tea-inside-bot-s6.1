
#ifndef teabot__lang__start_h
#define teabot__lang__start_h

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
$lang["en"]["data"]["start_group"] = "<code>/start</code> command can only be used in private message!";
$lang["en"]["data"]["start_private"] = "Send /start to show the commands list!";



// Indonesia
$lang["id"]["data"]["start_group"] = "Command <code>/start</code> hanya bisa dipakai di private message!";
$lang["id"]["data"]["start_private"] = "Kirim /start untuk menampilkan daftar perintah!";


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
