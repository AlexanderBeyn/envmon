#include <ESP8266WiFi.h>
#include <WiFiClient.h>

#include <ESP8266WebServer.h>

#include <DHT.h>

#include <Wire.h>
#include <SFE_MicroOLED.h>

#include <FS.h>

template<class T> inline Print &operator <<(Print &obj, T arg) { obj.print(arg); return obj; }
#define endl "\n"


struct config_s {
    String wifi_ssid;
    String wifi_password;
} config;

struct sensor_s {
    float humidity;
    float temperature;
    bool valid;
    unsigned long attempts = 0;
    unsigned long fails = 0;
} sensor;

void setup() {
    Serial.begin(9600);
    Serial.setTimeout(100);

    config_setup();
    sensor_setup();
    wifi_setup();
    web_server_setup();
    oled_setup();
    cli_setup();
}

void loop() {

    sensor_loop();
    web_server_loop();
    cli_loop();
}
