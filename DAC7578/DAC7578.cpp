#include "DAC7578.h"
#include "ThisThread.h"
#include <chrono>

// Multiple DAC constructor
DAC7578::DAC7578(I2C *i2c, uint8_t numDacs, uint8_t *addresses,
                 DigitalInOut *LDAC) {

  if (numDacs > 3) { // invalid
    this->numDacs = 1;
    return;
  }
this->numDacs = numDacs;
  this->i2c = i2c;
  this->LDAC = LDAC;
    this->LDAC->mode(OpenDrain);
  for (int i = 0; i < numDacs; ++i) {
    switch (addresses[i]) {
    case DAC7578_I2C_ADDRESS_1:
    ENABLED_ADDRESS |= 1;
    DACIndexToAddress[i] = DAC7578_I2C_ADDRESS_1;
    addressToDacIndex[DAC7578_I2C_ADDRESS_1] = i;
    break;
  case DAC7578_I2C_ADDRESS_2:
    ENABLED_ADDRESS |= (1 << 1);
    DACIndexToAddress[i] = DAC7578_I2C_ADDRESS_2;
    addressToDacIndex[DAC7578_I2C_ADDRESS_2] = i;
    break;
  case DAC7578_I2C_ADDRESS_3:
    ENABLED_ADDRESS |= (1 << 2);
    DACIndexToAddress[i] = DAC7578_I2C_ADDRESS_3;
    addressToDacIndex[DAC7578_I2C_ADDRESS_3] = i;
    } // endswitch
  }   // endfor

} // endDAC7578

DAC7578::DAC7578(I2C *i2c, uint8_t numDacs, uint8_t *addresses) {
  if (numDacs > 3) { // invalid
    this->numDacs = 1;
    return;
  }
    this->numDacs = numDacs;
  this->i2c = i2c;

  for (int i = 0; i < numDacs; ++i) {
    switch (addresses[i]) {
    case DAC7578_I2C_ADDRESS_1:
    ENABLED_ADDRESS |= 1;
    DACIndexToAddress[i] = DAC7578_I2C_ADDRESS_1;
    addressToDacIndex[DAC7578_I2C_ADDRESS_1] = i;
    break;
  case DAC7578_I2C_ADDRESS_2:
    ENABLED_ADDRESS |= (1 << 1);
    DACIndexToAddress[i] = DAC7578_I2C_ADDRESS_2;
    addressToDacIndex[DAC7578_I2C_ADDRESS_2] = i;
    break;
  case DAC7578_I2C_ADDRESS_3:
    ENABLED_ADDRESS |= (1 << 2);
    DACIndexToAddress[i] = DAC7578_I2C_ADDRESS_3;
    addressToDacIndex[DAC7578_I2C_ADDRESS_3] = i;
    } // endswitch
  }   // endfor
}

// Individual DAC constructor
DAC7578::DAC7578(I2C *i2c, uint8_t address, DigitalInOut *LDAC) {
  this->i2c = i2c;
  this->LDAC = LDAC;
  this->LDAC->mode(OpenDrain);
  this->numDacs = 1;
  switch (address) {
  case DAC7578_I2C_ADDRESS_1:
    ENABLED_ADDRESS |= 1;
    DACIndexToAddress[0] = DAC7578_I2C_ADDRESS_1;
    addressToDacIndex[DAC7578_I2C_ADDRESS_1] = 0;
    break;
  case DAC7578_I2C_ADDRESS_2:
    ENABLED_ADDRESS |= (1 << 1);
    DACIndexToAddress[0] = DAC7578_I2C_ADDRESS_2;
    addressToDacIndex[DAC7578_I2C_ADDRESS_2] = 0;
    break;
  case DAC7578_I2C_ADDRESS_3:
    ENABLED_ADDRESS |= (1 << 2);
    DACIndexToAddress[0] = DAC7578_I2C_ADDRESS_3;
    addressToDacIndex[DAC7578_I2C_ADDRESS_3] = 0;
  } // endswitch
}

DAC7578::DAC7578(I2C *i2c, uint8_t address) {
  this->i2c = i2c;
  this->numDacs = 1;
  switch (address) {
  case DAC7578_I2C_ADDRESS_1:
    ENABLED_ADDRESS |= 1;
    DACIndexToAddress[0] = DAC7578_I2C_ADDRESS_1;
    addressToDacIndex[DAC7578_I2C_ADDRESS_1] = 0;
    break;
  case DAC7578_I2C_ADDRESS_2:
    ENABLED_ADDRESS |= (1 << 1);
    DACIndexToAddress[0] = DAC7578_I2C_ADDRESS_2;
    addressToDacIndex[DAC7578_I2C_ADDRESS_2] = 0;
    break;
  case DAC7578_I2C_ADDRESS_3:
    ENABLED_ADDRESS |= (1 << 2);
    DACIndexToAddress[0] = DAC7578_I2C_ADDRESS_3;
    addressToDacIndex[DAC7578_I2C_ADDRESS_3] = 0;
  } // endswitch
}

