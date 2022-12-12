/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ThisThread.h"
#include "mbed.h"
#include "DAC7578.h"


int main() {
    I2C i2c(p28,p27);
    uint8_t addresses[3] = {DAC7578_I2C_ADDRESS_1, DAC7578_I2C_ADDRESS_2, DAC7578_I2C_ADDRESS_3};
    DAC7578 dacs = DAC7578(&i2c, 3, addresses);

    //Can use defines to choose channel
    dacs.setVoltage(DAC7578_CHANNEL_A, 0.75, 0);
    dacs.setVoltage(DAC7578_CHANNEL_A, 1.5, 1);
    dacs.setVoltage(DAC7578_CHANNEL_A, 2.25, 2);

    while (1) {
        //Can use numbers 0-7 to choose channel
        dacs.readDACVoltage(0, 0);
        dacs.readDACVoltage(0, 1);
        dacs.readDACVoltage(0, 2);
        ThisThread::sleep_for(2s);
    }
    
    
}
