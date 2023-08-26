
#include "LCD1602SlaveController.h"

LCD1602SlaveController::LCD1602SlaveController()
{
    // Constructor (can be empty for now)
}

void LCD1602SlaveController::begin1602slave(int slaveAddress, const char* a, const char* b, const char* c, const char* d)
{
    _slaveAddress = slaveAddress;
    Wire.begin();
    Wire.beginTransmission(_slaveAddress);
    Wire.write('I');
    Wire.write(a, 2);
    Wire.write(b, 2);
    Wire.write(c, 2);
    Wire.write(d, 2);
    Wire.endTransmission();
    delay(1000);
}

void LCD1602SlaveController::update1602slave(const char* e, const char* f, const char* g, const char* h)
{
    Wire.beginTransmission(_slaveAddress);
    Wire.write('U');
    Wire.write(e, 4);
    Wire.write(f, 4);
    Wire.write(g, 4);
    Wire.write(h, 4);
    Wire.endTransmission();
}

void LCD1602SlaveController::update1602slave(int e, int f, int g, int h)
{
    char e_str[5], f_str[5], g_str[5], h_str[5];

    snprintf(e_str, sizeof(e_str), "%d", e);
    snprintf(f_str, sizeof(f_str), "%d", f);
    snprintf(g_str, sizeof(g_str), "%d", g);
    snprintf(h_str, sizeof(h_str), "%d", h);

    update1602slave(e_str, f_str, g_str, h_str);
}

// ... rest of the file ...
