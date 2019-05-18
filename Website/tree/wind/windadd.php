<?php
   	include("../Connections/iotcnn.php");		//使用資料庫的呼叫程式
		//	Connection() ;
   	
   	$link=Connection();		//產生mySQL連線物件

	$temp0=$_GET["mac"];		//取得POST參數 : humidity
	$temp1=$_GET["ip"];		//取得POST參數 : humidity
	$temp2=$_GET["speed"];		//取得POST參數 : humidity
	$temp3=$_GET["way"];		//取得POST參數 : temperature
	$temp4=$_GET["waydir"];		//取得POST參數 : temperature
	$temp5=$_GET["temp"];		//取得POST參數 : temperature
	$temp6=$_GET["humid"];		//取得POST參數 : temperature



//	$query = "INSERT INTO `dhtdata` (`humidity`,`temperature`) VALUES ('".$temp1."','".$temp2."')"; 
	$query = "INSERT INTO `wind` (`mac`,`ip`,`speed`,`way`,`waydir`,`temp`,`humid`) VALUES ('".$temp0."','".$temp1."',".$temp2.",".$temp3.",".$temp4.",".$temp5.",".$temp6.")"; 
	//組成新增到dhtdata資料表的SQL語法
	echo $query ;
	


	if (mysql_query($query,$link))
		{
				echo "Successful <br>" ;
		}
		else
		{
				echo "Fail <br>" ;
		}
		
			;			//執行SQL語法
	echo "<br>" ;
	mysql_close($link);		//關閉Query
	   	echo $query ;

 
?>
