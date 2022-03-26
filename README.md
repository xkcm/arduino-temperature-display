# arduino-temperature-display
7-Segment display showing temperature logs from chosen device on the server machine (preferably using linux os).
* Arduino code: **C**
* Server: **JavaScript** (NodeJS).
### Used display
![display-schema](https://user-images.githubusercontent.com/42122011/160251458-4d689de0-dc7e-4dfa-915d-699996d23c3a.png)

I didn't use this exact display, but it works on the same principle.
## Setup
* Connect the display to the arduino and edit the pin numbers ([lines 1-3 in the C code](https://github.com/xkcm/arduino-temperature-display/blob/cd3a53796f315e859ec666b8f6bca3b0cfafcf69/arduino/sketch_mar20a.ino#L1)).
* Connect the Arduino with USB cable to the PC.
* Check available devices using `sensors -j`, edit the JavaScript code to use the correct device and sensor ([lines 18-19 in the JS code](https://github.com/xkcm/arduino-temperature-display/blob/cd3a53796f315e859ec666b8f6bca3b0cfafcf69/server/index.js#L18)).
* Run the server using NodeJS, it should automatically detect the Arduino USB port and start sending values.
