#include <SimpleFOC.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

const char* ssid = "";
const char* password = "";

MagneticSensorI2C sensor = MagneticSensorI2C(AS5600_I2C);
BLDCMotor motor = BLDCMotor(7);
BLDCDriver3PWM driver = BLDCDriver3PWM(16, 17, 5, 4);
InlineCurrentSense current_sense = InlineCurrentSense(0.01f, 50.0f, 39, 36);
Commander command = Commander(Serial);

void doMotor(char* cmd) { command.motor(&motor, cmd); }

void setup() {
  Serial.begin(115200);
  SimpleFOCDebug::enable(&Serial);
  Serial.println("Booting...");

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  ArduinoOTA.setHostname("FOC-Board-ESP32");

  ArduinoOTA
    .onStart([]() {
      motor.disable(); 
      Serial.println("OTA Started. Motor disabled.");
    })
    .onEnd([]() { Serial.println("\nEnd"); })
    .onProgress([](unsigned int progress, unsigned int total) { Serial.printf("Progress: %u%%\r", (progress / (total / 100))); })
    .onError([](ota_error_t error) { Serial.printf("Error[%u]", error); });

  ArduinoOTA.begin();

  Serial.println("Ready for OTA");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  


  Wire.begin(21, 22);
  Wire.setClock(400000);

  sensor.init();
  motor.linkSensor(&sensor);

  driver.voltage_power_supply = 12.0;
  driver.voltage_limit = 9.0; //Safety
  driver.init();
  current_sense.linkDriver(&driver);
  motor.linkDriver(&driver);

  motor.velocity_limit = 32.0; //Safety
  motor.init();

  current_sense.init();
  motor.linkCurrentSense(&current_sense);

  motor.current_limit = 3.0; //Safety
  
  motor.voltage_limit = 8.5; //Safety
  motor.voltage_sensor_align = 1.0;

  motor.PID_current_q.P = 0.2;
  motor.PID_current_q.I = 20.0;
  motor.PID_current_d.P = 0.2;
  motor.PID_current_d.I = 20.0;
  motor.LPF_current_q.Tf = 0.01;
  motor.LPF_current_d.Tf = 0.01;

  motor.PID_velocity.P = 0.80;
  motor.PID_velocity.I = 1.0;
  motor.PID_velocity.D = 0.005;
  motor.LPF_velocity.Tf = 0.05;

  motor.P_angle.P = 8.0;

  motor.torque_controller = TorqueControlType::foc_current;
  motor.controller = MotionControlType::angle;
 
  motor.initFOC();

  command.add('M', doMotor, (char*)"motor");
  command.verbose = VerboseMode::machine_readable;
}

void loop() {
  ArduinoOTA.handle();

  motor.loopFOC();
  motor.move();
  
  command.run();

  static long last_time = 0;
  if (millis() - last_time > 20) {
    Serial.print(">Target:"); Serial.println(motor.target);
    Serial.print(">Angle:");  Serial.println(motor.shaft_angle);
    Serial.print(">Vel:");    Serial.println(motor.shaft_velocity);
    Serial.print(">Iq:");     Serial.println(motor.current.q);
    Serial.print(">Id:");     Serial.println(motor.current.d);
    Serial.print(">Uq:");     Serial.println(motor.voltage.q);
    last_time = millis();
  }
}
