# stm32audio
Turn a midi keyboard into a custom synthesizer.  This is an Arduino project, but surround libraries are pure C.  This was done on purpose so tests can be executed instead of having to upload to test certain parts.  Struggled quite a bit to approximate sine with intergers as the STM32F1 does not have a floating point unit.  

Pics on the [wiki](https://github.com/richardwa/stm32audio/wiki)

## serial upload instructions for stm32f1 maple mini clone: 
- ftdi rx to PA9
- ftdi tx to PA10 
- ftdi vcc to 3.3v
- ftdi ground to ground
- ftdi set to 3.3V
- Boot0 at 1 position (top jumper when usb socket is to your left)
- sketch will run after upload, but not on power on if boot0 is at 1
- press reset if sketch is running to place board back to upload mode
