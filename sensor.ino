

#define DHTTYPE DHT22     // DHT Shield uses DHT 11
#define DHTPIN D4             // DHT Shield uses pin D4

static DHT dht(DHTPIN, DHTTYPE);


static unsigned long sensor_last_read = 0;
static const long sensor_interval = 2000;

static void read_sensor(void) {
    unsigned long currentMillis = millis();

    if (currentMillis - sensor_last_read < sensor_interval) {
        return;
    }
    sensor_last_read = currentMillis;

    sensor.attempts++;
    sensor.humidity = dht.readHumidity();
    sensor.temperature = dht.readTemperature();

    if (isnan(sensor.humidity) || isnan(sensor.temperature)) {
        sensor.valid = false;
        sensor.fails++;
        return;
    }
     
    sensor.valid = true;
    oled_display_sensors();
}

void sensor_setup(void) {
    dht.begin();
    sensor.valid = false;
}

void sensor_loop(void) {
    read_sensor();
}