// Set voltage of channel
void DAC7578::writeDACVoltage(uint8_t channel, float v, uint8_t index) {
    uint16_t data = v * (2 << 12) / 3.3;
    this->writeToRegister(DAC7578_WRITE_COMMAND, channel, data, index);
}
void DAC7578::updateDACVoltage(uint8_t channel, uint8_t index) {
    this->writeToRegister(DAC7578_UPDATE_COMMAND, channel, 0, index);
}

void DAC7578::setVoltage(uint8_t channel, float v, uint8_t index) {
    uint16_t data = v * (2 << 12) / 3.3;
    this->writeToRegister(DAC7578_WRITE_AND_UPDATE_COMMAND, channel, data, index);
}
void DAC7578::setVoltageAll(float v, uint8_t index) {
    uint16_t data = v * (2 << 12) / 3.3;
    this->writeToRegister(DAC7578_WRITE_AND_UPDATE_ALL_COMMAND, DAC7578_CHANNEL_ALL, data, index);
}

// Read functions

uint16_t DAC7578::readInputVoltage(uint8_t channel, uint8_t index) {
    return readFromRegister(DAC7578_READ_INPUT_COMMAND, channel, index);
}
uint16_t DAC7578::readDACVoltage(uint8_t channel, uint8_t index) {
    return readFromRegister(DAC7578_READ_COMMAND, channel, index);
}

uint16_t DAC7578::readPowerDown(uint8_t index) {
    return readFromRegister(DAC7578_READ_POWER_DOWN_COMMAND, 0, index);
}
uint16_t DAC7578::readClear(uint8_t index) {
    return readFromRegister(DAC7578_READ_CLEAR_COMMAND, 0, index);
}
uint16_t DAC7578::readLDAC(uint8_t index) {
    return readFromRegister(DAC7578_READ_LDAC_COMMAND, 0, index);
}

// Clear DAC channel register

void DAC7578::clearAllZeroScale(uint8_t index) {
    uint16_t data = DAC7578_CLR_ZERO_SCALE << 4;
    this->writeToRegister(DAC7578_WRITE_TO_CLEAR_COMMAND, 0, data, index);
}

void DAC7578::clearAllMidScale(uint8_t index) {
    uint16_t data = DAC7578_CLR_MID_SCALE << 4;
    this->writeToRegister(DAC7578_WRITE_TO_CLEAR_COMMAND, 0, data, index);
}

void DAC7578::clearAllFullScale(uint8_t index) {
    uint16_t data = DAC7578_CLR_FULL_SCALE << 4;
    this->writeToRegister(DAC7578_WRITE_TO_CLEAR_COMMAND, 0, data, index);
}

void DAC7578::disableClear(uint8_t index){
    uint16_t data = DAC7578_CLR_DISABLE << 4;
    this->writeToRegister(DAC7578_WRITE_TO_CLEAR_COMMAND, 0, data, index);
}

// Software Reset
void DAC7578::softwareReset(uint8_t control, uint8_t index) {
    uint16_t data = (control << 14);
    writeToRegister(DAC7578_SOFTWARE_RESET_COMMAND, 0, data, index);
}

// LDAC
void DAC7578::LDACSetAsync(uint8_t channels, uint8_t index) {
    uint16_t data = (channels << 15);
    writeToRegister(DAC7578_WRITE_TO_LDAC_COMMAND, 0, data, index);
}

void DAC7578::update() {
    if (LDAC_ENABLED) {

    *(this->LDAC) = 0;
    ThisThread::sleep_for(10ms);
    *(this->LDAC) = 1;

    }
}

// Power functions
void DAC7578::powerOn(uint8_t channels, uint8_t index) {
    uint16_t data = (DAC7578_POWER_ON << 13) | (channels << 5);
    writeToRegister(DAC7578_POWER_DOWN_DAC_COMMAND, 0, data, index);
}
void DAC7578::powerDown(uint8_t channels, uint8_t control, uint8_t index) {
    uint16_t data = (control << 13) | (channels << 5);
    writeToRegister(DAC7578_POWER_DOWN_DAC_COMMAND, 0, data, index);
}

void DAC7578::writeToRegister(uint8_t command, uint8_t channel, uint16_t data, uint8_t index) {
    if (!ENABLED_ADDRESS) return;
    this->buffer[0] = (command << 4) | channel;
    this->buffer[1] = data >> 8;
    this->buffer[2] = data & 0x0F;
    this->i2c->write(DACIndexToAddress[index], buffer, 3);
}
uint16_t DAC7578::readFromRegister(uint8_t command, uint8_t channel, uint8_t index) {
    if (!ENABLED_ADDRESS) return 0xFEFE; //This can be the error code
    this->buffer[0] = (command << 4) | channel;
    this->i2c->read(DACIndexToAddress[index], buffer, 3);

    return (uint16_t)(buffer[0] << 8) | buffer[1];
}