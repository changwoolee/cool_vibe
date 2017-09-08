# Cool Vibe


**Cool Vibe** is the glove for Virtual Reality, which provides haptic and thermal experience to you.  


This repository is about Raspberry Pi Core Module for ** Cool Vibe**.



## How To Run
    pi@raspberrypi:~ $ git clone https://github.com/changwoolee/cool_vibe.git
    pi@raspberrypi:~ $ cd cool_vibe
    pi@raspberrypi:~/cool_vibe $ make
    pi@raspberrypi:~/cool_vibe $ cd build/examples
    pi@raspberrypi:~/cool_vibe $ ./serial_test
    pi@raspberrypi:~/cool_vibe $ ./udp_test

 
## Modules


#### usb_serial  
usb_serial module sends vibration data to and receive temperature data from ** Cool Vibe** glove.  
Example is ready : serial_test

#### udp_server
udp_server module sends temp data to and receive peltier & vibration control data from PC(Unity3D VR Application).
   
   
#### _Todo List_
 * Control logic for peltier
 * Core Module