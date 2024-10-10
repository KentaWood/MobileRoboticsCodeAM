#include "display.h"
#include <Arduino.h>
#include <U8x8lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif

//
// Global state
//

// Network configuration
const char* SSID = "Pomona";
const int PORT = 8181; // Changed from uint16_t to int for consistency

// Display object
Display myDisplay(PORT, SSID);

// Removed redundant u8x8 object to prevent conflicts

void setup(void)
{
    // Initialize Serial for debugging (optional but recommended)
    Serial.begin(115200);
    while (!Serial) {
        ; // Wait for Serial to initialize
    }
    Serial.println("Initializing Display...");
    
    myDisplay.setup(); // Added missing semicolon
    Serial.println("Display Initialized.");
}

void loop(void){
    myDisplay.loopStep(); // Added missing semicolon
    delay(1000); // Optional: Add a delay to control update frequency

    // Optional: Add Serial output to monitor updates
    Serial.println("Display Updated.");
}
