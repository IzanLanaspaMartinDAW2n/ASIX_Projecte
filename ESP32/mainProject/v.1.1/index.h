/*
 * This ESP32 code is created by esp32io.com
 *
 * This ESP32 code is released in the public domain
 *
 * For more detail (instruction and wiring diagram), visit https://esp32io.com/tutorials/esp32-controls-led-via-web
 */

#ifndef WEBPAGE_H
#define WEBPAGE_H

const char* webpage = R"=====(
<!DOCTYPE html>
<html>
  <head>
    <link rel="icon" href="data:," />
    <title>COOKTECH</title>
    <style>
      body {
        font-family: Arial, sans-serif;
        background-color: #f2f2f2;
        color: #333;
        display: flex;
        justify-content: center;
        align-items: center;
        height: 100vh;
        margin: 0;
      }
      .card {
        background-color: #ffffff;
        padding: 20px;
        border-radius: 10px;
        box-shadow: 0 4px 8px rgba(0, 0, 0, 0.2);
        text-align: center;
        width: 300px;
      }
      .card h1 {
        margin-bottom: 20px;
        font-size: 24px;
        color: #007bff;
      }
      .card p {
        margin-bottom: 10px;
        font-size: 18px;
      }
      input[type="number"], input[type="submit"] {
        padding: 10px;
        margin: 10px 0;
        width: 100%;
        box-sizing: border-box;
      }
      input[type="submit"] {
        background-color: #007bff;
        color: white;
        border: none;
        border-radius: 5px;
        cursor: pointer;
      }
      input[type="submit"]:hover {
        background-color: #0056b3;
      }
    </style>
  </head>
  <body>
    <div class="card">
      <h1>COOKTECH</h1>
      <p>Modo TERMOSTATO</p>
      <form action="/thermostat" method="get">
        <label for="temp">Temperatura objetivo (&deg;C):</label>
        <input type="number" id="temp" name="temp" min="10" max="100" required />
        <input type="submit" value="Iniciar Termostato" />
      </form>
      <hr>
      <p>Modo COOK</p>
      <form action="/cook" method="get">
        <label for="time">Tiempo de cocci&oacute;n (min):</label>
        <input type="number" id="time" name="time" min="1" max="60" required />
        <input type="submit" value="Iniciar Temporizador" />
      </form>
    </div>
  </body>
</html>
)=====";

#endif
