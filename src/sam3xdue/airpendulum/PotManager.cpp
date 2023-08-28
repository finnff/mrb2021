#include "PotManager.h"

PotManager::PotManager(int* LCDPotInput, int multipliers[])
{
    this->LCDPotInput = LCDPotInput;
    for (int i = 0; i < 3; i++) {
        this->multipliers[i] = multipliers[i];
    }
}

void PotManager::begin()
{
    Wire.begin();
    pcf8574.begin();

    lcd.init();
    lcd.backlight();

    lcd.setCursor(0, 0); // Top left
    lcd.print(names[0]);
    lcd.print(": ");
    lcd.setCursor(7, 0); // Top right
    lcd.print("|");
    lcd.print(names[1]);
    lcd.print(": ");
    lcd.setCursor(0, 1); // Bottom left
    lcd.print(names[2]);
    lcd.print(": ");
    lcd.setCursor(7, 1);
    lcd.print("|");
    lcd.setCursor(12, 1); // Bottom right
    lcd.print(names[3]);
    lcd.print(": ");
}

void PotManager::debug_print()
{
    Serial.println("----- PotManager Debug Info -----");

    // Print pointer to LCDPotInput
    Serial.print("LCDPotInput Pointer: ");
    Serial.println((uintptr_t)LCDPotInput, HEX);
    Serial.print("LCDPotInput Value: ");
    Serial.println(*LCDPotInput);

    // Print Multiplier values
    Serial.print("Multipliers: ");
    for (int i = 0; i < 3; ++i) {
        Serial.print(multipliers[i]);
        Serial.print(' ');
    }
    Serial.println();

    // Print last read scaled pot values
    Serial.print("Last Scaled Pot Values: ");
    for (int i = 0; i < 4; ++i) {
        Serial.print(scaledPotValues[i]);
        Serial.print(' ');
    }
    Serial.println();

    // Read and display PCF8574 pin states
    for (int i = 0; i < 8; ++i) {
        bool state = pcf8574.read(i);
        Serial.print("PCF8574 Pin ");
        Serial.print(i);
        Serial.print(": ");
        Serial.println(state);
    }

    // You can add more debugging info as needed

    Serial.println("----- End Debug Info -----");
}

void PotManager::updateAndDisplay()
{
    int sensitivityLevels[4] = { 0, 0, 0, 0 };
    int MappedPotValues[4];

    for (int i = 0; i < 6; i += 2) {
        bool s1 = pcf8574.read(i);
        bool s2 = pcf8574.read(i + 1);
        sensitivityLevels[i / 2] = mapSensitivity(s1, s2);
    }

    for (int i = 0; i < 4; i++) {
        MappedPotValues[i] = map(analogRead(potPins[i]), 0, 1023, 0, 100);
        scaledPotValues[i] = (i < 3) ? MappedPotValues[i] * multipliers[i] : analogRead(potPins[i]);
        // return raw adc  for max res

        /* Serial.print(names[i]); */
        /* Serial.print(": "); */
        /* Serial.print(scaledPotValues[i]); */
        /* Serial.print(" (x"); */
        /* Serial.print(multipliers[sensitivityLevels[i]]); */
        /* Serial.print(") "); */
    }
    /* Serial.println(); */

    // ORiginal with pcf8574 setting power settinbg,
    /*     lcd.setCursor(2, 0); // Top left value */
    /*     lcd.print(MappedPotValues[0]); */
    /*     lcd.setCursor(5, 0); */
    /*     lcd.print(sensitivityLevels[0]); */
    /**/
    /*     lcd.print("|"); */
    /*     lcd.setCursor(10, 0); // Top right value */
    /*     lcd.print(MappedPotValues[1]); */
    /*     lcd.setCursor(14, 0); */
    /*     lcd.print(sensitivityLevels[1]); */
    /**/
    /*     lcd.setCursor(2, 1); // Bottom left value */
    /*     lcd.print(MappedPotValues[2]); */
    /*     lcd.setCursor(5, 1); */
    /*     lcd.print(sensitivityLevels[2]); */
    /*     lcd.print("|Y:"); */
    /*     lcd.print(*LCDPotInput); */
    /**/
    /*     lcd.setCursor(14, 1); // Bottom right value */
    /*     lcd.print(MappedPotValues[3]); */
    /* } */

    // set scalar PID directly from multipliers
    lcd.setCursor(2, 0); // Top left value
    lcd.print(scaledPotValues[0]);

    /* lcd.print("|"); */
    lcd.setCursor(10, 0); // Top right value
    lcd.print(scaledPotValues[1]);

    lcd.setCursor(2, 1); // Bottom left value
    lcd.print(scaledPotValues[2]);
    lcd.setCursor(7, 1); // Bottom left value
    lcd.print("|Y:");
    lcd.print(*LCDPotInput);

    lcd.setCursor(14, 1); // Bottom right value
    lcd.print(MappedPotValues[3]);
}

void PotManager::getScaledValues(int result[4])
{
    for (int i = 0; i < 4; i++) {
        result[i] = scaledPotValues[i];
    }
}

int PotManager::mapSensitivity(bool s1, bool s2)
{
    if (s1 && s2)
        return 0;
    else if (s1 && !s2)
        return 1;
    else if (!s1 && s2)
        return 2;
    else
        return 3;
}
