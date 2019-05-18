
<?php

$deg=$_GET["deg"];

?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>吳厝國小樹屋網站</title>
<style type="text/css">
#compass {
  width: 600px;
  height: 600px;
  background-image:url('images/compass6.jpg');
  position: relative;
}

#arrow {
  width: 10px;
  height: 380px;
  background-color:#F00;
  position: absolute;
  top: 110px;
  left: 295px;;
  -webkit-transform:rotate(<?php echo $deg?>deg);
  -moz-transform:rotate(<?php echo $deg?>deg);
  -o-transform:rotate(<?php echo $deg?>deg);
  -ms-transform:rotate(<?php echo $deg?>deg);
}
#arrowhead{
	width: 0;
	height: 0;
	position: absolute;
	background-color: transparent;
	top: -10px;
	left: -10px;
	border-style: solid;
	border-width: 0 15px 26.0px 15px;
	border-color: transparent transparent #007bff transparent;
}
</style>
</head>

<body>
<div id="compass">
  <div id="arrow"><div id="arrowhead"></div></div>
</div>
</body>
</html>