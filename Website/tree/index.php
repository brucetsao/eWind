<?php
$systime = array();
$temp = array();
$humid = array();

//SELECT * FROM  (SELECT * FROM `wind` WHERE 1 order by sysdatetime desc limit 0,60) as tt  order by sysdatetime asc
   	include("Connections/iotcnn.php");		//使用資料庫的呼叫程式
		//	Connection() ;
	$link=Connection();
	$result1=mysql_query("SELECT *  FROM `wind` WHERE 1 order by sysdatetime desc LIMIT 0,1",$link);
	$result2=mysql_query("SELECT * FROM  (SELECT * FROM `wind` WHERE 1 order by sysdatetime desc limit 0,50) as tt  order by sysdatetime asc",$link);

	
  if($result1!==FALSE){
	 while($row = mysql_fetch_array($result1)) {
		$deg =  $row["way"]  ;
		$windspeed = $row["speed"] ;
	 }
	 mysql_free_result($result1);
	 mysql_close();
  }

  if($result2!==FALSE){
	 while($row = mysql_fetch_array($result2)) {
			array_push($systime, $row["sysdatetime"]);
			array_push($temp, $row["temp"]);
			array_push($humid, $row["humid"]);
			
	 }
	 mysql_free_result($result2);
	 mysql_close();
  }


?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="X-UA-Compatible" content="IE=edge">
<meta http-equiv="refresh" content="20">
<meta http-equiv="Content-Type" content="text/html; charset="UTF-8" />
<!-- 告訴 Google 不要再搜索框裡面顯示網站鏈接-->
<meta name="google" content="nositelinkssearchbox">

<!-- 告訴 Google 不要翻譯這個頁面 -->
<meta name="google" content="notranslate">

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
<script src="/code/highcharts.js"></script>
<script src="/code/highcharts-more.js"></script>
<script src="/code/modules/exporting.js"></script>
<script src="/code/modules/export-data.js"></script>
<script src="/code/modules/exporting.js"></script>
<script src="/code/modules/export-data.js"></script>

