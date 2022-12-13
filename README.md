# DAC7578_Breakout
This is a breakout board for the Texas Instruments [DAC7578](https://www.ti.com/product/DAC7578). An mbed driver is also included.

## Motivation
The Mbed only supports one analog output pin. We wanted the user to have more analog outputs. The DAC7578 is a 12-bit 8 channel DAC that interfaces via I2C. This gives the user 8 more analog outputs with 12 bits of precision with the sacrifice of an I2C address, 2 pins for SDA and SCL and potentially a few more pins if the user wishes to use the DAC7578's additional functionalities.

## Additional Functionalities
The DAC7578 has an active low LDAC pin that allows the user to update user selected DAC channel registers asynchronously through the falling edge of the LDAC control signal. To use the DAC7578 in synchronous mode, keep LDAC tied low.

The DAC7578 also has an active low NCLR pin to clear the data stored in all the DAC channel registers to either zero scale, mid scale, or full scale voltages depending on the data stored in the clear code register. A clear activates on the falling edge of the NCLR pin.
## Block Diagram
![](./resources/BlockDiagram.png)
## Schematic
![](./resources/Schematic.png)

## 3D Photo
![](./resources/board3D.png)