#include <WiFi.h> // создание собственной Wi-Fi точки доступа на ESP32
#include <WebServer.h> // встроенный HTTP-сервер
#include <ESP32Servo.h> // управление сервой
#include <Wire.h> // I2C для датчика тока
#include <INA226.h> // датчик тока/напряжения

// найстроки сети
const char* AP_SSID = "Gripper_wifi";
const char* AP_PASSWORD = "gripper123";

//настройки сервы
const int SERVO_PIN = 5; // GPIO сигнала сервы(ШИМ)
const int OPEN_ANGLE = 160; // разомкнутое положение
const int CLOSE_ANGLE = 20; // сомкнутое положение

//настройки I2C для INA226
const int SDA_PIN = 6; 
const int SCL_PIN = 7; 
const float SHUNT_OHMS = 0.1; // резистор-шунт R100

//порог определения предмета в хвате
const float CURRENT_LIMIT_MA = 600; // выше этого тока(mA) считаем, что предмет зажат

Servo gripperServo;
WebServer server(80);
INA226 ina226(0x40, &Wire);
bool ina226Ready = false;

float Current_mA = 0; // последнее измеренное значение тока
float Voltage_V = 0; // последнее измеренное значение напряжения  <-- ДОБАВЛЕНО
unsigned long lastSensorRead = 0; // millis() последнего опроса датчика

//HTML-страница!!
const char HTML_PAGE[] = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Gripper</title>
  <style>
    body { display:flex; flex-direction:column; align-items:center; padding-top:60px; font-family:sans-serif; }
    button { padding:20px 40px; font-size:20px; margin:10px; cursor:pointer; }
    #indicator { margin-top:30px; font-size:26px; font-weight:bold; }
  </style>
</head>
<body>
  <button onclick="send('/open')">Разомкнуть</button>
  <button onclick="send('/close')">Сомкнуть</button>

  <div id="indicator">Предмет: —</div>

  <script>
    async function send(url) {
      await fetch(url, {method: 'POST'});
    }

    setInterval(async () => {
      const r = await fetch('/state');
      const d = await r.json();
      document.getElementById('indicator').textContent = d.objectDetected ? 'Предмет: ЕСТЬ' : 'Предмет: нет';
      document.getElementById('indicator').style.color = d.objectDetected ? 'green' : 'gray';
    }, 300);
  </script>
</body>
</html>
)rawliteral";
//Конец HTML

void handleIndex() {
  server.send(200, "text/html", HTML_PAGE);
}

void handleOpen() { // разводим клешни
  gripperServo.write(OPEN_ANGLE);
  server.send(200, "text/plain", "ok");
}

void handleClose() { // сводим клешни
  gripperServo.write(CLOSE_ANGLE);
  server.send(200, "text/plain", "ok");
}

void handleState() { // отдаём текущий ток и флаг "предмет есть/нет"
  bool objectDetected = Current_mA > CURRENT_LIMIT_MA; // сравниваем с порогом
  String json = "{\"current_mA\":" + String(Current_mA, 1) +
                ",\"voltage_V\":" + String(Voltage_V, 2) +  // <-- ДОБАВЛЕНО
                ",\"objectDetected\":" + (objectDetected ? "true" : "false") + "}";
  server.send(200, "application/json", json);
}

void setup() {
  delay(2000); // даём хосту время на переинициализацию USB-CDC после ребута
  Serial.begin(115200);

  Wire.begin(SDA_PIN, SCL_PIN);
  ina226Ready = ina226.begin();
  
  gripperServo.setPeriodHertz(50);
  gripperServo.attach(SERVO_PIN, 500, 2400);
  gripperServo.write(OPEN_ANGLE); // при старте хват открыт

  WiFi.softAP(AP_SSID, AP_PASSWORD);
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleIndex);
  server.on("/open", HTTP_POST, handleOpen);
  server.on("/close", HTTP_POST, handleClose);
  server.on("/state", HTTP_GET, handleState);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();

  if (ina226Ready && millis() - lastSensorRead >= 200) { // чекаем датчик каждые 200 мс
    lastSensorRead = millis();
    float shuntVoltage_mV = ina226.getShuntVoltage_mV(); // сырое падение на шунте
    Current_mA = shuntVoltage_mV / SHUNT_OHMS; // переводим в ток по закону Ома
    Voltage_V = ina226.getBusVoltage();
  }
  Serial.println("Ток:" + String(Current_mA,1) + " mA, Напряжение:" + String(Voltage_V,2) + " V");
}