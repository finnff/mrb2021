#MRB22-23 

1. Download arduino-cli pyserial-miniterm
2. gitclone
3. run `chmod +x ./CompileFlash.sh`   (or load into arduino IDE idk)


`./CompileFlash.sh -A [Architecture] -F ./airpendulum/airpendulum.ino -P [port] --serialmon 9600` 


Run both 
* `./CompileFlash.sh -A [Uno/Nano168/Nano328/Due/esp32/esp8266] -F ./airpendulum/airpendulum.ino -P /dev/tty* --serialmon 9600` on main arduino
* optional  `./CompileFlash.sh -A [Uno/Nano168/Nano328/Due/esp32/esp8266] -F ./airpendulum/LCD1602Slave/LCD1602Slave.ino -P /dev/tty* --serialmon 9600` on second 1602 slave arduino.

