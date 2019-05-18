<style>
.dropbtn {
    background-color: #3498DB;
    color: white;
    padding: 16px;
    font-size: 16px;
    border: none;
    cursor: pointer;
}

.dropbtn1 {
    background-color: #3498DB;
    color: white;
    padding: 16px;
    font-size: 16px;
    border: none;
    cursor: pointer;
}

.dropbtn2 {
    background-color: #3498DB;
    color: white;
    padding: 16px;
    font-size: 16px;
    border: none;
    cursor: pointer;
}

.dropbtn3 {
    background-color: #3498DB;
    color: white;
    padding: 16px;
    font-size: 16px;
    border: none;
    cursor: pointer;
}

.dropbtn:hover, .dropbtn:focus {
    background-color: #2980B9;
}

.dropbtn1:hover, .dropbtn1:focus {
    background-color: #2980B9;
}

.dropbtn2:hover, .dropbtn2:focus {
    background-color: #2980B9;
}

.dropbtn3:hover, .dropbtn3:focus {
    background-color: #2980B9;
}

.dropdown {
    position: relative;
    display: inline-block;
}

.dropdown1 {
    position: relative;
    display: inline-block;
}

.dropdown2 {
    position: relative;
    display: inline-block;
}

.dropdown3 {
    position: relative;
    display: inline-block;
}

.dropdown-content {
    display: none;
    position: absolute;
    background-color: #f1f1f1;
    min-width: 160px;
    overflow: auto;
    box-shadow: 0px 8px 16px 0px rgba(0,0,0,0.2);
    z-index: 1;
}

.dropdown-content1 {
    display: none;
    position: absolute;
    background-color: #f1f1f1;
    min-width: 160px;
    overflow: auto;
    box-shadow: 0px 8px 16px 0px rgba(0,0,0,0.2);
    z-index: 1;
}

.dropdown-content2 {
    display: none;
    position: absolute;
    background-color: #f1f1f1;
    min-width: 160px;
    overflow: auto;
    box-shadow: 0px 8px 16px 0px rgba(0,0,0,0.2);
    z-index: 1;
}

.dropdown-content3 {
    display: none;
    position: absolute;
    background-color: #f1f1f1;
    min-width: 160px;
    overflow: auto;
    box-shadow: 0px 8px 16px 0px rgba(0,0,0,0.2);
    z-index: 1;
}

.dropdown-content a {
    color: black;
    padding: 12px 16px;
    text-decoration: none;
    display: block;
}

.dropdown-content1 a {
    color: black;
    padding: 12px 16px;
    text-decoration: none;
    display: block;
}

.dropdown-content2 a {
    color: black;
    padding: 12px 16px;
    text-decoration: none;
    display: block;
}

.dropdown-content3 a {
    color: black;
    padding: 12px 16px;
    text-decoration: none;
    display: block;
}

.dropdown a:hover {background-color: #ddd;}
.dropdown1 a:hover {background-color: #ddd;}
.dropdown2 a:hover {background-color: #ddd;}
.dropdown3 a:hover {background-color: #ddd;}

.show {display: block;}
</style>
</head>
<body>

<script>
/* When the user clicks on the button, 
toggle between hiding and showing the dropdown content */
function myFunction() {
    document.getElementById("myDropdown").classList.toggle("show");
}
function myFunction1() {
    document.getElementById("myDropdown1").classList.toggle("show");
}
function myFunction2() {
    document.getElementById("myDropdown2").classList.toggle("show");
}
function myFunction3() {
    document.getElementById("myDropdown3").classList.toggle("show");
}

// Close the dropdown if the user clicks outside of it
window.onclick = function(event) {
  if (!event.target.matches('.dropbtn')) {

    var dropdowns = document.getElementsByClassName("dropdown-content");
    var i;
    for (i = 0; i < dropdowns.length; i++) {
      var openDropdown = dropdowns[i];
      if (openDropdown.classList.contains('show')) {
        openDropdown.classList.remove('show');
      }
    }
  }
  if (!event.target.matches('.dropbtn1')) {

    var dropdowns1 = document.getElementsByClassName("dropdown-content1");
    var i;
    for (i = 0; i < dropdowns1.length; i++) {
      var openDropdown = dropdowns1[i];
      if (openDropdown.classList.contains('show')) {
        openDropdown.classList.remove('show');
      }
    }
  }
  if (!event.target.matches('.dropbtn2')) {

    var dropdowns = document.getElementsByClassName("dropdown-content2");
    var i;
    for (i = 0; i < dropdowns2.length; i++) {
      var openDropdown = dropdowns2[i];
      if (openDropdown.classList.contains('show')) {
        openDropdown.classList.remove('show');
      }
    }
  }
  if (!event.target.matches('.dropbtn3')) {

    var dropdowns = document.getElementsByClassName("dropdown-content3");
    var i;
    for (i = 0; i < dropdowns3.length; i++) {
      var openDropdown = dropdowns3[i];
      if (openDropdown.classList.contains('show')) {
        openDropdown.classList.remove('show');
      }
    }
  }
}
</script>

<table width="100%" border="0">
  <tr>
    <td width="9%"><img src= "http://iot.nhri.org.tw/images/logoss.png" width="110" height="100" /></td>
    <td width="91%"><img src="http://iot.nhri.org.tw/images/title_ok.png" width="800" height="100" /></td>
  </tr>
</table>
<table width="100%" border="0">
  <tr>
 	<td width="70">
  			<a href="http://iot.nhri.org.tw/">Home</a>
	</td>
 	<td width="170">
  			<a href="http://iot.nhri.org.tw/ict1.php">Smart Home Application</a>
	</td>
 	<td width="200">
  			<a href="http://iot.nhri.org.tw/ict2p.php">Physiological Monitoring Healthcare System</a>
	</td>
 	<td width="80">
  			<a href="http://iot.nhri.org.tw/ict3.php">Lux RGB</a>	
	</td>
 	<td width="200">
			<div class="dropdown">
			<button onClick="myFunction3()" class="dropbtn3">Mosquito Trap</button>
				  <div id="myDropdown3" class="dropdown-content3">
					<a href="http://iot.nhri.org.tw/mosshow.php">查看蚊子</a>
					<a href="http://iot.nhri.org.tw/mosshow5.php">查看蚊子(公開版)</a>
                    <a href="http://iot.nhri.org.tw/mos/mosarealist.php">蚊子控管區域資料</a>
                    <a href="http://iot.nhri.org.tw/mos/mostypelist.php">蚊子類別資料</a>
                    <a href="http://iot.nhri.org.tw/mos/mosdevicelist.php">蚊子機基本資料</a>
			 	 </div>
			</div>
	</td>
  </tr>
</table>


