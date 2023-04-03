#include "motors.hpp"
#include "pinconfig.hpp"
#include "responder.hpp"

#ifdef ARDUINO
#include <Servo.h>
#include <EEPROM.h>

#else
#include <stdio.h>
#include <cstring>

#define pinMode(pin, mode)

class Servo
{
    int pin_;

public:
    void attach(int pin)
    {
        pin_ = pin;
    }

    bool attached()
    {
        return false;
    }

    void write(int value)
    {
        printf("Servo %d written angle %d\n", pin_, value);
    }
};

class _EEPROM_mock
{
public:
    uint8_t read(const uint8_t addr)
    {
        printf("Read at address %d", addr);
        return 0;
    }

    uint8_t write(const uint8_t addr, const uint8_t value)
    {
        printf("Write %d at address %d", value, addr);
    }

    void commit()
    {
        printf("Commit changes");
    }
} EEPROM;
#endif // ARDUINO

#define EEPROM_START_ADDR 0
#define EEPROM_MARK 0xC0DE

static FlapStatus flapStatus;
static FilterState filterState;

static Servo motors[NUM_MOTORS];

static const uint8_t openedAnglesDefault[NUM_MOTORS] = {
    [MotorID::FLAP] = 0,
    [MotorID::F1] = 0,
    [MotorID::F2] = 0,
    [MotorID::F3] = 0,
    [MotorID::F4] = 0,
};

static const uint8_t closedAnglesDefault[NUM_MOTORS] = {
    [MotorID::FLAP] = 90,
    [MotorID::F1] = 90,
    [MotorID::F2] = 90,
    [MotorID::F3] = 90,
    [MotorID::F4] = 90,
};

static uint8_t openedAngles[NUM_MOTORS];
static uint8_t closedAngles[NUM_MOTORS];
bool haveUnsavedCalibration = false;

static void loadAngles()
{
    uint8_t address = EEPROM_START_ADDR;
    uint16_t mark = EEPROM.read(address++);
    mark = mark << 8 | EEPROM.read(address++);

    if (mark == EEPROM_MARK)
    {
        for (uint8_t i = 0; i < NUM_MOTORS; i++)
        {
            uint8_t angle = EEPROM.read(address++);
            if (angle > 180)
                angle = openedAnglesDefault[i];
            openedAngles[i] = angle;
        }
        for (uint8_t i = 0; i < NUM_MOTORS; i++)
        {
            uint8_t angle = EEPROM.read(address++);
            if (angle > 180)
                angle = closedAnglesDefault[i];
            closedAngles[i] = angle;
        }
    }
    else
    {
        memcpy(openedAngles, openedAnglesDefault, sizeof(openedAngles));
        memcpy(closedAngles, closedAnglesDefault, sizeof(closedAngles));
    }
}

void motorsSaveAngles()
{
    uint8_t address = EEPROM_START_ADDR;

    EEPROM.write(address++, EEPROM_MARK >> 8);
    EEPROM.write(address++, EEPROM_MARK & 0xFF);

    for (uint8_t angle : openedAngles)
    {
        EEPROM.write(address++, angle);
    }
    for (uint8_t angle : closedAngles)
    {
        EEPROM.write(address++, angle);
    }
    EEPROM.commit();
    haveUnsavedCalibration = false;
}

bool motorsHaveUnsavedCalibrationData()
{
    return haveUnsavedCalibration;
}

void motorsInit()
{
    const uint8_t pins[NUM_MOTORS] = {PIN_S0, PIN_S1, PIN_S2, PIN_S3, PIN_S4};

    for (uint8_t i = 0; i < NUM_MOTORS; i++)
    {
        pinMode(pins[i], OUTPUT);
        motors[i].attach(pins[i]);
    }

    loadAngles();

    flapSet(FlapStatus::CLOSED);
    filterSet(FilterState::FS0);
}

void motorCalibrate(CalibrationData calibration)
{
    openedAngles[calibration.motorID] = calibration.openedAngle;
    closedAngles[calibration.motorID] = calibration.closedAngle;
    haveUnsavedCalibration = true;
}

void motorsGetCalibration(Response &response)
{
    response.type = ResponseType::DATA;
    memcpy(response.angles.opened, openedAngles, sizeof(openedAngles));
    memcpy(response.angles.closed, closedAngles, sizeof(closedAngles));
}

void flapSet(FlapStatus status)
{
    motors[MotorID::FLAP].write(status == FlapStatus::OPENED ? openedAngles[MotorID::FLAP] : closedAngles[MotorID::FLAP]);
    flapStatus = status;
}

FlapStatus flapGet()
{
    return flapStatus;
}

void filterSet(FilterState state)
{
    motors[MotorID::F1].write(state == FilterState::FS1 ? openedAngles[MotorID::F1] : closedAngles[MotorID::F1]);
    motors[MotorID::F2].write(state == FilterState::FS2 ? openedAngles[MotorID::F2] : closedAngles[MotorID::F2]);
    motors[MotorID::F3].write(state == FilterState::FS3 ? openedAngles[MotorID::F3] : closedAngles[MotorID::F3]);
    motors[MotorID::F4].write(state == FilterState::FS4 ? openedAngles[MotorID::F4] : closedAngles[MotorID::F4]);
    filterState = state;
}

FilterState filterGet()
{
    return filterState;
}
