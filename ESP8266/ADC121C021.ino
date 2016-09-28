// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// ADC121C021
// This code is designed to work with the ADC121C021_I2CADC I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/content/Analog-Digital-Converters?sku=ADC121C021_I2CADC#tabs-0-product_tabset-2

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Wire.h>

// ADC121C021 I2C address is 0x50(80)
#define Addr 0x50

const char* ssid = "your ssid network";
const char* password = "your password";
float raw_adc;

ESP8266WebServer server(80);

void handleroot()
{
  unsigned int data[2];

  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Calling conversion result register, 0x00(0)
  Wire.write(0x00);
  // Stop I2C transmission
  Wire.endTransmission();

  // Request 2 bytes of data
  Wire.requestFrom(Addr, 2);

  // Read 2 bytes of data
  // raw_adc msb, raw_adc lsb
  if (Wire.available() == 2)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
  }
  delay(300);

  // Convert the data to 12 bits
  int raw_adc = ((data[0] & 0x0F) * 256) + data[1];

  // Output data to serial monitor
  Serial.print("Digital value of Analog Input: ");
  Serial.println(raw_adc);

  // Output data to web server
  server.sendContent
  ("<html><head><meta http-equiv='refresh' content='5'</meta>"
   "<h1 style=text-align:center;font-size:300%;color:blue;font-family:britannic bold;>CONTROL EVERYTHING</h1>"
   "<h3 style=text-align:center;font-family:courier new;><a href=http://www.controleverything.com/ target=_blank>www.controleverything.com</a></h3><hr>"
   "<h2 style=text-align:center;font-family:tahoma;><a href=https://www.controleverything.com/content/Analog-Digital-Converters?sku=ADC121C021_I2CADC#tabs-0-product_tabset-2/ \n"
   "target=_blank>ADC121C021 Sensor I2C Mini Module</a></h2>");
  server.sendContent
  ("<h3 style=text-align:center;font-family:tahoma;>Digital value of Analog Input: " + String(raw_adc) );
}

void setup()
{
  // Initialise I2C communication as MASTER
  Wire.begin(2, 14);
  // Initialise serial communication, set baud rate = 115200
  Serial.begin(115200);

  // Connect to WiFi network
  WiFi.begin(ssid, password);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);

  // Get the IP address of ESP8266
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Start the server
  server.on("/", handleroot);
  server.begin();
  Serial.println("HTTP server started");
}

void loop()
{
  server.handleClient();
}

