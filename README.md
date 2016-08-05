# stm32audio
audio experiments on stm32
  serial upload instructions for stm32f1 maple mini clone: 
  ftdi rx to PA9
  ftdi tx to PA10 
  ftdi vcc to 3.3v
  ftdi ground to ground
  
  ftdi set to 3.3V
  Boot0 at 1 position (top jumper when usb socket is to your left)

  sketch will run after upload, but not on power on if boot0 is at 1

  press reset if sketch is running to place board back to upload mode
