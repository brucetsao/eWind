<?php

		$server="localhost";
		$user="iot";
		$pass="iot1234";
		$db="iot";
	   	
		$link = mysql_pconnect($server, $user, $pass);

		if (!$link) {
	    	die('MySQL ERROR: ' . mysql_error());
		}
		
		mysql_select_db($db) or die( 'MySQL ERROR: '. mysql_error() );


?>
