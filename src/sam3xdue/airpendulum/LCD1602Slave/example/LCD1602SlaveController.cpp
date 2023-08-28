
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
    Wire.write(a, 8);
    Wire.write(b, 8);
    Wire.write(c, 8);
    Wire.write(d, 8);
    Wire.endTransmission();
    delay(10);
}

void LCD1602SlaveController::update1602slave(String& e, String& f, String& g, String& h)
{
    Wire.beginTransmission(_slaveAddress);
    Wire.write('U');
    Wire.write(e.c_str());
    Wire.write('\0');
    Wire.write(f.c_str());
    Wire.write('\0');
    Wire.write(g.c_str());
    Wire.write('\0');
    Wire.write(h.c_str());
    Wire.write('\0');
    Wire.endTransmission();
}
