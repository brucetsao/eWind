<html>
  <head>
    <script type="text/javascript" src="https://www.gstatic.com/charts/loader.js"></script>
    <script type="text/javascript">
      google.charts.load('current', {'packages':['corechart']});
      google.charts.setOnLoadCallback(drawChart);

      function drawChart() {
        var data = google.visualization.arrayToDataTable([
         ['DateTime', 'Red', 'Green','Blue'],

      ['2018-09-28 06:32:41',96,183,176],['2018-09-28 06:32:37',98,183,176],['2018-09-28 06:32:33',97,183,176],['2018-09-28 06:32:29',97,183,176],['2018-09-28 06:32:25',97,183,176],['2018-09-28 06:32:21',97,183,176],['2018-09-28 06:32:17',97,183,176],['2018-09-28 06:32:13',97,183,176],['2018-09-28 06:32:09',97,183,176],['2018-09-28 06:32:06',97,183,176],['2018-09-28 06:32:02',97,183,176],['2018-09-28 06:31:58',97,183,176],['2018-09-28 06:31:54',97,183,176],['2018-09-28 06:31:50',97,183,176],['2018-09-28 06:31:46',97,183,176],['2018-09-28 06:31:42',97,183,176],['2018-09-28 06:31:38',97,183,176],['2018-09-28 06:31:35',97,183,176],['2018-09-28 06:31:31',97,183,176],['2018-09-28 06:31:27',97,183,176],
      ]);


        var options = {
          title: '111111',
          hAxis: {title: 'DateTime',  titleTextStyle: {color: '#333'}},
          vAxis: {title: 'Value' , minValue: 0} ,
          series: {
            0: { color: '#ff0000' },
            1: { color: '#00ff00' },
            2: { color: '#0000ff' },
          }		  
        };

        var chart = new google.visualization.AreaChart(document.getElementById('chart_div'));
        chart.draw(data, options);
      }
    </script>
  </head>
  <body>
    <div id="chart_div" style="width: 100%; height: 500px;"></div>
  </body>
</html>