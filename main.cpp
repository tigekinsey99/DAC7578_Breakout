/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "BufferedSerial.h"
#include "DAC7578.h"
#include "DigitalInOut.h"
#include "PinNames.h"
#include "ThisThread.h"
#include "mbed.h"
#include <chrono>
#include <cstdint>
#include <cstdlib>

#define NUM_DACS 3
#define INVALID_ARG 420
DigitalInOut clear1(p23);
DigitalInOut clear2(p22);
DigitalInOut clear3(p21);

BufferedSerial pc(USBTX, USBRX);
int main() {
  I2C i2c(p28, p27);
  i2c.frequency(100000);
  pc.set_baud(9600);
  clear1.mode(OpenDrain);
  clear2.mode(OpenDrain);
  clear3.mode(OpenDrain);
  clear1.output();
  clear2.output();
  clear3.output();
  clear1 = 1;
  clear2 = 1;
  clear3 = 1;
  uint8_t addresses[3] = {DAC7578_I2C_ADDRESS_2, DAC7578_I2C_ADDRESS_1,
                          DAC7578_I2C_ADDRESS_3};
  DAC7578 dacs = DAC7578(&i2c, NUM_DACS, addresses);

  char *sBuff = new char[4];
  int len = 0;
  int index = INVALID_ARG;
  int command = INVALID_ARG;
  float param = 420.0;
  char r[32];
  char msg[] = "Select index\r\n";
  char cMsg[] = "Select command\r\n";
  char vMsg[] = "Input voltage\r\n";
  char scaleMsg[] = "0 - zero scale, 1 - mid scale, 2 - full scale\r\n";
  char chMsg[] = "Select channel\r\n";
  while (1) {
    // index

    pc.write(msg, sizeof(msg));
    pc.read(sBuff, sizeof(sBuff));
    switch (sBuff[0]) {
    case '0':
      index = 0;
      break;
    case '1':
      index = 1;
      break;
    case '2':
      index = 2;
      break;
    }
    if (index == 420)
      break;
    // command

    pc.write(cMsg, sizeof(cMsg));
    pc.read(sBuff, sizeof(sBuff));
    int channel = INVALID_ARG;

    switch (sBuff[0]) {
    case 'c': // clear
      pc.write(scaleMsg, sizeof(scaleMsg));
      pc.read(sBuff, sizeof(sBuff));
      switch (sBuff[0]) {
      case '0':
        dacs.clearAllZeroScale(index);
        break;
      case '1':
        dacs.clearAllMidScale(index);
        break;
      case '2':
        dacs.clearAllFullScale(index);
        break;
      default:
        break;
      }
      switch (index) {
          case 0:
          clear1 = 0;
          ThisThread::sleep_for(50ms);
          clear1 = 1;
          break;
          case 1:
          clear2 = 0;
          ThisThread::sleep_for(50ms);
          clear2 = 1;
          break;
          case 2:
          clear3 = 0;
          ThisThread::sleep_for(50ms);
          clear3 = 1;
          break;
      }
      break;
    case 'v': // voltage

      pc.write(vMsg, sizeof(vMsg));
      pc.read(sBuff, sizeof(sBuff));
      param = atof(sBuff);
      if (param == 0.0)
        break;
      pc.write(chMsg, sizeof(chMsg));
      pc.read(sBuff, sizeof(sBuff));
      if (sBuff[0] < '0' || sBuff[0] > '8')
        break;
      channel = sBuff[0] - 48;
      dacs.setVoltage(channel, param, index);
      break;
    case 'r': // read voltage
      pc.write(chMsg, sizeof(chMsg));
      pc.read(&sBuff, 1);
      if (sBuff[0] < '0' || sBuff[0] > '8')
        break;
      channel = sBuff[0] - '0';
      sprintf(r,"DAC %d CHANNEL %d: %d\n", channel, index, dacs.readDACVoltage(channel, index));
      pc.write(r, sizeof(r));
      break;
    case 'l': // LDAC
      pc.write(chMsg, sizeof(chMsg));
      pc.read(&sBuff, 1);
      if (sBuff[0] < '0' || sBuff[0] > '8')
        break;
      channel = sBuff[0] - 48;
      dacs.LDACSetAsync(channel, index);
      break;
    }
  }
}
