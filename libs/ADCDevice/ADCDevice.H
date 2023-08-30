/**********************************************************************
 * Filename    : ADCDevice.H
 * Description : Header file of Freenove ADC Module library.
 * Author      : www.freenove.com
 * modification: 2020/03/06
 **********************************************************************/
#include <wiringPi.H>
#include <wiringPiI2C.H>
#include <stdio.h>

class ADCDevice
{

protected:
    int _fd;

public:
    int address;
    int cmd;

    ADCDevice();
    virtual ~ADCDevice(){};
    int detectI2C(const int addr);
    virtual int analogRead(UNU int chn);
    //~ADCDevice(){printf("Destructor ... \n");}
};

class PCF8591 : public ADCDevice
{
public:
    PCF8591(const int addr = 0x48);   // 0x48 is the default i2c address for PCF8591 Module.
    int analogRead(const int chn);    // PCF8591 has 4 ADC input pins, chn:0,1,2,3
    int analogWrite(const int value); // PCF8591 has DAC function
};

class ADS7830 : public ADCDevice
{
public:
    ADS7830(const int addr = 0x4b); // 0x4b is the default i2c address for ADS7830 Module.
    int analogRead(const int chn);  // ADS7830 has 8 ADC input pins, chn:0,1,2,3,4,5,6,7
};
