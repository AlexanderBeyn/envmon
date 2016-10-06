static ESP8266WebServer server(80);

static void handle_root(void) {
    server.send(200, "text/plain", "Get /env.json");
}

static void handle_env(void) {
    read_sensor();
    char response[100];
    const char *valid = sensor.valid ? "true" : "false";
    char chipID[17];
    snprintf(chipID, 17, "%x", ESP.getChipId());
    if (sensor.valid) {
        snprintf(response, 100, "{\"temperature\": %.1f, \"humidity\": %.1f, \"valid\": %s, \"id\": \"%s\"}", 
            sensor.temperature, sensor.humidity, valid, chipID);
    } else {
            snprintf(response, 100, "{\"valid\": %s, \"id\": \"%s\"}", valid, chipID);
    }
    server.send(200, "application/json", response);
}

void web_server_setup(void) {
    server.on("/", handle_root);
    server.on("/env.json", handle_env);
    server.begin();
}

void web_server_loop(void) {
    server.handleClient();
}

