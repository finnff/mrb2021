#ifndef LCD1602SlaveController_h
#define LCD1602SlaveController_h

#include <Arduino.h>
#include <Wire.h>

class LCD1602SlaveController {
public:
    LCD1602SlaveController();
    void begin1602slave(int slaveAddress, const char* a, const char* b, const char* c, const char* d);
    void update1602slave(const char* e, const char* f, const char* g, const char* h);
    void update1602slave(int e, int f, int g, int h);

private:
    int _slaveAddress;
};

#endif
