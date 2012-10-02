#include "Wire.h"
#define DS1307_ADDRESS 0x68

int tmp102Address = 0x48;
byte res;
int val;
int counter = 1;

void setup(){
  Wire.begin();
  Serial.begin(9600);
  delay(1000);
  Serial.println("MM/DD/YY HH:MM:SS,ReadingNumber,DegreeC,DegreeF,Light,Humidity,Sound");
}

void loop(){
  printDate();
  Serial.print(",");
  delay(500);
  printTemp();
  Serial.print(",");
  Serial.print(counter);
  Serial.print(",");
  //light
  Serial.print(analogRead(0));
  Serial.print(",");
  //humidity
  Serial.print(analogRead(1));
  Serial.print(",");
  //sound
  Serial.println(analogRead(2));
  counter++;
  delay(5000);
}

byte bcdToDec(byte val)  {
  // Convert binary coded decimal to normal decimal numbers
  return ( (val/16*10) + (val%16) );
}

void printDate(){

  // Reset the register pointer
  Wire.beginTransmission(DS1307_ADDRESS);
  Wire.write(0);
  Wire.endTransmission();

  Wire.requestFrom(DS1307_ADDRESS, 7);

  int second = bcdToDec(Wire.read());
  int minute = bcdToDec(Wire.read());
  int hour = bcdToDec(Wire.read() & 0b111111); //24 hour time
  int weekDay = bcdToDec(Wire.read()); //0-6 -> sunday - Saturday
  int monthDay = bcdToDec(Wire.read());
  int month = bcdToDec(Wire.read());
  int year = bcdToDec(Wire.read());

  //print the date EG   3/1/11 23:59:59
  Serial.print(month);
  Serial.print("/");
  Serial.print(monthDay);
  Serial.print("/");
  Serial.print(year);
  Serial.print(" ");
  Serial.print(hour);
  Serial.print(":");
  Serial.print(minute);
  Serial.print(":");
  Serial.print(second);

}
void printTemp(){
  Wire.beginTransmission(tmp102Address);
  Wire.write(0);
  Wire.endTransmission();

  Wire.requestFrom(tmp102Address,2); 

  byte MSB = Wire.read();
  byte LSB = Wire.read();

  int TemperatureSum = ((MSB << 8) | LSB) >> 4; //it's a 12bit int, using two's compliment for negative

  float celsius = TemperatureSum*0.0625;
  float fahrenheit = (TemperatureSum*0.1125) + 32;  

  //Serial.print("Celsius: ");
  Serial.print(celsius);

  //Serial.print("Fahrenheit: ");
  Serial.print(",");
  Serial.print(fahrenheit);
}

