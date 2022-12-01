#ifndef DAC7578_H
#define DAC7578_H

#include "PinNames.h"
#include "mbed.h"
#include <map>
/* I2C ADDRESSES */
#define DAC7578_I2C_ADDRESS_1 0b1001000 << 1//ADDR0 LOW
#define DAC7578_I2C_ADDRESS_2 0b1001010 << 1//ADDR0 HIGH
#define DAC7578_I2C_ADDRESS_3 0b1001100 << 1//ADDR0 Floating

///////////////////////////////
/* COMMAND BYTES */
#define DAC7578_WRITE_COMMAND                0
#define DAC7578_UPDATE_COMMAND               1
#define DAC7578_WRITE_AND_UPDATE_ALL_COMMAND 2
#define DAC7578_WRITE_AND_UPDATE_COMMAND     3
#define DAC7578_POWER_DOWN_DAC_COMMAND       4
#define DAC7578_WRITE_TO_CLEAR_COMMAND       5
#define DAC7578_WRITE_TO_LDAC_COMMAND        6
#define DAC7578_SOFTWARE_RESET_COMMAND       7

#define DAC7578_READ_INPUT_COMMAND      0
#define DAC7578_READ_COMMAND            1
#define DAC7578_READ_POWER_DOWN_COMMAND 4
#define DAC7578_READ_CLEAR_COMMAND      5
#define DAC7578_READ_LDAC_COMMAND       6
///////////////////////////////
/* ACCESS BYTES */
#define DAC7578_CHANNEL_A   0
#define DAC7578_CHANNEL_B   1
#define DAC7578_CHANNEL_C   2
#define DAC7578_CHANNEL_D   3
#define DAC7578_CHANNEL_E   4
#define DAC7578_CHANNEL_F   5
#define DAC7578_CHANNEL_G   6
#define DAC7578_CHANNEL_H   7
#define DAC7578_CHANNEL_ALL 15

///////////////////////////////
/* POWER DOWN CONTROL */
#define DAC7578_POWER_ON        0
#define DAC7578_POWER_DOWN_1K   1
#define DAC7578_POWER_DOWN_100k 2
#define DAC7578_POWER_DOWN_HIZ  3

///////////////////////////////
/* CLEAR CODE CONTROL */
#define DAC7578_CLR_ZERO_SCALE 0
#define DAC7578_CLR_MID_SCALE  1
#define DAC7578_CLR_FULL_SCALE 2
#define DAC7578_CLR_DISABLE    3

//////////////////////////////
/* SOFTWARE RESET CONTROL */
#define DAC7578_SOFTWARE_RESET_DEFAULT             0
#define DAC7578_SOFTWARE_RESET_SET_HIGH_SPEED      1
#define DAC7578_SOFTWARE_RESET_MAINTAIN_HIGH_SPEED 2

class DAC7578 {
public:
bool LDAC_ENABLED;
bool ADDRESS_1_ENABLED;
bool ADDRESS_2_ENABLED;
bool ADDRESS_3_ENABLED;

// Multiple DAC constructor
DAC7578(I2C * i2c, uint8_t numDacs, uint8_t * addresses, DigitalInOut * LDAC);
DAC7578(I2C * i2c, uint8_t numDacs, uint8_t * addresses);

// Individual DAC constructor
DAC7578(I2C * i2c, uint8_t address, DigitalInOut * LDAC);
DAC7578(I2C * i2c, uint8_t address);

// Set voltage of channel
void setVoltage(uint8_t channel, float v);
void setVoltageAll(float v);


// Read functions

uint16_t readInputVoltage(uint8_t channel, uint8_t index = 0);
uint16_t readDACVoltage(uint8_t channel, uint8_t index = 0);

uint16_t readPowerDown(uint8_t index = 0);
uint16_t readClear(uint8_t index = 0);
uint16_t readLDAC(uint8_t index = 0);

//Clear DAC register

void clearAllZeroScale(uint8_t index = 0);
void clearAllMidScale(uint8_t index = 0);
void clearAllFullScale(uint8_t index = 0);

void disableClear(uint8_t index = 0);

// Power functions
void powerOn(uint8_t channels = 0xFF, uint8_t index = 0);
void powerDown(uint8_t channels = 0xFF, uint8_t control = DAC7578_POWER_DOWN_HIZ, uint8_t index = 0);


// Software Reset
void softwareReset(uint8_t control = DAC7578_SOFTWARE_RESET_DEFAULT, uint8_t index = 0);

// LDAC
void LDACSetAsync(uint8_t channels = 0xFF, uint8_t index = 0);
void update();

private:
char buffer[3];
DigitalInOut * LDAC;
I2C * i2c;
uint8_t numDacs;
std::map<uint8_t, uint8_t> DACIndexToAddress;
std::map<uint8_t, uint8_t> addressToDacIndex;
void writeToRegister(uint8_t command, uint8_t channel, uint16_t data, uint8_t index = 0);
uint16_t readFromRegister(uint8_t command, uint8_t channel, uint8_t index = 0);

};

#endif /* DAC7578_H */
