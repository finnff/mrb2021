#include <Wire.h>
#include "LCD1602SlaveController.h"

// Address of the slave (must match the slave's setup)
const int SLAVE_ADDRESS = 0x0B; 
LCD1602SlaveController lcdSlaveController;

void setup() {
  Wire.begin(); // Join the bus as a master
  
  Serial.begin(9600);
  Serial.println("Master started. Initializing the slave LCD...");

  // Initialize the slave LCD with initial parameters
  lcdSlaveController.begin1602slave(SLAVE_ADDRESS, "Param1", "Param2", "Param3", "Param4");
  
  delay(1000);  // Just to give some breathing room between operations
}

void loop() {
  Serial.println("Sending update to slave LCD...");

  // Example strings for the update. In a real scenario, these could be sensor readings, calculations, etc.
  String e = "Loop1";
  String f = "Loop2";
  String g = "Loop3";
  String h = "Loop4";
  
  // Update the slave LCD with new parameters
  lcdSlaveController.update1602slave(e, f, g, h);

  delay(1000);  // Delay for 1 second
}
