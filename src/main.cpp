/* Copyright (C) 2023 Daniel Efimenko
 *      github.com/Danya0x07
 */

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <EEPROM.h>
#include <Button.h>
#include "dispatcher.hpp"
#include "motors.hpp"
#include "parser.hpp"
#include "responder.hpp"
#include "executor.hpp"
#include "pinconfig.hpp"

char inputBuffer[64];
char outputBuffer[4];
Command commandBuffer[100];
ProgramMemory programMemory(commandBuffer, sizeof(commandBuffer) / sizeof(Command));
ProgramRunner programRunner(commandBuffer);
CommandDispatcher commandDispatcher(programMemory, programRunner);
Timer waitTimer;
Executor executor(waitTimer);
Button button(PIN_BTN, HIGH);

/**
 * Асинхронный приём данных по последовательному порту.
 * Читаем из буфера Serial только тогда, когда данные перестали приходить
 * или их накопилось слишком много.
 */
bool haveSerialInput()
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
        CommandDispatcher::State dispatcherState = commandDispatcher.getState();
        Command command;

        if (dispatcherState == CommandDispatcher::REALTIME) {
            command.type = CommandType::SET_FLAP;
            command.flapStatus = flapGet() == FlapStatus::CLOSED ? FlapStatus::OPENED : FlapStatus::CLOSED;
            commandDispatcher.dispatch(command);
        } else if (dispatcherState == CommandDispatcher::EXECUTING) {
            command.type = CommandType::_EMERGENCY;
            commandDispatcher.dispatch(command);
        }
    }
}

void processInput()
{
    Command command;
    ParsingStatus status = parse(inputBuffer, command);

    if (status == ParsingStatus::OK) {
        reply(outputBuffer, Response::PARSING_OK);
        Serial.print(outputBuffer);
        
        CommandDispatcher::Status dispatchStatus = commandDispatcher.dispatch(command);
        if (dispatchStatus != CommandDispatcher::OK) {
            reply(outputBuffer, Response::DISPATCH_ERR);
            Serial.print(outputBuffer);
        }
    } else {
        reply(outputBuffer, Response::PARSING_ERR);
        Serial.print(outputBuffer);
    }
}

void executeCommands()
{
    if ((!executor.isExecuting() && programRunner.commandsAvailable())
        || programRunner.hasUrgentCommand()
    ) {
        Command command;
        programRunner.readCommand(command);
        executor.startExecuting(command, millis());
    }

    Executor::Event event = executor.run(millis());
    if (event == Executor::FINISHED && !programRunner.commandsAvailable()) {
        commandDispatcher.notifyExecutionFinished();
        reply(outputBuffer, Response::EXEC_FINISH);
        Serial.print(outputBuffer);
    }
}

void setup() 
{
    WiFi.mode(WIFI_OFF);
    WiFi.setSleepMode(WIFI_MODEM_SLEEP);
    WiFi.forceSleepBegin();
    Serial.begin(9600);
    EEPROM.begin(100);
    motorsInit();
    filterSet(FilterState::FS0);
    flapSet(FlapStatus::OPENED);
}

void loop()
{
    handleButton();
    if (haveSerialInput())
        processInput();
    executeCommands();
}
