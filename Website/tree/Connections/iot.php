<?php
# FileName="Connection_php_mysql.htm"
# Type="MYSQL"
# HTTP="true"

$hostname_iot = "localhost";
$database_iot = "iot";
$username_iot = "iot";
$password_iot = "iot1234";
$iot = mysql_pconnect($hostname_iot, $username_iot, $password_iot) or trigger_error(mysql_error(),E_USER_ERROR); 
		mysql_query("SET NAMES UTF8");
		session_start();
?>