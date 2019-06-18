
#ifndef teabot__lang__shell_h
#define teabot__lang__shell_h

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
$lang["en"]["data"]["shell_sudo_reject"] = "You are not in the sudoers file, this incident will be reported!";
$lang["en"]["data"]["shell_error"] = "Couldn't spawn shell with this user.";


// Indonesia
$lang["id"]["data"]["shell_sudo_reject"] = "Anda tidak masuk dalam file sudoers, kejadian ini akan dilaporkan!";
$lang["id"]["data"]["shell_error"] = "Tidak dapat membuka shell dengan user ini.";


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
