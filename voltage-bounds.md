# Voltage Boundaries - Setting a Minimum and Maximum
Adding a starting point to the incrementation allows for the focusing of a specific set of voltages. For example, rather than going from -10V to 70V, the ramp will go from 30V to 50V.
This is done digitally by setting a start point for the ramp up. It should be noted that the [ramp resolution](ramp-resolution.md) (~1.25mV) stays the same. To increase the resolution, a physical DC offset will need to be added.

The values are sent as percentages, so a value between 0 and 100.