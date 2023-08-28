// PotManager.h

#ifndef PotManager_h
#define PotManager_h

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <PCF8574.h>
#include <Wire.h>

class PotManager {
public:
    PotManager(int* LCDPotInput, int multipliers[]);
    void begin();
    void updateAndDisplay();
    void getScaledValues(int result[4]);
    void debug_print();

private:
    int mapSensitivity(bool s1, bool s2);
    PCF8574 pcf8574;
    LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);
    int* LCDPotInput;
    const int potPins[4] = { A1, A0, A2, A3 };
    char names[4] = { 'p', 'i', 'd', 'X' };
    int multipliers[4];
    int scaledPotValues[4];
};

#endif
