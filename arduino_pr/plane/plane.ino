#include <SPI.h>                      // Подключаем библиотеку SPI
#include <nRF24L01.h>                 // Подключаем библиотеку nRF24L01 
#include <RF24.h>                     // Подключаем библиотеку 
#include <Servo.h>

#define SERVOL 5
#define SERVOR 6
#define ENGI 3

RF24 radio(9,10);                     // Указываем номера выводов nRF24L01+ (CE, CSN)
Servo myservoL;
Servo myservoR;
Servo myengi;

const byte address[6] = "00001";
bool engine = false;



void setup(){
  Serial.begin(9600);
  while(!Serial);
  if (!radio.begin()) {
    Serial.println("Radio init failed!");
    while (1);
  }
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_250KBPS); 
  radio.startListening ();
  
  myservoL.attach(SERVOL);
  myservoR.attach(SERVOR);
//  myengi.attach(ENGI);/
  
  myengi.attach(ENGI, 1000, 2000);
 
  myengi.writeMicroseconds(1000);
  delay(3000);
}




void loop(){
  if (radio.available()){
    int data[3] = {};
    radio.read(&data, sizeof(data));
    for (int i = 0; i < 3; i++) {
      Serial.print(data[i]);
      Serial.print(" ");
    }
    Serial.println();

    // управление моторами и серваками
    if(!data[2]){
      engine=!engine;
    }
    if(engine){
//      myengi.writeMicroseconds(data[1]*1000./(1024.*10.)+1000);
//        myengi.write(data[1]/10);
        int throttle = map(data[1], 0, 1023, 1000, 2000);
        myengi.writeMicroseconds(throttle);
    }
    if(!engine){
        myengi.writeMicroseconds(1000);
        // myengi.write(0);
    }
    
    myservoL.write(data[0]/25);
    myservoR.write((1024 - data[0])/25);
  }
  
  else{
//    myengi.write(0);
  }
  
}















//void setup(){}
//void loop(){}
