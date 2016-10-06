static int connect_attempts = 10;

void wifi_connect(void) {
    wifi_connect(config.wifi_ssid.c_str(), config.wifi_password.c_str());
}

void wifi_connect(const char *ssid, const char *password) {
    if (WiFi.isConnected()) {
        Serial << "Disconnecting" << endl;
        WiFi.disconnect();
    }

    Serial << "Connecting to '" << ssid << "': ";
    WiFi.begin(ssid, password);

    int attempt = 1;
    while (WiFi.status() != WL_CONNECTED && attempt <= connect_attempts) {
        delay(500);
        Serial << ".";
        attempt++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial << "connected." << endl;
        Serial << "IP Address: " << WiFi.localIP() << endl;
    } else {
        Serial << "timed out." << endl;
    }
}

void wifi_setup(void) {
    wifi_connect();
}

void wifi_loop(void) {
    
}

