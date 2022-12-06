//Wojciech Zurek 2022

#include <SPI.h>

String SerialReceivedData = "";

int switchResetPin = 15;
int switchWPin = 2;
int switchXPin = 3;
int switchYPin = 4;
int switchZPin = 5;

int potAPin = 14;
int potBPin = 14;
int potCPin = 10;
int potDPin = 10;
int potEPin = 7;
int potFPin = 6;
int potGPin = 6;
int potHPin = 8;
int potIPin = 9;

void setup() {
  SPI.begin();
  Serial.begin(9600);

  //switches
  pinMode(switchResetPin, OUTPUT);
  pinMode(switchWPin, OUTPUT);
  pinMode(switchXPin, OUTPUT);
  pinMode(switchYPin, OUTPUT);
  pinMode(switchZPin, OUTPUT);

  digitalWrite(switchResetPin, HIGH);
  digitalWrite(switchWPin, HIGH);
  digitalWrite(switchXPin, HIGH);
  digitalWrite(switchYPin, HIGH);
  digitalWrite(switchZPin, HIGH);

  //potentiometers
  pinMode(potAPin, OUTPUT); //A same B
  pinMode(potCPin, OUTPUT); //C same D
  pinMode(potEPin, OUTPUT);
  pinMode(potFPin, OUTPUT); //F same G
  pinMode(potHPin, OUTPUT);
  pinMode(potIPin, OUTPUT);

  digitalWrite(potAPin, HIGH);
  digitalWrite(potCPin, HIGH);
  digitalWrite(potEPin, HIGH);
  digitalWrite(potFPin, HIGH);
  digitalWrite(potHPin, HIGH);
  digitalWrite(potIPin, HIGH);
}

void loop() {
  while(Serial.available()){
    delay(15);
    SerialReceivedData += char(Serial.read());
  }
  delay(50);
  //commands prompt
  if(SerialReceivedData == "reset"){
    setReset();
  }
  else if(SerialReceivedData == "modulePot2"){
    setModulePot2();
  }
  else if(SerialReceivedData != ""){ //check switch or potentiometer
    String firstSymbol = SerialReceivedData.substring(0, 1);
    SerialReceivedData = SerialReceivedData.substring(1);
    int SerialReceivedNumber = SerialReceivedData.toInt();
    if(firstSymbol == "W" || firstSymbol == "X" || firstSymbol == "Y" || firstSymbol == "Z"){
      setSwitch(firstSymbol, SerialReceivedNumber);
    }
    else if(firstSymbol == "A" || firstSymbol == "B" || firstSymbol == "C" || firstSymbol == "D" ||
    firstSymbol == "E" || firstSymbol == "F" || firstSymbol == "G" || firstSymbol == "H" || firstSymbol == "I"){
      setPotentiometer(firstSymbol, SerialReceivedNumber);
    }
  }
  SerialReceivedData = "";
}

void setReset(){
  digitalWrite(switchResetPin, LOW);
  delay(50);
  digitalWrite(switchResetPin, HIGH);

  //for potentiometers
  digitalWrite(potAPin, LOW);
  digitalWrite(potCPin, LOW);
  digitalWrite(potEPin, LOW);
  delay(30);
  digitalWrite(potFPin, LOW);
  digitalWrite(potHPin, LOW);
  digitalWrite(potIPin, LOW);
  delay(30);
  SPI.transfer(16);
  SPI.transfer(0);
  delay(50);
  digitalWrite(potAPin, HIGH);
  digitalWrite(potCPin, HIGH);
  digitalWrite(potEPin, HIGH);
  delay(30);
  digitalWrite(potFPin, HIGH);
  digitalWrite(potHPin, HIGH);
  digitalWrite(potIPin, HIGH);
  delay(30);

  digitalWrite(potBPin, LOW);
  digitalWrite(potCPin, LOW);
  digitalWrite(potFPin, LOW);
  delay(30);
  SPI.transfer(0);
  SPI.transfer(0);
  delay(50);
  digitalWrite(potBPin, HIGH);
  digitalWrite(potCPin, HIGH);
  digitalWrite(potFPin, HIGH);
  delay(50);
}

void setModulePot2(){
  setSwitch("Y", 192);
  delay(50);
  setSwitch("Z", 28);
}

void setSwitch(String const &firstSymbol, int const &SerialReceivedData){
  int pin = changeIdToPin(firstSymbol);

  digitalWrite(pin, LOW);
  delay(30);
  SPI.transfer(SerialReceivedData);
  delay(50);
  digitalWrite(pin, HIGH);
}

void setPotentiometer(String const &firstSymbol, int const &SerialReceivedData){
  int pin = changeIdToPin(firstSymbol);
  int channel = setChannel(firstSymbol);

  digitalWrite(pin, LOW);
  delay(30);
  SPI.transfer(channel);
  SPI.transfer(SerialReceivedData);
  delay(50);
  digitalWrite(pin, HIGH);
}

int changeIdToPin(String const &symbol){
  if(symbol == "W")
    return switchWPin;
  else if(symbol == "X")
    return switchXPin;
  else if(symbol == "Y")
    return switchYPin;
  else if(symbol == "Z")
    return switchZPin;
  else if(symbol == "A" || symbol == "B")
    return potAPin;
  else if(symbol == "C" || symbol == "D")
    return potCPin;
  else if(symbol == "E")
    return potEPin;
  else if(symbol == "F" || symbol == "G")
    return potFPin;
  else if(symbol == "H")
    return potHPin;
  else if(symbol == "I")
    return potIPin;
}

int setChannel(String const &symbol){
  if(symbol == "B" || symbol == "C" || symbol == "F")
    return 0;
  else
    return 16;
}
