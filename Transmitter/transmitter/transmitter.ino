#include <SPI.h>          // библиотека для работы с шиной SPI
#include <microDS18B20.h> // библиотека датчика температуры
#include "nRF24L01.h"     // библиотека радиомодуля
#include "RF24.h"         // ещё библиотека радиомодуля

MicroDS18B20 <3> ds;
 
RF24 radio(9, 10); // "создать" модуль на пинах 9 и 10 Для Уно

byte address[][6] = {"1Node", "2Node", "3Node", "4Node", "5Node", "6Node"}; //возможные номера труб
float counter[3];  // сюда пишем данные для передачи

void setup() {
  Serial.begin(9600);

  ds.setResolution(12); //разрешение датчика температуры bit
  
  pinMode(2, OUTPUT);
  pinMode(4, INPUT);
  pinMode(5, OUTPUT);
  digitalWrite(2, HIGH);
  digitalWrite(5, HIGH);
  
  //-----Настройки NRF-----
  radio.begin();              // активировать модуль
  radio.setAutoAck(1);        // режим подтверждения приёма, 1 вкл 0 выкл
  radio.setRetries(0, 15);    // (время между попыткой достучаться, число попыток)
  radio.enableAckPayload();   // разрешить отсылку данных в ответ на входящий сигнал
  radio.setPayloadSize(32);   // размер пакета, в байтах
  radio.openWritingPipe(address[0]);  // мы - труба 0, открываем канал для передачи данных
  radio.setChannel(0x60);             // выбираем канал (в котором нет шумов!)
  radio.setPALevel (RF24_PA_MIN);   // уровень мощности передатчика. На выбор RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  radio.setDataRate (RF24_250KBPS); // скорость обмена. На выбор RF24_2MBPS, RF24_1MBPS, RF24_250KBPS 
  //должна быть одинакова на приёмнике и передатчике!
  //при самой низкой скорости имеем самую высокую чувствительность и дальность!!
  radio.powerUp();        // начать работу
  radio.stopListening();  // не слушаем радиоэфир, мы передатчик
}

void loop() {
  measurement();
  radio_writer();
}

void measurement() {
  counter[0] = temp();
  counter[1] = ! digitalRead(4);
  counter[2] = analogRead(1) * 5.0 / 1023;
  Serial.println(counter[2]);
}

float temp(){
  ds.requestTemp(); 
  myDelay(1000);
  if (ds.readTemp()){
    return(ds.getTemp());
  }
}

void radio_writer() {
  radio.write(&counter, sizeof(counter));
}

void myDelay(uint32_t us) {
  uint32_t tmr = millis();
  while (millis() - tmr < us){
  }
}
