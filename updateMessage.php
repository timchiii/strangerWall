<?php
	$myfile = fopen("message", "w");
        $m = $_GET['message'];
		$m = strtolower($m);
		fwrite($myfile, $m);
	header("Location: /");
?>
