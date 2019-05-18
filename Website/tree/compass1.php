
<?php

$deg=$_GET["deg"];

?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>無標題文件</title>
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
  -webkit-transform:rotate(50deg);
  -moz-transform:rotate(50deg);
  -o-transform:rotate(50deg);
  -ms-transform:rotate(50deg);

 // -moz-transition: all 1s ease;
 // -webkit-transition: all 1s ease;
 // -o-transition: all 1s ease;
 // transition: all 1s ease;
}

#compass:#arrow {
  -webkit-transform:rotate(0deg);
  -moz-transform:rotate(0deg);
  -o-transform:rotate(0deg);
  -ms-transform:rotate(0deg);
//}​
</style>
</head>

<body>
<div id="compass">
  <div id="arrow"></div>
</div>
</body>
</html>