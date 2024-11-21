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
<!DOCTYPE HTML>
<html>
<head>
<link rel='icon' href='data:,'>
</head>
<p>LED state: <span style='color: red;'>%LED_STATE%</span></p>
<a href='/led1/on'>Turn ON</a>
<br><br>
<a href='/led1/off'>Turn OFF</a>
</html>
)=====";

#endif
