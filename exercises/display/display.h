#ifndef DISPLAY_H
#define DISPLAY_H

#include <U8x8lib.h>
#include <WiFi.h>

class Display {
public:
    // Constructor
    Display(int port, const char* ssid) 
        : port(port), ssid(ssid), u8x8(U8X8_PIN_NONE) {}

    // Initialize the display and WiFi
    void setup() {
        // Initialize the OLED display
        u8x8.begin();
        u8x8.setFlipMode(1);  // Rotate the display 180 degrees if needed

        // Initialize WiFi connection
        WiFi.begin(ssid);
        Serial.print("Connecting to WiFi");
        // Wait until connected
        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            Serial.print(".");
        }
        Serial.println("\nWiFi connected.");
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());

        // Store the IP address
        ip = WiFi.localIP();
    }

    // Update the display with current information
    void loopStep() {
        u8x8.setFont(u8x8_font_chroma48medium8_r);
        u8x8.clear();  // Clear the display before drawing new content
        
        // Display Title
        u8x8.drawString(0, 0, "AW & ML Robot");
        
        // Display Port Information
        char buffer[30];
        snprintf(buffer, sizeof(buffer), "Port: %d", port);
        u8x8.drawString(0, 1, buffer);


        ip = WiFi.localIP();
        snprintf(buffer, sizeof(buffer), "IP: %s", ip.toString().c_str());
        u8x8.drawString(0, 2, buffer);


        u8x8.setInverseFont(0);
    }

private:
    int port;                  // Member variable for port
    const char* ssid;          // Member variable for SSID
    U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8; // U8x8 display object
    IPAddress ip;              // Member variable for IP address
};

#endif // DISPLAY_H
