  #include <Arduino.h>
  
  /**
   * Print system info to the serial monitor as shown below:
   * 
   *  --- System Info ---
   *  model          ESP32
   *  cores          2
   *  features       WIFI b/g/n   BLE   BT
   *  revision       3
   *  full_revision  301
  */
  const char *chipModel[] = {"UNKOWN", "ESP32", "ESP32-S", "ESP32-S3", "ESP32-C3", "ESP32-H2"};
  const char *chipFeatures[] = {"FLASH", "WIFI b/g/n", "", "", "BLE", "BT", "IEEE 802.15.4", "PSRAM"};
  esp_chip_info_t chipInfo;
  void printSystemInfo()
  {
    esp_chip_info(&chipInfo);
    uint32_t b = 1;

    Serial.println("\nSystem Info\n-----------");
    Serial.printf("  Model          %s\n  Cores          %d\n", chipModel[chipInfo.model], chipInfo.cores);
    Serial.print("  Features       ");
    for (int i = 0; i < 8; i++)
    { // output the bit-coded features
      if (chipInfo.features & b) Serial.printf("%s   ", chipFeatures[i]);
      b <<= 1; // mask the next bit
    }
    Serial.printf("\n  Revision       %d\n  Full revision  %d\n", chipInfo.revision, chipInfo.full_revision);
    Serial.printf("  Heap size   %6d kB\n", ESP.getHeapSize()/1024);
    Serial.printf("  Free heap   %6d kB\n", ESP.getFreeHeap()/1024);
    Serial.printf("  Psram size  %6d kB\n", ESP.getPsramSize()/1024);
    Serial.printf("  Free Psram  %6d kB\n", ESP.getFreePsram()/1024);
  }