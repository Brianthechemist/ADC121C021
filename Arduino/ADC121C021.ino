// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// ADC121C021
// This code is designed to work with the ADC121C021_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/content/Analog-Digital-Converters?sku=ADC121C021_I2CADC#tabs-0-product_tabset-2

#include <Wire.h>

// I2C address of the ADC121C021 is 0x50(80) 
#define Addr 0x50

void setup()
{ 
    // Initialize I2C communication as MASTER
    Wire.begin();
    // Initialize serial communication, set baud rate = 9600
    Serial.begin(9600);
    delay(300);
}

void loop() 
{
    // Begin transmission with given device on I2C bus
    Wire.beginTransmission(Addr); 
    // Calling conversion result register, 0x00(0)
    Wire.write(0x00);
    // Stop I2C transmission on the device
    Wire.endTransmission();

    // Request 2 bytes
    Wire.requestFrom(Addr, 2);
    
    // Read 2 bytes of data, raw_adc msb, raw_adc lsb
    if(Wire.available() == 2)
    {  
        int MSB = Wire.read();
        int LSB = Wire.read();
    
        delay(300);
    
        // Converting the data to 12 bits
        int raw_adc = ((MSB * 256) + LSB) & 0x0FFF;
    
        // Output data to serial monitor
        Serial.print("Raw Value of the source is : ");
        Serial.println(raw_adc); 
    }
}

