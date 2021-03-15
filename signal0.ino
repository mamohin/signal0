/*
    передатчик первой версии (на nRF24)
    для сигналки на работе
    Author:     Andrew Mamohin
*/
 
#include <SPI.h> 
#include <nRF24L01.h> 
#include <RF24.h> 
 
// 9 и 10 - это номера пинов CE и CSN модуля nRF24L01+
RF24 radio(9, 8); 
int data[2] = {0, 0}; 
const int w1 = 3; // для 1 датчика
const unsigned long tid = 7000; // задержка при тревоге
const unsigned long tis = 10000; // задержка между передачами
unsigned long ti0 = 0;
unsigned long ti66 = 0;
unsigned long ti = 0; // время с запуска
 
void setup(){
 delay(1000);
 pinMode(w1, INPUT);
// pinMode(w2, INPUT);
 radio.begin(); 
 delay(1000);
 radio.setChannel(64); // задаем канал для передачи данных (от 0 до 127)
 radio.setDataRate(RF24_250KBPS); // Скорость передачи данных 1Мбит/сек. Возможны: RF24_250KBPS, RF24_1MBPS, RF24_2MBPS
 // Мощность передатчика -6dBm. 
 // Возможные можности: 
 // RF24_PA_MIN = -18dBm, RF24_PA_LOW = -12dBm, 
 // RF24_PA_HIGH = -6dBm, RF24_PA_MAX = 0dBm
 radio.setPALevel(RF24_PA_MAX); 
 radio.setAutoAck(false);
 delay(1000); 
 radio.powerUp();
 radio.stopListening();
 delay(1000);
 // Открываем трубу с идентификатором 0x1234567890 для передачи данных 
 // на одном канале можно открыть 6 разных труб 
 // они должны отличаться только последним байтом
 radio.openWritingPipe (0x1234567890LL); 
 data[1] = 80; 
}
 
void loop() {  

 ti = millis();
 
 if (ti0 < ti) {
  data[0] = 16;
  for (byte i = 0; i < 4; i++) {
   radio.writeFast(&data, sizeof(data)); 
   delay(100); 
  }
  ti0 = ti + tis;
 }

 if (ti < ti66) {
  data[0] = 66;
  radio.writeFast(&data, sizeof(data)); 
  ti0 = ti + tis;
  delay(100); 
 } else {
  if (digitalRead(w1)) ti66 = ti + tid;
 }

 delay(100); 
}