</head>
<body>
   <table border="1" cellspacing="1" cellpadding="1">
		<tr>
			<td>
            <div align="center">吳厝國小 目前風速
			  </div>
              <div id="compass">
				  <div id="arrow">
                  <div id="arrowhead"></div></div>
			</div>
			</td>
			<td>
            <div id="container" style="min-width: 400px; max-width: 400px; height: 400px; margin: 0 auto"></div>
            </td>
		</tr>
		<tr>
			<td colspan="2">
            
            
            <div id="container1" style="min-width: 800px; height: 600px; margin: 0 auto"></div>
            
            
            
            
            </td>
		</tr>



   </table>
   
   
   <div>
     <h2>蒲福風級表</h2>
     <p>　　下表為台灣中央氣象局現行的風級標準，中央氣象局採用的標準等同於1946年由WMO所公布的國際標準，陸地與海面情形就是辛普森及彼得森所做的對照描述。(資料來源：<a href="http://typhoon.ws/learn/reference/beaufort_scale">http://typhoon.ws/learn/reference/beaufort_scale</a>，<a href="https://www.cwb.gov.tw/V7/knowledge/encyclopedia/ty023.htm">https://www.cwb.gov.tw/V7/knowledge/encyclopedia/ty023.htm</a></p>
     <p>&nbsp;</p>
   </div>
   <table>
     <tbody>
       <tr>
         <td rowspan="3">級數</td>
         <td colspan="5">國際標準 (由WMO公布)</td>
         <td colspan="2">香港標準</td>
         <td colspan="3">風浪對照</td>
         <td rowspan="3">陸地情形；<br />
           海面情形</td>
       </tr>
       <tr>
         <td rowspan="2">名稱</td>
         <td colspan="4">風速</td>
         <td rowspan="2">名稱</td>
         <td>風速</td>
         <td rowspan="2">名稱</td>
         <td>一般</td>
         <td>最大</td>
       </tr>
       <tr>
         <td>m/s</td>
         <td>km/h</td>
         <td>knot</td>
         <td>mph</td>
         <td>km/h</td>
         <td colspan="2">m</td>
       </tr>
       <tr>
         <td>0</td>
         <td>無風<br />
           Calm</td>
         <td>0 - 0.2</td>
         <td>&lt; 1</td>
         <td>&lt; 1</td>
         <td>&lt; 1</td>
         <td>無風</td>
         <td>&lt; 2</td>
         <td>－</td>
         <td>－</td>
         <td>－</td>
         <td>靜，煙直上；<br />
           海面如鏡。</td>
       </tr>
       <tr>
         <td>1</td>
         <td>軟風<br />
           Light air</td>
         <td>0.3 - 1.5</td>
         <td>1 - 5</td>
         <td>1 - 3</td>
         <td>1 - 3</td>
         <td rowspan="2">輕微</td>
         <td>2 - 6</td>
         <td rowspan="2">微波</td>
         <td>0.1</td>
         <td>0.1</td>
         <td>炊煙可表示風向，風標不動；<br />
           海面有鱗狀波紋，波峰無泡沫。</td>
       </tr>
       <tr>
         <td>2</td>
         <td>輕風<br />
           Light breeze</td>
         <td>1.6 - 3.3</td>
         <td>6 - 11</td>
         <td>4 - 6</td>
         <td>4 - 7</td>
         <td>7 - 12</td>
         <td>0.2</td>
         <td>0.3</td>
         <td>風拂面，樹葉有聲，普通風標轉動；<br />
           微波明顯，波峰光滑未破裂。</td>
       </tr>
       <tr>
         <td>3</td>
         <td>微風<br />
           Gentle breeze</td>
         <td>3.4 - 5.4</td>
         <td>12 - 19</td>
         <td>7 - 10</td>
         <td>8 - 12</td>
         <td rowspan="2">和緩</td>
         <td>13 - 19</td>
         <td>小波</td>
         <td>0.6</td>
         <td>1.0</td>
         <td>樹葉及小枝搖動，旌旗招展；<br />
           小波，波峰開始破裂，泡沫如珠，波峰偶泛白沫。</td>
       </tr>
       <tr>
         <td>4</td>
         <td>和風<br />
           Moderate breeze</td>
         <td>5.5 - 7.9</td>
         <td>20 - 28</td>
         <td>11 - 16</td>
         <td>13 - 18</td>
         <td>20 - 30</td>
         <td>小浪</td>
         <td>1.0</td>
         <td>1.5</td>
         <td>塵沙飛揚，紙片飛舞，小樹幹搖動；<br />
           小波漸高，波峰白沫漸多。</td>
       </tr>
       <tr>
         <td>5</td>
         <td>清風<br />
           Fresh breeze</td>
         <td>8.0 - 10.7</td>
         <td>29 - 38</td>
         <td>17 - 21</td>
         <td>19 - 24</td>
         <td>清勁</td>
         <td>31 - 40</td>
         <td>中浪</td>
         <td>2.0</td>
         <td>2.5</td>
         <td>有葉之小樹搖擺，內陸水面有小波；<br />
           中浪漸高，波峰泛白沫，偶起浪花。</td>
       </tr>
       <tr>
         <td>6</td>
         <td>強風<br />
           Strong breeze</td>
         <td>10.8 - 13.8</td>
         <td>39 - 49</td>
         <td>22 - 27</td>
         <td>25 - 31</td>
         <td rowspan="2">強風</td>
         <td>41 - 51</td>
         <td rowspan="2">大浪</td>
         <td>3.0</td>
         <td>4.0</td>
         <td>大樹枝搖動，電線呼呼有聲，舉傘困難；<br />
           大浪形成，白沫範圍增大，漸起浪花。</td>
       </tr>
       <tr>
         <td>7</td>
         <td>疾風<br />
           Near gale</td>
         <td>13.9 - 17.1</td>
         <td>50 - 61</td>
         <td>28 - 33</td>
         <td>32 - 38</td>
         <td>52 - 62</td>
         <td>4.0</td>
         <td>5.5</td>
         <td>全樹搖動，迎風步行有阻力；<br />
           海面湧突，浪花白沫沿風成條吹起。</td>
       </tr>
       <tr>
         <td>8</td>
         <td>大風<br />
           Gale</td>
         <td>17.2 - 20.7</td>
         <td>62 - 74</td>
         <td>34 - 40</td>
         <td>39 - 46</td>
         <td rowspan="2">烈風</td>
         <td>63 - 75</td>
         <td>巨浪</td>
         <td>6.0</td>
         <td>7.5</td>
         <td>小枝吹折，逆風前進困難；<br />
           巨浪漸升，波峰破裂，浪花明顯成條沿風吹起。</td>
       </tr>
       <tr>
         <td>9</td>
         <td>烈風<br />
           Strong gale</td>
         <td>20.8 - 24.4</td>
         <td>75 - 88</td>
         <td>41 - 47</td>
         <td>47 - 54</td>
         <td>76 - 87</td>
         <td>猛浪</td>
         <td>7.0</td>
         <td>10.0</td>
         <td>煙突屋瓦等將被吹損；<br />
           猛浪驚濤，海面漸呈汹湧，浪花白沫增濃，<br />
           減低能見度。</td>
       </tr>
       <tr>
         <td>10</td>
         <td>暴風<br />
           Storm</td>
         <td>24.5 - 28.4</td>
         <td>89 - 102</td>
         <td>48 - 55</td>
         <td>55 - 63</td>
         <td rowspan="2">暴風</td>
         <td>88 - 103</td>
         <td rowspan="8">狂濤</td>
         <td>9.0</td>
         <td>12.5</td>
         <td>陸上不常見，見則拔樹倒屋或有其他損毀；<br />
           猛浪翻騰波峰高聳，浪花白沫堆集，<br />
           海面一片白浪，能見度減低。</td>
       </tr>
       <tr>
         <td>11</td>
         <td>狂風<br />
           Violent storm</td>
         <td>28.5 - 32.6</td>
         <td>103 - 117</td>
         <td>56 - 63</td>
         <td>64 - 72</td>
         <td>104 - 117</td>
         <td>11.5</td>
         <td>16.0</td>
         <td>陸上絕少，有則必有重大災害；<br />
           狂濤高可掩蔽中小海輪，海面全為白浪<br />
           掩蓋，能見度大減。</td>
       </tr>
       <tr>
         <td>12</td>
         <td>颶風<br />
           Hurricane</td>
         <td>32.7 - 36.9</td>
         <td>118 - 133</td>
         <td>64 - 71</td>
         <td>73 - 82</td>
         <td>颶風</td>
         <td>118 - 135</td>
         <td>14.0</td>
         <td>－</td>
         <td>－<br />
           空中充滿浪花白沫，能見度惡劣。</td>
       </tr>
       <tr>
         <td>13</td>
         <td>－</td>
         <td>37.0 - 41.4</td>
         <td>134 - 149</td>
         <td>72 - 80</td>
         <td>83 - 92</td>
         <td> </td>
         <td> </td>
         <td>－</td>
         <td>－</td>
         <td>－</td>
       </tr>
       <tr>
         <td>14</td>
         <td>－</td>
         <td>41.5 - 46.1</td>
         <td>150 - 166</td>
         <td>81 - 89</td>
         <td>93 - 103</td>
         <td> </td>
         <td> </td>
         <td>－</td>
         <td>－</td>
         <td>－</td>
       </tr>
       <tr>
         <td>15</td>
         <td>－</td>
         <td>46.2 - 50.9</td>
         <td>167 - 183</td>
         <td>90 - 99</td>
         <td>104 - 114</td>
         <td> </td>
         <td> </td>
         <td>－</td>
         <td>－</td>
         <td>－</td>
       </tr>
       <tr>
         <td>16</td>
         <td>－</td>
         <td>51.0 - 56.0</td>
         <td>184 - 201</td>
         <td>100 - 108</td>
         <td>115 - 125</td>
         <td> </td>
         <td> </td>
         <td>－</td>
         <td>－</td>
         <td>－</td>
       </tr>
       <tr>
         <td>17</td>
         <td>－</td>
         <td>56.1 - 61.2</td>
         <td>202 - 220</td>
         <td>109 - 118</td>
         <td>126 - 136</td>
         <td> </td>
         <td> </td>
         <td>－</td>
         <td>－</td>
         <td>－</td>
       </tr>
     </tbody>
   </table>
   <p>註：香港天文台公布的標準中，「節」(knot)亦同於國際標準，不同處在於「節」(knot)換算成「公里／每小時」(km/h)取不同的約略值。</p>
<script type="text/javascript">


Highcharts.chart('container', {

    chart: {
        type: 'gauge',
        plotBackgroundColor: null,
        plotBackgroundImage: null,
        plotBorderWidth: 0,
        plotShadow: false
    },

    title: {
        text: '吳厝國小 目前風速'
    },

    pane: {
        startAngle: -150,
        endAngle: 150,
        background: [{
            backgroundColor: {
                linearGradient: { x1: 0, y1: 0, x2: 0, y2: 1 },
                stops: [
                    [0, '#FFF'],
                    [1, '#333']
                ]
            },
            borderWidth: 0,
            outerRadius: '109%'
        }, {
            backgroundColor: {
                linearGradient: { x1: 0, y1: 0, x2: 0, y2: 1 },
                stops: [
                    [0, '#333'],
                    [1, '#FFF']
                ]
            },
            borderWidth: 1,
            outerRadius: '107%'
        }, {
            // default background
        }, {
            backgroundColor: '#DDD',
            borderWidth: 0,
            outerRadius: '105%',
            innerRadius: '103%'
        }]
    },

    // the value axis
    yAxis: {
        min: 0,
        max: 70,

        minorTickInterval: 'auto',
        minorTickWidth: 1,
        minorTickLength: 10,
        minorTickPosition: 'inside',
        minorTickColor: '#666',

        tickPixelInterval: 30,
        tickWidth: 2,
        tickPosition: 'inside',
        tickLength: 10,
        tickColor: '#666',
        labels: {
            step: 2,
            rotation: 'auto'
        },
        title: {
            text: 'm/s'
        },
        plotBands: [{
            from: 0,
            to: 10.7,
            color: '#55BF3B' // green
        }, {
            from: 10.8,
            to: 20.7,
            color: '#DDDF0D' // yellow
        }, {
            from: 20.8,
            to: 70,
            color: '#DF5353' // red
        }]
    },

    series: [{
        name: 'Speed',
        data: [<?php echo $windspeed ?>],
        tooltip: {
            valueSuffix: ' m/s'
        }
    }]

},
// Add some life
function (chart) {
    if (!chart.renderer.forExport) 
	{
        setInterval(function () 
		{
            var point = chart.series[0].points[0],
                newVal,
                inc = Math.round((Math.random() - 0.5) * 5);

            newVal = point.y + inc;
            if (newVal < 0 || newVal > 70) {
                newVal = point.y - inc;
            }

            point.update($windspeed);

        }, 3000);
    }
});

Highcharts.chart('container1', {
    chart: {
        zoomType: 'xy'
    },
    title: {
        text: '吳厝國小校園溫濕度'
    },
    subtitle: {
        text: '吳厝國小氣象偵測站'
    },
    xAxis: [{
        categories: [
		<?php
		for($i=0;$i < count($systime);$i=$i+1)
		{
			echo "'";
						echo $systime[$i];
			echo "',";
		}
			?>],
        crosshair: true
    }],
    yAxis: [{ // Primary yAxis
        labels: {
            format: '{value}°C',
            style: {
                color: Highcharts.getOptions().colors[1]
            }
        },
        title: {
            text: '溫度',
            style: {
                color: Highcharts.getOptions().colors[1]
            }
        }
    }, { // Secondary yAxis
        title: {
            text: '濕度',
            style: {
                color: Highcharts.getOptions().colors[0]
            }
        },
        labels: {
            format: '{value} %',
            style: {
                color: Highcharts.getOptions().colors[0]
            }
        },
        opposite: true
    }],
    tooltip: {
        shared: true
    },
    legend: {
        layout: 'vertical',
        align: 'left',
        x: 120,
        verticalAlign: 'top',
        y: 100,
        floating: true,
        backgroundColor: (Highcharts.theme && Highcharts.theme.legendBackgroundColor) || 'rgba(255,255,255,0.25)'
    },
    series: [{
        name: '濕度',
        type: 'column',
        yAxis: 1,
        data: [
		<?php
		for($i=0;$i < count($humid);$i=$i+1)
		{
			echo $humid[$i];
			echo ",";
		}
			?>],
        tooltip: {
            valueSuffix: ' mm'
        }

    }, {
        name: '溫度',
        type: 'spline',
        data: [
		<?php
		for($i=0;$i < count($temp);$i=$i+1)
		{
			echo $temp[$i];
			echo ",";
		}
			?>],
        tooltip: {
            valueSuffix: '°C'
        }
    }]
});
		</script>
</body>
</html>
