#include <Arduino.h>
#include <Servo.h>
#include <Button.h>
#include <pinconfig.hpp>

static char inputBuffer[32];
static Button button(PIN_BTN, HIGH);

Servo ser0, ser1, ser2, ser3, ser4;
#define BTN_PIN D1

/**
 * Асинхронный приём данных по последовательному порту.
 * Читаем из буфера Serial только тогда, когда данные перестали приходить
 * или их накопилось слишком много.
 */
bool handleSerialInput()
{
    static uint32_t lastInputTime;
    static uint8_t incomingBytesAmountPrev;
    uint8_t incomingBytesAmount = Serial.available();

    if (incomingBytesAmount != incomingBytesAmountPrev) {
        incomingBytesAmountPrev = incomingBytesAmount;
        lastInputTime = millis();
    }

    if (incomingBytesAmount && 
        (millis() - lastInputTime > 20 || incomingBytesAmount > 60))
    {
        int amount = Serial.readBytesUntil('\n', inputBuffer, sizeof(inputBuffer) - 1);
        inputBuffer[amount] = '\0';
        return true;
    }
    return false;
}

/**
 * Проверка и реакция на события кнопки.
 */
void handleButton()
{
    Button::Event event = button.getLastEvent();

    if (event == Button::PRESS) {
        
    }
}

void setup() {
  Serial.begin(9600);
  pinMode(D0, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(BTN_PIN, INPUT);
  ser0.attach(D7);
  ser1.attach(D6);
  ser2.attach(D5);
  ser3.attach(D3);
  ser4.attach(D2);
}

void loop() {
  digitalWrite(D0, 1);
  digitalWrite(D4, 0);
  ser0.write(0);
  ser1.write(180);
  ser2.write(0);
  ser3.write(180);
  ser4.write(0);
  delay(1000);
  digitalWrite(D0, 0);
  digitalWrite(D4, 1);
  ser0.write(180);
  ser1.write(0);
  ser2.write(180);
  ser3.write(0);
  ser4.write(180);
  delay(1000);
  Serial.println("Hello basstards!");
}