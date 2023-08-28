#ifndef LCD1602SlaveController_h
#define LCD1602SlaveController_h

#include <Arduino.h>
#include <Wire.h>

class LCD1602SlaveController {
public:
    LCD1602SlaveController();
    void begin1602slave(int slaveAddress, const char* a, const char* b, const char* c, const char* d);
    void update1602slave(String& e, String& f, String& g, String& h);

private:
    int _slaveAddress;
};

#endif
