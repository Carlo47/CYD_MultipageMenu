#include <Arduino.h>
#include <WiFi.h>

// WiFi credentials 
const char ssid[]     = "Your SSID";
const char password[] = "Your password";
const char NTP_SERVER_POOL[] = "ch.pool.ntp.org";
const char TIME_ZONE[]       = "MEZ-1MESZ-2,M3.5.0/02:00:00,M10.5.0/03:00:00";
const char HOST_NAME[]       = "ESP32-CYD"; 

// Forward declarations
void initWiFi();
void printConnectionDetails();
void printNearbyNetworks();     
void printDateTime(int format); // Format: Output
                                // 0:      hh:mm:ss
                                // 1:      YYYY-MM-DD
                                // 2:      January 15 2019 16:33:20 (Tuesday)
                                // 3:      2019-01-15 16:33:20
                                // 4:      Tue Jan 15 16:33:20 2019
                                // 5:      2019-01-15 16:51:18 02/2 MEZ +0100

/**
 * Establish the WiFi connection with router 
 * and set a hostname for the ESP32
*/
void initWiFi()
{
    WiFi.setHostname(HOST_NAME);
    WiFi.begin(ssid, password);

    // Try forever
    while (WiFi.status() != WL_CONNECTED) 
    {
      log_i("Connecting to WiFi ...");
      delay(1000);
    }
    log_i("... connected");

    // 👉 The next line prevents the interrupt at GPIO_NUM_36 
    //   from being triggered continuously. This undesirable 
    //   effect is due to a hardware error in the ESP32 chip.
    //   This workaround can be omitted if interrupt pins 36 
    //   and 39 are not used. 
    WiFi.setSleep(WIFI_PS_NONE);
    log_i("==> done");
}


/*
 * Print nearby WiFi networks with SSID und RSSI 
 */
void printNearbyNetworks()
{
  int n = WiFi.scanNetworks();
  Serial.printf(R"(
Nearby WiFi networks
--------------------
)");
  for (int i = 0; i < n; i++)
  {
    Serial.printf("%s\t%d\r\n", WiFi.SSID(i).c_str(), WiFi.RSSI(i));
  }
  Serial.println();
}


/**
 * Use a raw string literal to print a formatted
 * string of WiFi connection details
 */
void printConnectionDetails()
{
  Serial.printf(R"(
Connection Details
------------------
SSID       : %s
Hostname   : %s
IP-Address : %s
MAC-Address: %s
RSSI       : %d (received signal strength indicator)
)", WiFi.SSID().c_str(),
    //WiFi.hostname().c_str(),  // ESP8266
    WiFi.getHostname(),    // ESP32 
    WiFi.localIP().toString().c_str(),
    WiFi.macAddress().c_str(),
    WiFi.RSSI());
  Serial.printf("\n");
}
