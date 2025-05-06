#ifndef WEBPAGE_H
#define WEBPAGE_H

const char* webpage = R"=====(
<!DOCTYPE html>
<html lang="es">
  <head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>COOKTECH</title>
    <style>
      body {
        font-family: Arial, sans-serif;
        background: linear-gradient(to bottom, #87ceeb, #f2f2f2);
        color: #333;
        margin: 0;
        padding: 0;
        display: flex;
        justify-content: center;
        align-items: center;
        min-height: 100vh;
      }
      .container {
        background-color: rgba(255, 255, 255, 0.9);
        border-radius: 15px;
        padding: 20px;
        box-shadow: 0 4px 15px rgba(0, 0, 0, 0.2);
        max-width: 1000px;
        width: 90%;
      }
      .title {
        text-align: center;
        font-size: 36px;
        font-weight: bold;
        color: #007bff;
        position: relative;
        margin-bottom: 20px;
      }
      .title::after {
        content: "®";
        font-size: 16px;
        vertical-align: super;
      }
      .flex-container {
        display: flex;
        flex-direction: row;
        gap: 20px;
        justify-content: center;
      }
      .card {
        background-color: #ffffff;
        padding: 15px;
        border-radius: 10px;
        box-shadow: 0 8px 16px rgba(0, 0, 0, 0.1);
        border: 1px solid #ccc;
        text-align: center;
        flex: 1 1 calc(50% - 20px); /* Dos columnas en pantallas grandes */
        min-width: 280px; /* Una columna en móviles */
      }
      .card h2 {
        font-size: 20px;
        margin-bottom: 15px;
        color: #007bff;
      }
      .card form {
        display: flex;
        flex-direction: column;
        gap: 10px;
      }
      .autorName {
        font-weight: bold;
        color: #007bff;
      }
      .citation {
        text-align: center;
        margin-top: 20px;
        font-size: 14px;
        color: #555;
      }
      input[type="number"],
      input[type="text"],
      input[type="submit"] {
        padding: 10px;
        border: 1px solid #ccc;
        border-radius: 5px;
        font-size: 14px;
        width: 100%;
        box-sizing: border-box;
      }
      input[type="submit"],
      button {
        background-color: #007bff;
        color: white;
        font-weight: bold;
        cursor: pointer;
        border: none;
      }
      input[type="submit"]:hover,
      button:hover {
        background-color: #0056b3;
      }
      button {
        padding: 10px;
        border-radius: 5px;
        width: 100%;
        box-sizing: border-box;
      }
      #camera-image {
        max-width: 100%;
        border: 1px solid #ccc;
        border-radius: 10px;
        margin-top: 20px;
        display:none;
        margin:0 auto;
        width:80%;
      }
      @media screen and (max-width: 1100px) {
        .flex-container {
          flex-direction: column;
        }
        .card {
          flex: 1 1 100%;
        }
      }
    </style>
  </head>
  <body>
    <div class="container">
      <h1 class="title">COOKTECH</h1>
      <p id="sensordata" style="text-align:center;font-size:18px;margin:10px 0;">
        Temp: %TEMPERATURE% °C&nbsp;&nbsp;·&nbsp;&nbsp;Hum: %HUMIDITY% %
      </p>
      <div class="flex-container">
        <!-- Modo TERMOSTATO -->
        <div class="card">
          <h2>Modo TERMOSTATO</h2>
          <form action="/thermostat" method="get">
            <label for="temp">Temperatura objetivo (&deg;C):</label>
            <input
              type="number"
              id="temp"
              name="temp"
              min="10"
              max="100"
              required
            />
            <input type="submit" value="Iniciar Termostato" />
          </form>
        </div>
        <!-- Modo COOK -->
        <div class="card">
          <h2>Modo COOK</h2>
          <form action="/cook" method="get">
            <label for="time">Tiempo de cocci&oacute;n (min):</label>
            <input
              type="number"
              id="time"
              name="time"
              min="1"
              max="60"
              required
            />
            <input type="submit" value="Iniciar Temporizador" />
          </form>
        </div>
        <!-- Modo COMPROBACIÓN -->
        <div class="card">
          <h2>Modo COMPROBACIÓN</h2>
          <form action="/set_camera" method="get">
            <label for="ip">IP de la cámara:</label>
            <input
              type="text"
              id="ip"
              name="ip"
              placeholder="Ej: 10.0.110.162"
              required
            />
            <input type="submit" value="Establecer IP" />
          </form>
          <br />
          <button onclick="capturePhoto()">Capturar Foto desde Cámara</button>
        </div>
      </div>
      <img id="camera-image" src="" alt="Foto capturada" />
      <div class="citation">
        <p>
          Designed by
          <span class="autorName">CookTech corp.</span>
        </p>
      </div>
    </div>
    <script>
      function capturePhoto() {
        const img = document.getElementById("camera-image");
        img.src = "/camera?cacheBuster=" + new Date().getTime();
        img.style.display = "block"; // Muestra la imagen una vez que se actualice el src
      }
    </script>
  </body>
</html>
)=====";

#endif
