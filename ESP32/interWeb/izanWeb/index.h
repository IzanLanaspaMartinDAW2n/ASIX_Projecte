#ifndef WEBPAGE_H
#define WEBPAGE_H

const char* webpage = R"=====(
<!DOCTYPE HTML>
<html>
<head>
<link rel='icon' href='data:,'>
<style>
  body { font-family: Arial, sans-serif; text-align: center; }
  .button { padding: 10px; margin: 5px; border: none; background-color: #4CAF50; color: white; }
</style>
</head>
<body>
  <h1>Control de Temperatura</h1>
  <h2>Temperatura: <span id='temperature'>%TEMPERATURE%</span>°C</h2>
  <button class='button' onclick='changeTemperature("/increase")'>⬆️ Subir</button>
  <button class='button' onclick='changeTemperature("/decrease")'>⬇️ Bajar</button>
  <script>
    function changeTemperature(route) {
      fetch(route).then(response => response.text()).then(html => {
        document.body.innerHTML = html;
      });
    }
  </script>
</body>
</html>
)=====";

#endif
