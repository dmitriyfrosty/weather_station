#include <SPI.h>          // библиотека для работы с шиной SPI
#include "nRF24L01.h"     // библиотека радиомодуля
#include "RF24.h"         // ещё библиотека радиомодуля

RF24 radio(9, 10); // "создать" модуль на пинах 9 и 10 Для Уно
//RF24 radio(9,53); // для Меги

byte address[][6] = {"1Node", "2Node", "3Node", "4Node", "5Node", "6Node"}; //возможные номера труб
int counter[3];

void setup() {
  Serial.begin(9600);         // открываем порт для связи с ПК

  pinMode(2, OUTPUT);
  pinMode(3, INPUT_PULLUP);
  digitalWrite(2, HIGH);
  
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
  if (digitalRead(3) == 1){
    measurement();
    radio_writer();
    myDelayMicroseconds(60000000);
  }
  else{
    measurement();
    radio_writer();
    delay(100);
  }
}

void measurement() {
  counter[0] = analogRead(1);
  counter[1] = analogRead(4);
  counter[2] = digitalRead(3);
}

void radio_writer() {
  Serial.print("Sent: ");
  Serial.print(counter[0]);
  Serial.print(" ");
  Serial.print(counter[1]);
  Serial.print(" ");
  Serial.println(counter[2]);
  radio.write(&counter, sizeof(counter));
}

void myDelayMicroseconds(uint32_t us) {
  uint32_t tmr = micros();
  while (micros() - tmr < us){
    if (digitalRead(3) == 0) break;
  }
}
