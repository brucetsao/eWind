<?php
//＝＝＝＝　寫入資料範例一　＝＝＝＝＝
$fp = fopen("./tmp/mosdata.csv",'w');//建檔

fwrite($fp,'寫入的資料');//寫入資料到 $fp 所開啟的檔案內

fclose($fp);//關閉開啟的檔案


//＝＝＝＝　寫入檔案範例二　建立HTML 頁面＝＝＝＝＝
$html = 'test.html';//要建立的頁面名稱
$file = fopen($html,'w');//建檔

//寫入資料
fwrite($file,'<!doctype html>'."\n");
fwrite($file,'<html>'."\n");
fwrite($file,'<head>'."\n");
fwrite($file,'<meta charset="utf-8">'."\n");
fwrite($file,'<title>程式建立的 HTML 檔案</title>'."\n");
fwrite($file,'</head>'."\n");
fwrite($file,'<body>'."\n");
fwrite($file,'這是以 php fopen 建立的 HTML 頁面'."\n");
fwrite($file,'</body>'."\n");
fwrite($file,'</html>'."\n");

//關檔
fclose($file);

//判斷檔案是否存在
if(is_file($html)){
	echo '頁面已建立 立即 <a href="'.$html.'">進入查看</a>';
}