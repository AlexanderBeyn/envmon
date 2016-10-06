#define PIN_RESET 255
#define DC_JUMPER 0  // I2C Addres: 0 - 0x3C, 1 - 0x3D

static MicroOLED oled(PIN_RESET, DC_JUMPER);

void oled_display_sensors(void) {
    float temp = sensor.temperature * 9.0 / 5.0 + 32.0;

    oled.clear(PAGE);
    oled.setCursor(0, 0);
    oled.setFontType(3); // 12x48
    oled.printf("%2.0f", temp);

    oled.setFontType(1); // 8x16

    if (temp < 100.0) {
        oled.setCursor(25, 0);
        oled.print("F");
    }
    
    oled.setCursor(56, 0);
    oled.print("%");

    oled.setCursor(47, 16);
    oled.printf("%2.0f", sensor.humidity);
    
    oled.display();
}

void oled_setup(void) {
    oled.begin();
    oled.clear(ALL);
}


