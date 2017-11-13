<?php
	if(isset($_GET['mode'])) {
		if($_GET['mode'] == 'getmessage') {
			$myfile = fopen("message", "r") or die("Unable to open file!");
			echo fread($myfile,filesize("message"));
		}
	} else {

?>
<html>
	<style>
		p,input {
			font-size: 4.5vh;
		}
	</style>
	<body>
		<p>Current message: 
		<?php 
			$myfile = fopen("message", "r") or die("Unable to open file!");
			echo fread($myfile,filesize("message"));
		?>
		<form action="/updateMessage.php" method="get">
			<br>
			<input type="text" name="message">
			<br>
			<input type="submit" value="submit">
		</form>
	</body>
</html>
<?php
	}
	fclose($myfile);


?>
