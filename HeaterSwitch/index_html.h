const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>

<meta HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">

<style>

h1 {
  font-size: 2.0em;
  text-align: center; 
  vertical-align: middle; 
  max-width: 400px; 
  margin:0 auto;
}

p {
  font-size: 2.0em;
  text-align: center; 
  vertical-align: middle; 
  max-width: 400px; 
  margin:0 auto;
}

table {
  font-size: 2.0em;
  text-align: left; 
  vertical-align: middle; 
  margin:0 auto;
}

.button {
  font-size: 20px;;
  text-align: center; 
}

.slidecontainer {
  width: 100%;
}

.slider {
  -webkit-appearance: none;
  width: 90%;
  height: 24px;
  background: #d3d3d3;
  outline: none;
  opacity: 0.7;
  -webkit-transition: .2s;
  transition: opacity .2s;
}

.slider:hover {
  opacity: 1;
}

.slider::-webkit-slider-thumb {
  -webkit-appearance: none;
  appearance: none;
  width: 22px;
  height: 22px;
  background: #4CAF50;
  cursor: pointer;
}

.slider::-moz-range-thumb {
  width: 22px;
  height: 22px;
  background: #4CAF50;
  cursor: pointer;
}

</style>


<title>Thrmostat</title>
<br>
<hr>
<h1>Thermostat</h1>
<hr>
<br>
</head>

<body style="font-family: verdana,sans-serif" BGCOLOR="#819FF7">
  <table>
    <tr><td style="text-align:right;">Temperature:</td><td> <span id='temp'></span> °C</td><tr>
    <tr><td style="text-align:right;">Level:</td><td> <span id='level'></span> °C</td><tr>
    <tr><td style="text-align:right;">Status:</td><td> <span id='status'></span></td><tr>
  </table>

  <br>  
  <hr>
  <br>  
  <div class="slidecontainer">
    <p><input type="range" min="0" max="30" value="15" step="0.5" class="slider" id="myRange"></p>
  </div>
  <br>  
  <p> 
  <input type="button" class="button" value="Down" onclick="button_clicked('down')"> 
  <input type="button" class="button" value="  Up  " onclick="button_clicked('up')"> 
  </p>
  
  <br>

  <p> 
  <input type="button" class="button" value=" Auto " onclick="button_clicked('auto')">
  <input type="button" class="button" value=" On " onclick="button_clicked('on')">
  <input type="button" class="button" value=" Off " onclick="button_clicked('off')"> 
  </p>

  <br>
  <hr>
  
  <script>
  
    requestData(); // get intial data straight away 

    var slider = document.getElementById("myRange");
    var output = document.getElementById("level");
        
    slider.oninput = function() {
      var level;
      level = 1 * this.value;
      output.innerHTML = level.toFixed(2);
      var xhr = new XMLHttpRequest();
      xhr.open('GET', 'slider' + "?level=" + this.value, true);
      xhr.send();
    }

    function button_clicked(key) { 
      var xhr = new XMLHttpRequest();
      xhr.open('GET', key, true);
      xhr.send();
      requestData();
    }
  
    // request data updates every 500 milliseconds
    setInterval(requestData, 500);

    function requestData() {

      var xhr = new XMLHttpRequest();
      
      xhr.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {

          if (this.responseText) { // if the returned data is not null, update the values

            var data = JSON.parse(this.responseText);

            document.getElementById("temp").innerText = data.temp.toFixed(2);
            document.getElementById("status").innerText = data.status;
            
            output.innerHTML = data.level.toFixed(2);
            slider.value = data.level;
       
          } 
        } 
      }
      xhr.open('GET', 'temp', true);
      xhr.send();
    }
     
  </script>

</body>

</html>

)=====";
