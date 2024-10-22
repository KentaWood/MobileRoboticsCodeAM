#include "display.h"
#include "wscommunicator.h"
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
IPAddress clientIP;

// Display object
Display myDisplay(PORT, SSID);

//websocket Communicator Object 
const unsigned long HEARTBEAT_INTERVAL = 1000;
WSCommunicator wsCommunicator(SSID, PORT, HEARTBEAT_INTERVAL);



void setup(void)
{
    Serial.begin(115200);
    wsCommunicator.setup();

    while (!Serial) {
        ; 
    }
    Serial.println("Initializing Display...");
    
    myDisplay.setup(); 
    Serial.println("Display Initialized.");
}

void loop(void){
    wsCommunicator.loopStep();
    myDisplay.loopStep();
    Serial.println("Display Updated.");
}
