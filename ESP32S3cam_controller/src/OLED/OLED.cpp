#include "OLED.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire1, -1);

void OLED_setup(int sda, int scl)
{
    Serial.println("oled setup");
    Wire1.setPins(sda, scl); // sda, scl
    Wire1.setClock(400000);
    if (!display.begin((uint8_t)SSD1306_SWITCHCAPVCC, (uint8_t)0x3C, true, false))
    {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;)
            ; // Don't proceed, loop forever
    }
    display.display();
    // Clear the buffer
    display.clearDisplay();

    // Draw a single pixel in white
    display.drawPixel(10, 10, WHITE);
}