<?php
if (!isset($_GET['action'])) {
    die("No se recibió acción");
}

$esp32_ip = "http://10.0.110.244"; // IP fija del ESP32
$action = $_GET['action'];

if ($action == "thermostat" && isset($_GET['temp'])) {
    $url = "$esp32_ip/thermostat?temp=" . $_GET['temp'];
} elseif ($action == "cook" && isset($_GET['time'])) {
    $url = "$esp32_ip/cook?time=" . $_GET['time'];
} elseif ($action == "set_camera" && isset($_GET['ip'])) {
    $url = "$esp32_ip/set_camera?ip=" . $_GET['ip'];
} elseif ($action == "camera") {
    $url = "$esp32_ip/camera";
} else {
    die("Acción inválida");
}

// Realizar la solicitud al ESP32
$response = file_get_contents($url);
if ($action == "camera") {
    header("Content-Type: image/jpeg");
    echo $response;
} else {
    echo $response;
}
