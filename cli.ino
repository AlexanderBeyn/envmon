static String cli_input_line;


const char *wifi_statuses[] = {
  "idle",
  "no ssid available",
  "scan complete",
  "connected",
  "connect failed",
  "connection lost",
  "disconnected"
};

static void cli_cmd_show_network(String args) {
  Serial << "Network" << endl;
  Serial << "Status: " << WiFi.status() << " (" << wifi_statuses[WiFi.status()] << ")" << endl;
  Serial << "SSID: " << WiFi.SSID() << endl;
  Serial << "BSSID: " << WiFi.BSSIDstr() << endl;
  Serial << "MAC Address: " << WiFi.macAddress() << endl;
  Serial << "IP Address: " << WiFi.localIP() << endl;
  Serial << endl;
}

static void cli_cmd_show_sensor(String args) {
  Serial << "Sensor" << endl;
  Serial << "Valid: " << ( sensor.valid ? "true" : "false" ) << endl;
  Serial << "Temperature: " << sensor.temperature << " C" << endl;
  Serial << "Temperature: " << sensor.temperature * 9.0 / 5.0 + 32.0 << " F" << endl;
  Serial << "Humidity: " << sensor.humidity << " %" << endl;
  Serial << "Sensor reads: " << sensor.attempts - sensor.fails << " reads ( + " << sensor.fails << " fails)" << endl;
}

static void cli_cmd_show_fs(String args) {
  Serial << endl;
  struct FSInfo fs_info;
  SPIFFS.info(fs_info);
  Serial << "Filesystem (SPIFFS)" << endl;
  Serial << "totalBytes: " << fs_info.totalBytes << endl;
  Serial << "usedBytes: " << fs_info.usedBytes << endl;
  Serial << "blockSize: " << fs_info.blockSize << endl;
  Serial << "pageSize: " << fs_info.pageSize << endl;
  Serial << "maxOpenFiles: " << fs_info.maxOpenFiles << endl;
  Serial << "maxPathLength: " << fs_info.maxPathLength << endl;
}

static void cli_cmd_show_config(String args) {
  config_print();
}

static void cli_cmd_format(String args) {
  bool res;
  Serial << "Ending the filesystem" << endl;
  SPIFFS.end();
  Serial << "Formatting filesystem: " << ( SPIFFS.format() ? "true" : "false" ) << endl;
  Serial << "Starting filesystem: " << ( SPIFFS.begin() ? "true" : "false" ) << endl;
}

static void cli_cmd_save(String args) {
  config_write();
}

static void cli_cmd_set_wifi_ssid(String args) {
  config.wifi_ssid = args;
}

static void cli_cmd_set_wifi_password(String args) {
  config.wifi_password = args;
}

static void cli_cmd_wifi_reconnect(String args) {
  wifi_connect();
}

struct {
  const char *command;
  void (*function)(String args);
  const char *description;
} cli_commands[] = {
  {"help", cli_cmd_help, "- Print a list of commands"},
  {"format", cli_cmd_format, "- Format the filesystem"},
  {"save", cli_cmd_save, "- Save configuration to the filesystem"},
  {"set wifi ssid", cli_cmd_set_wifi_ssid, " <SSID> - Set WiFi SSID"},
  {"set wifi password", cli_cmd_set_wifi_password, " <PASSWORD> - Set WiFi password"},
  {"show config", cli_cmd_show_config, "- Show loaded configuration"},
  {"show fs", cli_cmd_show_fs, "- Show filesystem information"},
  {"show network", cli_cmd_show_network, "- Show network information"},
  {"show sensor", cli_cmd_show_sensor, "- Show sensor information"},
  {"wifi reconnect", cli_cmd_wifi_reconnect, "- Reconnect to WiFi"},
  {NULL, NULL}
};

static void cli_cmd_help(String args) {
  for (int i = 0; cli_commands[i].command != NULL; i++) {
    Serial << cli_commands[i].command << cli_commands[i].description << endl;
  }
}

static void cli_handle_line(void) {
  for (int i = 0; cli_commands[i].command != NULL; i++) {
    if (cli_input_line.startsWith(cli_commands[i].command)) {
      String args = cli_input_line.substring(strlen(cli_commands[i].command));
      args.trim();
      cli_commands[i].function(args);
      return;
    }
  }
  Serial << "Unknown command '" << cli_input_line << "'. Enter 'help' for a list of commands." << endl;
}

static void cli_read_char() {
  if (Serial.available() < 1) {
    return;
  }

  char c = Serial.read();

  switch (c) {
    case 13:    // CR
    case 10:    // NL
      Serial.println("");
      if (cli_input_line.length() > 0) {
        cli_handle_line();
        cli_input_line.remove(0);
      }
      Serial.print("> ");
      break;
    case 8:     // BS
    case 127:   // DEL
      if (cli_input_line.length() > 0) {
        cli_input_line.remove(cli_input_line.length() - 1);
        Serial.print("\b \b");
      }
      break;
    default:
      Serial.print(c);
      cli_input_line.concat(c);
  }
}

void cli_setup(void) {
  Serial.println("Enter 'help' for a list of commands.");
  Serial.print("> ");
}

void cli_loop(void) {
  cli_read_char();
}

