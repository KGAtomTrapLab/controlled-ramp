# Onboard Power Supply
Currently, the ramp needs to be driven using two external power supplies, one for 70V and one for -10V. While the voltage needed is high, the necessary current is low. Thus, an onboard power supply could be used.

The DC power connector for the Arduino is 12V(TODO: Find a source for this).

## Possible Solution: DC Boost Converter
Run the converter in parallel with the Arduino using the same power supply. 12V Boost Converters exist, such as this one: https://www.ti.com/product/LM5158

Should work

