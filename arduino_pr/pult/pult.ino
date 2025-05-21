#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9,10);
const byte address[6] = "00001";

const int axisX = A0;
const int axisY = A1;
const int buttonPin = 7 ;
int valX, valY, valButton = 0; // переменные для хранения значений осей и кнопки

void setup(){
  Serial.begin(9600);
  while (!Serial);
  if (!radio.begin()) {
    Serial.println("Radio init failed!");
    while (1);
  }
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);    // Уровень мощности
  radio.setDataRate(RF24_250KBPS);  // Скорость обмена
  radio.stopListening();
  Serial.println("Transmitter ready!"); // debug

  pinMode(buttonPin, INPUT_PULLUP);
}

void loop(){
  valX = analogRead(axisX);
  valY = analogRead(axisY);
  valButton = 1 - digitalRead(buttonPin);
  int values[] = {valX, valY, valButton};
  if (radio.write(&values, sizeof(values))) {} else {Serial.println("Send failed");}
  delay(1000);
}
