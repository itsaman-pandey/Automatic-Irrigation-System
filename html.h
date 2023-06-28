const char html_page[] PROGMEM = R"rawSrting(
<!DOCTYPE html>
<html>

<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Smart Irrigation System</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      margin: 0;
      padding: 20px;
      background-color: #f5f5f5;
    }

    h1 {
      text-align: center;
      color: #333;
    }

    .block {
      background-color: #fff;
      border-radius: 8px;
      padding: 20px;
      margin-bottom: 20px;
      box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
    }

    .sensor-block {
      display: flex;
      justify-content: space-between;
      align-items: center;
    }

    .sensor-label {
      font-size: 20px;
      color: #333;
      margin-right: 20px;
    }

    .sensor-value {
      font-size: 32px;
      color: #333;
    }

    #moisture {
      font-size: 60px;
      text-align: center;
      margin-bottom: 40px;
      color: #333;
    }

    #relayStatus {
      font-size: 30px;
      text-align: center;
      margin-bottom: 20px;
      color: #333;
    }

    #controls {
      display: flex;
      justify-content: center;
      margin-top: 40px;
    }

    #controls button {
      font-size: 20px;
      padding: 10px 20px;
      margin: 0 10px;
      background-color: #333;
      color: #fff;
      border: none;
      cursor: pointer;
    }

    #footer {
     font-size: 16px;
  text-align: center;
  margin-top: 50px;
  color: #666;
  background-color: #f5f5f5;
  padding: 10px;
  border-radius: 8px;
  box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
    }
  </style>
</head>

<body>
  <h1>Smart Irrigation System</h1>
   <img src="https://sabudh.org/wp-content/uploads/2022/08/vector-logo-sabudh-1.svg" alt="Logo" style="position: absolute; top: 10px; left: 10px; width: 100px; height: 100px; border-radius: 50%;">
  <div class="block">
    <div class="sensor-block">
      <span class="sensor-label">Temperature:</span>
      <span class="sensor-value" id="temperature">Loading</span> °C
    </div>
    <div class="sensor-block">
      <span class="sensor-label">Humidity:</span>
      <span class="sensor-value" id="humidity">Loading</span> %
    </div>
  </div>

  <div class="block">
    <h2>Moisture Level</h2>
    <div id="moisture">Loading</div>
  </div>

  <div class="block">
    <h2>Relay Status</h2>
    <div id="relayStatus">Loading</div>
  </div>

  <div id="controls">
    <button onclick="activateRelay()">ON PUMP</button>
    <button onclick="deactivateRelay()">OFF PUMP</button>
  </div>

  <div id="footer">
    <b>This project was developed by Aman Pandey under the mentorship of Mr. RAMANDEEP SINGH, MR. SONU, MR. SAURABH</b>
  </div>

  <script>
    function updateValues() {
      fetch("/readSensorValues")
        .then(response => response.json())
        .then(data => {
          document.getElementById("temperature").textContent = data.temperature.toFixed(2);
          document.getElementById("humidity").textContent = data.humidity.toFixed(2);
          updateTemperatureColor(data.temperature);
          updateHumidityColor(data.humidity);
        })
        .catch(error => {
          console.log("Error:", error);
        });

      fetch("/readMoisture")
        .then(response => response.text())
        .then(data => {
          document.getElementById("moisture").textContent = data;
        })
        .catch(error => {
          console.log("Error:", error);
        });

      fetch("/readRelayStatus")
        .then(response => response.text())
        .then(data => {
          document.getElementById("relayStatus").textContent = data;
        })
        .catch(error => {
          console.log("Error:", error);
        });
    }

    function activateRelay() {
      fetch("/activateRelay")
        .then(response => response.text())
        .then(data => {
          console.log(data);
        })
        .catch(error => {
          console.log("Error:", error);
        });
    }

    function deactivateRelay() {
      fetch("/deactivateRelay")
        .then(response => response.text())
        .then(data => {
          console.log(data);
        })
        .catch(error => {
          console.log("Error:", error);
        });
    }

    function updateTemperatureColor(temperature) {
      var temperatureElement = document.getElementById("temperature");
      if (temperature > 30) {
        temperatureElement.style.color = "red";
      } else if (temperature > 20) {
        temperatureElement.style.color = "green";
      } else {
        temperatureElement.style.color = "blue";
      }
    }

    function updateHumidityColor(humidity) {
      var humidityElement = document.getElementById("humidity");
      if (humidity > 80) {
        humidityElement.style.color = "red";
      } else if (humidity > 60) {
        humidityElement.style.color = "orange";
      } else {
        humidityElement.style.color = "blue";
      }
    }

    setInterval(updateValues, 5000);
    updateValues();
  </script>
</body>

</html>




)rawSrting";
