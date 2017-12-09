# Description

An Arudino code framework that can be used to detect slight pulses on a digital pin to then control "something".

The code has a "onTimer" and an "offTimer" logic, this is used to say something like:

"if vibration is detected for 20% of the readings during a 5 second window, then turn on"

and the offTimer is:

"if no vibration is detected for 10 seconds, then turn off"

