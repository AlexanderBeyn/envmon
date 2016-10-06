void config_read(void) {
    File f;
    f = SPIFFS.open("/config", "r");
    if (!f) {
        Serial << "Unable to open file for reading" << endl;
        return;
    }

    String name;
    String value;
    char c;
    bool in_value = false;
    while(f.available() > 0) {
        c = f.read();
        if (c == '\n') {
            name.trim();
            value.trim();
            if (name.equals("wifi_ssid")) {
                config.wifi_ssid = value;
            } else if (name.equals("wifi_password")) {
                config.wifi_password = value;
            }
            name.remove(0);
            value.remove(0);
            in_value = false;
            continue;
        }
        
        if (in_value) {
            value.concat(c);
        } else {
            if (c == '=') {
                in_value = true;
            } else {
                name.concat(c);
            }
        }
    }
}

void config_print(void) {
    Serial << "wifi_ssid=" << config.wifi_ssid << endl;
    Serial << "wifi_password=" << config.wifi_password << endl;
}

void config_write(void) {
    File f;
    f = SPIFFS.open("/config", "w");
    if (!f) {
        Serial << "Unable to open file for writing" << endl;
        return;
    }

    f << "wifi_ssid=" << config.wifi_ssid << endl;
    f << "wifi_password=" << config.wifi_password << endl;

    f.flush();
    f.close();
}

void config_setup(void) {
    Serial << "Starting filesystem: ";
    bool res = SPIFFS.begin();
    if (res) {
        Serial << "ok" << endl;
    } else {
        Serial << "failed" << endl;
        Serial << "Formatting filesystem: " << ( SPIFFS.format() ? "true" : "false" ) << endl;
        Serial << "Starting filesystem: " << ( SPIFFS.begin() ? "true" : "false" ) << endl;
    }

    config_read();
}

