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
    <style>
      body {
        font-family: Arial, sans-serif;
        background-color: grey;
        color: white;
        display: flex;
        justify-content: center;
        align-items: center;
        margin-top: 30%;
      }
      .card {
        background-color: #1e1e1e;
        padding: 20px;
        border-radius: 10px;
        box-shadow: 0 4px 8px rgba(0, 0, 0, 0.5);
        text-align: center;
        width: 90%;
        max-width: 700px;
      }
      .card h1 {
        margin-bottom: 20px;
        font-size: 24px;
        color: #00d1ff;
      }
      .card p {
        font-size: 18px;
        margin-bottom: 20px;
      }
      .buttons {
        display: flex;
        flex-direction: column;
        gap: 10px;
      }
      a {
        display: block;
        padding: 10px;
        text-decoration: none;
        color: white;
        background-color: #007bff;
        border-radius: 5px;
        text-align: center;
        transition: background-color 0.3s;
      }
      a:hover {
        background-color: #0056b3;
      }
    </style>
  </head>
  <body>
    <div class="card">
      <h1>ESP32 LED</h1>
      <p>Estado del LED: <span style="color: red">%MODE%</span></p>
      <div class="buttons">
        <a href="/led1/on">Encender</a>
        <a href="/led1/off">Apagar</a>
        <a href="/led/police">Modo police</a>
        <a href="/buzzer">Zumbador</a>
      </div>
    </div>
  </body>
</html>
)=====";

#endif
